#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SECRET_STORAGE "/proc/secret_storage"
#define SECRET_STORAGE_DELETE "/proc/secret_storage_delete"

void write_secret(const char *data) {
    FILE *file = fopen(SECRET_STORAGE, "w");
    if (file == NULL) {
        perror("Failed to open secret storage for writing");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", data);
    fclose(file);
}

void read_secrets() {
    char buffer[256];
    FILE *file = fopen(SECRET_STORAGE, "r");
    if (file == NULL) {
        perror("Failed to open secret storage for reading");
        exit(EXIT_FAILURE);
    }
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
}

void delete_secret(int id) {
    FILE *file = fopen(SECRET_STORAGE_DELETE, "w");
    if (file == NULL) {
        perror("Failed to open secret storage for deleting");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d", id);
    fclose(file);
}

void menu() {
    int choice;
    char data[256];
    int id;

    while (1) {
        printf("\nSecret Storage Menu:\n");
        printf("1. Write Secret\n");
        printf("2. Read Secrets\n");
        printf("3. Delete Secret\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter secret data: ");
                scanf(" %[^\n]", data);  // Read a line of input
                write_secret(data);
                break;
            case 2:
                read_secrets();
                break;
            case 3:
                printf("Enter secret ID to delete: ");
                scanf("%d", &id);
                delete_secret(id);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}

