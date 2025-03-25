#pragma once
#include <conio.h>
#include <atomic>
#include <csignal>
#include <iomanip>
#include <memory>
#include <vector>
#include "Bank.h"
#include "Characters.h"

inline std::atomic<bool> stopProgram = false;
inline std::atomic<bool> next = false;

inline void SignalHandler(int signum)
{
  stopProgram.store(true);
}

class Simulation
{
public:
  Simulation(bool multiThreaded)
      : m_multiThreaded(multiThreaded)
  {
    std::signal(SIGTERM, SignalHandler);

    m_bank = std::make_unique<Bank>(m_initialCash);
    InitializeAccounts();
    InitializeCharacters();
  }

  void Start()
  {
    PrintWelcomeMessage();

    if (m_multiThreaded)
    {
      RunParallel();
    }
    else
    {
      RunSequential();
    }

    PrintFinalSummary();
  }

private:
  void RunParallel()
  {
    std::vector<std::jthread> threads;
    threads.emplace_back(&Homer::Run, m_homer.get(), std::ref(stopProgram));
    threads.emplace_back(&Marge::Run, m_marge.get(), std::ref(stopProgram));
    threads.emplace_back(&Bart::Run, m_bart.get(), std::ref(stopProgram));
    threads.emplace_back(&Lisa::Run, m_lisa.get(), std::ref(stopProgram));
    threads.emplace_back(&Apu::Run, m_apu.get(), std::ref(stopProgram));
    threads.emplace_back(&Burns::Run, m_burns.get(), std::ref(stopProgram));
    threads.emplace_back(&Nelson::Run, m_nelson.get(), std::ref(stopProgram));
    threads.emplace_back(&Snake::Run, m_snake.get(), std::ref(stopProgram));
    threads.emplace_back(&Smithers::Run, m_smithers.get(), std::ref(stopProgram));

    threads.emplace_back([this]
                         {
      while (!stopProgram.load()) {
        WaitForUserInput();
        std::osyncstream(std::cout) << "\n=== SYSTEM STATUS ===\n";
        std::osyncstream(std::cout)
            << "Bank cash: $" << m_bank->GetCash() << "\n";
        std::osyncstream(std::cout)
            << "Total operations: " << m_bank->GetOperationsCount() << "\n\n";
      } });
  }

  void RunSequential()
  {
    while (!stopProgram.load())
    {
      WaitForUserInput();
      if (stopProgram.load())
        break;

      ClearScreen();

      m_homer->Next(m_characters);
      m_marge->Next(m_characters);
      m_bart->Next(m_characters);
      m_lisa->Next(m_characters);
      m_apu->Next(m_characters);
      m_burns->Next(m_characters);
      m_nelson->Next(m_characters);
      m_snake->Next(m_characters);
      m_smithers->Next(m_characters);

      PrintCurrentState();
    }
  }

  void PrintWelcomeMessage()
  {
    ClearScreen();
    std::cout << "============================================\n";
    std::cout << "    SPRINGFIELD BANKING SIMULATION\n";
    std::cout << "============================================\n\n";
    std::cout << "Initial balances:\n";
    std::cout << "- Homer:    $10,000\n";
    std::cout << "- Marge:    $100\n";
    std::cout << "- Bart:     $100 (cash)\n";
    std::cout << "- Lisa:     $100 (cash)\n";
    std::cout << "- Apu:      $200\n";
    std::cout << "- Burns:    $100,000\n";
    std::cout << "- Nelson:   $0\n";
    std::cout << "- Snake:    $0\n";
    std::cout << "- Smithers: $0\n\n";
    std::cout << "Press SPACE to start simulation, Q to quit...\n";

    char key;
    do
    {
      key = _getch();
    } while (key != ' ' && key != 'q');

    if (key == 'q')
    {
      stopProgram.store(true);
    }
  }

  void WaitForUserInput()
  {
    std::cout
        << "\nPress SPACE for next Next, C for continuous, Q to quit...\n";
    char key;
    do
    {
      key = _getch();
    } while (key != ' ' && key != 'q' && key != 'c');

    if (key == 'q')
    {
      stopProgram.store(true);
    }
    else if (key == 'c')
    {
      std::cout << "\nRunning continuously, press any key to stop...\n";
      while (!_kbhit() && !stopProgram.load())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
      if (_kbhit())
        _getch();
    }
  }

  void ClearScreen() { system("cls||clear"); }

  void PrintCurrentState()
  {
    std::cout << "============================================\n";
    std::cout << "    CURRENT STATE OF SPRINGFIELD BANK\n";
    std::cout << "============================================\n\n";

    std::cout << "Bank cash: $" << std::setw(9) << m_bank->GetCash() << "\n";
    std::cout << "Bank operations: " << m_bank->GetOperationsCount() << "\n\n";

    std::cout << "ACCOUNT BALANCES:\n";
    std::cout << "- Homer:    $" << std::setw(9) << m_homer->GetAccountBalance()
              << "\n";
    std::cout << "- Marge:    $" << std::setw(9) << m_marge->GetAccountBalance()
              << "\n";
    std::cout << "- Apu:      $" << std::setw(9) << m_apu->GetAccountBalance()
              << "\n";
    std::cout << "- Burns:    $" << std::setw(9) << m_burns->GetAccountBalance()
              << "\n";
    std::cout << "- Snake:    $" << std::setw(9) << m_snake->GetAccountBalance()
              << "\n";
    std::cout << "- Smithers: $" << std::setw(9)
              << m_smithers->GetAccountBalance() << "\n\n";

    std::cout << "CASH HOLDINGS:\n";
    std::cout << "- Bart:     $" << std::setw(9) << m_bart->GetCash() << "\n";
    std::cout << "- Lisa:     $" << std::setw(9) << m_lisa->GetCash() << "\n";
    std::cout << "- Nelson:   $" << std::setw(9) << m_nelson->GetCash()
              << "\n\n";

    std::cout << "Consistency check: " << (IsConsistent() ? "PASSED" : "FAILED")
              << "\n";
    std::cout << "============================================\n";
  }

  void PrintFinalSummary()
  {
    ClearScreen();
    std::cout << "============================================\n";
    std::cout << "    FINAL SIMULATION SUMMARY\n";
    std::cout << "============================================\n\n";

    std::cout << "Total bank operations: " << m_bank->GetOperationsCount()
              << "\n\n";

    std::cout << "FINAL ACCOUNT BALANCES:\n";
    std::cout << "- Homer:    $" << std::setw(9) << m_homer->GetAccountBalance()
              << "\n";
    std::cout << "- Marge:    $" << std::setw(9) << m_marge->GetAccountBalance()
              << "\n";
    std::cout << "- Apu:      $" << std::setw(9) << m_apu->GetAccountBalance()
              << "\n";
    std::cout << "- Burns:    $" << std::setw(9) << m_burns->GetAccountBalance()
              << "\n";
    std::cout << "- Snake:    $" << std::setw(9) << m_snake->GetAccountBalance()
              << "\n";
    std::cout << "- Smithers: $" << std::setw(9)
              << m_smithers->GetAccountBalance() << "\n\n";

    std::cout << "FINAL CASH HOLDINGS:\n";
    std::cout << "- Bart:     $" << std::setw(9) << m_bart->GetCash() << "\n";
    std::cout << "- Lisa:     $" << std::setw(9) << m_lisa->GetCash() << "\n";
    std::cout << "- Nelson:   $" << std::setw(9) << m_nelson->GetCash()
              << "\n\n";

    std::cout << "Bank cash reserves: $" << m_bank->GetCash() << "\n";
    std::cout << "Consistency check: " << (IsConsistent() ? "PASSED" : "FAILED")
              << "\n";
    std::cout << "============================================\n";
  }

  [[nodiscard]] bool IsConsistent() const
  {
    const Money bankCash = m_bank->GetCash();
    const Money totalCharacterCash =
        m_homer->GetCash() + m_marge->GetCash() + m_bart->GetCash() +
        m_lisa->GetCash() + m_apu->GetCash() + m_burns->GetCash() +
        m_nelson->GetCash() + m_snake->GetCash() + m_smithers->GetCash();

    const Money totalCharacterBalance =
        m_homer->GetAccountBalance() + m_marge->GetAccountBalance() +
        m_apu->GetAccountBalance() + m_burns->GetAccountBalance() +
        m_snake->GetAccountBalance() + m_smithers->GetAccountBalance();

    const Money totalMoney = totalCharacterBalance + totalCharacterCash;

    std::cout << "\nDEBUG INFO:\n";
    std::cout << "Bank cash: " << bankCash << "\n";
    std::cout << "Total accounts: " << totalCharacterBalance << "\n";
    std::cout << "Total character cash: " << totalCharacterCash << "\n";
    std::cout << "Initial cash: " << m_initialCash << "\n";
    std::cout << "Total money: " << totalMoney << "\n";

    return totalMoney == m_initialCash;
  }

private:
  bool m_multiThreaded;
  std::unique_ptr<Bank> m_bank;
  AccountId m_homerAccount;
  AccountId m_margeAccount;
  AccountId m_apuAccount;
  AccountId m_burnsAccount;
  AccountId m_snakeAccount;
  AccountId m_smithersAccount;

  std::unique_ptr<Homer> m_homer;
  std::unique_ptr<Marge> m_marge;
  std::unique_ptr<Bart> m_bart;
  std::unique_ptr<Lisa> m_lisa;
  std::unique_ptr<Apu> m_apu;
  std::unique_ptr<Burns> m_burns;
  std::unique_ptr<Nelson> m_nelson;
  std::unique_ptr<Snake> m_snake;
  std::unique_ptr<Smithers> m_smithers;

  Characters m_characters;
  Money m_initialCash = 110'500;

  void InitializeAccounts()
  {
    m_homerAccount = m_bank->OpenAccount();
    m_margeAccount = m_bank->OpenAccount();
    m_apuAccount = m_bank->OpenAccount();
    m_burnsAccount = m_bank->OpenAccount();
    m_snakeAccount = m_bank->OpenAccount();
    m_smithersAccount = m_bank->OpenAccount();
  }

  void InitializeCharacters()
  {
    m_homer = std::make_unique<Homer>(10'000, m_homerAccount, m_characters,
                                      *m_bank);
    m_marge = std::make_unique<Marge>(100, m_margeAccount, m_characters,
                                      *m_bank);
    m_bart = std::make_unique<Bart>(100, m_characters);
    m_lisa = std::make_unique<Lisa>(100, m_characters);
    m_apu =
        std::make_unique<Apu>(200, m_apuAccount, m_characters, *m_bank);
    m_burns = std::make_unique<Burns>(100'000, m_burnsAccount, m_characters,
                                      *m_bank);
    m_nelson = std::make_unique<Nelson>(0, m_characters);
    m_snake = std::make_unique<Snake>(0, m_snakeAccount, m_characters, *m_bank);
    m_smithers = std::make_unique<Smithers>(0, m_smithersAccount, m_characters,
                                            *m_bank);

    m_characters.homer = m_homer.get();
    m_characters.marge = m_marge.get();
    m_characters.bart = m_bart.get();
    m_characters.lisa = m_lisa.get();
    m_characters.apu = m_apu.get();
    m_characters.burns = m_burns.get();
    m_characters.nelson = m_nelson.get();
    m_characters.snake = m_snake.get();
    m_characters.smithers = m_smithers.get();
  }
};