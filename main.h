#define NAME_SIZE 20
#define ID_LENGTH 10
#define MONTHS 12

// I'm using linked list because it makes that list management easier.
// adding, deleting, updating, etc. is so much easier than using traditional arrays

// structure of an individual item
struct Item
{
    char name[NAME_SIZE];
    int baseStocks;
    int stocks;
    double price;
    double originalPrice;
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
void updateItem(struct Node **head);
void searchItem(struct Node **head);
void sellItem(struct Node **head, struct ProfitPerMonth monthlyProfits[]);
void addTestItem(struct Node **head, struct ProfitPerMonth monthlyProfits[], char name[], int stocks, double price);

// utils
char* getCurrentTime();
char* getCurrentDate();
char *generateId(char placeholder[]);
int getCurrentDateInt();

// main
void printLinkedlist(const struct Node *head);
void initMonthlyProfits(struct ProfitPerMonth monthlyProfits[]);
int getListSize(const struct Node *head);
void viewInventory(const struct Node *head);
void printActions();
void freeLinkedList(struct Node **head);
void viewReports(struct ProfitPerMonth monthlyProfits[]);