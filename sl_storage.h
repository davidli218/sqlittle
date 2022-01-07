#ifndef SQLITTLE_SL_STORAGE_H
#define SQLITTLE_SL_STORAGE_H

#include <stdint.h>


/**
 * Macros Start ->
 */

// < +++++++++++++++++++++++++++++ Macros +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

#define TABLE_MAX_PAGES 100
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

//                                                                           ||
// < ============================= Macros ============================= > __END


/**
 * Struct & Enum & Union Declaration Start ->
 */

// < ++++++++++++++++++++++++++ Data Storage ++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

typedef struct {
    uint32_t numRows;
    void *pages[TABLE_MAX_PAGES];
} Table;

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

//                                                                           ||
// < ========================== Data Storage ========================== > __END


/**
 * Function Declaration Start ->
 */

// < +++++++++++++++++++++++++++++ Tables +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

Table *newTable(void);

__attribute__((unused)) void freeTable(Table *);

//                                                                           ||
// < ============================= Tables ============================= > __END


// < ++++++++++++++++++++++++++++++ Rows ++++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

void *trackRow(Table *, uint32_t);

void writeRow(Row *, void *);

void readRow(void *, Row *);

void printRow(Row *);

//                                                                           ||
// < ============================== Rows ============================== > __END

#endif //SQLITTLE_SL_STORAGE_H
