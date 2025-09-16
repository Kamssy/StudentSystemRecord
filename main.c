#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define INITIAL_CAPACITY 2
#define MAX_NAME_LENGTH 100
#define MAX_USERNAME_LENGTH 50
#define FILENAME "students.txt"

struct Student {
    char name[MAX_NAME_LENGTH];
    int roll;
    float marks;
};

// === Memory Management Functions ===
struct Student* allocateMemory(int maxStudents) {
    struct Student* students = (struct Student*)malloc(maxStudents * sizeof(struct Student));
    if (students == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    return students;
}

struct Student* reallocateMemory(struct Student* students, int* maxStudents, int newSize) {
    struct Student* temp = (struct Student*)realloc(students, newSize * sizeof(struct Student));
    if (temp == NULL) {
        printf("Memory reallocation failed!\n");
        free(students);
        exit(1);
    }
    *maxStudents = newSize;
    return temp;
}

void freeMemory(struct Student* students) {
    free(students);
    printf("Memory freed successfully!\n");
}

// === Utility Functions ===
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printStudent(struct Student s) {
    printf("\nName: %s\n", s.name);
    printf("Roll Number: %d\n", s.roll);
    printf("Marks: %.2f\n", s.marks);
    printf("Status: %s\n", s.marks >= 40 ? "Passed" : "Failed");
}

void sortStudents(struct Student arr[], int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((ascending && arr[i].marks > arr[j].marks) ||
                (!ascending && arr[i].marks < arr[j].marks)) {
                struct Student temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int validateName(char *name) {
    if (strlen(name) <= 1) {
        printf("Name cannot be empty.\n");
        return 0;
    }
    for (int i = 0; name[i] != '\0' && name[i] != '\n'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            printf("Name should contain only letters and spaces.\n");
            return 0;
        }
    }
    return 1;
}

void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Add Student\n2. Show Students\n3. Modify Student\n");
    printf("4. Average Marks\n5. Sort Ascending\n6. Sort Descending\n");
    printf("7. Save to File\n8. Load from File\n0. Exit\n");
}

void promptMenuOrExit(char *username) {
    char menuChoice;
    printf("\nDo you want to go back to the menu? (y/n): ");
    scanf(" %c", &menuChoice);
    clearInputBuffer();

    if (tolower(menuChoice) != 'y') {
        char exitChoice;
        printf("\nDo you want to exit the program? (y/n): ");
        scanf(" %c", &exitChoice);
        clearInputBuffer();

        if (tolower(exitChoice) == 'y') {
            printf("Exiting... Goodbye, %s!\n", username);
            exit(0);
        }
    }
}

void saveToFile(struct Student* students, int count, const char* filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "Name: %s\n", students[i].name);
        fprintf(fp, "Roll Number: %d\n", students[i].roll);
        fprintf(fp, "Marks: %.2f\n", students[i].marks);
        fprintf(fp, "Result: %s\n\n", (students[i].marks >= 40) ? "Pass" : "Fail");
    }
    fclose(fp);
    printf("Student records saved to %s.\n", filename);
}

void loadFromFile(struct Student* students, int* count, int* maxStudents, const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("No existing file found.\n");
        return;
    }

    struct Student temp;
    while (fscanf(fp, "%99[^,],%d,%f\n", temp.name, &temp.roll, &temp.marks) == 3) {
        if (*count >= *maxStudents) {
            students = reallocateMemory(students, maxStudents, *maxStudents * 2);
        }
        students[*count] = temp;
        (*count)++;
    }

    fclose(fp);
    printf("Student records loaded from %s.\n", filename);

    printf("\nLoaded Student Records:\n");
    for (int i = 0; i < *count; i++) {
        printf("Name: %s | Roll No: %d | Marks: %.2f\n",
               students[i].name, students[i].roll, students[i].marks);
    }
}

// === MAIN FUNCTION ===
int main() {
    char username[MAX_USERNAME_LENGTH];
    int option;
    int count = 0;
    int maxStudents = INITIAL_CAPACITY;
    struct Student* students = allocateMemory(maxStudents);

    printf("Welcome to the Student Record System!\n");

    while (1) {
        printf("Please enter your name: ");
        if (fgets(username, sizeof(username), stdin)) {
            username[strcspn(username, "\n")] = '\0';
            if (strlen(username) > 0 && validateName(username)) {
                break;
            }
        }
    }

    printf("Hello, %s!\n", username);

    while (1) {
        displayMenu();
        printf("Choose: ");

        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (option == 1) {
            char again = 'y';
            while (again == 'y' || again == 'Y') {
                if (count >= maxStudents) {
                    students = reallocateMemory(students, &maxStudents, maxStudents * 2);
                }

                struct Student s;

                while (1) {
                    printf("Enter student name: ");
                    if (fgets(s.name, sizeof(s.name), stdin)) {
                        if (validateName(s.name)) {
                            s.name[strcspn(s.name, "\n")] = '\0';
                            break;
                        }
                    }
                }

                while (1) {
                    printf("Enter student roll number (1-100): ");
                    if (scanf("%d", &s.roll) != 1) {
                        printf("Invalid input. Please enter a number.\n");
                        clearInputBuffer();
                        continue;
                    }
                    if (s.roll < 1 || s.roll > 100) {
                        printf("Roll number must be between 1 and 100.\n");
                        clearInputBuffer();
                    } else {
                        int duplicate = 0;
                        for (int i = 0; i < count; i++) {
                            if (students[i].roll == s.roll) {
                                printf("Roll number %d already exists.\n", s.roll);
                                duplicate = 1;
                                break;
                            }
                        }
                        if (!duplicate) {
                            clearInputBuffer();
                            break;
                        }
                        clearInputBuffer();
                    }
                }

                while (1) {
                    printf("Enter student marks (0-100): ");
                    if (scanf("%f", &s.marks) != 1) {
                        printf("Invalid input. Please enter a number.\n");
                        clearInputBuffer();
                        continue;
                    }
                    if (s.marks < 0 || s.marks > 100) {
                        printf("Marks must be between 0 and 100.\n");
                    } else {
                        break;
                    }
                    clearInputBuffer();
                }

                students[count++] = s;
                printf("Student added successfully.\n");

                printf("\nDo you want to add another student? (y/n): ");
                scanf(" %c", &again);
                clearInputBuffer();

                if (tolower(again) == 'n') {
                    promptMenuOrExit(username);
                    break;
                }
            }
        }

        else if (option == 2) {
            if (count == 0) {
                printf("No students to show.\n");
            } else {
                int subOption;
                printf("\n1. Show all students\n2. Show a student by roll number\nChoose: ");
                if (scanf("%d", &subOption) != 1) {
                    printf("Invalid input.\n");
                    clearInputBuffer();
                    continue;
                }
                clearInputBuffer();

                if (subOption == 1) {
                    printf("\n--- All Students (%d) ---\n", count);
                    for (int i = 0; i < count; i++) {
                        printStudent(students[i]);
                    }
                } else if (subOption == 2) {
                    int rollSearch;
                    printf("Enter roll number to display: ");
                    if (scanf("%d", &rollSearch) != 1) {
                        printf("Invalid input.\n");
                        clearInputBuffer();
                        continue;
                    }
                    clearInputBuffer();

                    int found = 0;
                    for (int i = 0; i < count; i++) {
                        if (students[i].roll == rollSearch) {
                            printStudent(students[i]);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        printf("No student found with roll number %d.\n", rollSearch);
                    }
                } else {
                    printf("Invalid option.\n");
                }
            }
            promptMenuOrExit(username);
        }

        else if (option == 3) {
            if (count == 0) {
                printf("No students to modify.\n");
                promptMenuOrExit(username);
                continue;
            }

            int rollToModify;
            printf("Enter roll number of student to modify: ");
            if (scanf("%d", &rollToModify) != 1) {
                printf("Invalid input.\n");
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            int found = 0;
            for (int i = 0; i < count; i++) {
                if (students[i].roll == rollToModify) {
                    found = 1;
                    printf("\nCurrent student details:");
                    printStudent(students[i]);

                    printf("\nWhat would you like to do?\n");
                    printf("1. Delete record\n2. Change name\n3. Change roll number\n4. Change marks\nChoose: ");
                    int choice;
                    if (scanf("%d", &choice) != 1) {
                        printf("Invalid input.\n");
                        clearInputBuffer();
                        break;
                    }
                    clearInputBuffer();

                    if (choice == 1) {
                        for (int j = i; j < count - 1; j++) {
                            students[j] = students[j + 1];
                        }
                        count--;
                        printf("Student record deleted.\n");
                    }
                    else if (choice == 2) {
                        while (1) {
                            printf("Enter new name: ");
                            if (fgets(students[i].name, sizeof(students[i].name), stdin)) {
                                if (validateName(students[i].name)) {
                                    students[i].name[strcspn(students[i].name, "\n")] = '\0';
                                    printf("Name updated.\n");
                                    break;
                                }
                            }
                        }
                    }
                    else if (choice == 3) {
                        while (1) {
                            printf("Enter new roll number (1-100): ");
                            if (scanf("%d", &students[i].roll) != 1) {
                                printf("Invalid input. Try again.\n");
                                clearInputBuffer();
                            } else if (students[i].roll < 1 || students[i].roll > 100) {
                                printf("Roll number must be between 1 and 100.\n");
                                clearInputBuffer();
                            } else {
                                int duplicate = 0;
                                for (int j = 0; j < count; j++) {
                                    if (j != i && students[j].roll == students[i].roll) {
                                        printf("Roll number %d already exists.\n", students[i].roll);
                                        duplicate = 1;
                                        break;
                                    }
                                }
                                if (!duplicate) {
                                    clearInputBuffer();
                                    printf("Roll number updated.\n");
                                    break;
                                }
                                clearInputBuffer();
                            }
                        }
                    }
                    else if (choice == 4) {
                        while (1) {
                            printf("Enter new marks (0-100): ");
                            if (scanf("%f", &students[i].marks) != 1) {
                                printf("Invalid input.\n");
                                clearInputBuffer();
                            } else if (students[i].marks < 0 || students[i].marks > 100) {
                                printf("Marks must be between 0 and 100.\n");
                                clearInputBuffer();
                            } else {
                                clearInputBuffer();
                                printf("Marks updated.\n");
                                break;
                            }
                        }
                    }
                    else {
                        printf("Invalid modification option.\n");
                    }
                    break;
                }
            }

            if (!found) {
                printf("No student found with roll number %d.\n", rollToModify);
            }
            promptMenuOrExit(username);
        }

        else if (option == 4) {
            if (count == 0) {
                printf("No students to calculate average.\n");
            } else {
                float total = 0;
                for (int i = 0; i < count; i++) {
                    total += students[i].marks;
                }
                float avg = total / count;
                printf("Average Marks of %d students: %.2f\n", count, avg);
            }
            promptMenuOrExit(username);
        }

        else if (option == 5) {
            if (count == 0) {
                printf("No students to sort.\n");
            } else {
                sortStudents(students, count, 1);
                printf("\n--- Students Sorted in Ascending Order ---\n");
                for (int i = 0; i < count; i++) {
                    printStudent(students[i]);
                }
            }
            promptMenuOrExit(username);
        }

        else if (option == 6) {
            if (count == 0) {
                printf("No students to sort.\n");
            } else {
                sortStudents(students, count, 0);
                printf("\n--- Students Sorted in Descending Order ---\n");
                for (int i = 0; i < count; i++) {
                    printStudent(students[i]);
                }
            }
            promptMenuOrExit(username);
        }

        else if (option == 7) {
            saveToFile(students, count, FILENAME);
            promptMenuOrExit(username);
        }

        else if (option == 8) {
            loadFromFile(students, &count, &maxStudents, FILENAME);
            promptMenuOrExit(username);
        }

        else if (option == 0) {
            printf("Exiting... Goodbye, %s!\n", username);
            break;
        }

        else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    freeMemory(students);
    return 0;
}
