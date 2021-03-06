#ifndef SQLITTLE_SL_STORAGE_H
#define SQLITTLE_SL_STORAGE_H

#include <stdbool.h>
#include <stdint.h>


/**
 * Macros Start ->
 */

// < +++++++++++++++++++++++++++++ Macros +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

#define TABLE_MAX_PAGES 100
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 128

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

//                                                                           ||
// < ============================= Macros ============================= > __END


/**
 * Struct & Enum & Union Declaration Start ->
 */

// < ++++++++++++++++++++++++++ Data Storage ++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

typedef struct {
    int fileDescriptor;
    uint32_t fileLength;
    void *pages[TABLE_MAX_PAGES];
} Pager;

typedef struct {
    uint32_t numRows;
    void *pager;
} Table;

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];  /* 1B for '\0' character */
} Row;

typedef struct {
    Table *table;
    uint32_t rowIndex;

    /* Indicates whether the Cursor is one place behind the last element */
    bool isEndOfTable;
} Cursor;

//                                                                           ||
// < ========================== Data Storage ========================== > __END


/**
 * Function Declaration Start ->
 */

// < +++++++++++++++++++++++++++++ Tables +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

Table *openDB(const char *);

void closeDB(Table *);

Pager *openPager(const char *);

void flushPager(Pager *, uint32_t, uint32_t);

void *getPage(Pager *, uint32_t);

//                                                                           ||
// < ============================= Tables ============================= > __END


// < +++++++++++++++++++++++++++++ Cursor +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

Cursor *tableBegin(Table *);

Cursor *tableEnd(Table *);

void *cursorAddress(Cursor *cursor);

void cursorMoveForward(Cursor *);

//                                                                           ||
// < ============================= Cursor ============================= > __END


// < ++++++++++++++++++++++++++++++ Rows ++++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

void writeRow(Row *, void *);

void readRow(void *, Row *);

void printRow(Row *);

//                                                                           ||
// < ============================== Rows ============================== > __END

#endif //SQLITTLE_SL_STORAGE_H
