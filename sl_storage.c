#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sl_storage.h"


// < +++++++++++++++++++++++++++++ Consts +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

//                                                                           ||
// < ============================= Consts ============================= > __END


// < +++++++++++++++++++++++++++++ Tables +++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

Table *openDB(const char *fileName) {
    Pager *pager = openPager(fileName);

    Table *table = (Table *) malloc(sizeof(Table));
    table->pager = pager;
    table->numRows = pager->fileLength / ROW_SIZE;

    return table;
}

void closeDB(Table *table) {
    Pager *pager = table->pager;
    uint32_t numFullPages = table->numRows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < numFullPages; i++) {
        if (pager->pages[i] == NULL)
            continue;

        flushPager(pager, i, PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    uint32_t numAdditionalRows = table->numRows % ROWS_PER_PAGE;
    if (numAdditionalRows > 0) {
        uint32_t page_num = numFullPages;
        if (pager->pages[page_num] != NULL) {
            flushPager(pager, page_num, numAdditionalRows * ROW_SIZE);
            free(pager->pages[page_num]);
            pager->pages[page_num] = NULL;
        }
    }

    int result = close(pager->fileDescriptor);
    if (result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        void *page = pager->pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }

    free(pager);
    free(table);
}

Pager *openPager(const char *fileName) {
    int fd = open(fileName,
                  O_RDWR | O_CREAT,  /* R/W, Create if !exist */
                  S_IWUSR | S_IRUSR  /* Permission: Owner R/W */
    );

    /* Failed to open the file */
    if (fd == -1) {
        printf("Unable to open file: %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    off_t fileLength = lseek(fd, SEEK_SET, SEEK_END);

    Pager *pager = malloc(sizeof(Pager));
    pager->fileDescriptor = fd;
    pager->fileLength = fileLength;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
        pager->pages[i] = NULL;

    return pager;
}

void flushPager(Pager *pager, uint32_t pageNum, uint32_t size) {
    if (pager->pages[pageNum] == NULL) {
        printf("Tried to flush a Pager null page\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesWritten = write(pager->fileDescriptor, pager->pages[pageNum], size);

    if (bytesWritten == -1) {
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

void *getPage(Pager *pager, uint32_t pageNum) {
    if (pageNum > TABLE_MAX_PAGES) {
        printf("Fetch page number out of bounds. (%d > %d)\n", pageNum,
               TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[pageNum] == NULL) {
        void *page = malloc(PAGE_SIZE);
        uint32_t numPages = pager->fileLength / PAGE_SIZE;

        if (pager->fileLength % PAGE_SIZE)
            numPages++;

        if (pageNum <= numPages) {
            lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);
            ssize_t bytesRead = read(pager->fileDescriptor, page, PAGE_SIZE);
            if (bytesRead == -1) {
                printf("Error reading file: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[pageNum] = page;
    }

    return pager->pages[pageNum];
}

//                                                                           ||
// < ============================= Tables ============================= > __END


// < ++++++++++++++++++++++++++++++ Rows ++++++++++++++++++++++++++++++ > BEGIN
//                                                                           ||

void *trackRow(Table *table, uint32_t rowNum) {
    uint32_t pageNum = rowNum / ROWS_PER_PAGE;

    void *page = getPage(table->pager, pageNum);

    uint32_t rowOffset = rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;

    return page + byteOffset;
}

void writeRow(Row *source, void *destination) {
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void readRow(void *source, Row *destination) {
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void printRow(Row *row) {
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

//                                                                           ||
// < ============================== Rows ============================== > __END
