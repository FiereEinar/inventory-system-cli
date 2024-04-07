#include <stdio.h>
#include <string.h>
#include "main.h"

void addItemToStorage(struct Item item)
{
    FILE *file;

    file = fopen("items.csv", "a");

    fprintf(
        file, 
        "%s,%d,%d,%lf,%lf,%lf,%s,%s,%s\n",        // format of how they are written (csv)
        item.name,
        item.baseStocks,
        item.stocks,
        item.price,
        item.originalPrice,
        item.profit,
        item.dateAdded,
        item.lastUpdated,
        item.id        
    );

    if (ferror(file))
    {
        printf("There was a problem that occured when trying to add data in the items.csv file\n");
    }
    
    fclose(file);
}

void getItemsFromStorage(struct Node **head)
{
    FILE *file;
    struct Item newItem;
    int read = 0;

    file = fopen("items.csv", "r");

    do {
        read = fscanf(
            file,
            "%29[^,],%d,%d,%lf,%lf,%lf,%29[^,],%29[^,],%9[^,]\n",
            newItem.name,
            &newItem.baseStocks,
            &newItem.stocks,
            &newItem.price,
            &newItem.originalPrice,
            &newItem.profit,
            newItem.dateAdded,
            newItem.lastUpdated,
            newItem.id
        );

        size_t len = strlen(newItem.id);                                                
        if (len > 0 && newItem.id[len - 1] == '\n') newItem.id[len - 1] = '\0';         // clear the newline character from ID

        if (read != 9) {
            printf("There was a problem that occured when trying to read data from the items.csv file\n");
            printf("It is likely that someone touched the file.\n");
            return;
        }

        addItemFromStorage(head, newItem);
    } while (!feof(file));

    fclose(file);
}

void deleteItemFromStorageById(char *id)
{
    
}