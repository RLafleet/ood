#include "Bank.h"
#include <conio.h>
#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<bool> simulation_running(true);
AccountId marge_account, homer_account, bart_account, lisa_account, apu_account,
    burns_account;
Money transfer_amount = 60;
Money electric_payment = 40;
Money cash_amount = 20;
Money grocery_amount = 50;
Money salary_amount = 1000;
Money cash = 0;

class Character {
 protected:
  Bank& bank;
  AccountId account;

 public:
  Character(Bank& b, AccountId acc) : bank(b), account(acc) {}
  virtual ~Character() = default;
  virtual void run() = 0;

  void outputStatus(const std::string& name) {
    std::cout << name << " status:" << std::endl;
    std::cout << "  Account balance: " << bank.GetAccountBalance(account)
              << std::endl;
    std::cout << "  Total cash in the bank: " << bank.GetCash() << std::endl;
    std::cout << "  Cash in hand: " << cash << std::endl;
    std::cout << std::endl;
  }
};

class Homer : public Character {
 public:
  using Character::Character;
  void run() override {
    bank.TrySendMoney(account, marge_account, transfer_amount);
    bank.TrySendMoney(account, burns_account, electric_payment);
    if (bank.TryWithdrawMoney(account, cash_amount)) {
      cash += cash_amount;
    }
    outputStatus("Homer");
  }
};

class Marge : public Character {
 public:
  using Character::Character;
  void run() override {
    bank.TrySendMoney(account, apu_account, grocery_amount);
    outputStatus("Marge");
  }
};

class BartLisa : public Character {
  Money& cashRef;

 public:
  BartLisa(Bank& b, AccountId acc, Money& cash)
      : Character(b, acc), cashRef(cash) {}
  void run() override {
    if (cashRef >= 10) {
      cashRef -= 100;
      cash += 100;
    }
    outputStatus("BartLisa");
  }
};

class Apu : public Character {
 public:
  using Character::Character;
  void run() override {
    bank.TrySendMoney(account, burns_account, electric_payment);
    if (cash > 0) { //Bart and Lisa bought products
      bank.DepositMoney(account, cash);
      cash = 0;
    }
    outputStatus("Apu");
  }
};

class Burns : public Character {
 public:
  using Character::Character;
  void run() override {
    bank.TrySendMoney(account, homer_account, salary_amount);
    outputStatus("Burns");
  }
};

void signalHandler(int) {
  simulation_running.store(false, std::memory_order_relaxed);
}

int main(int argc, char* argv[]) {
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  bool multithreaded = (argc > 1 && std::string(argv[1]) == "parallel");
  Bank bank(10000);

  homer_account = bank.OpenAccount();
  marge_account = bank.OpenAccount();
  bart_account = bank.OpenAccount();
  lisa_account = bank.OpenAccount();
  apu_account = bank.OpenAccount();
  burns_account = bank.OpenAccount();

  Homer homer(bank, homer_account);
  Marge marge(bank, marge_account);
  BartLisa bartlisa(bank, bart_account, cash);
  Apu apu(bank, apu_account);
  Burns burns(bank, burns_account);

  bank.DepositMoney(homer_account, 1000);
  bank.DepositMoney(burns_account, 1100);
  bank.DepositMoney(apu_account, 1700);

  if (multithreaded) {
    std::vector<std::thread> threads;
    threads.emplace_back(&Homer::run, &homer);
    threads.emplace_back(&Marge::run, &marge);
    threads.emplace_back(&BartLisa::run, &bartlisa);
    threads.emplace_back(&Apu::run, &apu);
    threads.emplace_back(&Burns::run, &burns);
    for (auto& thread : threads) {
      thread.join();
    }
  } else {
    while (simulation_running) {
      std::cout << "Press SPACE to progress simulation, or Q to quit."
                << std::endl;
      char key;
      do {
        key = _getch();
      } while (key != ' ' && key != 'q');
      if (key == 'q')
        break;
      homer.run();
      marge.run();
      bartlisa.run();
      apu.run();
      burns.run();
    }
  }

  Money totalCash = bank.GetCash() + bank.GetAccountBalance(homer_account) +
                    bank.GetAccountBalance(marge_account) +
                    bank.GetAccountBalance(bart_account) +
                    bank.GetAccountBalance(lisa_account) +
                    bank.GetAccountBalance(apu_account) +
                    bank.GetAccountBalance(burns_account); 

  std::cout << "Total cash in the bank: " << totalCash << std::endl;
  return 0;
}