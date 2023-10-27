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

int saveDataTable(char dataTable[])
{
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
        fprintf(file, "%s %s\n", "Keying", "Valuing");
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

void handleDataTable(char command[], char dataTable[])
{
    if (sscanf(command, "open %s", dataTable) == 1)
    {
        printf("Command: OPEN\n");
        // printf("DataTable: %s\n", dataTable);
        openDataTable(dataTable);
    }
    else if (sscanf(command, "save %s", dataTable) == 1)
    {
        printf("Command: SAVE\n");
        // printf("DataTable: %s\n", dataTable);
        saveDataTable(dataTable);
    }
    else if (strcmp(command, "quit") == 0)
    {
        printf("Command: QUIT\n");
        startProgram = FALSE;
    }
    else
    {
        printf("Unknown command. Please try again.\n");
    }
}
int main()
{
    char command[100], dataTable[50];
    while (startProgram)
    {
        printf("Enter a command: ");
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

        printf("==============Start==============\n");
        handleDataTable(command, dataTable);
        printf("===============End===============\n");
    }

    return 0;
}
