#define NAME_SIZE 20
#define ID_LENGTH 10

// I'm using linked list because it makes that list management easier.
// adding, deleting, updating, etc. is so much easier than using traditional arrays

// structure of an individual item
struct Item
{
    char name[NAME_SIZE];
    int baseStocks;
    int stocks;
    double price;
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

// function prototypes
void printLinkedlist(const struct Node *head);
void addItem(struct Node **head);
void deleteItem(struct Node **head);
int getListSize(const struct Node *head);
void updateItem(struct Node **head);
void searchItem(struct Node **head);
void sellItem(struct Node **head);
void freeLinkedList(struct Node **head);
void addTestItem(struct Node **head, char name[], int stocks, double price);
char* getCurrentTime();
char* getCurrentDate();
char *generateId(char placeholder[]);
void printHelp();
void printActions();