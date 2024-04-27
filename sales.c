#include "main.h"

// fills the record with zero and sets the names of month
void sales_initMonthlyProfits(struct ReportPerMonth monthlyProfits[])
{
    char months[MONTHS][15] = 
    {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    // initialize the monthly data
    for (int i = 0; i < MONTHS; i++) 
    {
        strcpy(monthlyProfits[i].month, months[i]);
        monthlyProfits[i].costs = 0.0;
        monthlyProfits[i].additionalCosts = 0.0;
        monthlyProfits[i].revenue = 0.0;
        monthlyProfits[i].profit = 0.0;

        for (int j = 0; j < DAYS_IN_MONTH; j++)
        {
            monthlyProfits[i].day[j].costs = 0.0;
            monthlyProfits[i].day[j].additionalCosts = 0.0;
            monthlyProfits[i].day[j].revenue = 0.0;
            monthlyProfits[i].day[j].profit = 0.0;
        }
    }
}

// we don't subtract the additional costs on every purchased because it will cause some miscalculation
// we instead just calculate it based on month or day not by items
double sales_getTotalProfit(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].revenue - monthlyProfits[i].costs - monthlyProfits[i].additionalCosts;

    return total;
}

double sales_getTotalCosts(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].costs;

    return total;
}

double sales_getTotalRevenue(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].revenue;

    return total;
}

double sales_getTotalExtraCosts(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].additionalCosts;

    return total;
}

// per day total
double sales_getTotalProfitOfMonth(struct ReportPerMonth monthlyProfits[], int month)
{
    double total = 0;

    for (int i = 0; i < DAYS_IN_MONTH; i++)
        total += monthlyProfits[month].day[i].revenue - monthlyProfits[month].day[i].costs - monthlyProfits[month].day[i].additionalCosts;

    return total;
}

double sales_getTotalCostsOfMonth(struct ReportPerMonth monthlyProfits[], int month)
{
    double total = 0;

    for (int i = 0; i < DAYS_IN_MONTH; i++)
        total += monthlyProfits[month].day[i].costs;

    return total;
}

double sales_getTotalRevenueOfMonth(struct ReportPerMonth monthlyProfits[], int month)
{
    double total = 0;

    for (int i = 0; i < DAYS_IN_MONTH; i++)
        total += monthlyProfits[month].day[i].revenue;

    return total;
}

double sales_getTotalExtraCostsOfMonth(struct ReportPerMonth monthlyProfits[], int month)
{
    double total = 0;

    for (int i = 0; i < DAYS_IN_MONTH; i++)
        total += monthlyProfits[month].day[i].additionalCosts;

    return total;
}

//////

// same here, per month subtraction of additional costs instead of per item
double sales_getProfitForMonth(struct ReportPerMonth monthlyProfits)
{
    double profit = monthlyProfits.revenue - monthlyProfits.costs - monthlyProfits.additionalCosts;
    return profit;
}

// same here, per day subtraction of additional costs instead of per item
double sales_getProfitForDay(struct ReportPerDay day)
{
    double profit = day.revenue - day.costs - day.additionalCosts;
    return profit;
}

// updater functions for the records
void sales_updateRevenue(struct ReportPerMonth monthlyProfits[], double price, double quantity)
{
    monthlyProfits[utils_getCurrentMonthInt()].revenue += quantity * price;
    monthlyProfits[utils_getCurrentMonthInt()].day[utils_getCurrentDayInt()].revenue += quantity * price;
}

// updater functions for the records
void sales_updateProfit(struct ReportPerMonth monthlyProfits[], double profit, double quantity)
{
    monthlyProfits[utils_getCurrentMonthInt()].profit += quantity * profit;
    monthlyProfits[utils_getCurrentMonthInt()].day[utils_getCurrentDayInt()].profit += quantity * profit;
}

// updater functions for the records
void sales_updateCosts(struct ReportPerMonth monthlyProfits[], int addedStocks, double originalPrice)
{
    monthlyProfits[utils_getCurrentMonthInt()].costs += addedStocks * originalPrice;
    monthlyProfits[utils_getCurrentMonthInt()].day[utils_getCurrentDayInt()].costs += addedStocks * originalPrice;
}

// updater functions for the records
void sales_updateAdditionalCosts(struct ReportPerMonth monthlyProfits[], double additionalCosts)
{
    monthlyProfits[utils_getCurrentMonthInt()].additionalCosts += additionalCosts;
    monthlyProfits[utils_getCurrentMonthInt()].day[utils_getCurrentDayInt()].additionalCosts += additionalCosts;
}

// updater functions for the records
void sales_reduceCosts(struct ReportPerMonth monthlyProfits[], double deduction)
{
    monthlyProfits[utils_getCurrentMonthInt()].costs -= deduction;
    monthlyProfits[utils_getCurrentMonthInt()].day[utils_getCurrentDayInt()].costs -= deduction;
}
// end

// the updater function for the storage
void sales_updateReportsFromStorage(struct ReportPerMonth monthlyProfits[])
{
    storage_updatePerMonthReportDataFromStorage(utils_getCurrentMonthInt(), monthlyProfits[utils_getCurrentMonthInt()]);
    storage_updatePerDayDataFromStorage(monthlyProfits[utils_getCurrentMonthInt()].month, utils_getCurrentDayInt(), monthlyProfits[utils_getCurrentMonthInt()].day[utils_getCurrentDayInt()] );
}

// asks the user if he/she wants to deduct the total costs of the deleted item
void sales_reflectToMonthlyCostsOnDeletion(struct ReportPerMonth monthlyProfits[], double deduction)
{
    char action;

    display_newUserMessagePage("Deleting an Item", "", "Do you want to deduct the total costs of deleted item to your monthly costs report?[y/n]", "", "", "", "");
    fflush(stdin);
    scanf("%c", &action);

    // TODO: account for additional costs
    if (action == 'y' || action == 'Y') {
        sales_reduceCosts(monthlyProfits, deduction);
        sales_updateReportsFromStorage(monthlyProfits);
    }
}

// THESE FUNCTION LOOKS SO MESSYYYY 
void sales_editPerDayReportsHandler(struct ReportPerMonth monthlyProfits[], int month)
{
    display_salesPerDayReportPromptPage(monthlyProfits[month].day, monthlyProfits, month, "Enter the number of the day you want to edit:", "");
    int day;
    scanf("%d", &day);

    if (day < 1 || day > 31) {
        display_newUserMessagePage("Editing Reports", "", "Invalid input.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask the user what to edit
    display_salesPerDayReportPromptPage(monthlyProfits[month].day, monthlyProfits, month, "Options: [ 1. Costs | 2. Extra Costs | 3. Revenue ]", "What do you want to edit?");
    int option;
    scanf("%d", &option);

    // pointers that point to whatever the user wants to edit
    double *beingEditedPerDay;                                      // points to per day data
    double *beingEditedPerMonth;                                    // points to month data

    char editingInfo[100];
    sprintf(editingInfo, "Editing: %s %d", monthlyProfits[month].month, day);

    // determine what the user wants to edit and setting up the pointers, and constructing the message to show the user
    switch (option)
    {
    case 1:
        strcat(editingInfo, " - Costs");
        beingEditedPerDay = &monthlyProfits[month].day[day - 1].costs;
        beingEditedPerMonth = &monthlyProfits[month].costs;
        break;
    case 2:
        strcat(editingInfo, " - Extra Costs");
        beingEditedPerDay = &monthlyProfits[month].day[day - 1].additionalCosts;
        beingEditedPerMonth = &monthlyProfits[month].additionalCosts;
        break;
    case 3:
        strcat(editingInfo, " - Revenue");
        beingEditedPerDay = &monthlyProfits[month].day[day - 1].revenue;
        beingEditedPerMonth = &monthlyProfits[month].revenue;
        break;
    // case 4:
    //     strcat(editingInfo, " - Profits");
    //     beingEditedPerDay = &monthlyProfits[month].day[day - 1].profit;
        // beingEditedPerMonth = &monthlyProfits[month].profit;
    //     break;
    default:
        display_newUserMessagePage("Editing Reports", "", "Invalid option.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask for the new value and show to constructed message
    display_salesPerDayReportPromptPage(monthlyProfits[month].day, monthlyProfits, month, "Enter an edited value:", editingInfo);
    double newValue;
    scanf("%lf", &newValue);

    // use that pointer to update the option that was chosen
    (*beingEditedPerDay) = newValue;

    // recalculate the new total of the month edited
    double newTotal;
    switch (option)
    {
    case 1:
        newTotal = sales_getTotalCostsOfMonth(monthlyProfits, month);
        break;
    case 2:
        newTotal = sales_getTotalExtraCostsOfMonth(monthlyProfits, month);
        break;
    case 3:
        newTotal = sales_getTotalRevenueOfMonth(monthlyProfits, month);
        break;
    }

    // update the total of that month
    (*beingEditedPerMonth) = newTotal;
    // update the data from storage
    storage_updatePerDayDataFromStorage(monthlyProfits[month].month, day - 1, monthlyProfits[month].day[day - 1]);
    storage_updatePerMonthReportDataFromStorage(month, monthlyProfits[month]);
}

// i don't care if variables are not defined at the top
void sales_editPerMonthReportsHandler(struct ReportPerMonth monthlyProfits[])
{
    char action[2];

    // ask the user for the month
    display_salesReportPromptPage(monthlyProfits, "Enter the number of the month you want to edit:", "Enter 'b' to go back");
    scanf("%s", action);

    if (strcmp(action, "b") == 0 || strcmp(action, "B") == 0) return;

    // if the user enters the number of month, it will be a string. so convert it to int
    int month = atoi(action) - 1;

    // if the user input is out of bounds, don't proceed
    if (month < 0 || month > 11) {
        display_newUserMessagePage("Editing Reports", "", "Invalid input.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    // ask the user what to edit
    display_salesReportPromptPage(monthlyProfits, "Options: [ 1. Costs | 2. Extra Costs | 3. Revenue ]", "What do you want to edit?");
    int option;
    scanf("%d", &option);
    
    // pointer that points to whatever the user wants to edit
    double *beingEdited;

    char editingInfo[100] = "Editing: ";
    strcat(editingInfo, monthlyProfits[month].month);

    // determine what the user wants to edit and setting up the pointer
    switch (option)
    {
    case 1:
        strcat(editingInfo, " - Costs");
        beingEdited = &monthlyProfits[month].costs;
        break;
    case 2:
        strcat(editingInfo, " - Extra Costs");
        beingEdited = &monthlyProfits[month].additionalCosts;
        break;
    case 3:
        strcat(editingInfo, " - Revenue");
        beingEdited = &monthlyProfits[month].revenue;
        break;
    // case 4:
    //     strcat(editingInfo, " - Profits");
    //     beingEdited = &monthlyProfits[month].profit;
    //     break;
    default:
        display_newUserMessagePage("Editing Reports", "", "Invalid option.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }
    
    // ask for the new value
    display_salesReportPromptPage(monthlyProfits, "Enter an edited value:", editingInfo);
    double newValue;
    scanf("%lf", &newValue);

    // update the chosen option using that pointer
    (*beingEdited) = newValue;
    
    // update the storage data as well
    storage_updatePerMonthReportDataFromStorage(month, monthlyProfits[month]);
}
