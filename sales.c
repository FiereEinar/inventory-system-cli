#include <stdio.h>
#include <string.h>
#include "main.h"

void viewReports(struct ProfitPerMonth monthlyProfits[])
{
    salesReportPage(monthlyProfits);

    char back;
    bannerUserInput();
    fflush(stdin);
    scanf("%c", &back);
}

double getTotalProfit(struct ProfitPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].revenue - monthlyProfits[i].costs;

    return total;
}

double getTotalCosts(struct ProfitPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].costs;

    return total;
}

double getTotalRevenue(struct ProfitPerMonth monthlyProfits[])
{
    double total = 0;

    for (int i = 0; i < MONTHS; i++)
        total += monthlyProfits[i].revenue;

    return total;
}

void initMonthlyProfits(struct ProfitPerMonth monthlyProfits[])
{
    char months[MONTHS][15] = 
    {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    // initialize the monthly data
    for (int i = 0; i < MONTHS; ++i) 
    {
        strcpy(monthlyProfits[i].month, months[i]);
        monthlyProfits[i].costs = 0.0;
        monthlyProfits[i].revenue = 0.0;
        monthlyProfits[i].profit = 0.0;
    }
}