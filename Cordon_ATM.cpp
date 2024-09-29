#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

const int MAX_ACCOUNTS = 100;
const string ACCOUNTS_FILE = "accounts.txt";

// Define the abstract data type for an ATM account
struct Account {
    int accountNumber;
    string accountName;
    double balance;
    int pinCode;
};

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
bool flashDriveInserted = false; // Flag to track flash drive presence

// Function to save account data to a text file
void saveAccountsToFile() {
    ofstream file(ACCOUNTS_FILE);
    if (file.is_open()) {
        for (int i = 0; i < accountCount; i++) {
            file << accounts[i].accountNumber << " " << accounts[i].accountName << endl;
            file << accounts[i].balance << endl;
            file << accounts[i].pinCode << endl;
        }
        file.close();
    }
}

// Function to load account data from a text file
void loadAccountsFromFile() {
    ifstream file(ACCOUNTS_FILE);
    if (file.is_open()) {
        accountCount = 0;
        while (!file.eof()) {
            if (accountCount < MAX_ACCOUNTS) {
                file >> accounts[accountCount].accountNumber;
                file.ignore();
                getline(file, accounts[accountCount].accountName);
                file >> accounts[accountCount].balance;
                file >> accounts[accountCount].pinCode;
                accountCount++;
            } else {
                break;
            }
        }
        file.close();
    }
}

// Function to validate PIN
bool validatePin(int accountNumber, int pin) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber && accounts[i].pinCode == pin) {
            return true;
        }
    }
    return false;
}

// Function to securely capture PIN input
int getSecurePinInput() {
    int pin = 0;
    char ch;
    cout << "Enter your 4-digit PIN code: ";

    for (int i = 0; i < 4; i++) {
        ch = _getch();
        if (ch >= '0' && ch <= '9') {
            pin = pin * 10 + (ch - '0');
            cout << '*';
        } else {
            i--;
        }
    }
    cout << endl;

    return pin;
}

// Function to register a new ATM account
void registerAccount() {
    if (accountCount < MAX_ACCOUNTS) {
        cout << "Enter Account Number (5 digits): ";
        cin >> accounts[accountCount].accountNumber;
        cin.ignore();

        cout << "Enter Account Name: ";
        getline(cin, accounts[accountCount].accountName);

        cout << "Enter Initial Deposit (Min. 5000): ";
        cin >> accounts[accountCount].balance;

        if (accounts[accountCount].balance < 5000) {
            cout << "Initial deposit must be at least 5000." << endl;
            cin.ignore();
            cin.ignore();
            return;
        }

        cout << "Set a 4-digit PIN code: ";
        cin >> accounts[accountCount].pinCode;

        if (accounts[accountCount].pinCode < 1000 || accounts[accountCount].pinCode > 9999) {
            cout << "PIN code must be a 4-digit number." << endl;
            return;
        }

        cout << "Account registered successfully!" << endl;
        cin.ignore();
        cin.ignore();
        accountCount++;
    } else {
        cout << "Maximum account limit reached." << endl;
        cin.ignore();
        cin.ignore();
    }
}

// Function to perform a balance inquiry
void balanceInquiry(int accountNumber, int pin) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            cout << "Account Number: " << accounts[i].accountNumber << endl;
            cout << "Account Name: " << accounts[i].accountName << endl;
            cout << "Current Balance: $" << accounts[i].balance << endl;
            cin.ignore();
            cin.ignore();
            return;
        }
    }

    cout << "Account not found. Please check the account number." << endl;
    cin.ignore();
    cin.ignore();
}

// Function to perform a withdrawal
void withdraw(int accountNumber, int pin, double amount) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            if (amount <= 0 || amount > accounts[i].balance) {
                cout << "Invalid withdrawal amount." << endl;
                cin.ignore();
                cin.ignore();
            } else {
                accounts[i].balance -= amount;
                cout << "Withdrawal successful. New balance: $" << accounts[i].balance << endl;
                cin.ignore();
                cin.ignore();
            }
            return;
        }
    }

    cout << "Account not found. Please check the account number." << endl;
    cin.ignore();
    cin.ignore();
}

// Function to perform a deposit
void deposit(int accountNumber, int pin, double amount) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            if (amount <= 0) {
                cout << "Invalid deposit amount." << endl;
                cin.ignore();
                cin.ignore();
            } else {
                accounts[i].balance += amount;
                cout << "Deposit successful. New balance: $" << accounts[i].balance << endl;
                cin.ignore();
                cin.ignore();
            }
            return;
        }
    }

    cout << "Account not found. Please check the account number." << endl;
    cin.ignore();
    cin.ignore();
}

// Function to perform a fund transfer
void fundTransfer(int sourceAccountNumber, int targetAccountNumber, int pin, double amount) {
    int sourceAccountIndex = -1;
    int targetAccountIndex = -1;

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == sourceAccountNumber) {
            sourceAccountIndex = i;
        }
        if (accounts[i].accountNumber == targetAccountNumber) {
            targetAccountIndex = i;
        }
    }

    if (sourceAccountIndex == -1) {
        cout << "Source account not found. Please check the account number." << endl;
        cin.ignore();
        cin.ignore();
    } else if (targetAccountIndex == -1) {
        cout << "Target account not found. Please check the account number." << endl;
        cin.ignore();
        cin.ignore();
    } else {
        if (amount <= 0 || amount > accounts[sourceAccountIndex].balance) {
            cout << "Invalid fund transfer amount." << endl;
            cin.ignore();
            cin.ignore();
        } else {
            accounts[sourceAccountIndex].balance -= amount;
            accounts[targetAccountIndex].balance += amount;
            cout << "Fund transfer successful." << endl;
            cin.ignore();
            cin.ignore();
        }
    }
}

// Function to change PIN
void changePin(int accountNumber, int currentPin, int newPinCode) {
    int accountIndex = -1;

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1) {
        cout << "Account not found. PIN change canceled." << endl;
        cin.ignore();
        cin.ignore();
        return;
    }

    accounts[accountIndex].pinCode = newPinCode;

    cout << "PIN code changed successfully!" << endl;
    cin.ignore();
    cin.ignore();
}

// Function to detect a flash drive
bool detectFlashDrive() {
    char driveLetter = 'A'; // Start with drive letter A

    while (driveLetter <= 'Z') {
        string drivePath = string(1, driveLetter) + ":\\";
        UINT driveType = GetDriveType(drivePath.c_str());

        if (driveType == DRIVE_REMOVABLE) {
            return true; // Flash drive detected
        }

        driveLetter++;
    }

    return false; // Flash drive not detected
}

void flashDriveDetectionThread() {
    while (true) {
        if (detectFlashDrive()) {
            cout << "ATM card detected. You can now access the ATM menu." << endl;
            flashDriveInserted = true;
            break; // Exit thread
        }
        Sleep(1000); // Sleep for 1 second before checking again
    }
}

int main() {
    // Start a thread to continuously check for flash drive insertion
     cout << "Enter your ATM card:" << endl;
    thread detectionThread(flashDriveDetectionThread);

    while (!flashDriveInserted) {
        // Wait until the flash drive is detected
    }

    detectionThread.join(); // Join the detection thread

    int choice;
    loadAccountsFromFile();

    do {
        cout << "ATM Prototype Menu:" << endl;
        cout << "1. Register New Account" << endl;
        cout << "2. Balance Inquiry" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Deposit" << endl;
        cout << "5. Fund Transfer" << endl;
        cout << "6. Change PIN" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        int accountNumber, pin;

        switch (choice) {
            case 1:
                registerAccount();
                saveAccountsToFile();
                system("cls");
                break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                pin = getSecurePinInput();
                if (!validatePin(accountNumber, pin)) {
                    cout << "Invalid PIN code. Inquiry canceled." << endl;
                    cin.ignore();
                    cin.ignore();
                    system("cls");
                    break;
                }
                balanceInquiry(accountNumber, pin);
                system("cls");
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                pin = getSecurePinInput();
                if (!validatePin(accountNumber, pin)) {
                    cout << "Invalid PIN code. Withdrawal canceled." << endl;
                    cin.ignore();
                    cin.ignore();
                    system("cls");
                    break;
                }
                double withdrawAmount;
                cout << "Enter Withdraw Amount: ";
                cin >> withdrawAmount;
                withdraw(accountNumber, pin, withdrawAmount);
                saveAccountsToFile();
                system("cls");
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                pin = getSecurePinInput();
                if (!validatePin(accountNumber, pin)) {
                    cout << "Invalid PIN code. Deposit canceled." << endl;
                    cin.ignore();
                    cin.ignore();
                    system("cls");

                    break;
                }
                double depositAmount;
                cout << "Enter Deposit Amount: ";
                cin >> depositAmount;
                deposit(accountNumber, pin, depositAmount);
                saveAccountsToFile();
                system("cls");
                break;
            case 5:
                cout << "Enter Source Account Number: ";
                cin >> accountNumber;
                pin = getSecurePinInput();
                if (!validatePin(accountNumber, pin)) {
                    cout << "Invalid PIN code. Transfer canceled." << endl;
                    cin.ignore();
                    cin.ignore();
                    system("cls");
                    break;
                }
                int targetAccountNumber;
                cout << "Enter Target Account Number: ";
                cin >> targetAccountNumber;
                double transferAmount;
                cout << "Enter Transfer Amount: ";
                cin >> transferAmount;
                fundTransfer(accountNumber, targetAccountNumber, pin, transferAmount);
                saveAccountsToFile();
                system("cls");
                break;
            case 6:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                pin = getSecurePinInput();
                if (!validatePin(accountNumber, pin)) {
                    cout << "Invalid PIN code. Password change canceled." << endl;
                    cin.ignore();
                    cin.ignore();
                    system("cls");
                    break;
                }
                int newPinCode;
                cout << "Enter New 4-digit PIN Code: ";
                cin >> newPinCode;
                changePin(accountNumber, pin, newPinCode);
                saveAccountsToFile();
                system("cls");
                break;
            case 7:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cin.ignore();
                cin.ignore();
                system("cls");
                break;
        }
    } while (choice != 7);

    saveAccountsToFile();
    return 0;
}
