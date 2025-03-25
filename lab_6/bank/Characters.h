#pragma once
#include "CharactersBase.h"

#include <thread>

class Homer final : public CharacterWithCard
{
public:
  Homer(Money cash,
        AccountId accountId,
        Characters &characters,
        Bank &bank)
      : CharacterWithCard(cash, accountId, characters, bank) {}

  void Next(const Characters &characters) override
  {
    SendMoney(characters.marge->GetAccountId(), 500);

    SendMoney(characters.burns->GetAccountId(), 200);

    WithdrawMoney(100);
    TransferCash(*characters.bart, 50);
    TransferCash(*characters.lisa, 50);
  }
};

class Marge final : public CharacterWithCard
{
public:
  Marge(Money cash,
        AccountId accountId,
        Characters &characters,
        Bank &bank)
      : CharacterWithCard(cash, accountId, characters, bank) {}

  void Next(const Characters &characters) override
  {
    SendMoney(characters.apu->GetAccountId(), 300);
  }
};

class Bart final : public Character
{
public:
  Bart(Money cash, Characters &characters)
      : Character(cash, characters) {}

  void Next(const Characters &characters) override
  {
    TransferCash(*characters.apu, 10);
  }
};

class Lisa final : public Character
{
public:
  Lisa(Money cash, Characters &characters)
      : Character(cash, characters) {}

  void Next(const Characters &characters) override
  {
    TransferCash(*characters.apu, 10);
  }
};

class Apu final : public CharacterWithCard
{
public:
  Apu(Money cash,
      AccountId accountId,
      Characters &characters,
      Bank &bank)
      : CharacterWithCard(cash, accountId, characters, bank) {}

  void Next(const Characters &characters) override
  {
    DepositMoney(GetCash());

    SendMoney(characters.burns->GetAccountId(), 100);
  }
};

class Burns final : public CharacterWithCard
{
public:
  Burns(Money cash,
        AccountId accountId,
        Characters &characters,
        Bank &bank)
      : CharacterWithCard(cash, accountId, characters, bank) {}

  void Next(const Characters &characters) override
  {
    SendMoney(characters.homer->GetAccountId(), 1000);

    try
    {
      SendMoney(characters.smithers->GetAccountId(), 500);
    }
    catch (BankOperationError const &)
    {
    }
  }
};

class Nelson final : public Character
{
public:
  Nelson(Money cash, Characters &characters)
      : Character(cash, characters) {}

  void Next(const Characters &characters) override
  {
    characters.bart->StealCache(*this, 20);

    TransferCash(*characters.apu, 10);
  }
};

class Snake final : public CharacterWithCard
{
public:
  Snake(Money cash,
        AccountId accountId,
        Characters &characters,
        Bank &bank)
      : CharacterWithCard(cash, accountId, characters, bank) {}

  void Next(const Characters &characters) override
  {
    characters.homer->StealMoney(GetAccountId(), 100);

    SendMoney(characters.apu->GetAccountId(), 50);
  }
};

class Smithers final : public CharacterWithCard
{
public:
  Smithers(Money cash,
           AccountId accountId,
           Characters &characters,
           Bank &bank)
      : CharacterWithCard(cash, accountId, characters, bank) {}

  void Next(const Characters &characters) override
  {
    SendMoney(characters.apu->GetAccountId(), 200);

    CloseAccount();
    OpenAccount();
  }
};