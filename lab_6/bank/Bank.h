#pragma once
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <unordered_map>

using AccountId = unsigned long long;
using Money = long long;

class BankOperationError final : std::runtime_error
{
public:
  using runtime_error::runtime_error;
};

class Bank
{
public:
  // Инициализирует монетарную систему. cash — количество денег в наличном
  // обороте При отрицательном количестве денег, выбрасывается
  // BankOperationError
  explicit Bank(Money cash) : m_cash(cash)
  {
    if (m_cash < 0)
    {
      throw BankOperationError("initial cash cannot be negative");
    }
  }

  Bank(const Bank &) = delete;
  Bank &operator=(const Bank &) = delete;

  // Возвращает количество операций, выполненных банком (включая операции чтения
  // состояния) Для неблокирующего подсчёта операций используйте класс
  // std::atomic<unsigned long long> Вызов метода GetOperationsCount() не должен
  // участвовать в подсчёте
  [[nodiscard]] unsigned long long GetOperationsCount() const
  {
    return m_operationsCount.load();
  }

  // Перевести деньги с исходного счёта (srcAccountId) на целевой (dstAccountId)
  // Нельзя перевести больше, чем есть на исходном счёте
  // Нельзя перевести отрицательное количество денег
  // Исключение BankOperationError выбрасывается, при отсутствии счетов или
  // недостатке денег на исходном счёте
  // При отрицательном количестве переводимых денег выбрасывается
  // std::out_of_range
  void SendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount)
  {
    SendMoneyInternal(srcAccountId, dstAccountId, amount, true);
  }

  // Перевести деньги с исходного счёта (srcAccountId) на целевой (dstAccountId)
  // Нельзя перевести больше, чем есть на исходном счёте
  // Нельзя перевести отрицательное количество денег
  // При нехватке денег на исходном счёте возвращается false
  // Если номера счетов невалидны, выбрасывается BankOperationError
  // При отрицательном количестве денег выбрасывается std::out_of_range
  [[nodiscard]] bool TrySendMoney(AccountId srcAccountId,
                                  AccountId dstAccountId,
                                  Money amount)
  {
    return SendMoneyInternal(srcAccountId, dstAccountId, amount, false);
  }

  // Возвращает количество наличных денег в обороте
  [[nodiscard]] Money GetCash() const
  {
    std::shared_lock lock(m_cashMutex);
    return m_cash;
  }

  // Сообщает о количестве денег на указанном счёте
  // Если указанный счёт отсутствует, выбрасывается исключение
  // BankOperationError
  [[nodiscard]] Money GetAccountBalance(AccountId accountId) const
  {
    std::shared_lock bankLock(m_bankMutex);
    EnsureExist(accountId);
    auto &[balance, mutex] = m_accounts.at(accountId);
    std::shared_lock accountLock(mutex);
    return balance;
  }

  // Снимает деньги со счёта. Нельзя снять больше, чем есть на счете
  // Нельзя снять отрицательное количество денег
  // Снятые деньги добавляются к массе наличных денег
  // При невалидном номере счёта или отсутствии денег, выбрасывается исключение
  // BankOperationError При отрицательном количестве денег выбрасывается
  // std::out_of_range
  void WithdrawMoney(AccountId account, Money amount)
  {
    WithdrawMoneyInternal(account, amount, true);
  }

  // Попытаться снять деньги в размере amount со счёта account.
  // Объем денег в наличном обороте увеличивается на величину amount
  // При нехватке денег на счёте возвращается false, а количество наличных денег
  // остаётся неизменным При невалидном номере аккаунта выбрасывается
  // BankOperationError. При отрицательном количестве денег выбрасывается
  // std::out_of_range
  [[nodiscard]] bool TryWithdrawMoney(AccountId account, Money amount)
  {
    return WithdrawMoneyInternal(account, amount, false);
  }

  // Поместить наличные деньги на счёт. Количество денег в наличном обороте
  // уменьшается на величину amount.
  // Нельзя поместить больше, чем имеется денег в наличном обороте
  // Нельзя поместить на счёт отрицательное количество денег
  // Нельзя поместить деньги на отсутствующий счёт
  // При невалидном номере аккаунта или нехватке наличных денег в обороте
  // выбрасывается BankOperationError. При отрицательном количестве денег
  // выбрасывается std::out_of_range
  void DepositMoney(AccountId accountId, Money amount)
  {
    EnsureNotNegative(amount);
    std::shared_lock bankLock(m_bankMutex);
    EnsureExist(accountId);
    auto &[balance, mutex] = m_accounts.at(accountId);
    std::unique_lock accountLock(mutex);
    std::unique_lock cashLock(m_cashMutex);
    if (m_cash < amount)
    {
      throw BankOperationError("insufficient funds in cash");
    }

    balance += amount;
    m_cash -= amount;
    m_operationsCount.fetch_add(1);
  }

  // Открывает счёт в банке. После открытия счёта на нём нулевой баланс.
  // Каждый открытый счёт имеет уникальный номер.
  // Возвращает номер счёта
  [[nodiscard]] AccountId OpenAccount()
  {
    std::unique_lock bankLock(m_bankMutex);
    const auto id = m_nextAccountId++;
    m_accounts.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                       std::forward_as_tuple());
    m_operationsCount.fetch_add(1);

    return id;
  }

  // Закрывает указанный счёт.
  // Возвращает количество денег, которые были на счёте в момент закрытия
  // Эти деньги переходят в наличный оборот
  // При невалидном номере аккаунта выбрасывает BankOperationError
  [[nodiscard]] Money CloseAccount(AccountId accountId)
  {
    std::unique_lock bankLock(m_bankMutex);
    EnsureExist(accountId);
    const auto it = m_accounts.find(accountId);
    const auto balance = it->second.balance;

    std::unique_lock cashLock(m_cashMutex);

    m_accounts.erase(it);
    m_cash += balance;
    m_operationsCount.fetch_add(1);

    return balance;
  }

private:
  bool SendMoneyInternal(AccountId srcAccountId,
                         AccountId dstAccountId,
                         Money amount,
                         bool throwOnError)
  {
    EnsureNotNegative(amount);
    std::shared_lock bankLock(m_bankMutex);
    EnsureExist(srcAccountId, dstAccountId);
    auto &srcAccount = m_accounts.at(srcAccountId);
    auto &dstAccount = m_accounts.at(dstAccountId);
    std::scoped_lock lock(srcAccount.mutex, dstAccount.mutex);
    if (srcAccount.balance < amount)
    {
      if (throwOnError)
      {
        throw BankOperationError("insufficient funds on source account");
      }
      return false;
    }

    srcAccount.balance -= amount;
    dstAccount.balance += amount;
    m_operationsCount.fetch_add(1);

    return true;
  }

  // Определить WithdrawMoney через TryWithdrawMoney
  bool WithdrawMoneyInternal(AccountId accountId,
                             Money amount,
                             bool throwOnError)
  {
    EnsureNotNegative(amount);
    std::shared_lock bankLock(m_bankMutex);
    // возвращать итератор
    EnsureExist(accountId);
    auto &[balance, mutex] = m_accounts.at(accountId);
    std::unique_lock accountLock(mutex);
    // проверить граничные условия
    if (balance < amount)
    {
      if (throwOnError)
      {
        throw BankOperationError("insufficient funds on account");
      }
      return false;
    }

    std::unique_lock cashLock(m_cashMutex);

    balance -= amount;
    m_cash += amount;
    m_operationsCount.fetch_add(1);

    return true;
  }

  static void EnsureNotNegative(Money amount)
  {
    if (amount < 0)
    {
      throw std::out_of_range("amount cannot be negative");
    }
  }

  void EnsureExist(AccountId id) const
  {
    if (!m_accounts.contains(id))
    {
      throw BankOperationError("account does not exist");
    }
  }

  void EnsureExist(AccountId id1, AccountId id2) const
  {
    if (!m_accounts.contains(id1) || !m_accounts.contains(id2))
    {
      throw BankOperationError("account does not exist");
    }
  }

private:
  struct Account
  {
    Money balance = 0;
    mutable std::shared_mutex mutex;
  };

  Money m_cash;
  std::atomic<unsigned long long> m_operationsCount;
  std::unordered_map<AccountId, Account> m_accounts;
  mutable std::shared_mutex m_cashMutex;
  mutable std::shared_mutex m_bankMutex;
  AccountId m_nextAccountId = 0;
};