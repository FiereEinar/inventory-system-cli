#ifndef MAIN_H
#define MAIN_H
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      CONSTANTS
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define NAME_SIZE               30
#define ID_LENGTH               10
#define MONTHS                  12
#define SLEEP_TIME              1                               // the pause when showing information or errors before returning to previous page
#define DATE_LENGTH             30
#define MONTH_NAME_LEN          15
#define DAYS_IN_MONTH           31
#define CATEGORY_NAME_LEN       30                              // length of the name of an items category
#define MAX_CATEGORIES          30                              // maximum amount of categories
#define MAX_CART_ITEMS          15                              // maximum amount of item in the cart
#define MAX_RECEIPT_LENGTH      4096                            // the receipt is one giant string and this is the size

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      STRUCTS
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// structure of an individual item
struct Item
{
    char name[NAME_SIZE];
    int baseStocks;
    int stocks;
    double price;
    double originalPrice;
    double profit;
    char dateAdded[DATE_LENGTH];
    char lastUpdated[DATE_LENGTH];
    char id[ID_LENGTH];
    char category[CATEGORY_NAME_LEN];
};

// structure of an individual node in a linked list
struct Node
{
    struct Item data;                                   // contains the Item
    struct Node *next;                                  // a pointer that points to the next item
};

struct ReportPerDay
{
    double costs;
    double additionalCosts;
    double revenue;
    double profit;
};

// structure of sales report on each month, will be stored in array of 12(months)
struct ReportPerMonth
{
    char month[MONTH_NAME_LEN];                         // name of the month
    struct ReportPerDay day[DAYS_IN_MONTH];             // per day report stored in array[31]
    double costs;
    double additionalCosts;
    double revenue;
    double profit;
};

// structure of an individual item in a cart, will be stored in an array
struct CartItem 
{
    char name[NAME_SIZE];
    char itemId[ID_LENGTH];
    int quantity;
    double price;
    double profit;
};

// structure of a session cart
struct Cart 
{
    struct CartItem items[MAX_CART_ITEMS];
    int amountOfItems;
    char cartId[ID_LENGTH];
};

struct Settings
{
    int sortBy;
};

struct Settings settings;
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//      FUNCTION PROTOTYPES
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: main.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void viewInventory(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void viewCategories(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void viewPos(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[]);
void viewReceipts();
void viewReports(struct ReportPerMonth monthlyProfits[]);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: item.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void addItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void deleteItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[]);
void editItemHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void searchItemHandler(struct Node **head);
void viewItemDetails(struct Node **head);
void reflectToMonthlyCostsOnDeletion(struct ReportPerMonth monthlyProfits[], double deduction);
void addItemToList(struct Node **head, struct ReportPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, char category[], double additionalCost);
void restockItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[]);
struct Node *getItemById(struct Node **list, char itemId[]);
void addItemToLinkedList(struct Node **head, struct Item newItem);
void updateItemsCategory(struct Node **head, char *oldCategory, char *newCategory);
void getItemsByCategory(struct Node **head, char *category, struct Node **placeholder);
void getStockStatus(char *status, int stocks, int baseStocks);
void sortItemsHandler(struct Node **head);
void determineWhatToSort(char *placeholder);
void getSortedItems(struct Node **head, struct Node **output);
void sortItemsByLowStocks(struct Node **head, struct Node **output);
void insertItemAt(int index, struct Node **destination, struct Node **toInsert);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: utils.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
char* getCurrentTime();
char* getCurrentDate();
void generateId(char placeholder[]);
int getCurrentDateInt();
int getCurrentDayInt();
int getListSize(struct Node **head);
void freeLinkedList(struct Node **head);
void clearNewline(char *string);
void updateDate(char *placeholder);
void joinStocks(char *output, int stock, int base);
void toLowercase(char *str);
void capitalizeFirst(char *str);
void getStringPercentage(char *output, int number, int base);
double getPercentage(int number, int base);
void centerText(int length, char *text);
void clearAllNewline(char *str);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: sales.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void initMonthlyProfits(struct ReportPerMonth monthlyProfits[]);
double getTotalProfit(struct ReportPerMonth monthlyProfits[]);
double getTotalCosts(struct ReportPerMonth monthlyProfits[]);
double getTotalRevenue(struct ReportPerMonth monthlyProfits[]);
double getTotalExtraCosts(struct ReportPerMonth monthlyProfits[]);
double getProfitForMonth(struct ReportPerMonth monthlyProfits);
double getProfitForDay(struct ReportPerDay day);
void updateRevenue(struct ReportPerMonth monthlyProfits[], double price, double quantity);
void updateProfit(struct ReportPerMonth monthlyProfits[], double profit, double quantity);
void updateCosts(struct ReportPerMonth monthlyProfits[], int addedStocks, double originalPrice);
void updateAdditionalCosts(struct ReportPerMonth monthlyProfits[], double additionalCosts);
void reduceCosts(struct ReportPerMonth monthlyProfits[], double deduction);
void updateReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: display.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// RECEIPT PAGE
void recieptsPage();
void recieptsPromptPage(char *message1, char *message2);
void printRecieptsHeader();
// CART PAGE
void pointOfSalePromptPage(struct CartItem items[], int *amountOfItems, char *message1, char *message2);
void pointOfSalePage(struct CartItem items[], int *amountOfItems);
void printCartItemsHeader();
void printCartItems(struct CartItem items[], int *amountOfItems);
// CATEGORY PAGE
void itemCategoryPage(struct Node **head);
void categoryPromptPage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *message1, char *message2);
void categoryPage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void printCategoryHeader();
void printCategoryList(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);

void askUserToFullScreen();
void itemDataPromptPage(struct Item item, char *header, char *message1, char *message2);
void itemDataPage(struct Item item);
void newUserMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void userMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void printMinimumScreenHeight(int itemsLength);

// SALES REPORT PAGE
void salesReportPage(struct ReportPerMonth monthlyProfits[]);
void salesPerDayReportPage(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month);
void printPerDayReports(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month);
void printMonthlyReports(struct ReportPerMonth monthlyProfits[]);
void printReportHeader();
void printMonthlyReportHeader(char *month);
// INVENTORY PAGE
void inventoryPromptPage(struct Node **head, char *bottomMessage1, char *bottomMessage2);
void inventoryPage(struct Node **head);
void printItemList(struct Node **head);
void printItemHeader();
// MENU PAGE
void menuPage();

void topBar(char *text);
void bannerFullBorder();
void bannerBlankBorderTextCen (char *text);
void bannerBlankBorderText (char *text);
void bannerBlankBorderTextLeft (char *text);
void bannerBlankBorder ();
void bannerUserInput();
void bannerFullBorderSection();
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: storage.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void addItemToStorage(struct Item item);
void getItemsFromStorage(struct Node **head);
void deleteItemFromStorageById(char *id);
void editItemFromStorageById(char *id, struct Item item);
void checkReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void initializeReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void getReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void updateReportDataFromStorage(int month, int day, struct ReportPerMonth monthData, struct ReportPerDay dayData);
void updatePerDayData(char *month, int day, struct ReportPerDay dayData);
void addCategoryToStorage(char *category);
void getCategoriesFromStorage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void deleteCategoryFromStorage(char *category);
void editCategoryFromStorage(char *oldCategory, char *newCategory);
void addRecieptToStorage(char *reciept, char *id);
void addRecieptMetaDataToStorage(char *recieptId, char *datePurchased);
void printReceiptsMetaDataFromStorage(int *counter);
int getReceiptFromStorageById(char *id, char *receiptBuffer);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: category.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void addCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void deleteCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void addCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *categoryToAdd);
void editCategoryHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
int isValidCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *category);
void categoryPreview(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void itemCategoryPrompter(char *placeholder, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void viewCategoryItems(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: pos.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void addCartItemHandler(struct Cart *cart, struct Node **head);
void addItemToCart(struct Cart *cart, int quantity, struct Item item);
void deleteCartItemHandler(struct Cart *cart);
void resetCartHandler(struct Cart *cart);
void resetCart(struct Cart *cart);
void checkoutHandler(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[]);
void generateReceipt(struct Cart *cart, char *receiptBuffer);
void saveRecieptMetaData(char *recieptId);
void viewReceiptHandler();
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#endif
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/*

    ███╗  ██╗       ███████╗         ██████╗        ██╗  ██╗                                                    ██╗  ██╗
    ████╗ ██║       ╚═██║══╝        ██╔════╝        ██║ ██╔╝                                                    ╚██╗██╔╝
    ██╔██╗██║         ██║           ██║             █████╔╝             https://github.com/FiereEinar            ╚███╔╝ 
    ██║╚████║         ██║           ██║             ██╔═██╗                                                      ██╔██╗ 
    ██║ ╚███║       ███████╗        ╚██████╗        ██║  ██╗                                                    ██╔╝ ██╗  ██╗
    ╚═╝  ╚══╝       ╚══════╝         ╚═════╝        ╚═╝  ╚═╝                                                    ╚═╝  ╚═╝  ╚═╝

*/
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
