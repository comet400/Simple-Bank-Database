#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <functional>

#pragma warning(disable : 4996)

using namespace std;

class Database
{
private:
    string password;
    int accountID;
    string accountName;
    double balance;
    string email;

public:
    void setPassword(string password) { this->password = password; }
    void setAccountID(int accountID) { this->accountID = accountID; }
    void setAccountName(string accountName) { this->accountName = accountName; }
    void setBalance(double balance) { this->balance = balance; }
    void setEmail(string email) { this->email = email; }

    string getPassword() { return password; }
    int getAccountID() { return accountID; }
    string getAccountName() { return accountName; }
    double getBalance() { return balance; }
    string getEmail() { return email; }

    Database(string password, int accountID, string accountName, double balance, string email)
    {
        this->password = password;
        this->accountID = accountID;
        this->accountName = accountName;
        this->balance = balance;
        this->email = email;
    }
};

// Prototypes
void createCSVFileIfNotExists(string bankName);
void addBankDatabases(vector<string>& bankDatabases);
void printAvailableBanks(vector<string>& bankDatabases);
void createAllBanks(vector<string>& bankDatabases);
void promptUserBank(vector<string>& bankDatabases);
void addAccountToDatabase(const string& bankName, Database& account);
bool checkSameEmail(const string& bankName, const string& email);
int generateAccountID(const string& bankName, const string& email);
bool checkAccountPassword(const string& bankName, const string& email, const string& password);
void promptUserToCreateAccount(const string& bankName);
void loginToAccount(const string& bankName);
void promptContinue();
void validateCin();
void userLoggedMenu(const string& bankName, const string& email);
void updateAccountBalance(const string& bankName, int accountID, double newBalance);
void checkLoggedAccount(const string& bankName, const string& email);
void handleWithDraw(const string& bankName, const string& email);
int searchCurrentEmailID(const string& bankName, const string& email);
void handleDeposit(const string& bankName, const string& email);
string getAccountBalance(const string& bankName, const string& email);
void printAvailableBanks(vector<string>& bankDatabases);
void deleteAccount(const string& bankName, const string& email);
void deleteBankDatabase(vector<string>& bankDatabases);
void showAllAccountsInBank();
void deleteAllAccountsInBank();
void changeAccountBalance();
void changeAccountPassword();
void handleADM_Inputs(vector<string>& bankDatabases);
void createNewBankDatabase(vector<string>& bankDatabases);





