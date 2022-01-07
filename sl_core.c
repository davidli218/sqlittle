#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sl_core.h"
#include "sl_storage.h"


// < +++++++++++++++++++++++++++++ CL I/O +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

InputBuffer *newInputBuffer() {
    InputBuffer *inputBuffer = malloc(sizeof(InputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->bufferLength = 0;
    inputBuffer->inputLength = 0;

    return inputBuffer;
}

void closeInputBuffer(InputBuffer *inputBuffer) {
    free(inputBuffer->buffer);
    free(inputBuffer);
}

void printPrompt() { printf("sqlittle >"); }

void readInput(InputBuffer *inputBuffer) {
    ssize_t bytesRead =
            getline(&(inputBuffer->buffer), &(inputBuffer->bufferLength), stdin);

    if (bytesRead <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    inputBuffer->inputLength = bytesRead - 1;
    inputBuffer->buffer[bytesRead - 1] = 0;
}

//                                                                           ||
// < ============================= CL I/O ============================= > __END


// < +++++++++++++++++++++++ Parse Meta Command +++++++++++++++++++++++ > BEGIN
//                                                                           ||

MetaCommandResult executeMetaCommand(InputBuffer *inputBuffer) {
    if (strcmp(inputBuffer->buffer, ".exit") == 0) {
        closeInputBuffer(inputBuffer);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED;
    }
}

//                                                                           ||
// < ======================= Parse Meta Command ======================= > __END


// < +++++++++++++++++++++++ Parse SQL Command +++++++++++++++++++++++ > _BEGIN
//                                                                           ||

PrepareResult prepareStatement(InputBuffer *inputBuffer, Statement *statement) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
                inputBuffer->buffer, "insert %d %s %s",
                &(statement->rowToInsert.id),
                statement->rowToInsert.username,
                statement->rowToInsert.email
        );

        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;
        }

        return PREPARE_SUCCESS;
    }
    if (strncmp(inputBuffer->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED;
}

ExecuteResult executeStatement(Statement *statement, Table *table) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            return executeInsert(statement, table);
        case (STATEMENT_SELECT):
            return executeSelect(statement, table);
    }
}

//                                                                           ||
// < ======================= Parse SQL Command ======================= > ___END


// < ++++++++++++++++++++++ Execute SQL Command ++++++++++++++++++++++ > _BEGIN
//                                                                           ||

extern int TABLE_MAX_ROWS;

ExecuteResult executeInsert(Statement *statement, Table *table) {
    if (table->numRows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row *rowToInsert = &(statement->rowToInsert);

    writeRow(rowToInsert, trackRow(table, table->numRows));
    table->numRows++;

    return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(__attribute__((unused)) Statement *statement, Table *table) {
    Row row;

    for (uint32_t i = 0; i < table->numRows; i++) {
        readRow(trackRow(table, i), &row);
        printRow(&row);
    }

    return EXECUTE_SUCCESS;
}

//                                                                           ||
// < ====================== Execute SQL Command ====================== > ___END
