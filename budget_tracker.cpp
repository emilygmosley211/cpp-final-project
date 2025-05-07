#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include "budget_tracker.h"

using namespace std;

struct Entry {
    string type;
    string description;
    double amount;
};

const int MAX_ENTRIES = 100;
Entry budgetEntries[MAX_ENTRIES];
int entryCount = 0;

void printRainbow(const string& text);
void addEntry(string type);
void viewEntries();
void saveToFile();
void loadFromFile();
bool isValidAmount(const string& str);
double calculateTotal(const string& type);
void printSummary();
void monthlyGoalChecker(double goal, double current, int month = 1);
void clearScreen();
void welcomeBanner();

void runBudgetTracker() {
    loadFromFile();
    int choice;
    string input;
    double goal = 0;

    welcomeBanner();
    do {
        cout << "\n";
        printRainbow("1. Add Income");
        printRainbow("2. Add Expense");
        printRainbow("3. View Entries");
        printRainbow("4. Summary");
        printRainbow("5. Set Monthly Saving Goal");
        printRainbow("6. Save and Exit");
        cout << "Enter choice: ";
        getline(cin, input);

        if (!isValidAmount(input)) {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        choice = stoi(input);
        switch (choice) {
            case 1: addEntry("Income"); break;
            case 2: addEntry("Expense"); break;
            case 3: viewEntries(); break;
            case 4: printSummary(); break;
            case 5: {
                cout << "Enter your monthly saving goal: ";
                getline(cin, input);
                if (isValidAmount(input)) {
                    goal = stod(input);
                    monthlyGoalChecker(goal, calculateTotal("Income") - calculateTotal("Expense"));
                } else {
                    cout << "Invalid amount.\n";
                }
                break;
            }
            case 6: saveToFile(); printRainbow("Goodbye!"); break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

void addEntry(string type) {
    if (entryCount >= MAX_ENTRIES) {
        cout << "Entry limit reached.\n";
        return;
    }

    string desc, amtStr;
    double amt;
    cout << "Enter description: ";
    getline(cin, desc);
    cout << "Enter amount: ";
    getline(cin, amtStr);

    if (!isValidAmount(amtStr)) {
        cout << "Invalid amount.\n";
        return;
    }

    amt = stod(amtStr);
    budgetEntries[entryCount++] = {type, desc, amt};
    printRainbow("Entry added successfully.");
}

void viewEntries() {
    cout << "\n--- Entries ---\n";
    for (int i = 0; i < entryCount; ++i) {
        cout << budgetEntries[i].type << " | "
             << setw(20) << left << budgetEntries[i].description
             << " | $" << fixed << setprecision(2) << budgetEntries[i].amount << "\n";
    }
}

void saveToFile() {
    ofstream out("budget_data.txt");
    for (int i = 0; i < entryCount; ++i) {
        out << budgetEntries[i].type << "," << budgetEntries[i].description << "," << budgetEntries[i].amount << "\n";
    }
    out.close();
}

void loadFromFile() {
    ifstream in("budget_data.txt");
    if (!in) return;

    string type, desc, amtStr;
    double amt;
    while (getline(in, type, ',') && getline(in, desc, ',') && getline(in, amtStr)) {
        amt = stod(amtStr);
        budgetEntries[entryCount++] = {type, desc, amt};
    }
    in.close();
}

bool isValidAmount(const string& str) {
    for (char c : str) {
        if (!isdigit(c) && c != '.') return false;
    }
    return !str.empty();
}

double calculateTotal(const string& type) {
    double sum = 0;
    for (int i = 0; i < entryCount; ++i) {
        if (budgetEntries[i].type == type) sum += budgetEntries[i].amount;
    }
    return sum;
}

void printSummary() {
    double income = calculateTotal("Income");
    double expense = calculateTotal("Expense");
    double balance = income - expense;

    cout << "\n--- Summary ---\n";
    cout << "Total Income:  $" << fixed << setprecision(2) << income << "\n";
    cout << "Total Expense: $" << expense << "\n";
    cout << "Balance:       $" << balance << "\n";
}

void monthlyGoalChecker(double goal, double current, int month) {
    if (month > 12) return;
    cout << "Month " << month << ": ";
    if (current >= goal) {
        printRainbow("✔ Goal met!");
    } else {
        printRainbow("✖ Goal not met.");
    }
    monthlyGoalChecker(goal, current, month + 1);
}

void printRainbow(const string& text) {
    const string colors[] = {"\033[31m", "\033[33m", "\033[32m", "\033[36m", "\033[34m", "\033[35m"};
    for (size_t i = 0; i < text.length(); ++i) {
        cout << colors[i % 6] << text[i];
    }
    cout << "\033[0m\n";
}

void welcomeBanner() {
    printRainbow("***********************");
    printRainbow("*  Budget Tracker 💰  *");
    printRainbow("***********************\n");
}
