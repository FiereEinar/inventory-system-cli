#define NAME_SIZE 30
#define ID_LENGTH 10
#define MONTHS 12
#define SLEEP_TIME 1                                    // the pause when showing information or errors before returning to previous page
#define DATE_LENGTH 30
#define MONTH_NAME_LEN 15
#define DAYS_IN_MONTH 31

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
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//      FUNCTION PROTOTYPES
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: main.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void viewInventory(struct Node **head, struct ReportPerMonth monthlyProfits[]);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: item.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void addItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[]);
void deleteItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[]);
void editItemHandler(struct Node **head);
void viewItemDetails(struct Node **head);
void searchItemHandler(struct Node **head);
void reflectToMonthlyCostsOnDeletion(struct ReportPerMonth monthlyProfits[], double deduction);
void sellItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[]);
void addItemToList(struct Node **head, struct ReportPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, double additionalCost);
void restockItem(struct Node **head, struct ReportPerMonth monthlyProfits[]);
struct Node *getItemById(struct Node **list, char itemId[]);
void addItemToLinkedList(struct Node **head, struct Item newItem);
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
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: sales.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void viewReports(struct ReportPerMonth monthlyProfits[]);
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
void askUserToFullScreen();
void itemDataPage(struct Item item);
void newUserMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void userMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void printMinimumScreenHeight(int itemsLength);
void salesReportPage(struct ReportPerMonth monthlyProfits[]);
void salesPerDayReportPage(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month);
void printPerDayReports(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month);
void printMonthlyReports(struct ReportPerMonth monthlyProfits[]);
void printReportHeader();
void printMonthlyReportHeader(char *month);
void inventoryPage(struct Node **head);
void printItemList(struct Node **head);
void printItemHeader();
void menuPage();
void topBar(char *text);
void bannerFullBorder();
void bannerBlankBorderTextCen (char *text);
void bannerBlankBorderText (char *text);
void bannerBlankBorderTextLeft (char *text);
void bannerBlankBorder ();
void bannerUserInput();
void bannerFullBorderSection ();
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: storage.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void addItemToStorage(struct Item item);
void getItemsFromStorage(struct Node **head);
void deleteItemFromStorageById(char *id);
void editItemFromStorageById(char *id, struct Item item);
void initReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void updateReportDataFromStorage(int month, int day, struct ReportPerMonth monthData, struct ReportPerDay dayData);
void updatePerDayData(char *month, int day, struct ReportPerDay dayData);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
