#define NAME_SIZE 20
#define ID_LENGTH 10
#define MONTHS 12
#define SLEEP_TIME 2

// structure of an individual item
struct Item
{
    char name[NAME_SIZE];
    int baseStocks;
    int stocks;
    double price;
    double originalPrice;
    double profit;
    char dateAdded[30];
    char lastUpdated[30];
    char id[ID_LENGTH];
};

// structure of an individual node in a linked list
struct Node
{
    struct Item data;       // contains the Item
    struct Node *next;      // a pointer that points to the next item
};

// structure of sales report on each month, will be stored in array of 12(months)
struct ProfitPerMonth
{
    char month[15];
    double costs;
    double revenue;
    double profit;
};

// function prototypes

// item
void addItemHandler(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void deleteItemHandler(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void editItemHandler(struct Node **head);
void searchItem(struct Node **head);
void reflectToMonthlyCostsOnDeletion(struct ProfitPerMonth monthlyProfits[], double deduction);
void sellItem(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void addItemToList(struct Node **head, struct ProfitPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, double additionalCost);
void restockItem(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
struct Node *getItemById(struct Node **list, char itemId[]);

// utils
char* getCurrentTime();
char* getCurrentDate();
void generateId(char placeholder[]);
int getCurrentDateInt();

// main
void printLinkedlist(const struct Node *head);
void initMonthlyProfits(struct ProfitPerMonth monthlyProfits[]);
int getListSize(struct Node **head);
void viewInventory(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void viewItemDetails(struct Node **head);
double getTotalProfit(struct ProfitPerMonth monthlyProfits[]);
double getTotalCosts(struct ProfitPerMonth monthlyProfits[]);
double getTotalRevenue(struct ProfitPerMonth monthlyProfits[]);
void printActions();
void freeLinkedList(struct Node **head);
void viewReports(struct ProfitPerMonth monthlyProfits[]);
// void printItemData(struct Item item);

// display
void itemDataPage(struct Item item);
void newUserMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void userMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void printMinimumScreenHeight(int itemsLength);
void salesReportPage(struct ProfitPerMonth monthlyProfits[]);
void printMonthlyReports(struct ProfitPerMonth monthlyProfits[]);
void printReportHeader();
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
