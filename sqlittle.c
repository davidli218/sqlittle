#include <stdbool.h>
#include <stdio.h>
#include "sl_core.h"
#include "sl_storage.h"


int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {
    InputBuffer *inputBuffer = newInputBuffer();

    Table *table = newTable();

    while (true) {
        printPrompt();
        readInput(inputBuffer);

        /* Process Meta Command */
        if (inputBuffer->buffer[0] == '.') {
            switch (executeMetaCommand(inputBuffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED):
                    printf("Unrecognized meta command '%s'\n", inputBuffer->buffer);
                    continue;
            }
        }

        /* Prepare SQL statement */
        Statement statement;
        switch (prepareStatement(inputBuffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_NEGATIVE_ID):
                printf("ID must be positive.\n");
                continue;
            case (PREPARE_PARAM_TOO_LONG):
                printf("Parameter is too long.\n");
                continue;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case (PREPARE_UNRECOGNIZED):
                printf("Unrecognized keyword at start of '%s'.\n", inputBuffer->buffer);
                continue;
        }

        /* Execute SQL statement */
        switch (executeStatement(&statement, table)) {
            case (EXECUTE_SUCCESS):
                printf("Executed.\n");
                break;
            case (EXECUTE_TABLE_FULL):
                printf("Error: Table is full.\n");
                break;
        }

    }
}
