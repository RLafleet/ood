#include "Bank.h"
#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<bool> simulation_running(true);
AccountId marge_account, homer_account, bart_account, lisa_account, apu_account,
    burns_account;
Money transfer_amount = 100;
Money electric_payment = 50;
Money cash_amount = 200;
Money grocery_amount = 150;
Money salary_amount = 1000;
Money cash = 0;

class Character {
 protected:
  Bank& bank;
  AccountId account;
  std::chrono::milliseconds delay{100};
  std::chrono::steady_clock::time_point last_output_time;

 public:
  Character(Bank& b, AccountId acc)
      : bank(b),
        account(acc),
        last_output_time(std::chrono::steady_clock::now()) {}
  virtual ~Character() = default;
  virtual void run() = 0;

  // Метод для вывода состояния
  void outputStatus(const std::string& name) {
    auto current_time = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(current_time -
                                                         last_output_time)
            .count() >= 2) {
      last_output_time = current_time;
      std::cout << name << " status:" << std::endl;
      std::cout << "  Account balance: " << bank.GetAccountBalance(account)
                << std::endl;
      std::cout << "  Total cash in the bank: " << bank.GetCash() << std::endl;
      std::cout << std::endl;
    }
  }
};

class Homer : public Character {
 public:
  using Character::Character;
  void run() override {
    while (simulation_running) {
      bank.TrySendMoney(account, marge_account, transfer_amount);
      bank.TrySendMoney(account, burns_account, electric_payment);
      if (bank.TryWithdrawMoney(account, cash_amount)) {
        bank.DepositMoney(bart_account,
                          cash_amount / 2);  // bart needs to be defined
        bank.DepositMoney(lisa_account,
                          cash_amount / 2);  // lisa needs to be defined
      }
      outputStatus("Homer");  // Выводим статус каждые 2 секунды
      std::this_thread::sleep_for(delay);
    }
  }
};

class Marge : public Character {
 public:
  using Character::Character;
  void run() override {
    while (simulation_running) {
      bank.TrySendMoney(account, apu_account, grocery_amount);
      outputStatus("Marge");  // Выводим статус каждые 2 секунды
      std::this_thread::sleep_for(delay);
    }
  }
};

class Apu : public Character {
 public:
  using Character::Character;
  void run() override {
    while (simulation_running) {
      bank.TrySendMoney(account, burns_account, electric_payment);
      if (cash > 0) {
        bank.DepositMoney(account, cash);
        cash = 0;
      }
      outputStatus("Apu");  // Выводим статус каждые 2 секунды
      std::this_thread::sleep_for(delay);
    }
  }
};

class Burns : public Character {
 public:
  using Character::Character;
  void run() override {
    while (simulation_running) {
      bank.TrySendMoney(account, homer_account, salary_amount);
      outputStatus("Burns");  // Выводим статус каждые 2 секунды
      std::this_thread::sleep_for(delay);
    }
  }
};

void signalHandler(int) {
  simulation_running.store(false, std::memory_order_relaxed);
}

int main(int argc, char* argv[]) {
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  Bank bank(1000000);

  // Open accounts for all characters
  homer_account = bank.OpenAccount();
  marge_account = bank.OpenAccount();
  bart_account = bank.OpenAccount();
  lisa_account = bank.OpenAccount();
  apu_account = bank.OpenAccount();
  burns_account = bank.OpenAccount();

  // Create character objects
  Homer homer(bank, homer_account);
  Marge marge(bank, marge_account);
  Apu apu(bank, apu_account);
  Burns burns(bank, burns_account);

  // Create a thread for each character's run method
  std::vector<std::thread> threads;
  threads.emplace_back(&Homer::run, &homer);
  threads.emplace_back(&Marge::run, &marge);
  threads.emplace_back(&Apu::run, &apu);
  threads.emplace_back(&Burns::run, &burns);

  // Join threads
  for (auto& thread : threads) {
    thread.join();
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
