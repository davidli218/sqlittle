#ifndef SQLITTLE_SL_CORE_H
#define SQLITTLE_SL_CORE_H

#include <stdlib.h>
#include "sl_storage.h"


/**
 * Struct & Enum & Union Declaration Start ->
 */

// < +++++++++++++++++++++++++++++ CL I/O +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

typedef struct {
    char *buffer;
    size_t bufferLength;
    ssize_t inputLength;
} InputBuffer;

//                                                                           ||
// < ============================= CL I/O ============================= > __END


// < ++++++++++++++++++++++++ Execution Status ++++++++++++++++++++++++ > BEGIN
//                                                                           ||

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED
} PrepareResult;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

//                                                                           ||
// < ======================== Execution Status ======================== > __END


// < ++++++++++++++++++++++++++ SQL Command ++++++++++++++++++++++++++ > _BEGIN
//                                                                           ||

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row rowToInsert;
} Statement;

//                                                                           ||
// < ========================== SQL Command ========================== > ___END


/**
 * Function Declaration Start ->
 */

// < +++++++++++++++++++++++++++++ CL I/O +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

InputBuffer *newInputBuffer(void);

void closeInputBuffer(InputBuffer *);

void printPrompt(void);

void readInput(InputBuffer *);

//                                                                           ||
// < ============================= CL I/O ============================= > __END


// < +++++++++++++++++++++++ Parse Meta Command +++++++++++++++++++++++ > BEGIN
//                                                                           ||

MetaCommandResult executeMetaCommand(InputBuffer *);

//                                                                           ||
// < ======================= Parse Meta Command ======================= > __END


// < +++++++++++++++++++++++ Parse SQL Command +++++++++++++++++++++++ > _BEGIN
//                                                                           ||

PrepareResult prepareStatement(InputBuffer *, Statement *);

ExecuteResult executeInsert(Statement *, Table *);

//                                                                           ||
// < ======================= Parse SQL Command ======================= > ___END


// < ++++++++++++++++++++++ Execute SQL Command ++++++++++++++++++++++ > _BEGIN
//                                                                           ||

ExecuteResult executeSelect(__attribute__((unused)) Statement *, Table *);

ExecuteResult executeStatement(Statement *, Table *);

//                                                                           ||
// < ====================== Execute SQL Command ====================== > ___END

#endif //SQLITTLE_SL_CORE_H
