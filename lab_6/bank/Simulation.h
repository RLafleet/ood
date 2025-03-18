#include "Bank.h"
#include <conio.h>
#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class Simulation {
 public:
  Simulation() : bank_(10000) {
    InitializeAccounts();
    InitializeCharacters();
    SetupInitialBalances();
  }

  void Run(bool multithreaded) {
    if (multithreaded) {
      RunParallel();
    } else {
      RunSequential();
    }
    PrintFinalSummary();
  }

  static void SignalHandler(int) {
    simulation_running_.store(false, std::memory_order_relaxed);
  }

 private:
  Bank bank_;
  std::mutex output_mutex_;
  static std::atomic<bool> simulation_running_;

  struct {
    AccountId homer;
    AccountId marge;
    AccountId bart;
    AccountId lisa;
    AccountId apu;
    AccountId burns;
  } accounts_;

  struct {
    Money transfer = 60;
    Money electric = 51;
    Money grocery = 50;
    Money salary = 1000;
  } amounts_;

  class Character {
   public:
    Character(Bank& bank, AccountId account, Simulation& sim)
        : bank_(bank), account_(account), sim_(sim) {}
    virtual ~Character() = default;
    virtual void Run() = 0;

   protected:
    Bank& bank_;
    AccountId account_;
    Simulation& sim_;

    void OutputStatus(const std::string& name) {
      std::lock_guard<std::mutex> lock(sim_.output_mutex_);
      std::cout << name << " status:" << std::endl;
      std::cout << "  Account balance: " << bank_.GetAccountBalance(account_)
                << std::endl;
      std::cout << "  Total cash in the bank: " << bank_.GetCash() << std::endl
                << std::endl;
    }

    void PassCashTo(Character& other, Money amount) {
      if (amount <= 0)
        return;
      if (bank_.TryWithdrawMoney(account_, amount)) {
        other.ReceiveCash(amount);
        std::lock_guard<std::mutex> lock(sim_.output_mutex_);
        std::cout << "Passed $" << amount << " from " << account_ << " to "
                  << other.account_ << std::endl;
      }
    }

    virtual void ReceiveCash(Money amount) {
      if (amount > 0) {
        bank_.DepositMoney(account_, amount);
        std::lock_guard<std::mutex> lock(sim_.output_mutex_);
        std::cout << account_ << " received $" << amount << std::endl;
      }
    }
  };

  class Homer : public Character {
   public:
    Homer(Bank& bank, AccountId account, Simulation& sim)
        : Character(bank, account, sim) {}

    void Run() override {
      bank_.TrySendMoney(account_, sim_.accounts_.marge,
                         sim_.amounts_.transfer);
      bank_.TrySendMoney(account_, sim_.accounts_.burns,
                         sim_.amounts_.electric);
      OutputStatus("Homer");
    }
  };

  class Marge : public Character {
   public:
    Marge(Bank& bank, AccountId account, Simulation& sim)
        : Character(bank, account, sim) {}

    void Run() override {
      bank_.TrySendMoney(account_, sim_.accounts_.apu, sim_.amounts_.grocery);
      OutputStatus("Marge");
    }
  };

  class BartLisa : public Character {
   public:
    BartLisa(Bank& bank, AccountId account, Simulation& sim)
        : Character(bank, account, sim) {}

    void Run() override { OutputStatus("Bart & Lisa"); }

    void ReceiveCash(Money amount) override {
      if (amount > 0) {
        bank_.DepositMoney(account_, amount);
        std::lock_guard<std::mutex> lock(sim_.output_mutex_);
        std::cout << account_ << " received $" << amount << std::endl;
      }
    }
  };

  class Apu : public Character {
   public:
    Apu(Bank& bank, AccountId account, Simulation& sim)
        : Character(bank, account, sim) {}

    void Run() override {
      bank_.TrySendMoney(account_, sim_.accounts_.burns,
                         sim_.amounts_.electric);
      OutputStatus("Apu");
    }
  };

  class Burns : public Character {
   public:
    Burns(Bank& bank, AccountId account, Simulation& sim)
        : Character(bank, account, sim) {}

    void Run() override {
      bank_.TrySendMoney(account_, sim_.accounts_.homer, sim_.amounts_.salary);
      OutputStatus("Mr. Burns");
    }
  };

  std::unique_ptr<Homer> homer_;
  std::unique_ptr<Marge> marge_;
  std::unique_ptr<BartLisa> bart_lisa_;
  std::unique_ptr<Apu> apu_;
  std::unique_ptr<Burns> burns_;

  void InitializeAccounts() {
    accounts_.homer = bank_.OpenAccount();
    accounts_.marge = bank_.OpenAccount();
    accounts_.bart = bank_.OpenAccount();
    accounts_.lisa = bank_.OpenAccount();
    accounts_.apu = bank_.OpenAccount();
    accounts_.burns = bank_.OpenAccount();
  }

  void InitializeCharacters() {
    homer_ = std::make_unique<Homer>(bank_, accounts_.homer, *this);
    marge_ = std::make_unique<Marge>(bank_, accounts_.marge, *this);
    bart_lisa_ = std::make_unique<BartLisa>(bank_, accounts_.bart, *this);
    apu_ = std::make_unique<Apu>(bank_, accounts_.apu, *this);
    burns_ = std::make_unique<Burns>(bank_, accounts_.burns, *this);
  }

  void SetupInitialBalances() {
    bank_.DepositMoney(accounts_.homer, 1000);
    bank_.DepositMoney(accounts_.burns, 1100);
    bank_.DepositMoney(accounts_.apu, 1700);
  }

  void RunParallel() {
    std::vector<std::jthread> threads;
    while (simulation_running_) {
      WaitForUserInput();
      if (!simulation_running_)
        break;

      threads.emplace_back([this] { homer_->Run(); });
      threads.emplace_back([this] { marge_->Run(); });
      threads.emplace_back([this] { bart_lisa_->Run(); });
      threads.emplace_back([this] { apu_->Run(); });
      threads.emplace_back([this] { burns_->Run(); });
    }
  }

  void RunSequential() {
    while (simulation_running_) {
      WaitForUserInput();
      if (!simulation_running_)
        break;

      homer_->Run();
      marge_->Run();
      bart_lisa_->Run();
      apu_->Run();
      burns_->Run();
    }
  }

  void WaitForUserInput() {
    {
      std::lock_guard<std::mutex> lock(output_mutex_);
      std::cout << "Press SPACE to progress simulation, or Q to quit.\n";
    }
    char key;
    do {
      key = _getch();
    } while (key != ' ' && key != 'q');

    if (key == 'q') {
      simulation_running_ = false;
    }
  }

  void PrintFinalSummary() {
    std::lock_guard<std::mutex> lock(output_mutex_);
    const Money total = bank_.GetCash() +
                        bank_.GetAccountBalance(accounts_.homer) +
                        bank_.GetAccountBalance(accounts_.marge) +
                        bank_.GetAccountBalance(accounts_.bart) +
                        bank_.GetAccountBalance(accounts_.lisa) +
                        bank_.GetAccountBalance(accounts_.apu) +
                        bank_.GetAccountBalance(accounts_.burns);

    std::cout << "\nFinal banking summary:\n";
    std::cout << "Total cash in the system: " << total << "\n";
    std::cout << "Detailed balances:\n";
    std::cout << "- Homer:  " << bank_.GetAccountBalance(accounts_.homer)
              << "\n";
    std::cout << "- Marge:  " << bank_.GetAccountBalance(accounts_.marge)
              << "\n";
    std::cout << "- Bart:   " << bank_.GetAccountBalance(accounts_.bart)
              << "\n";
    std::cout << "- Lisa:   " << bank_.GetAccountBalance(accounts_.lisa)
              << "\n";
    std::cout << "- Apu:    " << bank_.GetAccountBalance(accounts_.apu) << "\n";
    std::cout << "- Burns:  " << bank_.GetAccountBalance(accounts_.burns)
              << "\n";
  }
};