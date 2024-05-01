#include "main.h"

void settings_changeShopNameHandler()
{
    char currentShopName[100] = "Current shop name - ";
    char newShopName[50];

    strcat(currentShopName, settings.shopName);

    display_newUserMessagePage("Changing Shop Name", "Enter 'b' to go back", currentShopName, "Enter a new shop name:", "", "", "");
    fflush(stdin);
    fgets(newShopName, SHOP_LOC_LEN, stdin);
    utils_clearNewline(newShopName);

    if (strcmp(newShopName, "b") == 0 || strcmp(newShopName, "B") == 0) return;

    // TODO: fix this?
    if (strlen(newShopName) > SHOP_NAME_LEN - 1) {
        display_newUserMessagePage("Changing Shop Name", "", "Shop name is too long, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    if (strlen(newShopName) <= 1) {
        display_newUserMessagePage("Changing Shop Name", "", "Shop name is too short, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    settings_changeShopName(newShopName);
    storage_updateSettingsFromStorage();

    display_newUserMessagePage("Changing Shop Name", "", "Shop name changed successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void settings_changeShopLocationHandler()
{
    char currentShopLocation[100] = "Current shop location - ";
    char newShopLocation[50];

    strcat(currentShopLocation, settings.shopLocation);

    display_newUserMessagePage("Changing Shop Location", "Enter 'b' to go back", currentShopLocation, "Enter a new shop location:", "", "", "");
    fflush(stdin);
    fgets(newShopLocation, SHOP_LOC_LEN, stdin);
    utils_clearNewline(newShopLocation);

    if (strcmp(newShopLocation, "b") == 0 || strcmp(newShopLocation, "B") == 0) return;

    if (strlen(newShopLocation) > SHOP_LOC_LEN) {
        display_newUserMessagePage("Changing Shop Name", "", "Shop location is too long, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    if (strlen(newShopLocation) <= 1) {
        display_newUserMessagePage("Changing Shop Name", "", "Shop location is too short, please try again.", "", "", "", "");
        sleep(SLEEP_TIME);
        return;
    }

    settings_changeShopLocation(newShopLocation);
    storage_updateSettingsFromStorage();
    
    display_newUserMessagePage("Changing Shop Name", "", "Shop location changed successfully!", "", "", "", "");
    sleep(SLEEP_TIME);
}

void settings_changeShopLocation(char *newShopLocation)
{
    strcpy(settings.shopLocation, newShopLocation);
}

void settings_changeShopName(char *newShopName)
{
    strcpy(settings.shopName, newShopName);
}
