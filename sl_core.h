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
    size_t bufferSize;
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
    PREPARE_UNRECOGNIZED,
    PREPARE_NEGATIVE_ID,
    PREPARE_PARAM_TOO_LONG
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


// < +++++++++++++++++++++++++ Parse Command +++++++++++++++++++++++++ > _BEGIN
//                                                                           ||

MetaCommandResult executeMetaCommand(InputBuffer *, Table*);

PrepareResult prepareStatement(InputBuffer *, Statement *);

ExecuteResult executeStatement(Statement *, Table *);

//                                                                           ||
// < ========================= Parse Command ========================= > ___END


// < +++++++++++++++++++++++ Parse SQL Command +++++++++++++++++++++++ > _BEGIN
//                                                                           ||

static PrepareResult prepareInsert(InputBuffer *, Statement *);

static PrepareResult prepareSelect(Statement *);

//                                                                           ||
// < ======================= Parse SQL Command ======================= > ___END


// < ++++++++++++++++++++++ Execute SQL Command ++++++++++++++++++++++ > _BEGIN
//                                                                           ||

static ExecuteResult executeInsert(Statement *, Table *);

static ExecuteResult executeSelect(__attribute__((unused)) Statement *, Table *);

//                                                                           ||
// < ====================== Execute SQL Command ====================== > ___END

#endif //SQLITTLE_SL_CORE_H
