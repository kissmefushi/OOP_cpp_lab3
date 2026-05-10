#include <iostream>
#include <string>
#include <cmath>
#include <Windows.h> 

class BankDeposit {
protected:
    std::string clientName;
    double initialAmount;
    double interestRate;

public:
    BankDeposit();
    BankDeposit(std::string name, double amount, double rate);
    BankDeposit(const BankDeposit& other); // Конструктор копіювання

    virtual ~BankDeposit();

    std::string getClientName() const;
    double getInitialAmount() const;
    double getInterestRate() const;
    virtual std::string getTypeName() const = 0;

    virtual double calculateCompoundInterest(int months) const = 0;
};

class StandardDeposit : public BankDeposit {
public:
    StandardDeposit();
    StandardDeposit(std::string name, double amount, double rate);
    StandardDeposit(const StandardDeposit& other); // Конструктор копіювання
    
    std::string getTypeName() const override;

    double calculateCompoundInterest(int months) const override;
};

class CapitalizedDeposit : public BankDeposit {
public:
    CapitalizedDeposit();
    CapitalizedDeposit(std::string name, double amount, double rate);
    CapitalizedDeposit(const CapitalizedDeposit& other); // Конструктор копіювання
    
    std::string getTypeName() const override;

    double calculateCompoundInterest(int months) const override;
};

class PremiumDeposit : public BankDeposit {
private:
    double bonusRate;
public:
    PremiumDeposit();
    PremiumDeposit(std::string name, double amount, double rate, double bonus);
    PremiumDeposit(const PremiumDeposit& other); // Конструктор копіювання

    std::string getTypeName() const override;

    double calculateCompoundInterest(int months) const override;
};

class DepositContainer {
private:
    BankDeposit** deposits;
    int capacity;
    int currentCount;

public:
    DepositContainer(int cap = 10);
    ~DepositContainer();

    void addDeposit(BankDeposit* dep);

    void printAllDeposits(int months) const;

    double calculateTotalLiabilities(int months) const;
};

BankDeposit::BankDeposit() 
    : clientName("Невідомо"), initialAmount(0.0), interestRate(0.0) {}

BankDeposit::BankDeposit(std::string name, double amount, double rate)
    : clientName(name), initialAmount(amount), interestRate(rate) {
}

BankDeposit::BankDeposit(const BankDeposit& other)
    : clientName(other.clientName), initialAmount(other.initialAmount), interestRate(other.interestRate) {
}

BankDeposit::~BankDeposit() {}

std::string BankDeposit::getClientName() const { 
    return clientName; 
}
double BankDeposit::getInitialAmount() const { 
    return initialAmount; 
}
double BankDeposit::getInterestRate() const { 
    return interestRate; 
}

StandardDeposit::StandardDeposit() : BankDeposit() {}

StandardDeposit::StandardDeposit(std::string name, double amount, double rate)
    : BankDeposit(name, amount, rate) {
}

StandardDeposit::StandardDeposit(const StandardDeposit& other) 
    : BankDeposit(other) {}

std::string StandardDeposit::getTypeName() const { 
    return "Стандартний"; 
}
double StandardDeposit::calculateCompoundInterest(int months) const {
    double years = months / 12.0;
    return initialAmount * (1.0 + (interestRate / 100.0) * years);
}

CapitalizedDeposit::CapitalizedDeposit() : BankDeposit() {}

CapitalizedDeposit::CapitalizedDeposit(std::string name, double amount, double rate)
    : BankDeposit(name, amount, rate) {
}

CapitalizedDeposit::CapitalizedDeposit(const CapitalizedDeposit& other) 
    : BankDeposit(other) {}

std::string CapitalizedDeposit::getTypeName() const { 
    return "З капіталізацією"; 
}
double CapitalizedDeposit::calculateCompoundInterest(int months) const {
    double monthlyRate = (interestRate / 100.0) / 12.0;
    return initialAmount * std::pow(1.0 + monthlyRate, months);
}

PremiumDeposit::PremiumDeposit() 
    : BankDeposit(), bonusRate(0.0) {}

PremiumDeposit::PremiumDeposit(std::string name, double amount, double rate, double bonus)
    : BankDeposit(name, amount, rate), bonusRate(bonus) {
}

PremiumDeposit::PremiumDeposit(const PremiumDeposit& other)
    : BankDeposit(other), bonusRate(other.bonusRate) {
}

std::string PremiumDeposit::getTypeName() const { 
    return "Преміум"; 
}

double PremiumDeposit::calculateCompoundInterest(int months) const {
    double totalRate = interestRate + bonusRate;
    double monthlyRate = (totalRate / 100.0) / 12.0;
    return initialAmount * std::pow(1.0 + monthlyRate, months);
}

DepositContainer::DepositContainer(int cap) : capacity(cap), currentCount(0) {
    deposits = new BankDeposit * [capacity];
}

DepositContainer::~DepositContainer() {
    for (int i = 0; i < currentCount; ++i) {
        delete deposits[i];
    }
    delete[] deposits;
}

void DepositContainer::addDeposit(BankDeposit* dep) {
    if (currentCount < capacity) {
        deposits[currentCount++] = dep;
    }
    else {
        std::cout << "Помилка: Контейнер заповнений!" << std::endl;
    }
}

void DepositContainer::printAllDeposits(int months) const {
    for (int i = 0; i < currentCount; ++i) {
        std::cout << i + 1 << ". " << deposits[i]->getTypeName() << std::endl;
        std::cout << "Клієнт: " << deposits[i]->getClientName() << std::endl;
        std::cout << "Сума: " << deposits[i]->getInitialAmount() << " грн" << std::endl;
        std::cout << "Ставка: " << deposits[i]->getInterestRate() << "%" << std::endl;
        std::cout << "Після " << months << " міс.: "
        << deposits[i]->calculateCompoundInterest(months) << " грн\n" << std::endl;
    }
}

double DepositContainer::calculateTotalLiabilities(int months) const {
    double total = 0.0;
    for (int i = 0; i < currentCount; ++i) {
        // Поліморфний виклик
        total += deposits[i]->calculateCompoundInterest(months);
    }
    return total;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // контейнер із запасом на 5 елементів
    DepositContainer bank(5);

    //об'єкти похідних класів
    bank.addDeposit(new StandardDeposit("Олексій", 10000.0, 10.0));       // Стандартний (10% річних)
    bank.addDeposit(new CapitalizedDeposit("Марія", 15000.0, 9.0));       // З капіталізацією (9% річних)
    bank.addDeposit(new PremiumDeposit("Віктор", 50000.0, 8.0, 2.0));     // Преміум (8% + 2% бонус)

    int periodInMonths = 24; // Розрахунок на 2 роки (24 місяці)

    std::cout << "Розрахунок зобов'язань за вкладами" << std::endl;
    std::cout << "Розрахунковий період: " << periodInMonths << " місяців.\n" << std::endl;

    bank.printAllDeposits(periodInMonths);
    // Ключова функціональність класу-контейнера
    double totalLiabilities = bank.calculateTotalLiabilities(periodInMonths);

    std::cout << "Сумарні зобов'язання банку (з урахуванням складних відсотків): "
        << totalLiabilities << " грн." << std::endl;

    return 0;
}