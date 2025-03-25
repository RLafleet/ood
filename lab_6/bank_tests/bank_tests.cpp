#define CATCH_CONFIG_MAIN
#include <sstream>
#include <vector>
#include "../../Catch2/catch.hpp"
#include "../bank/Bank.h"

TEST_CASE("Bank initialization", "[bank]") {
  SECTION("Valid initialization") {
    REQUIRE_NOTHROW(Bank(1000));
  }
  SECTION("Negative cash initialization throws error") {
    REQUIRE_THROWS_AS(Bank(-100), BankOperationError);
  }
}

TEST_CASE("Account operations", "[bank]") {
  Bank bank(1000);

  SECTION("Open account") {
    AccountId id = bank.OpenAccount();
    REQUIRE(bank.GetAccountBalance(id) == 0);
  }

  SECTION("Close account with balance") {
    AccountId id = bank.OpenAccount();
    bank.DepositMoney(id, 500);
    REQUIRE(bank.CloseAccount(id) == 500);
    REQUIRE(bank.GetCash() == 1000 + 500);
  }
}

TEST_CASE("Money transfers", "[bank]") {
  Bank bank(1000);
  AccountId acc1 = bank.OpenAccount();
  AccountId acc2 = bank.OpenAccount();
  bank.DepositMoney(acc1, 500);

  SECTION("Valid transfer") {
    REQUIRE_NOTHROW(bank.SendMoney(acc1, acc2, 200));
    REQUIRE(bank.GetAccountBalance(acc1) == 300);
    REQUIRE(bank.GetAccountBalance(acc2) == 200);
  }

  SECTION("Insufficient funds throws error") {
    REQUIRE_THROWS_AS(bank.SendMoney(acc1, acc2, 600), BankOperationError);
  }

  SECTION("Negative amount throws out_of_range") {
    REQUIRE_THROWS_AS(bank.SendMoney(acc1, acc2, -50), std::out_of_range);
  }
}

TEST_CASE("Withdraw and deposit", "[bank]") {
  Bank bank(1000);
  AccountId acc = bank.OpenAccount();
  bank.DepositMoney(acc, 500);

  SECTION("Valid withdraw") {
    REQUIRE_NOTHROW(bank.WithdrawMoney(acc, 200));
    REQUIRE(bank.GetAccountBalance(acc) == 300);
    REQUIRE(bank.GetCash() == 1200);
  }

  SECTION("Insufficient funds throws error") {
    REQUIRE_THROWS_AS(bank.WithdrawMoney(acc, 600), BankOperationError);
  }

  SECTION("Negative withdraw throws out_of_range") {
    REQUIRE_THROWS_AS(bank.WithdrawMoney(acc, -100), std::out_of_range);
  }
}

TEST_CASE("Multiple accounts and transactions", "[bank]") {
  Bank bank(2000);
  AccountId acc1 = bank.OpenAccount();
  AccountId acc2 = bank.OpenAccount();
  AccountId acc3 = bank.OpenAccount();
  bank.DepositMoney(acc1, 500);
  bank.DepositMoney(acc2, 300);

  SECTION("Multiple valid transfers") {
    REQUIRE_NOTHROW(bank.SendMoney(acc1, acc2, 200));
    REQUIRE_NOTHROW(bank.SendMoney(acc2, acc3, 100));
    REQUIRE(bank.GetAccountBalance(acc1) == 300);
    REQUIRE(bank.GetAccountBalance(acc2) == 200);
    REQUIRE(bank.GetAccountBalance(acc3) == 100);
  }
}

TEST_CASE("Account closure edge cases", "[bank]") {
  Bank bank(1000);
  AccountId acc = bank.OpenAccount();
  REQUIRE_THROWS_AS(bank.CloseAccount(acc + 1), BankOperationError);
}

TEST_CASE("TrySendMoney failure", "[bank]") {
  Bank bank(1000);
  AccountId acc1 = bank.OpenAccount();
  AccountId acc2 = bank.OpenAccount();
  REQUIRE_FALSE(bank.TrySendMoney(acc1, acc2, 100));
}

TEST_CASE("TryWithdrawMoney failure", "[bank]") {
  Bank bank(1000);
  AccountId acc = bank.OpenAccount();
  REQUIRE_FALSE(bank.TryWithdrawMoney(acc, 100));
}

TEST_CASE("Deposit with insufficient cash", "[bank]") {
  Bank bank(1000);
  AccountId acc = bank.OpenAccount();
  REQUIRE_THROWS_AS(bank.DepositMoney(acc, 1100), BankOperationError);
}

TEST_CASE("Check operation count", "[bank]") {
  Bank bank(1000);
  AccountId acc1 = bank.OpenAccount();
  AccountId acc2 = bank.OpenAccount();
  bank.DepositMoney(acc1, 500);
  bank.SendMoney(acc1, acc2, 200);
  REQUIRE(bank.GetOperationsCount() >= 4);
}

TEST_CASE("Get balance of nonexistent account", "[bank]") {
  Bank bank(1000);
  REQUIRE_THROWS_AS(bank.GetAccountBalance(9999), BankOperationError);
}
