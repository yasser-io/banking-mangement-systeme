
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define FILENAME "bank_data.dat"

typedef struct {
    int accountNumber;
    char name[50];
    double balance;
    char accountType[20];
} BankAccount;

BankAccount accounts[MAX_ACCOUNTS];
int accountCount = 0;

void displayMenu();
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void displayAllAccounts();
void searchAccount();
void deleteAccount();
void saveData();
void loadData();
int findAccount(int accountNumber);

int main() {
    loadData();

    int choice;

    printf("=== Banking Management System ===\n");

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                displayAllAccounts();
                break;
            case 6:
                searchAccount();
                break;
            case 7:
                deleteAccount();
                break;
            case 8:
                saveData();
                printf("Thank you for using the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 8);

    return 0;
}

void displayMenu() {
    printf("\n=== Main Menu ===\n");
    printf("1. Create New Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("5. Display All Accounts\n");
    printf("6. Search Account\n");
    printf("7. Delete Account\n");
    printf("8. Exit\n");
}

void createAccount() {
    if(accountCount >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts! Maximum limit reached.\n");
        return;
    }

    BankAccount newAccount;

    printf("\n=== Create New Account ===\n");

    newAccount.accountNumber = 1000 + accountCount + 1;

    printf("Generated Account Number: %d\n", newAccount.accountNumber);

    printf("Enter customer name: ");
    getchar();
    fgets(newAccount.name, 50, stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = 0;

    printf("Enter account type (saving/current): ");
    fgets(newAccount.accountType, 20, stdin);
    newAccount.accountType[strcspn(newAccount.accountType, "\n")] = 0;

    printf("Enter initial balance: ");
    scanf("%lf", &newAccount.balance);

    if(newAccount.balance < 0) {
        printf("Balance cannot be negative!\n");
        return;
    }

    accounts[accountCount] = newAccount;
    accountCount++;

    printf("Account created successfully!\n");
    saveData();
}

void depositMoney() {
    int accountNumber;
    double amount;

    printf("\n=== Deposit Money ===\n");
    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    int index = findAccount(accountNumber);

    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    if(amount <= 0) {
        printf("Amount must be positive!\n");
        return;
    }

    accounts[index].balance += amount;
    printf("Deposit successful!\n");
    printf("New balance: %.2lf\n", accounts[index].balance);
    saveData();
}

void withdrawMoney() {
    int accountNumber;
    double amount;

    printf("\n=== Withdraw Money ===\n");
    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    int index = findAccount(accountNumber);

    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    if(amount <= 0) {
        printf("Amount must be positive!\n");
        return;
    }

    if(amount > accounts[index].balance) {
        printf("Insufficient balance!\n");
        printf("Available balance: %.2lf\n", accounts[index].balance);
        return;
    }

    accounts[index].balance -= amount;
    printf("Withdrawal successful!\n");
    printf("New balance: %.2lf\n", accounts[index].balance);
    saveData();
}

void checkBalance() {
    int accountNumber;

    printf("\n=== Check Balance ===\n");
    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    int index = findAccount(accountNumber);

    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("\n=== Account Information ===\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Customer Name: %s\n", accounts[index].name);
    printf("Account Type: %s\n", accounts[index].accountType);
    printf("Current Balance: %.2lf\n", accounts[index].balance);
}

void displayAllAccounts() {
    if(accountCount == 0) {
        printf("No accounts to display!\n");
        return;
    }

    printf("\n=== All Accounts ===\n");
    printf("==================================================================\n");
    printf("Account No.\tCustomer Name\t\tAccount Type\tBalance\n");
    printf("==================================================================\n");

    double totalBalance = 0;

    for(int i = 0; i < accountCount; i++) {
        printf("%d\t\t%-20s\t%-10s\t%.2lf\n",
               accounts[i].accountNumber,
               accounts[i].name,
               accounts[i].accountType,
               accounts[i].balance);
        totalBalance += accounts[i].balance;
    }

    printf("==================================================================\n");
    printf("Total Balance: %.2lf\n", totalBalance);
    printf("Total Accounts: %d\n", accountCount);
}

void searchAccount() {
    int accountNumber;

    printf("\n=== Search Account ===\n");
    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    int index = findAccount(accountNumber);

    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("\n=== Search Results ===\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Customer Name: %s\n", accounts[index].name);
    printf("Account Type: %s\n", accounts[index].accountType);
    printf("Current Balance: %.2lf\n", accounts[index].balance);
}

void deleteAccount() {
    int accountNumber;

    printf("\n=== Delete Account ===\n");
    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    int index = findAccount(accountNumber);

    if(index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Are you sure you want to delete account %s (Number: %d)? (y/n): ",
           accounts[index].name, accounts[index].accountNumber);

    char confirm;
    getchar();
    scanf("%c", &confirm);

    if(confirm == 'y' || confirm == 'Y') {
        for(int i = index; i < accountCount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }

        accountCount--;
        printf("Account deleted successfully!\n");
        saveData();
    } else {
        printf("Delete operation cancelled.\n");
    }
}

int findAccount(int accountNumber) {
    for(int i = 0; i < accountCount; i++) {
        if(accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

void saveData() {
    FILE *file = fopen(FILENAME, "wb");
    if(file == NULL) {
        printf("Error saving data!\n");
        return;
    }

    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(BankAccount), accountCount, file);
    fclose(file);
}

void loadData() {
    FILE *file = fopen(FILENAME, "rb");
    if(file == NULL) {
        printf("No previous data found. Starting new system...\n");
        return;
    }

    fread(&accountCount, sizeof(int), 1, file);
    fread(accounts, sizeof(BankAccount), accountCount, file);
    fclose(file);

    printf("Successfully loaded %d accounts.\n", accountCount);
}
