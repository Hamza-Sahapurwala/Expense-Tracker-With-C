#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>  


void clear_screen();
void clear_input_buffer();
void print_header(const char *title);
void display_main_menu();
void display_user_menu(const char *username);
void signup();
void newsignup(char u[]);
void login();
int logincheck(char u[], char p[]);
int signupcheck(char u[], char p[]);
void login_successful(char u[]);
void add_record(char u[]);
int category(char *c);
void see_record(char u[]);
void edit_record(char u[]);
void delete_record(char u[]);
void generate_bar_graph(char u[]);
#define USER_DATA_PATH ".\\UserData\\" 

int main() {
    int inputforuser;

    while (1) {
        clear_screen();
        print_header("EXPENSE MANAGEMENT SYSTEM");
        display_main_menu();

        if (scanf("%d", &inputforuser) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            printf("Press Enter to continue...");
            getchar(); 
            getchar(); 
            continue;
        }
        clear_input_buffer(); 

        switch (inputforuser) {
            case 1:
                login();
                break;
            case 2:
                signup();
                break;
            case 3:
                clear_screen();
                printf("Thank you for using the Expense Management System!\n");
                printf("Return For Managing Your Expenses!\n");
                return 0;
            default:
                printf("Invalid choice. Please choose from 1-3.\n");
                printf("Press Enter to continue...");
                getchar(); 
        }
    }
    return 0; 
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear"); 
#endif
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_header(const char *title) {
    printf("========================================\n");
    printf("        %s\n", title);
    printf("========================================\n\n");
}

void display_main_menu() {
    printf("Welcome!!!\n\n");
    printf("What would you like to do?\n");
    printf("1. Login\n");
    printf("2. Sign-Up\n");
    printf("3. Exit\n");
    printf("Enter 1-3: ");
}

void signup() {
    char username[100], password[100];
    clear_screen();
    print_header("SIGN-UP SCREEN");

    printf("Enter Username (No Spaces): ");
    scanf("%99s", username); 
    clear_input_buffer();

    printf("Enter Password (No Spaces, min 4 chars): ");
    scanf("%99s", password); 
    clear_input_buffer();

    if (signupcheck(username, password)) {
        FILE *sign = fopen("users.csv", "a");
        if (sign == NULL) {
            perror("Error opening users.csv for signup");
            printf("Press Enter to continue...");
            getchar();
            return;
        }
        fprintf(sign, "%s|%s\n", username, password);
        fclose(sign);

        newsignup(username); 

        printf("\nSignup successful! You will now be logged in.\n");
        printf("Press Enter to continue...");
        getchar();
        login_successful(username);
    } else {
        printf("\nSignup failed. Please check the requirements or try a different username.\n");
        printf("Press Enter to continue...");
        getchar();
    }
}

void newsignup(char u[]) {
    char filename[200];
    sprintf(filename, "%s%s.csv", USER_DATA_PATH, u); 

    FILE *newsign = fopen(filename, "w"); 
    if (newsign == NULL) {
        perror("Error creating user data file");
        #ifdef _WIN32
            char command[250];
            sprintf(command, "mkdir %s", USER_DATA_PATH);
            system(command);
        #else
            char command[250];
            sprintf(command, "mkdir -p %s", USER_DATA_PATH); 
            system(command);
        #endif
        newsign = fopen(filename, "w");
        if (newsign == NULL) {
            printf("Could not create user data directory or file: %s\n", filename);
            printf("Please ensure the directory '%s' exists.\n", USER_DATA_PATH);
            return;
        }
    }
    fclose(newsign);
    printf("User data file created: %s\n", filename); 
}

void login() {
    char username[100], password[100];
    clear_screen();
    print_header("LOGIN SCREEN");

    printf("Enter Username (No Spaces): ");
    scanf("%99s", username);
    clear_input_buffer();

    printf("Enter Password (No Spaces): ");
    scanf("%99s", password);
    clear_input_buffer();

    if (logincheck(username, password)) {
        printf("\nLogin Successful!\n");
        printf("Press Enter to continue...");
        getchar();
        login_successful(username);
    } else {
        printf("\nFailed Login! Invalid username or password. Try Again.\n");
        printf("Press Enter to continue...");
        getchar();
    }
}

int logincheck(char u[], char p[]) {
    FILE *f = fopen("users.csv", "r");
    if (f == NULL) {
        printf("No user accounts found. Please sign up first.\n");
        return 0;
    }

    char line[200]; 
    while (fgets(line, sizeof(line), f)) {
        char fileUsername[100], filePassword[100];
        line[strcspn(line, "\n")] = 0; 

        char *token = strtok(line, "|");
        if (token != NULL) {
            strncpy(fileUsername, token, sizeof(fileUsername) - 1);
            fileUsername[sizeof(fileUsername)-1] = '\0'; 

            token = strtok(NULL, "|");
            if (token != NULL) {
                strncpy(filePassword, token, sizeof(filePassword) - 1);
                filePassword[sizeof(filePassword)-1] = '\0'; 

                if (strcmp(u, fileUsername) == 0 && strcmp(p, filePassword) == 0) {
                    fclose(f);
                    return 1; 
                }
            }
        }
    }
    fclose(f);
    return 0; 
}

int signupcheck(char u[], char p[]) {
    if (strlen(u) == 0) {
        printf("Username can't be empty.\n");
        return 0;
    }
    if (strchr(u, ' ') != NULL || strchr(u, '|') != NULL) { 
        printf("Username cannot contain spaces or '|' character.\n");
        return 0;
    }
    if (strlen(p) == 0) {
        printf("Password can't be empty.\n");
        return 0;
    }
    if (strlen(p) < 4) {
        printf("Password must be at least 4 characters long.\n");
        return 0;
    }
    if (strchr(p, ' ') != NULL || strchr(p, '|') != NULL) { 
        printf("Password cannot contain spaces or '|' character.\n");
        return 0;
    }


    FILE *f = fopen("users.csv", "r");
    if (f == NULL) {
        return 1; 
    }

    char line[200];
    while (fgets(line, sizeof(line), f)) {
        char fileUsername[100];
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, "|"); 
        if (token != NULL) {
            strncpy(fileUsername, token, sizeof(fileUsername) -1);
            fileUsername[sizeof(fileUsername)-1] = '\0';

            if (strcmp(u, fileUsername) == 0) {
                printf("The Username '%s' has already been taken!\n", u);
                fclose(f);
                return 0; 
            }
        }
    }
    fclose(f);
    return 1; 
}

void display_user_menu(const char *username) {
    char title[100];
    sprintf(title, "USER: %s - MENU", username);
    print_header(title);
    printf("What would you like to do?\n");
    printf("1. Add Record\n");
    printf("2. Delete Record\n");
    printf("3. Edit Record\n");
    printf("4. See Records\n");
    printf("5. See Bar Graph of Expenses\n");
    printf("6. Logout (Go Back to Login/SignUp Page)\n");
    printf("7. Exit Application\n");
    printf("Enter 1-7: ");
}

void login_successful(char u[]) {
    int input;
    while (1) {
        clear_screen();
        display_user_menu(u);

        if (scanf("%d", &input) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            printf("Press Enter to continue...");
            getchar();
            getchar();
            continue;
        }
        clear_input_buffer();

        switch (input) {
            case 1: add_record(u); break;
            case 2: delete_record(u); break;
            case 3: edit_record(u); break;
            case 4: see_record(u); break;
            case 5: generate_bar_graph(u); break;
            case 6:
                printf("Logging out...\n");
                printf("Press Enter to return to the main menu...");
                getchar();
                return; 
            case 7:
                clear_screen();
                printf("Thank you for using the Expense Management System!\n");
                printf("Exiting application...\n");
                exit(0);
            default:
                printf("Invalid choice. Please choose between 1-7.\n");
                printf("Press Enter to continue...");
                getchar();
        }
         if (input >= 1 && input <= 5) { 
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
}

int category(char *c_out) { 
    int choice;
    clear_screen();
    print_header("SELECT CATEGORY");
    while (1) {
        printf("Enter Your Category:\n");
        printf("1. Food\n");
        printf("2. Transport\n");
        printf("3. Clothing\n");
        printf("4. Miscellaneous\n"); 
        printf("Enter 1-4: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            printf("Press Enter to try again...");
            getchar();
            getchar();
            clear_screen();
            print_header("SELECT CATEGORY");
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: strcpy(c_out, "Food"); return 0;
            case 2: strcpy(c_out, "Transport"); return 0;
            case 3: strcpy(c_out, "Clothing"); return 0;
            case 4: strcpy(c_out, "Miscellaneous"); return 0; 
            default:
                printf("Invalid choice. Please choose from 1-4.\n");
                printf("Press Enter to try again...");
                getchar();
                clear_screen();
                print_header("SELECT CATEGORY");
        }
    }
}

void add_record(char u[]) {
    char filename[200];
    sprintf(filename, "%s%s.csv", USER_DATA_PATH, u);

    float amt;
    char date[12], cat[30], line[256];
    int current_id = 0; 

    clear_screen();
    print_header("ADD NEW RECORD");

    FILE *read_file = fopen(filename, "r");
    if (read_file != NULL) {
        while (fgets(line, sizeof(line), read_file)) {
            int temp_id;
            if (sscanf(line, "%d|", &temp_id) == 1) {
                if (temp_id > current_id) {
                    current_id = temp_id;
                }
            }
        }
        fclose(read_file);
    } else {
        printf("Note: No existing records found or file not accessible. Starting with ID 1.\n");
    }
    int new_id = current_id + 1;


    printf("Enter Amount: ");
    if (scanf("%f", &amt) != 1) {
        printf("Invalid amount entered.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("Enter Date (e.g., YYYY-MM-DD): ");
    scanf("%11s", date); 
    clear_input_buffer();

    category(cat); 

    FILE *addrecord_file = fopen(filename, "a"); 
    if (addrecord_file == NULL) {
        perror("Error opening user data file for adding record");
        printf("Failed to open file: %s\n", filename);
        return;
    }

    fprintf(addrecord_file, "%d|%.2f|%s|%s\n", new_id, amt, date, cat);
    fclose(addrecord_file);

    printf("\nRecord added successfully with ID: %d!\n", new_id);
}

void see_record(char u[]) {
    char filename[200], line[256]; 
    sprintf(filename, "%s%s.csv", USER_DATA_PATH, u);

    clear_screen();
    print_header("VIEW RECORDS");

    FILE *seerecord = fopen(filename, "r");
    if (seerecord == NULL) {
        printf("No records found for user '%s' or file could not be opened.\n", u);
        printf("Path: %s\n", filename);
        return;
    }

    printf("%-5s | %-10s | %-12s | %-15s\n", "ID", "Amount", "Date", "Category");
    printf("------|------------|--------------|-----------------\n");

    int count = 0;
    while (fgets(line, sizeof(line), seerecord)) {
        char id_str[10], amount_str[20], date_str[20], category_str[50];
        line[strcspn(line, "\n")] = 0; 

        char *token = strtok(line, "|");
        if (token) strncpy(id_str, token, sizeof(id_str)-1); else strcpy(id_str, "N/A");
        id_str[sizeof(id_str)-1] = '\0';

        token = strtok(NULL, "|");
        if (token) strncpy(amount_str, token, sizeof(amount_str)-1); else strcpy(amount_str, "N/A");
        amount_str[sizeof(amount_str)-1] = '\0';

        token = strtok(NULL, "|");
        if (token) strncpy(date_str, token, sizeof(date_str)-1); else strcpy(date_str, "N/A");
        date_str[sizeof(date_str)-1] = '\0';

        token = strtok(NULL, "\n"); 
        if (token) strncpy(category_str, token, sizeof(category_str)-1); else strcpy(category_str, "N/A");
        category_str[sizeof(category_str)-1] = '\0';

        printf("%-5s | %-10s | %-12s | %-15s\n", id_str, amount_str, date_str, category_str);
        count++;
    }
    fclose(seerecord);

    if (count == 0) {
        printf("\nNo records found in the file.\n");
    } else {
        printf("------|------------|--------------|-----------------\n");
        printf("Total records: %d\n", count);
    }
}

struct TempRecord {
    int id;
    float amount;
    char date[50];
    char category[50];
};

void edit_record(char u[]) {
    char filename[200];
    sprintf(filename, "%s%s.csv", USER_DATA_PATH, u);

    clear_screen();
    print_header("EDIT RECORD");

    see_record(u); 
    printf("\n----------------------------------------\n");
    printf("           EDIT RECORD SECTION\n");
    printf("----------------------------------------\n");


    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No records found to edit or file cannot be opened: %s\n", filename);
        return;
    }

    struct TempRecord records[200]; 
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < 200) {
        if (sscanf(line, "%d|%f|%49[^|]|%49[^\n]", &records[count].id, &records[count].amount, records[count].date, records[count].category) == 4) {
            count++;
        }
    }
    fclose(file);

    if (count == 0) {
        printf("No valid records found in the file to edit.\n");
        return;
    }

    int id_to_edit, found_index = -1;
    printf("\nEnter the ID of the record to edit: ");
    if (scanf("%d", &id_to_edit) != 1) {
        printf("Invalid ID format.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    for (int i = 0; i < count; i++) {
        if (records[i].id == id_to_edit) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("Record with ID %d not found.\n", id_to_edit);
        return;
    }

    printf("\nRecord Found: ID: %d, Amount: %.2f, Date: %s, Category: %s\n",
           records[found_index].id, records[found_index].amount, records[found_index].date, records[found_index].category);

    int choice;
    printf("\nWhat do you want to edit?\n");
    printf("1. Amount\n2. Date\n3. Category\n4. Cancel\n");
    printf("Enter 1-4: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice format.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    switch (choice) {
        case 1:
            printf("Enter new Amount: ");
            if (scanf("%f", &records[found_index].amount) != 1) {
                printf("Invalid amount format.\n"); clear_input_buffer(); return;
            }
            clear_input_buffer();
            break;
        case 2:
            printf("Enter new Date (YYYY-MM-DD): ");
            scanf("%49s", records[found_index].date);
            clear_input_buffer();
            break;
        case 3:
            category(records[found_index].category); 
            break;
        case 4:
            printf("Edit cancelled.\n");
            return;
        default:
            printf("Invalid choice. Edit cancelled.\n");
            return;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing changes");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%.2f|%s|%s\n", records[i].id, records[i].amount, records[i].date, records[i].category);
    }
    fclose(file);

    printf("\nRecord edited successfully!\n");
}

void delete_record(char u[]) {
    char filename[200];
    sprintf(filename, "%s%s.csv", USER_DATA_PATH, u);

    clear_screen();
    print_header("DELETE RECORD");

    see_record(u); 
    printf("\n----------------------------------------\n");
    printf("         DELETE RECORD SECTION\n");
    printf("----------------------------------------\n");


    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No records found to delete or file cannot be opened: %s\n", filename);
        return;
    }

    struct TempRecord records[200];
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < 200) {
         if (sscanf(line, "%d|%f|%49[^|]|%49[^\n]", &records[count].id, &records[count].amount, records[count].date, records[count].category) == 4) {
            count++;
        }
    }
    fclose(file);

    if (count == 0) {
        printf("No valid records found in the file to delete.\n");
        return;
    }

    int id_to_delete;
    printf("\nEnter the ID of the record to delete: ");
     if (scanf("%d", &id_to_delete) != 1) {
        printf("Invalid ID format.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int found = 0;
    file = fopen(filename, "w"); 
    if (file == NULL) {
        perror("Error opening file for writing changes after delete");
        return;
    }

    int new_id_counter = 1; 
    for (int i = 0; i < count; i++) {
        if (records[i].id == id_to_delete) {
            found = 1;
        } else {
            fprintf(file, "%d|%.2f|%s|%s\n", new_id_counter, records[i].amount, records[i].date, records[i].category);
            new_id_counter++;
        }
    }
    fclose(file);

    if (found) {
        printf("\nRecord with ID %d deleted successfully. Records have been re-indexed.\n", id_to_delete);
    } else {
        printf("\nRecord with ID %d not found.\n", id_to_delete);
    }
}

void generate_bar_graph(char u[]) {
    char filename[200];
    sprintf(filename, "%s%s.csv", USER_DATA_PATH, u);

    clear_screen();
    print_header("EXPENSE BAR GRAPH");

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No records found to generate graph for user '%s'.\n", u);
        printf("Path: %s\n", filename);
        return;
    }

    float food = 0, transport = 0, clothing = 0, misc = 0;
    char line[256], category_val[50]; 
    float amount;
    int id; 
    char date[50]; 

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d|%f|%49[^|]|%49[^\n]", &id, &amount, date, category_val) == 4) {
            if (strcmp(category_val, "Food") == 0)
                food += amount;
            else if (strcmp(category_val, "Transport") == 0)
                transport += amount;
            else if (strcmp(category_val, "Clothing") == 0)
                clothing += amount;
            else if (strcmp(category_val, "Miscellaneous") == 0) 
                misc += amount;
        }
    }
    fclose(file);

    if (food == 0 && transport == 0 && clothing == 0 && misc == 0) {
        printf("No expense data to plot.\n");
        return;
    }

    FILE *data_file = fopen("graphdata.dat", "w");
    if (!data_file) {
        perror("Failed to create graphdata.dat");
        return;
    }
    fprintf(data_file, "Category Amount\n"); 
    fprintf(data_file, "Food %.2f\n", food);
    fprintf(data_file, "Transport %.2f\n", transport);
    fprintf(data_file, "Clothing %.2f\n", clothing);
    fprintf(data_file, "Miscellaneous %.2f\n", misc); 
    fclose(data_file);

    printf("Attempting to generate bar graph using Gnuplot...\n");
    printf("If Gnuplot is not installed or not in PATH, this will fail.\n");
    printf("Data for graph written to graphdata.dat\n\n");
    printf("Food: %.2f\n", food);
    printf("Transport: %.2f\n", transport);
    printf("Clothing: %.2f\n", clothing);
    printf("Miscellaneous: %.2f\n\n", misc);


    FILE *gnuplot_pipe;
#ifdef _WIN32
    gnuplot_pipe = _popen("gnuplot -persistent", "w");
#else
    if (system("command -v gnuplot >/dev/null 2>&1") != 0) {
         printf("Gnuplot is not installed or not in PATH. Cannot generate graph.\n");
         printf("Please install Gnuplot and ensure it's in your system's PATH.\n");
         printf("You can view the data in 'graphdata.dat'.\n");
         return;
    }
    gnuplot_pipe = popen("gnuplot -persistent", "w");
#endif

    if (gnuplot_pipe) {
        fprintf(gnuplot_pipe, "set title 'Expenses by Category for %s'\n", u);
        fprintf(gnuplot_pipe, "set style data histograms\n");
        fprintf(gnuplot_pipe, "set style fill solid 0.7 border -1\n"); 
        fprintf(gnuplot_pipe, "set boxwidth 0.8 relative\n"); 
        fprintf(gnuplot_pipe, "set xlabel 'Category'\n");
        fprintf(gnuplot_pipe, "set ylabel 'Amount Spent'\n");
        fprintf(gnuplot_pipe, "set yrange [0:*]\n"); 
        fprintf(gnuplot_pipe, "set style histogram clustered gap 1\n"); 
        fprintf(gnuplot_pipe, "plot 'graphdata.dat' using 2:xticlabels(1) title 'Expenses' lc rgb '#406090'\n"); // Using xticlabels and color

#ifdef _WIN32
        _pclose(gnuplot_pipe);
#else
        pclose(gnuplot_pipe);
#endif
        printf("Gnuplot command sent. Check for Gnuplot window.\n");
    } else {
        printf("Failed to launch Gnuplot.\n");
        printf("Please ensure Gnuplot is installed and in your system's PATH.\n");
        printf("You can manually plot 'graphdata.dat' using Gnuplot with the following commands:\n");
        printf("  set title 'Expenses by Category for %s'\n", u);
        printf("  set style data histograms\n");
        printf("  set style fill solid 0.7 border -1\n");
        printf("  set boxwidth 0.8 relative\n");
        printf("  set xlabel 'Category'\n");
        printf("  set ylabel 'Amount Spent'\n");
        printf("  set yrange [0:*]\n");
        printf("  plot 'graphdata.dat' using 2:xticlabels(1) title 'Expenses' lc rgb '#406090'\n");
    }
}