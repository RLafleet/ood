#pragma once
#include <iostream>
#include <syncstream>
#include "Bank.h"

class Character;
class CharacterWithCard;

struct Characters
{
  CharacterWithCard *homer = nullptr;
  CharacterWithCard *marge = nullptr;
  Character *lisa = nullptr;
  Character *bart = nullptr;
  CharacterWithCard *apu = nullptr;
  CharacterWithCard *burns = nullptr;
  Character *nelson = nullptr;
  CharacterWithCard *snake = nullptr;
  CharacterWithCard *smithers = nullptr;
};

class Character
{
public:
  explicit Character(Money cash, Characters &characters)
      : m_cash(cash), m_characters(characters) {}

  Money GetCash() const
  {
    std::shared_lock lock(m_mutex);
    return m_cash;
  }

  bool TransferCash(Character &recipient, Money amount)
  {
    const auto spent = SpendCash(amount);
    if (spent)
    {
      recipient.AddCash(amount);
    }

    return spent;
  }

  bool StealCache(Character &thief, Money amount)
  {
    return TransferCash(thief, amount);
  }

  void Run(const std::atomic_bool &stopFlag)
  {
    while (!stopFlag.load())
    {
      Next(m_characters);
    }
  }

  virtual void Next(Characters const &characters) = 0;

  virtual ~Character() = default;

  bool SpendCash(Money amount)
  {
    std::unique_lock lock(m_mutex);
    if (m_cash < amount)
    {
      return false;
    }

    m_cash -= amount;
    return true;
  }

  void AddCash(Money amount)
  {
    std::unique_lock lock(m_mutex);
    m_cash += amount;
  }

private:
  Money m_cash;
  mutable std::shared_mutex m_mutex;
  Characters &m_characters;
};

class CharacterWithCard : public Character
{
public:
  CharacterWithCard(Money cash,
                    AccountId accountId,
                    Characters &characters,
                    Bank &bank)
      : Character(cash, characters), m_accountId(accountId), m_bank(bank)
  {
    DepositMoney(GetCash());
  }

  bool SendMoney(AccountId dstAccountId, Money amount)
  {
    return m_bank.TrySendMoney(m_accountId, dstAccountId, amount);
  }

  bool StealMoney(AccountId thief, Money amount)
  {
    std::unique_lock lock(m_mutex);
    return m_bank.TrySendMoney(m_accountId, thief, amount);
  }

  AccountId GetAccountId() const
  {
    std::shared_lock lock(m_mutex);
    return m_accountId;
  }

  void CloseAccount() { AddCash(m_bank.CloseAccount(m_accountId)); }

  void OpenAccount()
  {
    std::unique_lock lock(m_mutex);
    m_accountId = m_bank.OpenAccount();
  }

  bool DepositMoney(Money amount)
  {
    std::unique_lock lock(m_mutex);
    const auto success = SpendCash(amount);
    if (success)
    {
      m_bank.DepositMoney(m_accountId, amount);
    }

    return success;
  }

  bool WithdrawMoney(Money amount)
  {
    const auto success = m_bank.TryWithdrawMoney(m_accountId, amount);
    if (success)
    {
      AddCash(amount);
    }
    return success;
  }

  Money GetAccountBalance() const
  {
    return m_bank.GetAccountBalance(m_accountId);
  }

private:
  mutable std::shared_mutex m_mutex;
  AccountId m_accountId;
  Bank &m_bank;
};