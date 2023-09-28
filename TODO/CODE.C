#include "MAIN.H"

// Task struct type:
typedef struct {
    int id;
    char title[20]; 
    char description[100]; 
    char deadline[20]; 
    char status[10];
} Task;

// A function that clean the buffer: 
void clean_buffer() {
    while (getchar() != '\n');
}

// A function that ask the user if he want to exit:
void back_menu() {
    char exit;
    do {
        printf("\t>Do you want to exit? (y/n): "); 
        if (scanf(" %c", &exit) != 1) exit = 'n';
        clean_buffer();
    } while (exit != 'y');
}

// A function that hundle input errors in case of string:
void input_string_handler(char* title, int string_size, const char* content) {
    char string[string_size];

    // Display the message:
    puts(content);
    printf("\t: ");
    fgets(string, string_size, stdin);
    // Converting the string to the rigth format: 
    if (string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0'; 
    else clean_buffer();
    strcpy(title, string); 
}

// A function that display the menu:
int display_menu() {
    int task_number;
    do {
        printf("\n\n--------------------------SIMPLE TODO APPLICATION:------------------------------\n");
        printf("1.Add A New Task: \n"); 
        printf("2.Add Multiple New Tasks: \n"); 
        printf("3.Display All The Tasks: \n"); 
        printf("4.Update A Taks: \n"); 
        printf("5.Delete A Task by Its ID: \n"); 
        printf("6.Search Tasks: \n");
        printf("7.Statistics: \n");
        printf("8.Exit The Program: \n");
        printf("> "); 
        if (scanf("%d", &task_number) != 1) task_number = 0; 
        clean_buffer();
    } while (task_number < 1 || task_number > 9);

    return task_number;
}

/* Adding new task with id, title, description, deadline and status */
void current_time(char* current_time) {
    time_t t; 
    time(&t); 

    struct tm *timeinfo = localtime(&t);
    char formattedTime[20]; // To store the formatted time
    // Format the time as "SS/MM/HH/DD/MM/YYYY"
    strftime(formattedTime, sizeof(formattedTime), "%d/%m/%Y", timeinfo);
    strcpy(current_time, formattedTime);
}

bool isValidDate(int day, int month, int year) {
    // Getting the current time on That format "DD/MM/YYYY"
    char formattedTime[20]; // With calling current_time Now formattedTime is on that format ""DD/MM/YYYY"
    current_time(formattedTime);

    // Parsing day, month and year into separate d, m, y variables:
    char d[3]; 
    char m[3]; 
    char y[5]; 
    sscanf(formattedTime, "%2[^/]/%2[^/]/%4[^/]", d, m, y); 

    int currentDay = atoi(d);  
    int currentMonth = atoi(m);
    int currentYear = atoi(y);

    // Check if the entered year is within a reasonable range
    if (year < currentYear || year > currentYear + 5) {
        return false;
    }

    // Check if the entered month is within a valid range
    if (month < 1 || month > 12) {
        return false;
    }

    // Check if the entered day is within a valid range for the given month
    int maxDay = 31; // Maximum days in a month (initial value for simplicity)

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;
    } else if (month == 2) {
        // Check for leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            maxDay = 29;
        } else {
            maxDay = 28;
        }
    }

    if (day < 1 || day > maxDay) {
        return false;
    }

    // Finally, check if the entered date is in the future
    if (year < currentYear || (year == currentYear && (month < currentMonth || (month == currentMonth && day < currentDay)))) {
        return false;
    }

    return true;
}

// Asking the user to enter the date (The deadline):
void input_deadline(char* dead) {
    char input[20];
    // The date entered by the user:
    int second, minute, hour, day, month, year;
    second = 0; 
    minute = 0; 
    hour   = 0; 
    // Assuming the date format "DD/MM/YYYY" won't exceed 12 characters
    bool validDate = false; // Initialize validDate as false

    do {
        printf("\tEnter a date in the format DD/MM/YYYY: \n\t: ");
        fgets(input, sizeof(input), stdin);
        // Cleaning the buffer from unexpected stored characters:
        if (input[strlen(input) - 1] != '\n') clean_buffer();
        else input[strlen(input) - 1] = '\0';

        // Check if the input matches the expected format and if the date is valid
        if (sscanf(input, "%d/%d/%d", &day, &month, &year) == 3 && isValidDate(day, month, year)) {
            validDate = true; // Set validDate to true if the date is valid
        } else {
            printf("\tInvalid date format or date. Please enter a valid date in DD/MM/YYYY format.\n");
        }

    } while (!validDate); // Continue looping until a valid date is entered

    strcpy(dead, input);
}

// A function that add a new task:
void add_new_task(Task* tasks, int* tasks_index, int* size, int caller_checker, int* id_tracking) {
    // Reallocation the memory when It should be: 
    // Check if the tasks_index exceeds the current size - 1 (since the index is 0-based)
    if (*tasks_index >= *size - 1) {
        // Reallocate memory for tasks, doubling the size
        *size *= 2;
        tasks = (Task*)realloc(tasks, *size * sizeof(Task));
        if (tasks == NULL) {
            printf("\tMemory allocation failed.\n");
            exit(1); // Exit the program or handle the error as appropriate
        }
    }

    int * id = &tasks[*(tasks_index)].id; 
    char* title = tasks[*(tasks_index)].title; 
    char* description = tasks[*(tasks_index)].description;
    char* status = tasks[*(tasks_index)].status;

    // Increasing Id By 1, So each task will get a unic ID:
    (* id_tracking)++;
    (* id) = (* id_tracking);
    // Asking the user to enter the title of the task:
    input_string_handler(title, 20, "\tEnter The Title Of The Task (Must Start By A Character): "); 
    
    // Asking the user to enter the description of the task: 
    input_string_handler(description, 100, "\tEnter The description Of The Task (Must Start By A Character): ");   

    // Asking the user to enter the deadline of the task:
    input_deadline(tasks[* tasks_index].deadline);

    // Asking the user to enter the status of the task:
    int choise;
    do {
        printf("\tEnter The status Of The Task: \n"); 
        printf("\t1->TODO\n");
        printf("\t2->DOING\n");
        printf("\t3->DONE\n\t");
        if(scanf("%d", &choise) != 1) choise = 0;
        clean_buffer();
    } while(choise != 1 && choise != 2 && choise != 3);
    switch(choise) {
        case 1: strcpy(status, "TODO");
        break; 
        case 2: strcpy(status, "DOING");
        break; 
        case 3: strcpy(status, "DONE");
        break;
    }
    // Increasing tasks_index by 1: 
    (* tasks_index)++;

    // Back to the menu:
    if (caller_checker == 1) back_menu();
}

// A function that add multiple new tasks: 
void add_multiple_new_tasks(Task* tasks, int* tasks_index, int* size, int* id_tracking) {
    // Determining the Number of tasks first of all:
    int tasks_number;
    do {
        printf("\tEnter the number of tasks that you want to add (number must be > 0): ");
        if (scanf("%d", &tasks_number) != 1) tasks_number = 0;
        clean_buffer();
    } while (tasks_number <= 0); 
    
    // Calling the add_new_task function n number depending on the tasks_number:
    for (int i = 0; i < tasks_number; i++) {
        printf("\n\t>>Task %d:\n", i + 1);

        // 0 as the third argument is just to avoid displaying the `back_menu()` function when calling `add_new_task()` function multiple times:
        add_new_task(tasks, tasks_index, size, 0, id_tracking);
    }

    // back to the menu: 
    back_menu();
}

// A function that check if there is a tasks on the list to handle or not:
int is_there_a_task(int* tasks_index) {
    if (* tasks_index == 0) {
        printf("\tThere is no task on the list.\n");
        return 0;
    }
    return 1;
}

/* Display the list of all tasks (Identifier, Title, Description, Deadline, Status): */
// In Alphatbetically Order:
void display_alphabetically(Task* tasks, int* tasks_index) {
    // Bubble Sort Algorithm: 
    for (int i = 0; i < * tasks_index; i++) {
        for (int j = 1; j < * tasks_index - i; j++) {
            if (strcmp(tasks[j - 1].title, tasks[j].title) > 0) {
                Task swap = tasks[j - 1]; 
                tasks[j - 1] = tasks[j]; 
                tasks[j] = swap;
            }
        }
    }
}

// By Deadline:
void display_by_deadline(Task* tasks, int* tasks_index) {
    // Bubble Sort Algorithm on the parsing dates:
    for (int i = 0; i < *tasks_index - 1; i++) {
        for (int j = 0; j < *tasks_index - i - 1; j++) {
            // Parsing1 day, month, and year into separate d, m, y variables for the current task
            char d1[3];
            char m1[3];
            char y1[5];
            sscanf(tasks[j].deadline, "%2[^/]/%2[^/]/%4[^/]", d1, m1, y1);

            int currentDay1 = atoi(d1);
            int currentMonth1 = atoi(m1);
            int currentYear1 = atoi(y1);

            // Parsing2 day, month, and year into separate d, m, y variables for the next task
            char d2[3];
            char m2[3];
            char y2[5];
            sscanf(tasks[j + 1].deadline, "%2[^/]/%2[^/]/%4[^/]", d2, m2, y2);

            int currentDay2 = atoi(d2);
            int currentMonth2 = atoi(m2);
            int currentYear2 = atoi(y2);

            if (currentYear2 < currentYear1 ||
                (currentYear2 == currentYear1 && currentMonth2 < currentMonth1) ||
                (currentYear2 == currentYear1 && currentMonth2 == currentMonth1 && currentDay2 < currentDay1)) {
                Task swap = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = swap;
            }
        }
    }
}

// Display the tasks that their Deadline less or equal than 3 Days:
void display_3_days(Task* tasks, int* tasks_index) {
    // Get the current date
    char formattedTime[20];
    current_time(formattedTime);
    char dLocal[3];
    char mLocal[3];
    char yLocal[5];
    sscanf(formattedTime, "%2[^/]/%2[^/]/%4[^/]", dLocal, mLocal, yLocal);

    int currentDayLocal = atoi(dLocal);
    int currentMonthLocal = atoi(mLocal);
    int currentYearLocal = atoi(yLocal);

    // Iterate through tasks and display those with a deadline within 3 days
    for (int i = 0; i < *tasks_index; i++) {
        // Parsing the task's deadline to d, m, y:
        char d[3];
        char m[3];
        char y[5];
        sscanf(tasks[i].deadline, "%2[^/]/%2[^/]/%4[^/]", d, m, y);

        int taskDay = atoi(d);
        int taskMonth = atoi(m);
        int taskYear = atoi(y);

        // Calculate the difference in days between the task's deadline and the current date
        int daysDifference = (taskYear - currentYearLocal) * 365 + (taskMonth - currentMonthLocal) * 30 + (taskDay - currentDayLocal);

        // Display the task if the deadline is within 3 days or less
        if (daysDifference <= 3) {
            printf("\t>> %s\n", tasks[i].title);
            printf("\t-- ID          : %d\n", tasks[i].id);
            printf("\t-- DESCRIPTION : %s\n", tasks[i].description);
            printf("\t-- DEADLINE    : %s\n", tasks[i].deadline);
            printf("\t-- STATUS      : %s\n", tasks[i].status);
            printf("\n");
        }
    }

    back_menu();
}


// A function that display all the tasks: 
void display_all_tasks(Task* tasks, int* tasks_index) {
    int checker = is_there_a_task(tasks_index);
    // Handling the kind of displaying the stocked tasks:
    if (checker == 1) {
        int choise;
        do {
            printf("\t1.Sort tasks alphabetically: \n"); 
            printf("\t2.Sort tasks by deadline: \n"); 
            printf("\t3.Show tasks whose deadline is in 3 days or less: \n");
            printf("\t> ");
            if (scanf("%d", &choise) != 1) choise = 0;
            clean_buffer();
        } while (choise < 1 || choise > 3);
        
        switch(choise) {
            case 1: display_alphabetically(tasks, tasks_index);
            break; 
            case 2: display_by_deadline(tasks, tasks_index);
            break; 
            case 3: display_3_days(tasks, tasks_index);
            break;
        }

        for (int i = 0; i < * tasks_index; i++) {
            printf("\n\t>> %s\n", tasks[i].title);
            printf("\t-- ID          : %d\n", tasks[i].id);
            printf("\t-- DESCRIPTION : %s\n", tasks[i].description);
            printf("\t-- DEADLINE    : %s\n", tasks[i].deadline);
            printf("\t-- STATUS      : %s\n", tasks[i].status);
            printf("\n");
        }
    }

    back_menu();
}

// A function that update task: 
int find_index_ID(Task* tasks, int* tasks_index, int task_id) {
    for (int i = 0; i < * tasks_index; i++) {
        if (tasks[i].id == task_id) {
            return i;
        }
    }
    return -1;
}

void update_task(Task* tasks, int* tasks_index) {
    int checker = is_there_a_task(tasks_index); 

    if (checker == 1) {
        int task_id;
        do {
            printf("\tEnter the ID of the task To update: "); 
            if(scanf("%d", &task_id) != 1) task_id = 0;
            clean_buffer();
        } while (task_id <= 0);
    
        int index_id = find_index_ID(tasks, tasks_index, task_id);
        // Checking if the entered ID is valid or not:
        if (index_id != -1) {
            int choise;
            do {
                do {
                    printf("\t1.Description %s\n", tasks[index_id].description); 
                    printf("\t2.Status %s\n", tasks[index_id].status); 
                    printf("\t3.Deadline %s\n", tasks[index_id].deadline);
                    printf("\t4.Exit\n"); 
                    printf("\t1-4>");
                    if (scanf("%d", &choise) != 1) choise = 0;
                    clean_buffer();
                } while (choise < 1 || choise > 4);

                switch(choise) {
                    case 1: input_string_handler(tasks[index_id].description, 100, "\tEnter the new Description:");
                    break; 
                    case 2: input_string_handler(tasks[index_id].status, 10, "\tEnter the new Status:");
                    break; 
                    case 3: input_deadline(tasks[index_id].deadline);
                    break;
                }
            } while (choise != 4);

        }else printf("\tNot valid ID!\n");
    }

    back_menu();
}

// A function that delete a task by its ID: 
void delete_task(Task* tasks, int* tasks_index) {
    int checker = is_there_a_task(tasks_index); 

    if (checker == 1) {
        int task_id;
        do {
            printf("\tEnter the ID of the task To Delete: "); 
            if(scanf("%d", &task_id) != 1) task_id = 0;
            clean_buffer();
        } while (task_id <= 0);
    
        int index_id = find_index_ID(tasks, tasks_index, task_id);

        // Checking if the entered ID is valid or not:
        if (index_id != -1) {
            // Each delete we should decrease the (* tasks_index) by 1 to keep controllong the tracking of tasks correctly:
            --(* tasks_index );
            for (int i = index_id; i < * tasks_index; i++) {
                tasks[i] = tasks[i + 1]; 
            }
            printf("\tTask deleted!\n");

        } else printf("\tNot valid ID!\n");
    }

    back_menu();
}

// A function that search a task by its ID: 
void search_by_id(Task* tasks, int* tasks_index) {
    int task_id;
    do {
        printf("\tEnter the ID of the task To Search: "); 
        if(scanf("%d", &task_id) != 1) task_id = 0;
        clean_buffer();
    } while (task_id <= 0);
    
    int index_id = find_index_ID(tasks, tasks_index, task_id);
    if (index_id != -1) {
        for (int i = 0; i < * tasks_index; i++) {
            if (tasks[i].id == task_id) {
                printf("\t>> %s\n", tasks[i].title);
                printf("\t-- ID          : %d\n", tasks[i].id);
                printf("\t-- DESCRIPTION : %s\n", tasks[i].description);
                printf("\t-- DEADLINE    : %s\n", tasks[i].deadline);
                printf("\t-- STATUS      : %s\n", tasks[i].status);
                printf("\n");
            }
        }
    } else printf("\tNot a Valid ID!\n");
}

// A function that search a task by its TITLE: 
void search_by_title(Task* tasks, int* tasks_index) {
    char target_title[20];
    printf("\tEnter the title of the task: ");
    fgets(target_title, sizeof(target_title), stdin);

    if (target_title[strlen(target_title) - 1] == '\n') target_title[strlen(target_title) - 1] = '\0';
    else clean_buffer();
    
    int found = 0;
    for (int i = 0; i < *tasks_index; i++) {
        if (strcmp(tasks[i].title, target_title) == 0) {
            found++;
            printf("\t>> %s\n", tasks[i].title);
            printf("\t-- ID          : %d\n", tasks[i].id);
            printf("\t-- DESCRIPTION : %s\n", tasks[i].description);
            printf("\t-- DEADLINE    : %s\n", tasks[i].deadline);
            printf("\t-- STATUS      : %s\n", tasks[i].status);
            printf("\n");
        }
    }
    
    if (found == 0) printf("\tNot a Valid Title!\n");
}

// Search Function:
void search_Tasks(Task* tasks, int* tasks_index) {
    int checker = is_there_a_task(tasks_index); 

    if (checker == 1) {
        int choise;
        do {
            printf("\t1.Search By ID:\n"); 
            printf("\t2.Search By Title\n"); 
            printf("\t1-2> ");
            if (scanf("%d", &choise) != 1) choise = 0;
            clean_buffer();
        } while (choise < 1 || choise > 2);

        switch(choise) {
            case 1: search_by_id(tasks, tasks_index);
            break; 
            case 2: search_by_title(tasks, tasks_index);
            break; 
        }
    }

    back_menu();
}

/* A function that display the statitistics such us complete, uncomplete works, the rest time of each task compared to its deadline: */
void statistics(Task* tasks, int* tasks_index) {
    int checker = is_there_a_task(tasks_index);
    if (checker == 1) {
        int completed = 0;
        int incomplete = 0;

        for (int i = 0; i < *tasks_index; i++) {
            if (strcmp(tasks[i].status, "DONE") == 0) {
                completed++;
            } else {
                incomplete++;
            }
        }

        printf("\t>STATISTICS OF THE TASKS:\n");
        printf("\tTotal tasks: %d\n", *tasks_index);
        printf("\tCompleted tasks: %d\n", completed);
        printf("\tIncomplete tasks: %d\n", incomplete);
    }
    
    back_menu();
}

int main() {
    int size = 100;
    int tasks_index = 0;
    Task* tasks = (Task*)calloc(size, sizeof(Task));

    // The first value of id set to 0:
    int id_tracking = 0;
    int exit_program = 1;
    do {
        int task_number = display_menu();
        switch(task_number) {
            // 1 as the third argument is just to display the `back_menu()` function at `add_new_task()` function:
            case 1: add_new_task(tasks, &tasks_index, &size, 1, &id_tracking); 
            break; 
            case 2: add_multiple_new_tasks(tasks, &tasks_index, &size, &id_tracking); 
            break; 
            case 3: display_all_tasks(tasks, &tasks_index); 
            break;
            case 4: update_task(tasks, &tasks_index); 
            break; 
            case 5: delete_task(tasks, &tasks_index); 
            break; 
            case 6: search_Tasks(tasks, &tasks_index); 
            break;
            case 7: statistics(tasks, &tasks_index); 
            break;
            
            case 8: exit_program = 0;
            break;
        }
    } while (exit_program);

    // Reallocation of taskes: 
    free(tasks); 

    return 0;
}

// END OF PROGRAM:
//s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s