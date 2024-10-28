#include "Bank.h"

int main(void)
{
    vector<string> bankDatabases;
    addBankDatabases(bankDatabases);
    createAllBanks(bankDatabases);
    printAvailableBanks(bankDatabases);
    promptUserBank(bankDatabases);

    return 0;
} 