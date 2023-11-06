#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct
{
    char key[50];
    float value;
} Record;

int startProgram = TRUE;
// Allocate Memory
Record *recordPairs = NULL;
int pairCount = 0;

int dataTableOpen = FALSE;

int saveDataTable(char dataTable[])
{
    // Check if datatable is open
    if (!dataTableOpen)
    {
        printf("No data table is open. Use the 'open' command to open a data table.\n");
        return -1;
    }

    //check for empty memory
    if (recordPairs == NULL)
    {
        printf("Failed to write empty memory.\n");
        return -1;
    }

    FILE *file = fopen(dataTable, "w"); // Open the file for writing

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
        printf("Data saved to %s.\n", dataTable);
        // Free dynamically allocated memory
        free(recordPairs);
        recordPairs = NULL;
        pairCount = 0;
        return 0;
    }
}

int openDataTable(char dataTable[])
{
    // Assuming a maximum of 100 key-value pairs & allocate memory for key-value pairs dynamically
    char line[100], header[100];
    FILE *file = fopen(dataTable, "r");

    if (file == NULL)
    {
        perror("Failed to open the file");
        return -1;
    }
    else
    {
        // Skip the header lines
        if (fgets(header, sizeof(header), file) == NULL)
        {
            perror("Failed to read the header lines.\n");
            fclose(file);
            return -1;
        }
        printf("Selected DataTable: %s\n", dataTable);
        while (fgets(line, sizeof(line), file) != NULL)
        {
            // Resize the array to accommodate a new record
            recordPairs = realloc(recordPairs, (pairCount + 1) * sizeof(Record));

            // Read and store the key-value pair
            if (sscanf(line, "%49s %f", recordPairs[pairCount].key, &recordPairs[pairCount].value) == 2)
            {
                pairCount++;
            }
        }

        fclose(file);

        // Print the read data
        printf("-----------%s-----------\n", dataTable);
        for (int i = 0; i < pairCount; i++)
        {
            printf("Key: %s, Value: %.2f\n", recordPairs[i].key, recordPairs[i].value);
        }
        printf("---------------------------------\n");
        return 0;
    }
}

int showAllRecords() {
    if (recordPairs == NULL) {
        printf("There are no records to show.\n");
        return;
    }
    for (int i = 0; i < pairCount; i++) {
        printf("Key: %s, Value: %.2f\n", recordPairs[i].key, recordPairs[i].value);
    }
    printf("==================================\n");
    printf("There are in total %d records found\n", pairCount);
}

int getIndexForInsert(char key[])
{
    for (int i = 0; i < pairCount; i++)
    {
        /* compare the key in the loop against the key user input */
        /* if the key in the loop matches the user input key */
        if (strcasecmp(recordPairs[i].key, key) == 0)
        {
            /*printf("The record with Key='%s' already exists in the database.\n", key);*/
            /* if theres a match return 0 */
            return 0;
        }
    }
    /* if no match return -1 */
    return -1;
}

int insertNewRecord(char key[], float newValue)
{
    int index = getIndexForInsert(key);

    if (index == -1) // key not found
    {
        strcpy(recordPairs[pairCount].key, key);
        recordPairs[pairCount].value = newValue;
        printf("A new record of Key='%s', Value='%.2f' is successfully inserted\n", key, newValue);
    }
    else // Key found
    { 
        printf("The record with Key='%s' already exists in the database.\n", key);
    }
}

int updateDataTable(char key[], float newValue)
{
    int found = FALSE;

    for (int i = 0; i < pairCount; i++)
    {
        if (strcasecmp(recordPairs[i].key, key) == 0)
        {
            recordPairs[i].value = newValue;
            found = TRUE;
            printf("Record with key '%s' has been updated to %.2f.\n", key, newValue);
            break;
        }
    }

    if (!found)
    {   
        printf("Record with key '%s' not found.\n", key);
        return -1;
    }
    return 0;
}

void handleDataTable(char command[], char dataTable[])
{
    if (sscanf(command, "open %s", dataTable) == 1)
    {
        printf("==============OPEN==============\n\n");
        dataTableOpen = openDataTable(dataTable) == 0;
    }
    else if (sscanf(command, "save %s", dataTable) == 1)
    {
        printf("==============SAVE==============\n\n");
        // printf("DataTable: %s\n", dataTable);
        saveDataTable(dataTable);
    }
    else if (strcmp(command, "quit") == 0)
    {
        printf("==============QUIT==============\n\n");
        startProgram = FALSE;
    }
    else if (dataTableOpen)
    {
        // Initialize variables
        char key[50]; 
        float newValue = 0.0;

        if (strcmp(command, "show all") == 0)
        {
            printf("============SHOW ALL============\n\n");
            showAllRecords();
        }
        else if (sscanf(command, "insert %s %f", recordPairs[pairCount].key, &recordPairs[pairCount].value) == 2)
        {
            printf("=============INSERT=============\n\n");
            insertNewRecord(recordPairs[pairCount].key,recordPairs[pairCount].value);
            pairCount++;
        }
        else if (sscanf(command, "query %s", dataTable) == 1)
        {
            printf("=============QUERY=============\n\n");
            // Implement the QUERY function here
        }
        else if (sscanf(command, "update %s %f", key, &newValue) == 2)
        {
            printf("=============UPDATE=============\n\n");
            updateDataTable(key,newValue);
        }
        else if (sscanf(command, "delete %s", dataTable) == 1)
        {
            printf("=============DELETE=============\n\n");
            // Implement the DELETE function here
        }
        else
        {
            printf("=============ERROR=============\n\n");
            printf("Unknown command. Please try again.\n");
        }
    }
    else
    {
        printf("No data table is open. Use the 'open' command to open a data table.\n");
    }
}

int main()
{
    char command[100], dataTable[50];
    while (startProgram)
    {
        if (dataTableOpen)
        {
            printf("Enter a command (SHOW ALL, INSERT, QUERY, UPDATE, DELETE, SAVE, or QUIT): ");
        }
        else
        {
            printf("Enter a command (OPEN or QUIT): ");
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

        handleDataTable(command, dataTable);
        printf("===============End===============\n\n");
    }

    return 0;
}
