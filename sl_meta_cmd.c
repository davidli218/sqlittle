#include <stdio.h>
#include <stdlib.h>
#include "sl_core.h"
#include "sl_storage.h"


MetaCommandResult meta_exit(InputBuffer *inputBuffer, Table *table) {
    closeInputBuffer(inputBuffer);
    closeDB(table);
    exit(EXIT_SUCCESS);
}

MetaCommandResult meta_help(void) {
    printf("Help: No content yet.\n");
    return META_COMMAND_SUCCESS;
}
