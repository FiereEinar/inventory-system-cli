#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "main.h"

// TODO: add error checking

void addItemToStorage(struct Item item)
{
    FILE *file;
    // make the folder
    mkdir("storedata");

    // open the file inside that folder
    file = fopen("storedata/items.csv", "a");

    // write the data
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
        printf("There was a problem that occured when trying to add data in the items.csv file\n");
    
    // close the file after writing it
    fclose(file);
}

void getItemsFromStorage(struct Node **head)
{
    FILE *file;
    struct Item newItem;
    int read = 0;

    // open the file
    file = fopen("storedata/items.csv", "r");

    // if there's no current records the go back
    if (file == NULL) return;

    // if there is a record,
    do {
        // get the items on each line
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

        clearNewline(newItem.id);      
        
        // if it didn't read all 9 properties
        if (read != 9) {
            printf("There was a problem that occured when trying to read data from the items.csv file\n");
            printf("It is likely that someone touched the file.\n");
            return;
        }
        // after getting the data, add it to the head
        addItemToLinkedList(head, newItem);
    } while (!feof(file));

    // close the file
    fclose(file);
}

// to delete an item, we rewrite everything in a temporary file EXEPT for the item being deleted
// after that the temporary file will contain the new list without the item that was deleted
// then we simply rename that temporary file to storage file and deleted the old storage file, kinda like swapping them
void deleteItemFromStorageById(char *id)
{
    FILE *file;
    FILE *temp;
    struct Item newItem;
    int maxLine = 2000;                             // max line inside a file

    // original file and temporary file
    char filename[50] = "storedata/items.csv";
    char tempFile[50] = "storedata/temp__items.csv";

    char placeholder[maxLine];                      // placeholder for data inside the file

    file = fopen(filename, "r");                    // "r" reading the original file
    temp = fopen(tempFile, "w");                    // "w" writing to temporary file

    if (file == NULL || temp == NULL) {
        printf("Error opening the files.");
        sleep(SLEEP_TIME);                          // gives us time to read
        return;
    }

    // fgets() gets the while 1 line
    while (fgets(placeholder, maxLine, file) != NULL) {
        // sscanf() to extract the data and store it in newItem, all we need here is the ID
        sscanf(
            placeholder,
            "%29[^,],%d,%d,%lf,%lf,%lf,%29[^,],%29[^,],%9[^,]",
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

        // clear the newline character from ID
        //  I AM SO MAD AT THIS, TOOK ME LONG TO FIGURE OUT AN ISSUE, IT WAS JUST A NEWLINE THE ENTIRE TIME
        clearNewline(newItem.id);

        // if it match the ID, don't include it, which basically deletes it
        if (strcmp(newItem.id, id) != 0) fputs(placeholder, temp);
    }

    // close both files
    fclose(file);
    fclose(temp);

    // rename the temporary file and delete the old file
    remove(filename);
    rename(tempFile, filename);
}

// when it comes to editing a certain data in a file, it is the same in deleting an item
// but instead of NOT writing the data, we write the NEW data, effectively replacing it
void editItemFromStorageById(char *id, struct Item editedItem)
{
    FILE *file;
    FILE *temp;
    struct Item currentItem;
    int maxLine = 2000;                             // max line inside a file

    char filename[50] = "storedata/items.csv";
    char tempFile[50] = "storedata/temp__items.csv";

    char placeholder[maxLine];                      // placeholder for data inside the file

    file = fopen(filename, "r");
    temp = fopen(tempFile, "w");

    if (file == NULL || temp == NULL) {
        printf("Error opening the files.");
        sleep(SLEEP_TIME);                          // gives us time to read
        return;
    }

    while (fgets(placeholder, maxLine, file) != NULL) {
        sscanf(
            placeholder,
            "%29[^,],%d,%d,%lf,%lf,%lf,%29[^,],%29[^,],%9[^,]",
            currentItem.name,
            &currentItem.baseStocks,
            &currentItem.stocks,
            &currentItem.price,
            &currentItem.originalPrice,
            &currentItem.profit,
            currentItem.dateAdded,
            currentItem.lastUpdated,
            currentItem.id
        );

        clearNewline(currentItem.id);

        // if it matches the ID, don't include it, 
        if (strcmp(currentItem.id, id) != 0) fputs(placeholder, temp);
        // but instead put the new item
        else fprintf(
            temp, 
            "%s,%d,%d,%lf,%lf,%lf,%s,%s,%s\n",
            editedItem.name,
            editedItem.baseStocks,
            editedItem.stocks,
            editedItem.price,
            editedItem.originalPrice,
            editedItem.profit,
            editedItem.dateAdded,
            editedItem.lastUpdated,
            editedItem.id        
        );
    }

    fclose(file);
    fclose(temp);

    remove(filename);
    rename(tempFile, filename);
}

// this thing is a MESS, it basically just reads the current record saved, if there is none then create one
// TODO: break this function into multiple chunks
void initReportsFromStorage(struct ReportPerMonth monthlyProfits[])
{
    FILE *file;
    FILE *tempFile;

    // make the folders
    mkdir("storedata");
    mkdir("storedata/reports");
    mkdir("storedata/reports/days");

    int read = 0;
    int tempFileLength = 50;

    // where the per month data is stored
    char monthFilename[] = "storedata/reports/monthlyReport.csv";  
    // the per day report is stored in a /day folder, it will get concatenated with the name of the month
    // example: storedata/reports/days/January.csv
    char daysFilename[] = "storedata/reports/days/";
    char extension[] = ".csv";

    // if there's no current record, initialize it with our current record which contains zeros. otherwise, get the records

    struct stat st;                                         // i don't know what this is, all I know is that it checks if the file exists
    if (stat(monthFilename, &st) != 0) {
        // if there's no current record, initialize it with our own record, which is just full of zeros at this point 
        file = fopen(monthFilename, "w+");

        for (int i = 0; i < MONTHS; i++) {
            // write the per month record
            fprintf(
                file, 
                "%lf,%lf,%lf,%lf\n",                        // format of how they are written (csv)
                monthlyProfits[i].costs,     
                monthlyProfits[i].additionalCosts,     
                monthlyProfits[i].revenue,     
                monthlyProfits[i].profit
            );

            // the records per day is stored in a file with their respective month name, we're just concatenating it here to get the right filename
            char currentFilename[tempFileLength];
            strcpy(currentFilename, daysFilename);
            strcat(currentFilename, monthlyProfits[i].month);       // dynamic month name
            strcat(currentFilename, extension);
            // example: storedata/reports/days/January.csv

            tempFile = fopen(currentFilename, "w+");

            // write the per day record of that month
            for (int j = 0; j < DAYS_IN_MONTH; j++) {
                fprintf(
                    tempFile, 
                    "%lf,%lf,%lf,%lf\n", 
                    monthlyProfits[i].day[j].costs,     
                    monthlyProfits[i].day[j].additionalCosts,     
                    monthlyProfits[i].day[j].revenue,     
                    monthlyProfits[i].day[j].profit
                );
            }

            fclose(tempFile);
        }

        fclose(file);
    } else {
        // if there is a record, then we get it
        file = fopen(monthFilename, "r");

        for (int i = 0; i < MONTHS; i++) {
            // copy the per month record
            read = fscanf(
                file, 
                "%lf,%lf,%lf,%lf\n",
                &monthlyProfits[i].costs,     
                &monthlyProfits[i].additionalCosts,     
                &monthlyProfits[i].revenue,     
                &monthlyProfits[i].profit
            );

            if (read != 4) {
                printf("There was a problem that occured when trying to read data from the monthlyReports.csv file\n");
                printf("It is likely that someone touched the file.\n");
                sleep(SLEEP_TIME);
                // return;
            }
            
            // same here, dynamic month name
            char currentFilename[tempFileLength];
            strcpy(currentFilename, daysFilename);
            strcat(currentFilename, monthlyProfits[i].month);
            strcat(currentFilename, extension);

            tempFile = fopen(currentFilename, "r");

            for (int j = 0; j < DAYS_IN_MONTH; j++) {
                // copy the per day record of that month
                read = fscanf(
                    tempFile, 
                    "%lf,%lf,%lf,%lf\n", 
                    &monthlyProfits[i].day[j].costs,     
                    &monthlyProfits[i].day[j].additionalCosts,     
                    &monthlyProfits[i].day[j].revenue,     
                    &monthlyProfits[i].day[j].profit
                );

                if (read != 4) {
                    printf("There was a problem that occured when trying to read data from the [monthName].csv file\n");
                    printf("It is likely that someone touched the file.\n");
                    sleep(SLEEP_TIME);
                    // return;
                }
            }

            fclose(tempFile);
        }

        fclose(file);
    }
}

// the same logic for editing an item, except we're using index(month) instead of ID
void updateReportDataFromStorage(int month, int day, struct ReportPerMonth monthData, struct ReportPerDay dayData)
{
    FILE *file;
    FILE *temp;

    char placeholder[500];
    int currenIndex = 0;

    char filename[] = "storedata/reports/monthlyReport.csv";
    char tempFilename[] = "storedata/reports/temp__monthlyReport.csv";

    file = fopen(filename, "r");
    temp = fopen(tempFilename, "w");

    while (fgets(placeholder, 500, file) != NULL) {
        // if this is the current month being updated, replace it with updated data, otherwise put the old one
        if (currenIndex == month) {
            fprintf(
                temp,
                "%lf,%lf,%lf,%lf\n",
                monthData.costs,
                monthData.additionalCosts,
                monthData.revenue,
                monthData.profit 
            );
        } else {
            fputs(placeholder, temp);
        }

        currenIndex++;
    }

    fclose(file);
    fclose(temp);

    remove(filename);
    rename(tempFilename, filename);

    updatePerDayData(monthData.month, day, dayData);
}

// same also here, except we need to get the month name and concatenate it with filename to get the correct filename
void updatePerDayData(char *month, int day, struct ReportPerDay dayData)
{
    FILE *file;
    FILE *temp;

    char filename[50] = "storedata/reports/days/";
    // concat the month
    strcat(filename, month);                                
    strcat(filename, ".csv");
    char tempFilename[] = "storedata/reports/days/temp__monthName.csv";

    char placeholder[500];
    int currenIndex = 0;

    file = fopen(filename, "r");
    temp = fopen(tempFilename, "w");

    while (fgets(placeholder, 500, file) != NULL) {
        // if the current index is the one being updated, then don't write the old data but instead write the new data
        if (currenIndex == day) {
            // writes the new data
            fprintf(
                temp,
                "%lf,%lf,%lf,%lf\n",
                dayData.costs,
                dayData.additionalCosts,
                dayData.revenue,
                dayData.profit
            );
        } else {
            // writes the old data
            fputs(placeholder, temp);
        }
        currenIndex++;
    }

    fclose(file);
    fclose(temp);

    remove(filename);
    rename(tempFilename, filename);
}