#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USER 100
#define DB_FILE "database.txt"

typedef struct {
    char username[512];
    char password[512];
    double balance;
} User;

User users[MAX_USER];
int userCount = 0;
int currentUserIndex = -1;

void saveDB() {
    FILE *file = fopen(DB_FILE, "w");
    if (!file) {
        perror("Error: Unable to open database file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %.2f\n", users[i].username, users[i].password, users[i].balance);
    }

    fclose(file);
}

void loadDB() {
    FILE *file = fopen(DB_FILE, "r");
    if (!file) {
        saveDB();
        return;
    }

    while (fscanf(file, "%49s %49s %lf", users[userCount].username, users[userCount].password, &users[userCount].balance) == 3) {
        userCount++;
        if (userCount >= MAX_USER) {
            printf("User limit reached.\n");
            break;
        }
    }

    fclose(file);
}

void createAccount() {
    if (userCount >= MAX_USER) {
        printf("User limit reached.");
        return;
    }

    printf("Username: ");
    scanf("%s", users[userCount].username);

    for (int i = 0; users[userCount].username[i]; i++) {
        if (users[userCount].username[i] == ' ' || users[userCount].username[i] == ':') {
            users[userCount].username[i] = '\\';
        }
    }

    if (strlen(users[userCount].username) >= sizeof(users[userCount].username)) {
        printf("Error: Username is too long.\n");
        return;
    }

    printf("Password: ");
    scanf("%s", users[userCount].password);

    for (int i = 0; users[userCount].password[i]; i++) {
        if (users[userCount].password[i] == ' ' || users[userCount].password[i] == ':') {
            users[userCount].password[i] = '\\';
        }
    }

    if (strlen(users[userCount].password) >= sizeof(users[userCount].password)) {
        printf("Error: Password is too long.\n");
        return;
    }

    users[userCount].balance = 100.0;
    userCount++;

    printf("Registration successful.");
}

int findUserIndex(const char *username) {
    char modifiedUsername[512];
    int j = 0;
    for (int i = 0; username[i]; i++) {
        if (username[i] == '\\' && (username[i + 1] == ' ' || username[i + 1] == ':')) {
            modifiedUsername[j++] = username[++i];
        } else {
            modifiedUsername[j++] = username[i];
        }
    }
    modifiedUsername[j] = '\0';

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, modifiedUsername) == 0) {
            return i;
        }
    }
    return -1;
}

void loginAccount() {
    char username[512];
    char password[512];

    printf("Username: ");
    scanf("%s", username);

    if (strlen(username) >= sizeof(username)) {
        printf("Error: Username is too long.\n");
        return;
    }

    printf("Password: ");
    scanf("%s", password);

    if (strlen(password) >= sizeof(password)) {
        printf("Error: Password is too long.\n");
        return;
    }

    int userIndex = findUserIndex(username);

    if (userIndex != -1 && strcmp(users[userIndex].password, password) == 0) {
        currentUserIndex = userIndex;
        printf("Login successful!\n");
    } else {
        printf("Incorrect username or password.\n");
    }
}

void deposit() {
    double amount;
    printf("Amount to deposit: ");
    scanf("%lf", &amount);

    users[currentUserIndex].balance += amount;
    printf("Deposit successful\nBalance: %.2f\n", users[currentUserIndex].balance);

    saveDB();
}

void withdraw() {
    double amount;
    printf("Amount to withdraw: ");
    scanf("%lf", &amount);

    if (amount <= users[currentUserIndex].balance) {
        users[currentUserIndex].balance -= amount;
        printf("Withdrawal successful\nBalance: %.2f\n", users[currentUserIndex].balance);

        saveDB();
    } else {
        printf("Insufficient balance.\n");
    }
}

void seeBalance() {
    printf("Balance: %.2f\n", users[currentUserIndex].balance);
}

int main() {
    loadDB();

    int choice;

    while (1) {
        printf("\n1 => Register\n2 => Login\n");
        if (currentUserIndex != -1) {
            printf("3 => Deposit\n4 => Withdraw\n5 => Check Balance\n");
        }

        printf("0 => Exit\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                saveDB();
                exit(EXIT_SUCCESS);
            case 1:
                createAccount();
                break;
            case 2:
                loginAccount();
                break;
            case 3:
                if (currentUserIndex != -1) {
                    deposit();
                } else {
                    printf("Please log in to continue.\n");
                }
                break;
            case 4:
                if (currentUserIndex != -1) {
                    withdraw();
                } else {
                    printf("Please log in to continue.\n");
                }
                break;
            case 5:
                if (currentUserIndex != -1) {
                    seeBalance();
                } else {
                    printf("Please log in to continue.\n");
                }
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}
