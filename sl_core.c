#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sl_core.h"
#include "sl_storage.h"
#include "sl_meta_cmd.h"


// < +++++++++++++++++++++++++++++ CL I/O +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

InputBuffer *newInputBuffer() {
    InputBuffer *inputBuffer = malloc(sizeof(InputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->bufferSize = 0;
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
            getline(&(inputBuffer->buffer), &(inputBuffer->bufferSize), stdin);

    if (bytesRead <= 0) {
        printf("Errors occurs when reading the command input!\n");
        exit(EXIT_FAILURE);
    }

    inputBuffer->inputLength = bytesRead - 1;  /* Subtract the length of the delimiter */
    inputBuffer->buffer[bytesRead - 1] = 0;  /* Change the delimiter to null terminator of string */
}

//                                                                           ||
// < ============================= CL I/O ============================= > __END


// < +++++++++++++++++++++++++ Parse Command +++++++++++++++++++++++++ > _BEGIN
//                                                                           ||

MetaCommandResult executeMetaCommand(InputBuffer *inputBuffer, Table *table) {
    if (strcmp(inputBuffer->buffer, ".exit") == 0)
        return meta_exit(inputBuffer, table);
    else if (strcmp(inputBuffer->buffer, ".help") == 0)
        return meta_help();

    return META_COMMAND_UNRECOGNIZED;
}

PrepareResult prepareStatement(InputBuffer *inputBuffer, Statement *statement) {
    if (strncmp(inputBuffer->buffer, "insert", 6) == 0)
        return prepareInsert(inputBuffer, statement);
    else if (strncmp(inputBuffer->buffer, "select", 6) == 0)
        return prepareSelect(statement);

    return PREPARE_UNRECOGNIZED;
}

ExecuteResult executeStatement(Statement *statement, Table *table) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            return executeInsert(statement, table);
        case (STATEMENT_SELECT):
            return executeSelect(table);
    }
}

//                                                                           ||
// < ========================= Parse Command ========================= > ___END


// < +++++++++++++++++++++++ Parse SQL Command +++++++++++++++++++++++ > _BEGIN
//                                                                           ||

static PrepareResult prepareInsert(InputBuffer *inputBuffer, Statement *statement) {
    statement->type = STATEMENT_INSERT;

    strtok(inputBuffer->buffer, " ");  /* Skip keyword insert */
    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");

    if (id_string == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    uint32_t id = (uint32_t) strtoul(id_string, NULL, 10);
    if (id < 0) {
        return PREPARE_NEGATIVE_ID;
    }
    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_PARAM_TOO_LONG;
    }
    if (strlen(email) > COLUMN_EMAIL_SIZE) {
        return PREPARE_PARAM_TOO_LONG;
    }

    statement->rowToInsert.id = id;
    strcpy(statement->rowToInsert.username, username);
    strcpy(statement->rowToInsert.email, email);

    return PREPARE_SUCCESS;
}

static PrepareResult prepareSelect(Statement *statement) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
}

//                                                                           ||
// < ======================= Parse SQL Command ======================= > ___END


// < ++++++++++++++++++++++ Execute SQL Command ++++++++++++++++++++++ > _BEGIN
//                                                                           ||

extern int TABLE_MAX_ROWS;

static ExecuteResult executeInsert(Statement *statement, Table *table) {
    if (table->numRows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row *rowToInsert = &(statement->rowToInsert);
    Cursor *cursor = tableEnd(table);

    writeRow(rowToInsert, cursorAddress(cursor));
    table->numRows++;

    free(cursor);

    return EXECUTE_SUCCESS;
}

static ExecuteResult executeSelect(Table *table) {
    Cursor *cursor = tableBegin(table);
    Row row;

    while (!(cursor->isEndOfTable)) {
        readRow(cursorAddress(cursor), &row);
        printRow(&row);
        cursorMoveForward(cursor);
    }

    free(cursor);

    return EXECUTE_SUCCESS;
}

//                                                                           ||
// < ====================== Execute SQL Command ====================== > ___END
