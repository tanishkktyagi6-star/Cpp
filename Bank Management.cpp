#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "accounts.dat"

typedef struct {
    int accountNumber;
    char name[50];
    int pin;
    double balance;
} Account;

void createAccount(void);
void depositMoney(void);
void withdrawMoney(void);
void checkBalance(void);
void displayAccount(void);
void updateAccount(void);
void deleteAccount(void);
int accountExists(int accountNumber);

int main(void) {
    int choice;

    while (1) {
        printf("\n=====================================\n");
        printf("       BANK MANAGEMENT SYSTEM\n");
        printf("=====================================\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Display Account Details\n");
        printf("6. Update Account\n");
        printf("7. Delete Account\n");
        printf("8. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");

            while (getchar() != '\n') {
                // Clear invalid input
            }

            continue;
        }

        switch (choice) {
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
                displayAccount();
                break;

            case 6:
                updateAccount();
                break;

            case 7:
                deleteAccount();
                break;

            case 8:
                printf("\nThank you for using the Bank Management System.\n");
                exit(EXIT_SUCCESS);

            default:
                printf("\nInvalid choice. Please select from 1 to 8.\n");
        }
    }

    return 0;
}

int accountExists(int accountNumber) {
    FILE *file;
    Account account;

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        return 0;
    }

    while (fread(&account, sizeof(Account), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void createAccount(void) {
    FILE *file;
    Account account;

    printf("\n---------- CREATE ACCOUNT ----------\n");

    printf("Enter account number: ");
    scanf("%d", &account.accountNumber);

    if (accountExists(account.accountNumber)) {
        printf("An account with this number already exists.\n");
        return;
    }

    printf("Enter account holder name: ");
    scanf(" %[^\n]", account.name);

    printf("Create a 4-digit PIN: ");
    scanf("%d", &account.pin);

    if (account.pin < 1000 || account.pin > 9999) {
        printf("PIN must contain exactly 4 digits.\n");
        return;
    }

    printf("Enter initial deposit amount: ");
    scanf("%lf", &account.balance);

    if (account.balance < 0) {
        printf("Initial balance cannot be negative.\n");
        return;
    }

    file = fopen(FILE_NAME, "ab");

    if (file == NULL) {
        printf("Unable to open the account file.\n");
        return;
    }

    if (fwrite(&account, sizeof(Account), 1, file) != 1) {
        printf("Error while saving the account.\n");
        fclose(file);
        return;
    }

    fclose(file);

    printf("\nAccount created successfully.\n");
    printf("Account Number: %d\n", account.accountNumber);
    printf("Account Holder: %s\n", account.name);
    printf("Current Balance: Rs. %.2lf\n", account.balance);
}

void depositMoney(void) {
    FILE *file;
    Account account;
    int accountNumber;
    int pin;
    double amount;
    int found = 0;

    printf("\n---------- DEPOSIT MONEY ----------\n");

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    printf("Enter PIN: ");
    scanf("%d", &pin);

    file = fopen(FILE_NAME, "rb+");

    if (file == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&account, sizeof(Account), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            found = 1;

            if (account.pin != pin) {
                printf("Incorrect PIN.\n");
                fclose(file);
                return;
            }

            printf("Enter amount to deposit: ");
            scanf("%lf", &amount);

            if (amount <= 0) {
                printf("Deposit amount must be greater than zero.\n");
                fclose(file);
                return;
            }

            account.balance += amount;

            fseek(file, -(long)sizeof(Account), SEEK_CUR);

            if (fwrite(&account, sizeof(Account), 1, file) != 1) {
                printf("Unable to update the account.\n");
                fclose(file);
                return;
            }

            printf("\nMoney deposited successfully.\n");
            printf("Deposited Amount: Rs. %.2lf\n", amount);
            printf("New Balance: Rs. %.2lf\n", account.balance);

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(file);
}

void withdrawMoney(void) {
    FILE *file;
    Account account;
    int accountNumber;
    int pin;
    double amount;
    int found = 0;

    printf("\n---------- WITHDRAW MONEY ----------\n");

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    printf("Enter PIN: ");
    scanf("%d", &pin);

    file = fopen(FILE_NAME, "rb+");

    if (file == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&account, sizeof(Account), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            found = 1;

            if (account.pin != pin) {
                printf("Incorrect PIN.\n");
                fclose(file);
                return;
            }

            printf("Enter amount to withdraw: ");
            scanf("%lf", &amount);

            if (amount <= 0) {
                printf("Withdrawal amount must be greater than zero.\n");
                fclose(file);
                return;
            }

            if (amount > account.balance) {
                printf("Insufficient account balance.\n");
                printf("Available Balance: Rs. %.2lf\n", account.balance);
                fclose(file);
                return;
            }

            account.balance -= amount;

            fseek(file, -(long)sizeof(Account), SEEK_CUR);

            if (fwrite(&account, sizeof(Account), 1, file) != 1) {
                printf("Unable to update the account.\n");
                fclose(file);
                return;
            }

            printf("\nMoney withdrawn successfully.\n");
            printf("Withdrawn Amount: Rs. %.2lf\n", amount);
            printf("Remaining Balance: Rs. %.2lf\n", account.balance);

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(file);
}

void checkBalance(void) {
    FILE *file;
    Account account;
    int accountNumber;
    int pin;
    int found = 0;

    printf("\n---------- CHECK BALANCE ----------\n");

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    printf("Enter PIN: ");
    scanf("%d", &pin);

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&account, sizeof(Account), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            found = 1;

            if (account.pin != pin) {
                printf("Incorrect PIN.\n");
                fclose(file);
                return;
            }

            printf("\nAccount Holder: %s\n", account.name);
            printf("Available Balance: Rs. %.2lf\n", account.balance);

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(file);
}

void displayAccount(void) {
    FILE *file;
    Account account;
    int accountNumber;
    int pin;
    int found = 0;

    printf("\n---------- ACCOUNT DETAILS ----------\n");

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    printf("Enter PIN: ");
    scanf("%d", &pin);

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&account, sizeof(Account), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            found = 1;

            if (account.pin != pin) {
                printf("Incorrect PIN.\n");
                fclose(file);
                return;
            }

            printf("\n=====================================\n");
            printf("Account Number : %d\n", account.accountNumber);
            printf("Account Holder : %s\n", account.name);
            printf("Current Balance: Rs. %.2lf\n", account.balance);
            printf("=====================================\n");

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(file);
}

void updateAccount(void) {
    FILE *file;
    Account account;
    int accountNumber;
    int pin;
    int newPin;
    int found = 0;
    char newName[50];

    printf("\n---------- UPDATE ACCOUNT ----------\n");

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    printf("Enter current PIN: ");
    scanf("%d", &pin);

    file = fopen(FILE_NAME, "rb+");

    if (file == NULL) {
        printf("No account records found.\n");
        return;
    }

    while (fread(&account, sizeof(Account), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            found = 1;

            if (account.pin != pin) {
                printf("Incorrect PIN.\n");
                fclose(file);
                return;
            }

            printf("Enter new account holder name: ");
            scanf(" %[^\n]", newName);

            printf("Enter new 4-digit PIN: ");
            scanf("%d", &newPin);

            if (newPin < 1000 || newPin > 9999) {
                printf("PIN must contain exactly 4 digits.\n");
                fclose(file);
                return;
            }

            strcpy(account.name, newName);
            account.pin = newPin;

            fseek(file, -(long)sizeof(Account), SEEK_CUR);

            if (fwrite(&account, sizeof(Account), 1, file) != 1) {
                printf("Unable to update the account.\n");
                fclose(file);
                return;
            }

            printf("\nAccount updated successfully.\n");

            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(file);
}

void deleteAccount(void) {
    FILE *file;
    FILE *temporaryFile;
    Account account;
    int accountNumber;
    int pin;
    int found = 0;

    printf("\n---------- DELETE ACCOUNT ----------\n");

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    printf("Enter PIN: ");
    scanf("%d", &pin);

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        printf("No account records found.\n");
        return;
    }

    temporaryFile = fopen("temporary.dat", "wb");

    if (temporaryFile == NULL) {
        printf("Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    while (fread(&account, sizeof(Account), 1, file) == 1) {
        if (account.accountNumber == accountNumber) {
            if (account.pin != pin) {
                printf("Incorrect PIN.\n");

                fclose(file);
                fclose(temporaryFile);
                remove("temporary.dat");

                return;
            }

            found = 1;
        } else {
            fwrite(&account, sizeof(Account), 1, temporaryFile);
        }
    }

    fclose(file);
    fclose(temporaryFile);

    if (found) {
        remove(FILE_NAME);
        rename("temporary.dat", FILE_NAME);

        printf("\nAccount deleted successfully.\n");
    } else {
        remove("temporary.dat");
        printf("Account not found.\n");
    }
}