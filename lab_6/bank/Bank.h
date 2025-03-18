#include <atomic>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

using AccountId = unsigned long long;
using Money = long long;

class BankOperationError : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};

class Bank
{
public:
  static constexpr AccountId INVALID_ACCOUNT = -1;

  explicit Bank(Money initialCash) : cash(initialCash)
  {
    if (initialCash < 0)
    {
      throw BankOperationError("Negative initial cash");
    }
  }

  Bank(const Bank &) = delete;
  Bank &operator=(const Bank &) = delete;

  unsigned long long GetOperationsCount() const
  {
    return operationsCount.load();
  }

  void SendMoney(AccountId src, AccountId dst, Money amount)
  {
    IncrementOperationCount();
    ValidateAmountNonNegative(amount);

    std::lock_guard<std::mutex> lock(dataMutex);

    auto &srcAccount = GetAccount(src);
    auto &dstAccount = GetAccount(dst);

    if (srcAccount.balance < amount)
    {
      throw BankOperationError("Insufficient funds at sending");
    }

    srcAccount.balance -= amount;
    dstAccount.balance += amount;
  }

  bool TrySendMoney(AccountId src, AccountId dst, Money amount)
  {
    IncrementOperationCount();
    ValidateAmountNonNegative(amount);

    std::lock_guard<std::mutex> lock(dataMutex);

    auto srcIt = accounts.find(src);
    auto dstIt = accounts.find(dst);
    if (srcIt == accounts.end() || dstIt == accounts.end())
    {
      throw BankOperationError("Account not found");
    }

    if (srcIt->second.balance < amount)
    {
      return false;
    }

    srcIt->second.balance -= amount;
    dstIt->second.balance += amount;
    return true;
  }

  Money GetCash() const
  {
    IncrementOperationCount();
    std::lock_guard<std::mutex> lock(dataMutex);
    return cash;
  }

  Money GetAccountBalance(AccountId account) const
  {
    IncrementOperationCount();
    std::lock_guard<std::mutex> lock(dataMutex);
    return GetAccount(account).balance;
  }

  void WithdrawMoney(AccountId account, Money amount)
  {
    IncrementOperationCount();
    ValidateAmountNonNegative(amount);

    std::lock_guard<std::mutex> lock(dataMutex);

    auto &acc = GetAccount(account);
    if (acc.balance < amount)
    {
      throw BankOperationError("Insufficient funds");
    }

    acc.balance -= amount;
    cash += amount;
  }

  bool TryWithdrawMoney(AccountId account, Money amount)
  {
    IncrementOperationCount();
    ValidateAmountNonNegative(amount);

    std::lock_guard<std::mutex> lock(dataMutex);

    auto it = accounts.find(account);
    if (it == accounts.end())
    {
      throw BankOperationError("Account not found");
    }

    if (it->second.balance < amount)
    {
      return false;
    }

    it->second.balance -= amount;
    cash += amount;
    return true;
  }

  void DepositMoney(AccountId account, Money amount)
  {
    IncrementOperationCount();
    ValidateAmountNonNegative(amount);

    std::lock_guard<std::mutex> lock(dataMutex);

    if (cash < amount)
    {
      throw BankOperationError("Not enough cash in reserve to deposit");
    }

    cash -= amount;

    auto &acc = GetAccount(account);
    acc.balance += amount;
  }

  AccountId OpenAccount()
  {
    IncrementOperationCount();
    std::lock_guard<std::mutex> lock(dataMutex);
    AccountId newId = nextAccountId++;
    accounts[newId] = AccountInfo{};
    return newId;
  }

  Money CloseAccount(AccountId account)
  {
    IncrementOperationCount();
    std::lock_guard<std::mutex> lock(dataMutex);

    auto it = accounts.find(account);
    if (it == accounts.end())
    {
      throw BankOperationError("Account not found");
    }

    Money balance = it->second.balance;
    cash += balance;
    accounts.erase(it);
    return balance;
  }

private:
  struct AccountInfo
  {
    Money balance = 0;
  };

  mutable std::mutex dataMutex;
  Money cash;
  std::unordered_map<AccountId, AccountInfo> accounts;
  AccountId nextAccountId = 1;
  mutable std::atomic<unsigned long long> operationsCount{0};

  void IncrementOperationCount() const
  {
    operationsCount.fetch_add(1, std::memory_order_relaxed);
  }

  void ValidateAmountNonNegative(Money amount) const
  {
    if (amount < 0)
    {
      throw std::out_of_range("Negative amount");
    }
  }

  AccountInfo &GetAccount(AccountId account)
  {
    auto it = accounts.find(account);
    if (it == accounts.end())
    {
      throw BankOperationError("Account not found");
    }
    return it->second;
  }

  const AccountInfo &GetAccount(AccountId account) const
  {
    auto it = accounts.find(account);
    if (it == accounts.end())
    {
      throw BankOperationError("Account not found");
    }
    return it->second;
  }
};