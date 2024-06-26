#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

// Constants
const float VERSION_NUMBER = 1.0;
const char *GROUP_MEMBERS[] = {"Timothy See (2302189)", "Hariz Darwisy Bin Adan (2302221)", "Lan Zexi (2303456)", "Siti Faziera Binti Razale (2303416)", "Chua Yi Xuan (2302090)", "Khairunnisa Bte Yunos (2302147)"};
const char *ODT_COMMANDS[] = {"SHOW ALL", "INSERT <key> <value>", "QUERY <key>", "UPDATE  <key> <newValue>", "DELETE <key>", "SAVE <filename>.txt", "QUIT"};
const char *CDT_COMMANDS[] = {"OPEN <filename>.txt", "QUIT"};

// Structs
typedef struct
{
    char key[50];
    float value;
} Record;

// Initial initalization
Record *recordPairs = NULL;
int startProgram = TRUE;
int maxRecords = 100;
int pairCount = 0;
int dataTableOpen = FALSE;

/*
 * Function:  saveDataTable 
 * --------------------
 * Writes datatable stored in memory into a .txt file, then resets variables.
 *
 *  dataTable: A .txt file to write datatable into.
 *
 *  returns: -1 on error (empty memory / failure to open file for writing )
 *            0 on successful writing to specified .txt file
 */
int saveDataTable(char dataTable[])
{
    FILE *file = fopen(dataTable, "w"); // Open the file for writing

    // Check if datatable is open
    if (!dataTableOpen)
    {
        printf("No data table is open. Use the 'open' command to open a data table.\n\n");
        return -1;
    }

    // check for empty memory
    if (recordPairs == NULL)
    {
        printf("Failed to write empty memory.\n\n");
        return -1;
    }

    // check if file is open
    if (file == NULL)
    {
        perror("Failed to open the file for writing");
        fclose(file);
        return -1;
    }
    else
    {

        // Write the data to the file
        fprintf(file, "%s %s\n", "Key", "Value");
        for (int i = 0; i < pairCount; i++)
        {
            fprintf(file, "%s %.2f\n", recordPairs[i].key, recordPairs[i].value);
        }

        fclose(file); // Close the file
        // Free dynamically allocated memory
        free(recordPairs);
        recordPairs = NULL;
        dataTableOpen = FALSE;
        pairCount = 0;
        printf("Datatable saved to %s.\n\n", dataTable);

        return 0;
    }
}

/*
 * Function:  openDataTable 
 * --------------------
 * Reads data table from .txt file and loads into memory, preparing data for manipulation.
 *
 *  dataTable: A .txt file to read datatable from.
 *
 *  returns: -1 on error (memory allocation issues / reading file issues)
 *            0 on successful loading of datatable into memory
 */
int openDataTable(char dataTable[])
{
    char line[100], header[100];
    FILE *file = fopen(dataTable, "r");

    if (file == NULL)
    {
        perror("Failed to open the file");
        return -1;
    }
    else
    {
        // Initialize or allocate memory for recordPairs
        recordPairs = malloc(maxRecords * sizeof(Record));

        if (recordPairs == NULL)
        {
            perror("Memory allocation failed.\n");
            fclose(file);
            return -1;
        }
        // Skip the header lines
        if (fgets(header, sizeof(header), file) == NULL)
        {
            perror("Failed to read the header lines.\n");
            fclose(file);
            return -1;
        }
        printf("Loaded datatable: %s\n\n", dataTable);

        while (fgets(line, sizeof(line), file) != NULL)
        {
            // reallocate the memory fpr recordPairs if it exceeds
            if (pairCount >= maxRecords)
            {
                maxRecords += 10;
                Record *temp = realloc(recordPairs, maxRecords * sizeof(Record));

                if (temp == NULL)
                {
                    perror("Memory allocation failed.\n");
                    fclose(file);
                    free(recordPairs);
                    return -1;
                }

                recordPairs = temp;
            }
            // stores each line of the .txt file into key and value
            if (sscanf(line, "%49s %f", recordPairs[pairCount].key, &recordPairs[pairCount].value) == 2)
            {
                pairCount++;
            }
        }

        fclose(file);
        return 0;
    }
}

/*
 * Function:  showAllRecord 
 * --------------------
 * Prints all records of datatable currently stored in memory.
 */
void showAllRecord(){
    if (recordPairs == NULL)
    {
        printf("There are no records to show.\n");
    }
    else
    {
        printf("-------------------------------------------------------------------\n");
        printf("| %-50s | %-10s |\n", "Key", "Value");
        printf("|----------------------------------------------------|------------|\n");

        for (int i = 0; i < pairCount; ++i)
        {
            printf("| %-50s | %-10.2f |\n", recordPairs[i].key, recordPairs[i].value);
        }
        printf("-------------------------------------------------------------------\n");
        printf("\nThere are in total %d records found.\n\n", pairCount);
    }
}

/*
 * Function:  insertRecord 
 * --------------------
 * Inserts new record of key,value into datatable.
 *
 *  key: Key to be inserted.
 *  newValue: Value associated with new key.
 *
 *  returns: 0 on failed insertion, existing key found.
 *          -1 on successful insertion, no existing key found.
 */
int insertRecord(char key[], float newValue){
    // int index = getIndexForInsert(key);
    int match = FALSE;

    for (int i = 0; i < pairCount; i++)
    {
        /* compare the key in the loop against the key user input */
        /* if the key in the loop matches the user input key */
        if (strcasecmp(recordPairs[i].key, key) == 0)
        {
            printf("Key='%s' already exists in the database.\n\n", key);
            match = TRUE;
            break;
        }
    }

    if (!match)
    {
        strcpy(recordPairs[pairCount].key, key);
        recordPairs[pairCount].value = newValue;
        pairCount++;
        printf("Key='%s', Value='%.2f' is successfully inserted.\n\n", key, newValue);
        match = FALSE;

        return 0;
    }

    return -1;
}

/*
 * Function:  queryRecord 
 * --------------------
 * Query a record from the datatable.
 *
 *  key: Key to query.
 */
void queryRecord(char queryKey[]){
    // check if table is empty
    if (recordPairs == NULL)
    {
        printf("Error: Datatable not loaded in memory.\n\n");
        return;
    }

    int i = 0;
    int found = FALSE;

    while (i < pairCount && !found)
    {
        if (strcasecmp(recordPairs[i].key, queryKey) == 0)
        {
            found = TRUE;

            printf("Record of Key=%s found, Value=%.2f is found in the database.\n\n", recordPairs[i].key, recordPairs[i].value);
        }
        i++;
    }

    if (!found)
    {
        printf("No record with Key=%s found in the database.\n\n", queryKey);
    }
}

/*
 * Function:  updateRecord 
 * --------------------
 * Update value of key in the current datatable.
 *
 *  key: Key to update.
 *  newValue: New value of key.
 *
 *  returns: 0 on key successfully updated.
 *          -1 on key not found in datatable.
 */
int updateRecord(char key[], float newValue){
    int found = FALSE;

    for (int i = 0; i < pairCount; i++)
    {
        if (strcasecmp(recordPairs[i].key, key) == 0)
        {
            recordPairs[i].value = newValue;
            found = TRUE;
            printf("Record with key '%s' has been updated to %.2f.\n\n", key, newValue);
            break;
        }
    }

    if (!found)
    {
        printf("Record with key '%s' not found.\n\n", key);
        return -1;
    }
    return 0;
}

/*
 * Function:  deleteRecord 
 * --------------------
 * Delete key,value pair in the current datatable.
 *
 *  key: Key to delete.
 */
void deleteRecord(char delKey[]){
    if (recordPairs == NULL)
    {
        printf("Error: Datatable not loaded in memory.\n\n");
        return;
    }

    int found = FALSE;
    int i = 0;

    while (i < pairCount && !found)
    {
        if (strcasecmp(recordPairs[i].key, delKey) == 0)
        {
            found = TRUE;

            if (pairCount > 1)
            {
                pairCount--;

                // Move the last record to the position of the deleted record
                recordPairs[i] = recordPairs[pairCount];
            }
            else
            {
                // If there's only one record, just decrement pairCount
                pairCount = 0;
            }

            printf("Record of Key=%s is successfully deleted.\n\n", delKey);
        }
        i++;
    }

    if (!found)
    {
        printf("No record with Key=%s found in the database.\n\n", delKey);
    }
}

/*
 * Function:  handleDataTable 
 * --------------------
 * Core Function that handles processing of EzDB commands, such as
 * SHOW ALL, INSERT, QUERY, UPDATE, DELETE, SAVE, OPEN, QUIT
 *
 *  command: user inputted command.
 *  dataTable: user inputted datatable.
 */
void handleDataTable(char command[], char dataTable[]){
    // Initialize variables
    char key[50];
    float newValue = 0.0;
    float value;

    if (strcmp(command, "quit") == 0)
    {
        printf("Thank you for using INF1002 P2_3 EzDB!\n\n");
        startProgram = FALSE;
    }

    else if (dataTableOpen)
    {

        if (strcmp(command, "show all") == 0)
        {
            showAllRecord();
        }
        else if (sscanf(command, "insert %s %f", key, &value) == 2)
        {
            insertRecord(key, value);
        }
        else if (sscanf(command, "query %s", key) == 1)
        {
            queryRecord(key);
        }
        else if (sscanf(command, "update %s %f", key, &newValue) == 2)
        {
            updateRecord(key, newValue);
        }
        else if (sscanf(command, "delete %s", key) == 1)
        {
            deleteRecord(key);
        }
        else if (sscanf(command, "save %s", dataTable) == 1)
        {
            saveDataTable(dataTable);
        }
        else
        {
            printf("Unknown command. Please try again.\n\n");
        }
    }
    else if (!dataTableOpen)
    {
        if (sscanf(command, "open %s", dataTable) == 1)
        {
            dataTableOpen = openDataTable(dataTable) == 0;
        }
    }
    else
    {
        printf("No data table is open. Use the 'open' command to open a data table.\n\n");
    }
}

/*
 * Function:  main 
 * --------------------
 * Main function that is the base of the application. Prints available commands at different stages
 * of application. Reads and prepares user input for processing.
 *
 */
int main(){
    char command[100], dataTable[50];
    int numNames = sizeof(GROUP_MEMBERS) / sizeof(GROUP_MEMBERS[0]);
    int sizeODT = sizeof(ODT_COMMANDS) / sizeof(ODT_COMMANDS[0]);
    int sizeCDT = sizeof(CDT_COMMANDS) / sizeof(CDT_COMMANDS[0]);

    printf("INF1002 Group P2_3 EzDB [Version %.1f]\n", VERSION_NUMBER);
    printf("\n");
    printf("Declaration\n\n");
    printf("SIT policy on copying does not allow the students to copy source code as well as assessment solutions\n");
    printf("from another person or other places. It is the students responsibility to guarantee that their assessment\n");
    printf("solutions are their own work. Meanwhile, the students must also ensure that their work is not accessible\n");
    printf("by others. Where such plagiarism is detected, both of the assessments involved will receive ZERO mark.\n");
    printf("We hereby declare that:\n");
    printf("We fully understand and agree to the abovementioned plagiarism policy.\n");
    printf("We did not copy any code from others or from other places.\n");
    printf("We did not share our codes with others or upload to any other places for public access.\n");
    printf("We agree that our project will receive Zero mark if there is any plagiarism detected.\n\nquiquiquit");
    printf("Declared by: Group Name: P2-3\n");
    printf("Team members:\n");
        for (int i = 0; i < numNames; i++)
    {
        printf("%d. %s\n", i + 1, GROUP_MEMBERS[i]);
    }
    printf("Date: 22/11/2023\n");


    printf("\n");
    while (startProgram)
    {

        if (dataTableOpen)
        {
            printf("***********************\n");
            printf("| Available Commands: |\n");
            printf("***********************\n");

            for (int i = 0; i < sizeODT; i++)
            {
                printf("%d - %s\n", i + 1, ODT_COMMANDS[i]);
            }
            printf("\n\nEnter a command (SHOW ALL, INSERT, QUERY, UPDATE, DELETE, SAVE, or QUIT): ");
        }
        else
        {
            printf("***********************\n");
            printf("| Available Commands: |\n");
            printf("***********************\n");

            for (int i = 0; i < sizeCDT; i++)
            {
                printf("%d - %s\n", i + 1, CDT_COMMANDS[i]);
            }

            printf("\n\nEnter a command (OPEN or QUIT): ");
        }

        fgets(command, sizeof(command), stdin);

        // Remove newline because fgets includes \n
        if (command[strlen(command) - 1] == '\n')
        {
            command[strlen(command) - 1] = '\0';
        }

        for (int i = 0; command[i]; i++)
        {
            command[i] = tolower(command[i]);
        }
        printf("\n");
        handleDataTable(command, dataTable);
    }

    return 0;
}
