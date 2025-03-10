#define CATCH_CONFIG_MAIN
#include <sstream>
#include <vector>
#include "../../Catch2/catch.hpp"
#include "../bank/Bank.h"

TEST_CASE("Bank initialization", "[Bank]") {
  SECTION("Initialization with positive cash amount") {
    Bank bank(1000);
    REQUIRE(bank.GetCash() == 1000);
  }

  SECTION(
      "Initialization with negative cash amount throws BankOperationError") {
    REQUIRE_THROWS_AS(Bank(-1000), BankOperationError);
  }
}

TEST_CASE("Bank operations count", "[Bank]") {
  Bank bank(1000);
  REQUIRE(bank.GetOperationsCount() == 0);

  SECTION("Operations count increases after operations") {
    AccountId account1 = bank.OpenAccount();
    AccountId account2 = bank.OpenAccount();
    bank.DepositMoney(account1, 100);
    bank.SendMoney(account1, account2, 100);
    REQUIRE(bank.GetOperationsCount() > 0);
  }
}

TEST_CASE("Open and close account", "[Bank]") {
  Bank bank(1000);

  SECTION("Open account returns unique account ID") {
    AccountId account1 = bank.OpenAccount();
    AccountId account2 = bank.OpenAccount();
    REQUIRE(account1 != account2);
  }

  SECTION("Close account returns balance and adds to cash") {
    AccountId account = bank.OpenAccount();
    bank.DepositMoney(account, 500);
    REQUIRE(bank.GetCash() == 500);
    Money balance = bank.CloseAccount(account);
    REQUIRE(balance == 500);
    REQUIRE(bank.GetCash() == 1000);
  }

  SECTION("Close non-existent account throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.CloseAccount(999), BankOperationError);
  }
}

TEST_CASE("Send money", "[Bank]") {
  Bank bank(1000);
  AccountId account1 = bank.OpenAccount();
  AccountId account2 = bank.OpenAccount();
  bank.DepositMoney(account1, 500);

  SECTION("Send money between accounts") {
    bank.SendMoney(account1, account2, 300);
    REQUIRE(bank.GetAccountBalance(account1) == 200);
    REQUIRE(bank.GetAccountBalance(account2) == 300);
  }

  SECTION("Send more money than available throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.SendMoney(account1, account2, 600),
                      BankOperationError);
  }

  SECTION("Send negative amount throws std::out_of_range") {
    REQUIRE_THROWS_AS(bank.SendMoney(account1, account2, -100),
                      std::out_of_range);
  }

  SECTION("Send money to non-existent account throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.SendMoney(account1, 999, 100), BankOperationError);
  }
}

TEST_CASE("Try send money", "[Bank]") {
  Bank bank(1000);
  AccountId account1 = bank.OpenAccount();
  AccountId account2 = bank.OpenAccount();
  bank.DepositMoney(account1, 500);

  SECTION("Try send money between accounts") {
    REQUIRE(bank.TrySendMoney(account1, account2, 300) == true);
    REQUIRE(bank.GetAccountBalance(account1) == 200);
    REQUIRE(bank.GetAccountBalance(account2) == 300);
  }

  SECTION("Try send more money than available returns false") {
    REQUIRE(bank.TrySendMoney(account1, account2, 600) == false);
  }

  SECTION("Try send negative amount throws std::out_of_range") {
    REQUIRE_THROWS_AS(bank.TrySendMoney(account1, account2, -100),
                      std::out_of_range);
  }

  SECTION("Try send money to non-existent account throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.TrySendMoney(account1, 999, 100),
                      BankOperationError);
  }
}

TEST_CASE("Withdraw money", "[Bank]") {
  Bank bank(1000);
  AccountId account = bank.OpenAccount();
  bank.DepositMoney(account, 500);

  SECTION("Withdraw money from account") {
    bank.WithdrawMoney(account, 300);
    REQUIRE(bank.GetAccountBalance(account) == 200);
    REQUIRE(bank.GetCash() == 800);
  }

  SECTION("Withdraw more money than available throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.WithdrawMoney(account, 600), BankOperationError);
  }

  SECTION("Withdraw negative amount throws std::out_of_range") {
    REQUIRE_THROWS_AS(bank.WithdrawMoney(account, -100), std::out_of_range);
  }

  SECTION("Withdraw from non-existent account throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.WithdrawMoney(999, 100), BankOperationError);
  }
}

TEST_CASE("Try withdraw money", "[Bank]") {
  Bank bank(1000);
  AccountId account = bank.OpenAccount();
  bank.DepositMoney(account, 500);

  SECTION("Try withdraw money from account") {
    REQUIRE(bank.TryWithdrawMoney(account, 300) == true);
    REQUIRE(bank.GetAccountBalance(account) == 200);
    REQUIRE(bank.GetCash() == 800);
  }

  SECTION("Try withdraw more money than available returns false") {
    REQUIRE(bank.TryWithdrawMoney(account, 600) == false);
    REQUIRE(bank.GetCash() == 500);
  }

  SECTION("Try withdraw negative amount throws std::out_of_range") {
    REQUIRE_THROWS_AS(bank.TryWithdrawMoney(account, -100), std::out_of_range);
  }

  SECTION("Try withdraw from non-existent account throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.TryWithdrawMoney(999, 100), BankOperationError);
  }
}

TEST_CASE("Deposit money", "[Bank]") {
  Bank bank(1000);
  AccountId account = bank.OpenAccount();

  SECTION("Deposit money to account") {
    bank.DepositMoney(account, 300);
    REQUIRE(bank.GetAccountBalance(account) == 300);
    REQUIRE(bank.GetCash() == 700);
    REQUIRE(bank.TryWithdrawMoney(account, 300) == true);
    REQUIRE(bank.GetCash() == 1000);
  }

  SECTION(
      "Deposit more money than available in cash throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.DepositMoney(account, 10100), BankOperationError);
  }

  SECTION("Deposit negative amount throws std::out_of_range") {
    REQUIRE_THROWS_AS(bank.DepositMoney(account, -100), std::out_of_range);
  }

  SECTION("Deposit to non-existent account throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.DepositMoney(999, 100), BankOperationError);
  }
}

TEST_CASE("Get account balance", "[Bank]") {
  Bank bank(1000);
  AccountId account = bank.OpenAccount();
  bank.DepositMoney(account, 500);

  SECTION("Get balance of existing account") {
    REQUIRE(bank.GetAccountBalance(account) == 500);
  }

  SECTION("Get balance of non-existent account throws BankOperationError") {
    REQUIRE_THROWS_AS(bank.GetAccountBalance(999), BankOperationError);
  }
}