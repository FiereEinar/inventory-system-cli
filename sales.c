#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"

// we don't subtract the additional costs on every purchased because it will cause some miscalculation
// we instead just calculate it based on month or day not by items
double getTotalProfit(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].revenue - monthlyProfits[i].costs - monthlyProfits[i].additionalCosts;

    return total;
}

double getTotalCosts(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].costs;

    return total;
}

double getTotalRevenue(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].revenue;

    return total;
}

double getTotalExtraCosts(struct ReportPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].additionalCosts;

    return total;
}

// fills the record with zero and sets the names of month
void initMonthlyProfits(struct ReportPerMonth monthlyProfits[])
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

// same here, per month subtraction of additional costs instead of per item
double getProfitForMonth(struct ReportPerMonth monthlyProfits)
{
    double profit = monthlyProfits.revenue - monthlyProfits.costs - monthlyProfits.additionalCosts;
    return profit;
}

// same here, per day subtraction of additional costs instead of per item
double getProfitForDay(struct ReportPerDay day)
{
    double profit = day.revenue - day.costs - day.additionalCosts;
    return profit;
}

// updater functions for the records
void updateRevenue(struct ReportPerMonth monthlyProfits[], double price, double quantity)
{
    monthlyProfits[getCurrentDateInt()].revenue += quantity * price;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].revenue += quantity * price;
}

// updater functions for the records
void updateProfit(struct ReportPerMonth monthlyProfits[], double profit, double quantity)
{
    monthlyProfits[getCurrentDateInt()].profit += quantity * profit;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].profit += quantity * profit;
}

// updater functions for the records
void updateCosts(struct ReportPerMonth monthlyProfits[], int addedStocks, double originalPrice)
{
    monthlyProfits[getCurrentDateInt()].costs += addedStocks * originalPrice;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].costs += addedStocks * originalPrice;
}

// updater functions for the records
void updateAdditionalCosts(struct ReportPerMonth monthlyProfits[], double additionalCosts)
{
    monthlyProfits[getCurrentDateInt()].additionalCosts += additionalCosts;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].additionalCosts += additionalCosts;
}

// updater functions for the records
void reduceCosts(struct ReportPerMonth monthlyProfits[], double deduction)
{
    monthlyProfits[getCurrentDateInt()].costs -= deduction;
    monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()].costs -= deduction;
}
// end

// the updater function for the storage
void updateReportsFromStorage(struct ReportPerMonth monthlyProfits[])
{
    updateReportDataFromStorage(getCurrentDateInt(), getCurrentDayInt(), monthlyProfits[getCurrentDateInt()], monthlyProfits[getCurrentDateInt()].day[getCurrentDayInt()] );
}