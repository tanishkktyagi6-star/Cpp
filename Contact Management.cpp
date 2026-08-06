#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "contacts.dat"

typedef struct {
    int id;
    char name[50];
    char phone[20];
    char email[60];
    char address[100];
} Contact;

void addContact(void);
void displayContacts(void);
void searchContact(void);
void updateContact(void);
void deleteContact(void);
int contactExists(int id);

int main(void) {
    int choice;

    while (1) {
        printf("\n====================================\n");
        printf("      CONTACT MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Add Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Update Contact\n");
        printf("5. Delete Contact\n");
        printf("6. Exit\n");
        printf("====================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");

            while (getchar() != '\n') {
                // Clear input buffer
            }

            continue;
        }

        switch (choice) {
            case 1:
                addContact();
                break;

            case 2:
                displayContacts();
                break;

            case 3:
                searchContact();
                break;

            case 4:
                updateContact();
                break;

            case 5:
                deleteContact();
                break;

            case 6:
                printf("\nThank you for using Contact Management System.\n");
                exit(EXIT_SUCCESS);

            default:
                printf("Invalid choice. Please select from 1 to 6.\n");
        }
    }

    return 0;
}

int contactExists(int id) {
    FILE *file;
    Contact contact;

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        return 0;
    }

    while (fread(&contact, sizeof(Contact), 1, file) == 1) {
        if (contact.id == id) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void addContact(void) {
    FILE *file;
    Contact contact;

    printf("\n---------- ADD CONTACT ----------\n");

    printf("Enter Contact ID: ");
    scanf("%d", &contact.id);

    if (contactExists(contact.id)) {
        printf("Contact with this ID already exists.\n");
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", contact.name);

    printf("Enter Phone Number: ");
    scanf(" %[^\n]", contact.phone);

    printf("Enter Email Address: ");
    scanf(" %[^\n]", contact.email);

    printf("Enter Address: ");
    scanf(" %[^\n]", contact.address);

    file = fopen(FILE_NAME, "ab");

    if (file == NULL) {
        printf("Unable to open contact file.\n");
        return;
    }

    if (fwrite(&contact, sizeof(Contact), 1, file) != 1) {
        printf("Error while saving contact.\n");
        fclose(file);
        return;
    }

    fclose(file);

    printf("\nContact added successfully.\n");
}

void displayContacts(void) {
    FILE *file;
    Contact contact;
    int count = 0;

    printf("\n---------- ALL CONTACTS ----------\n");

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        printf("No contacts found.\n");
        return;
    }

    while (fread(&contact, sizeof(Contact), 1, file) == 1) {
        printf("\n====================================\n");
        printf("Contact ID : %d\n", contact.id);
        printf("Name       : %s\n", contact.name);
        printf("Phone      : %s\n", contact.phone);
        printf("Email      : %s\n", contact.email);
        printf("Address    : %s\n", contact.address);
        printf("====================================\n");

        count++;
    }

    if (count == 0) {
        printf("No contacts available.\n");
    }

    fclose(file);
}

void searchContact(void) {
    FILE *file;
    Contact contact;
    int id;
    int found = 0;

    printf("\n---------- SEARCH CONTACT ----------\n");

    printf("Enter Contact ID: ");
    scanf("%d", &id);

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        printf("No contacts found.\n");
        return;
    }

    while (fread(&contact, sizeof(Contact), 1, file) == 1) {
        if (contact.id == id) {
            found = 1;

            printf("\nContact Found\n");
            printf("====================================\n");
            printf("Contact ID : %d\n", contact.id);
            printf("Name       : %s\n", contact.name);
            printf("Phone      : %s\n", contact.phone);
            printf("Email      : %s\n", contact.email);
            printf("Address    : %s\n", contact.address);
            printf("====================================\n");

            break;
        }
    }

    if (!found) {
        printf("Contact not found.\n");
    }

    fclose(file);
}

void updateContact(void) {
    FILE *file;
    Contact contact;
    int id;
    int found = 0;

    printf("\n---------- UPDATE CONTACT ----------\n");

    printf("Enter Contact ID: ");
    scanf("%d", &id);

    file = fopen(FILE_NAME, "rb+");

    if (file == NULL) {
        printf("No contacts found.\n");
        return;
    }

    while (fread(&contact, sizeof(Contact), 1, file) == 1) {
        if (contact.id == id) {
            found = 1;

            printf("Enter New Name: ");
            scanf(" %[^\n]", contact.name);

            printf("Enter New Phone Number: ");
            scanf(" %[^\n]", contact.phone);

            printf("Enter New Email Address: ");
            scanf(" %[^\n]", contact.email);

            printf("Enter New Address: ");
            scanf(" %[^\n]", contact.address);

            fseek(file, -(long)sizeof(Contact), SEEK_CUR);

            if (fwrite(&contact, sizeof(Contact), 1, file) != 1) {
                printf("Unable to update contact.\n");
                fclose(file);
                return;
            }

            printf("\nContact updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Contact not found.\n");
    }

    fclose(file);
}

void deleteContact(void) {
    FILE *file;
    FILE *temporaryFile;
    Contact contact;
    int id;
    int found = 0;

    printf("\n---------- DELETE CONTACT ----------\n");

    printf("Enter Contact ID: ");
    scanf("%d", &id);

    file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        printf("No contacts found.\n");
        return;
    }

    temporaryFile = fopen("temporary.dat", "wb");

    if (temporaryFile == NULL) {
        printf("Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    while (fread(&contact, sizeof(Contact), 1, file) == 1) {
        if (contact.id == id) {
            found = 1;
        } else {
            fwrite(&contact, sizeof(Contact), 1, temporaryFile);
        }
    }

    fclose(file);
    fclose(temporaryFile);

    if (found) {
        remove(FILE_NAME);
        rename("temporary.dat", FILE_NAME);

        printf("\nContact deleted successfully.\n");
    } else {
        remove("temporary.dat");
        printf("Contact not found.\n");
    }
}