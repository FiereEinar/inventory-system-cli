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
    storage_updateReportDataFromStorage(utils_getCurrentMonthInt(), utils_getCurrentDayInt(), monthlyProfits[utils_getCurrentMonthInt()], monthlyProfits[utils_getCurrentMonthInt()].day[utils_getCurrentDayInt()] );
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
