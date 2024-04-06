#define NAME_SIZE 20
#define ID_LENGTH 10
#define MONTHS 12

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
void addItem(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void deleteItem(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void editItem(struct Node **head);
void searchItem(struct Node **head);
void reflectToMonthlyCostsOnDeletion(struct ProfitPerMonth monthlyProfits[], double deduction);
void sellItem(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void addTestItem(struct Node **head, struct ProfitPerMonth monthlyProfits[], char name[], int stocks, double price);
void restockItem(struct Node **head, struct ProfitPerMonth monthlyProfits[]);

// utils
char* getCurrentTime();
char* getCurrentDate();
void generateId(char placeholder[]);
int getCurrentDateInt();

// main
void printLinkedlist(const struct Node *head);
void initMonthlyProfits(struct ProfitPerMonth monthlyProfits[]);
int getListSize(const struct Node *head);
void viewInventory(const struct Node *head);
double getTotalProfit(struct ProfitPerMonth monthlyProfits[]);
double getTotalCosts(struct ProfitPerMonth monthlyProfits[]);
void printActions();
void freeLinkedList(struct Node **head);
void viewReports(struct ProfitPerMonth monthlyProfits[]);