#define CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>

using namespace std;

struct Date {
    unsigned day;
    unsigned month;
    unsigned year;

    void Output() const;
    void Input();
};

class Spending {
    float money;
    int category;
    Date date;
    enum PaymentType { CARD, WALLET };
    PaymentType paymentType;

public:
    Spending();
    Spending(float mon, int cat, Date d, PaymentType type);
    void Output() const;
    void Input();
    float GetMoney() const;
    int GetCategory() const;
    bool operator<(const Spending& b) const;
    const Date& GetDate() const;
    PaymentType GetPaymentType() const;
};

bool isLeap(int year);
int diff_between_dates(Date begin_date, Date end_date);

class MoneyStorage {
protected:
    string number;
    float totalMoney;
    multiset<Spending> spendings;

public:
    MoneyStorage();
    MoneyStorage(const string& num);
    MoneyStorage(multiset<Spending>& m, const string& num, float t);
    virtual ~MoneyStorage() {}
    virtual void Output() = 0;
    void Input();
    void TopUp();
    void AddSpending();
    const string& GetNumber() const;
    multiset<Spending> GetSpendings() const;
};

class Card : public MoneyStorage {
    string bankName;

public:
    Card();
    Card(const string& num, const string& name);
    Card(multiset<Spending>& m, const string& num, const string& name);
    void Output();
    void Input();
};

class Wallet : public MoneyStorage {
    string walletType;

public:
    Wallet();
    Wallet(const string& num, const string& type);
    Wallet(multiset<Spending>& m, const string& num, const string& type);
    void Output();
    void Input();
};

class FinanceManagement {
    set<MoneyStorage*> storages;

public:
    FinanceManagement();
    ~FinanceManagement();
    void AddStorage(MoneyStorage* ptr);
    void DeleteStorage(const string& number, const string& storageType);
    void Output();
    void TopUpStorage();
    void AddSpendingToStorage();
    void CreateCard();
    void CreateWallet();
    void ShowCards();
    void ShowWallets();
    multiset<Spending> GetSpendings() const;
    void GenerateAndSaveExpenseReport();
    void GenerateAndSaveRating();
};

void Date::Output() const {
    cout << setw(2) << setfill('0') << day << ".";
    cout << setw(2) << setfill('0') << month << ".";
    cout << setw(4) << year;
}

void Date::Input() {
    cout << "Enter day: ";
    cin >> day;
    cout << "Enter month: ";
    cin >> month;
    cout << "Enter year: ";
    cin >> year;
}

bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int diff_between_dates(Date begin_date, Date end_date) {
    int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    int days = 0;

    if (begin_date.year == end_date.year && begin_date.month == end_date.month) {
        days += end_date.day - begin_date.day;
    }
    else {
        days += days_in_month[begin_date.month - 1] - begin_date.day;

        for (int i = begin_date.month + 1; i < end_date.month; ++i) {
            days += days_in_month[i - 1];
        }

        days += end_date.day;
    }

    return days;
}

Spending::Spending() : money(0), category(0), paymentType(CARD) {}

Spending::Spending(float mon, int cat, Date d, PaymentType type) : money(mon), category(cat), date(d), paymentType(type) {}

void Spending::Output() const {
    cout << "Money: " << money << endl;
    cout << "Category: " << category << endl;
    cout << "Date: ";
    date.Output();
    cout << endl;
}

void Spending::Input() {
    cout << "Enter amount of money you have spent: ";
    cin >> money;
    cout << "Enter category (1 - products, 2 - Drug Stores , 3 - Pets, 4 - Clothes and Shoes, 5 - household items, 6 - Construction materials, 7 - Taxi, 8 - entertainment, 9 - Gas Stations): ";
    cin >> category;
    date.Input();
}

float Spending::GetMoney() const {
    return money;
}

int Spending::GetCategory() const {
    return category;
}

bool Spending::operator<(const Spending& b) const {
    return money < b.money;
}

const Date& Spending::GetDate() const {
    return date;
}

Spending::PaymentType Spending::GetPaymentType() const {
    return paymentType;
}

MoneyStorage::MoneyStorage() : totalMoney(0) {}

MoneyStorage::MoneyStorage(const string& num) : number(num), totalMoney(0) {}

MoneyStorage::MoneyStorage(multiset<Spending>& m, const string& num, float t) : number(num), spendings(m), totalMoney(t) {}

void MoneyStorage::Input() {
    cout << "Enter number: ";
    cin >> number;
}

void MoneyStorage::TopUp() {
    float amount;
    cout << "Enter amount to top up: ";
    cin >> amount;
    totalMoney += amount;
}

void MoneyStorage::AddSpending() {
    Spending s;
    s.Input();
    spendings.insert(s);
    totalMoney -= s.GetMoney();
}

const string& MoneyStorage::GetNumber() const {
    return number;
}

multiset<Spending> MoneyStorage::GetSpendings() const {
    return spendings;
}

Card::Card() : MoneyStorage(), bankName("") {}

Card::Card(const string& num, const string& name) : MoneyStorage(num), bankName(name) {}

Card::Card(multiset<Spending>& m, const string& num, const string& name) : MoneyStorage(m, num, 0), bankName(name) {}

void Card::Output() {
    cout << "Card number: " << number << endl;
    cout << "Bank name: " << bankName << endl;
    cout << "Total money: " << totalMoney << endl;
    cout << "Spendings:" << endl;
    for (const auto& s : spendings) {
        s.Output();
    }
}

void Card::Input() {
    MoneyStorage::Input();
    cout << "Enter bank name: ";
    cin >> bankName;
}

Wallet::Wallet() : MoneyStorage(), walletType("") {}

Wallet::Wallet(const string& num, const string& type) : MoneyStorage(num), walletType(type) {}

Wallet::Wallet(multiset<Spending>& m, const string& num, const string& type) : MoneyStorage(m, num, 0), walletType(type) {}

void Wallet::Output() {
    cout << "Wallet number: " << number << endl;
    cout << "Wallet type: " << walletType << endl;
    cout << "Total money: " << totalMoney << endl;
    cout << "Spendings:" << endl;
    for (const auto& s : spendings) {
        s.Output();
    }
}

void Wallet::Input() {
    MoneyStorage::Input();
    cout << "Enter wallet name: ";
    cin >> walletType;
}

FinanceManagement::FinanceManagement() {}

FinanceManagement::~FinanceManagement() {
    for (auto storage : storages) {
        delete storage;
    }
}

void FinanceManagement::AddStorage(MoneyStorage* ptr) {
    storages.insert(ptr);
}

void FinanceManagement::DeleteStorage(const string& number, const string& storageType) {
    for (auto it = storages.begin(); it != storages.end(); ++it) {
        if ((*it)->GetNumber() == number) {
            if (storageType == "card" && dynamic_cast<Card*>(*it)) {
                delete* it;
                storages.erase(it);
                cout << "Card with number " << number << " deleted successfully." << endl;
                return;
            }
            else if (storageType == "wallet" && dynamic_cast<Wallet*>(*it)) {
                delete* it;
                storages.erase(it);
                cout << "Wallet with number " << number << " deleted successfully." << endl;
                return;
            }
            else {
                cout << "Invalid storage type for deletion." << endl;
                return;
            }
        }
    }
    cout << "Storage with number " << number << " not found." << endl;
}

void FinanceManagement::Output() {
    cout << "Finance Management System" << endl;
    for (auto storage : storages) {
        storage->Output();
    }
}

void FinanceManagement::TopUpStorage() {
    string number;
    cout << "Enter storage number to top up: ";
    cin >> number;
    for (auto storage : storages) {
        if (storage->GetNumber() == number) {
            storage->TopUp();
            cout << "Storage topped up successfully." << endl;
            return;
        }
    }
    cout << "Storage not found." << endl;
}

void FinanceManagement::AddSpendingToStorage() {
    string number;
    cout << "Enter storage number to add spending: ";
    cin >> number;
    for (auto storage : storages) {
        if (storage->GetNumber() == number) {
            storage->AddSpending();
            cout << "Spending added successfully." << endl;
            return;
        }
    }
    cout << "Storage not found." << endl;
}

void FinanceManagement::CreateCard() {
    string num;
    string name;
    while (true) {
        cout << "Enter a card number (16 digits): ";
        cin >> num;
        if (num.length() == 16) {
            cout << "Success!" << endl;
            break;
        }
        else {
            cout << "Invalid card number! Your card must be 16 digits long. Please try again." << endl;
        }
    }

    cout << "Enter bank name: ";
    cin >> name;
    MoneyStorage* card = new Card(num, name);
    AddStorage(card);
}

void FinanceManagement::CreateWallet() {
    string num;
    string name;

    while (true) {
        cout << "Enter wallet ID (8 digits): ";
        cin >> num;

        if (num.length() == 8) {
            cout << "Success!" << endl;
            break;
        }
        else {
            cout << "Invalid wallet ID! Your wallet ID must be 8 digits long. Please try again." << endl;
        }
    }

    cout << "Enter wallet name: ";
    cin >> name;
    MoneyStorage* wallet = new Wallet(num, name);
    AddStorage(wallet);
}

void FinanceManagement::ShowCards() {
    cout << "Cards:" << endl;
    for (auto storage : storages) {
        if (dynamic_cast<Card*>(storage)) {
            storage->Output();
        }
    }
}

void FinanceManagement::ShowWallets() {
    cout << "Wallets:" << endl;
    for (auto storage : storages) {
        if (dynamic_cast<Wallet*>(storage)) {
            storage->Output();
        }
    }
}

multiset<Spending> FinanceManagement::GetSpendings() const {
    multiset<Spending> allSpendings;
    for (auto storage : storages) {
        multiset<Spending> storageSpendings = storage->GetSpendings();
        allSpendings.insert(storageSpendings.begin(), storageSpendings.end());
    }
    return allSpendings;
}

void FinanceManagement::GenerateAndSaveExpenseReport() {
    Date start, end;
    cout << "Enter start date (dd mm yyyy): ";
    start.Input();
    cout << "Enter end date (dd mm yyyy): ";
    end.Input();
    multiset<Spending> allSpendings = GetSpendings();

    // Проверка наличия затрат в указанном диапазоне дат
    bool hasExpenses = false;
    for (const auto& spending : allSpendings) {
        const Date& spendingDate = spending.GetDate();
        if (spendingDate.year >= start.year && spendingDate.year <= end.year &&
            spendingDate.month >= start.month && spendingDate.month <= end.month &&
            spendingDate.day >= start.day && spendingDate.day <= end.day) {
            hasExpenses = true;
            break;
        }
    }

    // Генерация и сохранение отчета о затратах
    string filename = "Report.txt";
    ofstream file(filename);
    if (file.is_open()) {
        if (hasExpenses) {
            file << "Expense Report:" << endl;
            for (const auto& spending : allSpendings) {
                const Date& spendingDate = spending.GetDate();
                if (spendingDate.year >= start.year && spendingDate.year <= end.year &&
                    spendingDate.month >= start.month && spendingDate.month <= end.month &&
                    spendingDate.day >= start.day && spendingDate.day <= end.day) {
                    file << spendingDate.day << "." << spendingDate.month << "." << spendingDate.year << ": " << spending.GetMoney() << " - Category " << spending.GetCategory() << endl;
                }
            }
        }
        else {
            file << "No rating and expenses" << endl;
        }
        file.close();
        cout << "Expense report has been saved to " << filename << endl;
    }
    else {
        cout << "Unable to open file " << filename << " for writing." << endl;
    }
}

void FinanceManagement::GenerateAndSaveRating() {
    Date start, end;
    cout << "Enter start date (dd mm yyyy): ";
    start.Input();
    cout << "Enter end date (dd mm yyyy): ";
    end.Input();
    multiset<Spending> allSpendings = GetSpendings();

    // Проверка наличия затрат в указанном диапазоне дат
    bool hasExpenses = false;
    for (const auto& spending : allSpendings) {
        const Date& spendingDate = spending.GetDate();
        if (spendingDate.year >= start.year && spendingDate.year <= end.year &&
            spendingDate.month >= start.month && spendingDate.month <= end.month &&
            spendingDate.day >= start.day && spendingDate.day <= end.day) {
            hasExpenses = true;
            break;
        }
    }

    // Создание карты категорий и подсчет суммарных расходов по категориям
    map<int, float> categorySpendings;
    for (const auto& spending : allSpendings) {
        const Date& spendingDate = spending.GetDate();
        if (spendingDate.year >= start.year && spendingDate.year <= end.year &&
            spendingDate.month >= start.month && spendingDate.month <= end.month &&
            spendingDate.day >= start.day && spendingDate.day <= end.day) {
            categorySpendings[spending.GetCategory()] += spending.GetMoney();
        }
    }

    // Поиск категории с максимальными расходами
    int maxCategory = -1;
    float maxSpending = 0;
    for (const auto& entry : categorySpendings) {
        if (entry.second > maxSpending) {
            maxSpending = entry.second;
            maxCategory = entry.first;
        }
    }

    // Генерация и сохранение рейтинга категорий
    string filename = "Rating.txt";
    ofstream file(filename);
    if (file.is_open()) {
        if (hasExpenses) {
            file << "Category Rating:" << endl;
            for (const auto& entry : categorySpendings) {
                file << "Category " << entry.first << ": " << entry.second << " - ";
                if (entry.first == maxCategory) {
                    file << "(Maximum Spending)" << endl;
                }
                else {
                    file << endl;
                }
            }
        }
        else {
            file << "No rating and expenses" << endl;
        }
        file.close();
        cout << "Category rating has been saved to " << filename << endl;
    }
    else {
        cout << "Unable to open file " << filename << " for writing." << endl;
    }
}

int main() {
    FinanceManagement system;

    unsigned operation;
    do {
        cout << "Choose operation" << endl;
        cout << "1 - Create a new card" << endl;
        cout << "2 - Create a new wallet" << endl;
        cout << "3 - Top up storage" << endl;
        cout << "4 - Add spending to storage" << endl;
        cout << "5 - Show cards" << endl;
        cout << "6 - Show wallets" << endl;
        cout << "7 - Delete a card or a wallet" << endl;
        cout << "8 - Generate and save expense report" << endl;
        cout << "9 - Generate and save rating" << endl;
        cout << "10 - Exit" << endl << endl;
        cin >> operation;
        switch (operation) {
        case 1:
            system.CreateCard();
            break;
        case 2:
            system.CreateWallet();
            break;
        case 3:
            system.TopUpStorage();
            break;
        case 4:
            system.AddSpendingToStorage();
            break;
        case 5:
            system.ShowCards();
            break;
        case 6:
            system.ShowWallets();
            break;
        case 7: {
            string storageNumber;
            string storageType;
            cout << "Enter storage number to delete: ";
            cin >> storageNumber;
            cout << "Enter storage type (card or wallet): ";
            cin >> storageType;
            system.DeleteStorage(storageNumber, storageType);
            break;
        }
        case 8:
            system.GenerateAndSaveExpenseReport();
            break;
        case 9:
            system.GenerateAndSaveRating();
            break;
        case 10:
            cout << "Bye!" << endl;
            break;
        default:
            cout << "This operation doesn't exist!" << endl;
        }
        cout << endl;
    } while (operation != 10);
}