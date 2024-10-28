#include "Bank.h"

void createCSVFileIfNotExists(string bankName)
{
    bankName += ".csv";

    ifstream checkFile(bankName);
    if (!checkFile.is_open())
    {
        ofstream file(bankName);
        if (file.is_open())
        {
            file << "Bank Name,Password,Account ID,Account Name,Balance,Email\n";
            file.close();
            cout << "Created new CSV file: " << bankName << endl;
        }
        else
        {
            cerr << "Error: Unable to create file." << endl;
        }
    }

    checkFile.close();
}

void addBankDatabases(vector<string>& bankDatabases)
{
    bankDatabases = {
        "Chase", "BankOfAmerica", "WellsFargo", "CitiBank", "HSBC",
        "Barclays", "DeutscheBank", "Santander", "BNPParibas", "GoldmanSachs"
    };
}

void printAvailableBanks(const vector<string>& bankDatabases)
{
    system("cls");
    cout << "Available banks:\n\n";
    for (const string& bank : bankDatabases)
    {
        cout << bank << endl;
    }
    cout << endl;
}

void createAllBanks(vector<string>& bankDatabases)
{
    for (const string& bank : bankDatabases)
    {
        createCSVFileIfNotExists(bank);
    }
}

bool checkSameEmail(const string& bankName, const string& email)
{
    ifstream file(bankName + ".csv");
    string line, tempEmail;
    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, tempEmail, ','); // skip bank name
        for (int i = 0; i < 5; i++) getline(ss, tempEmail, ','); // read the email column

        if (tempEmail == email) return true;
    }
    return false;
}

// Function to generate account ID by checking the last used ID
int generateAccountID(const string& bankName, const string& email)
{
    // Using std::hash to create a hash based on the email
    std::hash<string> hash_fn;
    size_t hash_value = hash_fn(email);

    // Convert the hashed value to an integer
    int accountID = static_cast<int>(hash_value % 100000); // Limit accountID to 5 digits for simplicity

    // Ensure the ID is not too small (to maintain some level of consistency with prior system)
    if (accountID < 1000)
        accountID += 1000;

    return accountID;
}

void addAccountToDatabase(const string& bankName, Database& account)
{
    ofstream outFile;
    outFile.open(bankName + ".csv", ios::app);

    if (!outFile.is_open())
    {
        cerr << "Error: Unable to open file for appending." << endl;
        return;
    }

    outFile << bankName << "," << account.getPassword() << "," << account.getAccountID() << ","
        << account.getAccountName() << "," << account.getBalance() << "," << account.getEmail() << endl;

    outFile.close();
}

void promptUserToCreateAccount(const string& bankName)
{
    system("cls");

    string accountName, password, email;
    double balance;

    cout << "Enter email: ";
    cin >> email;
    validateCin();
    while (cin.fail()) {
        cout << "Enter email: ";
        cin >> email;
        validateCin();
    }

    if (checkSameEmail(bankName, email))
    {
        cout << "Error: This email is already used. Please try another one.\n";
    }
    else
    {
        cout << "Enter password: ";
        cin >> password;
        validateCin();
        while (cin.fail()) {
            cout << "Enter password: ";
            cin >> password;
            validateCin();
        }

        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, accountName);
        validateCin();
        while (cin.fail()) {
            cout << "Enter your name: ";
            cin >> accountName;
            validateCin();
        }

        cout << "Enter balance: ";
        cin >> balance;
        validateCin();
        while (cin.fail()) {
            cout << "Enter balance: ";
            cin >> balance;
            validateCin();
        }

        int accountID = generateAccountID(bankName, email);

        Database account(password, accountID, accountName, balance, email);
        addAccountToDatabase(bankName, account);

        cout << "Account created successfully with ID: " << accountID << endl;
    }
    promptContinue();
}

void loginToAccount(const string& bankName)
{
    system("cls");

    string email, password;
    cout << "Enter email: ";
    cin >> email;
    validateCin();
    while (cin.fail()) {
        cout << "Enter email: ";
        cin >> email;
        validateCin();
    }

    if (checkSameEmail(bankName, email))
    {
        cout << "Enter password: ";
        cin >> password;
        validateCin();
        while (cin.fail()) {
            cout << "Enter password: ";
            cin >> password;
            validateCin();
        }

        if (checkAccountPassword(bankName, email, password))
        {
            cout << "Login successful.\n";
            promptContinue();
            userLoggedMenu(bankName, email);
        }
        else
        {
            cout << "Incorrect password. Please try again.\n";
        }
    }
    else
    {
        cout << "Account not found. Please try again.\n";
    }
    promptContinue();
}

bool checkAccountPassword(const string& bankName, const string& email, const string& password)
{
    ifstream file(bankName + ".csv");
    string line, tempEmail, tempPassword;
    while (getline(file, line))
    {
        stringstream ss(line);
        string temp;

        getline(ss, temp, ',');

        getline(ss, tempPassword, ',');
        for (int i = 0; i < 4; i++) getline(ss, tempEmail, ',');

        if (tempEmail == email && tempPassword == password) return true;
    }
    return false;
}

void promptUserBank(vector<string>& bankDatabases)
{
    string bankName;
    cout << "Enter bank name: ";
    cin >> bankName;
    validateCin();
    while (cin.fail()) {
        cout << "Enter bank name: ";
        cin >> bankName;
        validateCin();
    }

    if (bankName == "ADM")
    {
        handleADM_Inputs(bankDatabases);
    }

    if (find(bankDatabases.begin(), bankDatabases.end(), bankName) != bankDatabases.end())
    {
        cout << "Bank name exists." << endl;

        int option;
        do
        {
            system("cls");
            cout << "========================================\n";
            cout << "|           Account Sign In            |\n";
            cout << "========================================\n";
            cout << "|  1. Create Account                   |\n";
            cout << "|  2. Login to Account                 |\n";
            cout << "|  3. Exit                             |\n";
            cout << "========================================\n";
            cout << "Enter your choice: ";
            cin >> option;
            validateCin();
            while (cin.fail()) {
                cout << "Enter your choice: ";
                cin >> option;
                validateCin();
            }

            switch (option)
            {
            case 1:
                promptUserToCreateAccount(bankName);
                break;
            case 2:
                loginToAccount(bankName);
                break;
            case 3:
                cout << "Exiting the bank system.\n";
                printAvailableBanks(bankDatabases);
                promptUserBank(bankDatabases);
                break;
            default:
                cout << "Invalid option. Please choose again.\n";
                promptContinue(); // Added promptContinue() here
            }
        } while (option != 3);
    }
    else
    {
        cout << "Error: Bank name does not exist. Please choose an existing bank." << endl;
        promptContinue(); // Added promptContinue() here
    }
}

void promptContinue()
{
    cout << "Press any key to continue...";
    cin.ignore();
    cin.get();
}

void validateCin()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void userLoggedMenu(const string& bankName, const string& email)
{
    int option;
    do
    {
        system("cls");
        cout << "========================================\n";
        cout << "|              Your Account            |\n";
        cout << "========================================\n";
        cout << "|  1. Deposit                          |\n";
        cout << "|  2. Withdraw                         |\n";
        cout << "|  3. Check Account                    |\n";
        cout << "|  4. Delete your account              |\n"; // Added new option "Delete your account"
        cout << "|  5. Exit                             |\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";

        cin >> option;
        validateCin();
        while (cin.fail()) {
            cout << "Enter your choice: ";
            cin >> option;
            validateCin();
        }

        switch (option)
        {
        case 1:
            handleDeposit(bankName, email);
            break;
        case 2:
            handleWithDraw(bankName, email);
            break;
        case 3:
            checkLoggedAccount(bankName, email);
            break;
        case 4:
            deleteAccount(bankName, email); // Added new function to delete account
            break;
        case 5:
            cout << "Exiting the bank system.\n";
            return;
            break;
        default:
            cout << "Invalid option. Please choose again.\n";
            promptContinue(); // Added promptContinue() here
        }
    } while (option != 4);
}

void deleteAccount(const string& bankName, const string& email)
{
    string password;
    int accountID;
    char securityPrompt;
    cout << "Enter account password: ";
    cin >> password;
    validateCin();
    while (cin.fail()) {
        cout << "Enter account password: ";
        cin >> password;
        validateCin();
    }

    if (!checkAccountPassword(bankName, email, password))
    {
        cout << "Incorrect password. Please try again.\n";
        promptContinue(); // Added promptContinue() here
        userLoggedMenu(bankName, email);
        return;
    }

    cout << "Are you sure you want to delete your account? (Y/N): ";
    cin >> securityPrompt;
    validateCin();
    while (cin.fail()) {
        cout << "Are you sure you want to delete your account? (Y/N): ";
        cin >> securityPrompt;
        validateCin();
    }

    if (securityPrompt == 'Y' || securityPrompt == 'y')
    {
        accountID = searchCurrentEmailID(bankName, email);
        ifstream file(bankName + ".csv");
        ofstream tempFile("temp.csv");

        if (!file.is_open() || !tempFile.is_open())
        {
            cerr << "Error: Could not open file for account deletion." << endl;
            promptContinue(); // Added promptContinue() here
            return;
        }

        string line;
        bool accountFound = false;

        while (getline(file, line))
        {
            stringstream ss(line);

            // Assuming the CSV columns are: BankName, Password, AccountID, AccountName, Balance, Email
            string bankNameColumn, password, tempAccountID, accountName, balance, tempEmail;

            // Extract columns in order
            getline(ss, bankNameColumn, ',');  // Bank name
            getline(ss, password, ',');        // Password
            getline(ss, tempAccountID, ',');   // Account ID
            getline(ss, accountName, ',');     // Account name
            getline(ss, balance, ',');         // Balance
            getline(ss, tempEmail, ',');       // Email

            try {
                int existingID = stoi(tempAccountID);  // Convert string to int for comparison

                if (existingID == accountID) {
                    // Skip the line for the account to be deleted
                    accountFound = true;
                }
                else {
                    // Write the line to the temp file
                    tempFile << bankNameColumn << "," << password << "," << tempAccountID << ","
                        << accountName << "," << balance << "," << tempEmail << '\n';
                }
            }
            catch (invalid_argument& e) {
                cerr << "Error: Invalid account ID in the CSV file: " << e.what() << endl;
            }
        }

        file.close();
        tempFile.close();

        if (!accountFound) {
            cout << "Account not found for deletion." << endl;
        }
        else {
            // Replace the original file with the updated temp file
            ofstream originalFile(bankName + ".csv", ios::trunc);
            ifstream tempInputFile("temp.csv");

            if (!originalFile.is_open() || !tempInputFile.is_open()) {
                cerr << "Error: Could not finalize account deletion." << endl;
                promptContinue(); // Added promptContinue() here
                return;
            }

            while (getline(tempInputFile, line)) {
                originalFile << line << '\n';
            }

            originalFile.close();
            tempInputFile.close();

            // Clear the temporary file
            ofstream clearTempFile("temp.csv", ios::trunc);
            clearTempFile.close();

            cout << "Account deleted successfully." << endl;
        }

    }
    else
    {
        cout << "Account deletion cancelled.\n";
        userLoggedMenu(bankName, email);
        return;
    }
}

void handleWithDraw(const string& bankName, const string& email)
{
    string password;
    int accountID;
    double amount;
    cout << "Enter account password: ";
    cin >> password;
    validateCin();
    while (cin.fail()) {
        cout << "Enter account password: ";
        cin >> password;
        validateCin();
    }

    if (!checkAccountPassword(bankName, email, password))
    {
        cout << "Incorrect password. Please try again.\n";
        promptContinue(); // Added promptContinue() here
        return;
    }

    cout << "Enter amount to withdraw: ";
    cin >> amount;
    validateCin();
    while (cin.fail()) {
        cout << "Enter amount to withdraw: ";
        cin >> amount;
        validateCin();
    }

    accountID = searchCurrentEmailID(bankName, email);
    double currentBalance = stod(getAccountBalance(bankName, email));
    if (amount > currentBalance)
    {
        cout << "Error: Insufficient balance.\n";
        promptContinue(); // Added promptContinue() here
        return;
    }

    updateAccountBalance(bankName, accountID, currentBalance - amount);
    cout << "Withdrawal successful. New balance: " << currentBalance - amount << endl;
    promptContinue(); // Added promptContinue() here
}

void handleDeposit(const string& bankName, const string& email)
{
    string password;
    int accountID;
    double amount;
    cout << "Enter account password: ";
    cin >> password;
    validateCin();
    while (cin.fail()) {
        cout << "Enter account password: ";
        cin >> password;
        validateCin();
    }

    if (!checkAccountPassword(bankName, email, password))
    {
        cout << "Incorrect password. Please try again.\n";
        promptContinue(); // Added promptContinue() here
        return;
    }

    cout << "Enter amount to deposit: ";
    cin >> amount;
    validateCin();
    while (cin.fail()) {
        cout << "Enter amount to deposit: ";
        cin >> amount;
        validateCin();
    }

    accountID = searchCurrentEmailID(bankName, email);
    double currentBalance = stod(getAccountBalance(bankName, email));
    updateAccountBalance(bankName, accountID, currentBalance + amount);
    cout << "Deposit successful. New balance: " << currentBalance + amount << endl;
    promptContinue(); // Added promptContinue() here
}

int searchCurrentEmailID(const string& bankName, const string& email)
{
    ifstream file(bankName + ".csv");

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << bankName + ".csv" << endl;
        promptContinue(); // Added promptContinue() here
        return 0;
    }

    string line, tempEmail;

    // Loop through each line in the file
    while (getline(file, line))
    {
        stringstream ss(line);

        // Assuming the CSV structure is: BankName, Password, AccountID, AccountName, Balance, Email
        string bankNameColumn, password, tempID, accountName, balance;

        // Extract all the columns
        getline(ss, bankNameColumn, ',');  // Skip bank name
        getline(ss, password, ',');        // Skip password
        getline(ss, tempID, ',');          // Account ID
        getline(ss, accountName, ',');     // Skip account name
        getline(ss, balance, ',');         // Skip balance
        getline(ss, tempEmail, ',');       // Email column

        // Compare the extracted email with the target email
        if (tempEmail == email) {
            try {
                // Convert the account ID to an int
                int accountID = stoi(tempID); // Use stoi for int conversion
                file.close();
                return accountID;
            }
            catch (invalid_argument& e) {
                cerr << "Error: Invalid account ID in the CSV file: " << e.what() << endl;
                file.close();
                promptContinue(); // Added promptContinue() here
                return 0;
            }
        }
    }

    // Close the file after reading
    file.close();

    // If no account is found, return 0
    cerr << "Error: No account found for the email: " << email << endl;
    promptContinue(); // Added promptContinue() here
    return 0;
}


void checkLoggedAccount(const string& bankName, const string& email)
{
    ifstream file(bankName + ".csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << bankName + ".csv" << endl;
        promptContinue(); // Added promptContinue() here
        return;
    }

    string line, tempEmail;
    while (getline(file, line))
    {
        stringstream ss(line);

        // Assuming CSV columns are: BankName, Password, AccountID, AccountName, Balance, Email
        string bankNameColumn, password, accountID, accountName, balance;

        // Extract columns in order
        getline(ss, bankNameColumn, ',');  // Skip bank name
        getline(ss, password, ',');        // Skip password
        getline(ss, accountID, ',');       // Get account ID
        getline(ss, accountName, ',');     // Skip account name
        getline(ss, balance, ',');         // Get balance
        getline(ss, tempEmail, ',');       // Get email

        // Check if this is the right account based on the email
        if (tempEmail == email) {
            // Display account details

            cout << "\nCurrent Account Details:" << endl;
            cout << "Account ID: " << accountID << endl;
            cout << "Balance: $" << fixed << setprecision(2) << stod(balance) << endl;

            file.close();
            promptContinue();
            return;
        }
    }

    // If no account was found with the email
    cout << "No account found for the email: " << email << endl;
    file.close();
    promptContinue(); // Added promptContinue() here
}


void updateAccountBalance(const string& bankName, int accountID, double newBalance)
{
    ifstream file(bankName + ".csv");
    ofstream tempFile("temp.csv");

    if (!file.is_open() || !tempFile.is_open())
    {
        cerr << "Error: Could not open file for updating balance." << endl;
        return;
    }

    string line;
    bool accountFound = false;

    while (getline(file, line))
    {
        stringstream ss(line);

        // Assuming the CSV columns are: BankName, Password, AccountID, AccountName, Balance, Email
        string bankNameColumn, password, tempAccountID, accountName, balance, email;

        // Extract columns in order
        getline(ss, bankNameColumn, ',');  // Bank name
        getline(ss, password, ',');        // Password
        getline(ss, tempAccountID, ',');   // Account ID
        getline(ss, accountName, ',');     // Account name
        getline(ss, balance, ',');         // Balance
        getline(ss, email, ',');           // Email

        try {
            int existingID = stoi(tempAccountID);  // Convert string to int for comparison

            if (existingID == accountID) {
                // Update balance
                balance = to_string(newBalance);
                accountFound = true;
            }

            // Write updated (or unchanged) line to the temp file
            tempFile << bankNameColumn << ","
                << password << ","
                << tempAccountID << ","
                << accountName << ","
                << balance << ","
                << email << '\n';
        }
        catch (invalid_argument& e) {
            cerr << "Error: Invalid account ID in the CSV file: " << e.what() << endl;
        }
    }

    file.close();
    tempFile.close();

    // If account wasn't found, inform the user
    if (!accountFound) {
        cerr << "Error: Account ID " << accountID << " not found." << endl;
        return;
    }

    // Replace the original file with the updated temp file
    ofstream originalFile(bankName + ".csv", ios::trunc);
    ifstream tempInputFile("temp.csv");

    if (!originalFile.is_open() || !tempInputFile.is_open()) {
        cerr << "Error: Could not open file for final copy." << endl;
        return;
    }

    while (getline(tempInputFile, line)) {
        originalFile << line << '\n';
    }

    originalFile.close();
    tempInputFile.close();

    // Clear the contents of the temporary file
    ofstream clearTempFile("temp.csv", ios::trunc);
    clearTempFile.close();
}


string getAccountBalance(const string& bankName, const string& email)
{
    ifstream file(bankName + ".csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << bankName + ".csv" << endl;
        promptContinue(); // Added promptContinue() here
        return "";
    }

    string line, tempEmail;

    // Loop through each line in the file
    while (getline(file, line))
    {
        stringstream ss(line);

        // Assuming the CSV structure is: BankName, Password, AccountID, AccountName, Balance, Email
        string bankNameColumn, password, tempID, accountName, balance;

        // Extract all the columns
        getline(ss, bankNameColumn, ',');  // Skip bank name
        getline(ss, password, ',');        // Skip password
        getline(ss, tempID, ',');          // Account ID
        getline(ss, accountName, ',');     // Skip account name
        getline(ss, balance, ',');         // Balance column
        getline(ss, tempEmail, ',');       // Email column

        // Compare the extracted email with the target email
        if (tempEmail == email) {
            // Found the account, return the balance
            file.close();
            return balance;
        }
    }

    // Close the file after reading
    file.close();

    // If no account is found, print an error and return an empty string
    cerr << "No account found for the email: " << email << endl;
    promptContinue(); // Added promptContinue() here
    return "";
}


void printAvailableBanks(vector<string>& bankDatabases)
{
    system("cls"); // Clear screen before displaying available banks
    cout << "Available banks: \n\n";
    for (const string& bank : bankDatabases)
    {
        cout << bank << endl;
    }
    cout << endl;
}

void AdminCommandsMenu()
{
    system("cls"); // Clears the screen for a cleaner menu display
    cout << "========================================" << endl;
    cout << "|          ADM PROMPTS TERMINAL        |" << endl;
    cout << "========================================" << endl;
    cout << "|  1. Show all bank databases          |" << endl;
    cout << "|  2. Create a new bank database       |" << endl;
    cout << "|  3. Delete a bank database           |" << endl;
    cout << "|  4. Show all accounts in a bank      |" << endl;
    cout << "|  5. Delete all accounts in a bank    |" << endl;
    cout << "|  6. Change account balance           |" << endl;
    cout << "|  7. Change account password          |" << endl;
    cout << "|  8. Exit                             |" << endl;
    cout << "========================================" << endl;
    cout << "Select an option: ";
}

void handleADM_Inputs(vector<string>& bankDatabases)
{
    int option;
    do
    {
        AdminCommandsMenu();
        cin >> option;
        validateCin();
        while (cin.fail()) {
            AdminCommandsMenu();
            cin >> option;
            validateCin();
        }

        switch (option)
        {
        case 1:
            printAvailableBanks(bankDatabases);
            promptContinue();
            break;
        case 2:
            createNewBankDatabase(bankDatabases);
            promptContinue();
            break;
        case 3:
            // Delete a bank database
            deleteBankDatabase(bankDatabases);
            promptContinue();
            break;
        case 4:
            // Show all accounts in a bank
            showAllAccountsInBank();
            promptContinue();
            break;
        case 5:
            // Delete all accounts in a bank
            deleteAllAccountsInBank();
            promptContinue();
            break;
        case 6:
            // Change account balance
            changeAccountBalance();
            promptContinue();
            break;
        case 7:
            // Change account password
            changeAccountPassword();
            promptContinue();
            break;
        case 8:
            cout << "Exiting the admin terminal.\n";
            exit(0);
            break;
        default:
            cout << "Invalid option. Please choose again.\n";
            promptContinue();
        }
    } while (option != 8);
}

void createNewBankDatabase(vector<string>& bankDatabases)
{
    string bankName;
    cout << "Enter the bank name to create: ";
    cin >> bankName;
    validateCin();

    auto it = find(bankDatabases.begin(), bankDatabases.end(), bankName);
    if (it == bankDatabases.end())
    {
        bankDatabases.push_back(bankName);
        createCSVFileIfNotExists(bankName);
        cout << "Bank database '" << bankName << "' created successfully.\n";
    }
    else
    {
        cout << "Error: Bank name already exists.\n";
    }
}

void deleteBankDatabase(vector<string>& bankDatabases)
{
    string bankName;
    cout << "Enter the bank name to delete: ";
    cin >> bankName;
    validateCin();

    auto it = find(bankDatabases.begin(), bankDatabases.end(), bankName);
    if (it != bankDatabases.end())
    {
        bankName += ".csv";
        if (remove(bankName.c_str()) == 0)
        {
            cout << "Bank database '" << bankName << "' deleted successfully.\n";
        }
        else
        {
            cerr << "Error: Unable to delete the bank database.\n";
        }
    }
    else
    {
        cout << "Error: Bank name does not exist.\n";
    }
}

void showAllAccountsInBank()
{
    string bankName;
    cout << "Enter the bank name to view all accounts: ";
    cin >> bankName;
    validateCin();

    ifstream file(bankName + ".csv");
    if (!file.is_open())
    {
        cerr << "Error: Could not open the bank database.\n";
        return;
    }

    string line;
    bool firstLine = true;
    cout << "Accounts in " << bankName << ":\n";
    while (getline(file, line))
    {
        if (firstLine) {
            firstLine = false; // Skip the header row
            continue;
        }
        cout << line << endl; // Print account details
    }

    file.close();
}

void deleteAllAccountsInBank()
{
    string bankName;
    cout << "Enter the bank name to delete all accounts: ";
    cin >> bankName;
    validateCin();

    ofstream file(bankName + ".csv", ios::trunc);
    if (file.is_open())
    {
        file << "Bank Name,Password,Account ID,Account Name,Balance,Email\n"; // Recreate header
        file.close();
        cout << "All accounts in '" << bankName << "' have been deleted.\n";
    }
    else
    {
        cerr << "Error: Could not clear the accounts in the bank.\n";
    }
}

void changeAccountBalance()
{
    string bankName;
    int accountID;
    double newBalance;

    cout << "Enter the bank name: ";
    cin >> bankName;
    validateCin();

    cout << "Enter the account ID: ";
    cin >> accountID;
    validateCin();

    cout << "Enter the new balance: ";
    cin >> newBalance;
    validateCin();

    if (accountID != 0)
    {
        updateAccountBalance(bankName, accountID, newBalance);
    }
    else
    {
        cout << "Error: Account not found.\n";
    }
}


void changeAccountPassword()
{
    string bankName, newPassword;
    int accountID;

    cout << "Enter the bank name: ";
    cin >> bankName;
    validateCin();

    cout << "Enter the account ID: ";
    cin >> accountID;
    validateCin();

    cout << "Enter the new password: ";
    cin >> newPassword;
    validateCin();

    ifstream file(bankName + ".csv");
    ofstream tempFile("temp.csv");

    if (!file.is_open() || !tempFile.is_open())
    {
        cerr << "Error: Could not open file for password update.\n";
        return;
    }

    string line;
    bool accountFound = false;

    while (getline(file, line))
    {
        stringstream ss(line);

        string bankNameColumn, password, tempAccountID, accountName, balance, tempEmail;
        getline(ss, bankNameColumn, ',');
        getline(ss, password, ',');
        getline(ss, tempAccountID, ',');
        getline(ss, accountName, ',');
        getline(ss, balance, ',');
        getline(ss, tempEmail, ',');

        // Convert string tempAccountID to an integer for comparison
        int existingAccountID = stoi(tempAccountID);

        if (existingAccountID == accountID)
        {
            password = newPassword;
            accountFound = true;
        }

        tempFile << bankNameColumn << "," << password << "," << tempAccountID << ","
            << accountName << "," << balance << "," << tempEmail << '\n';
    }

    file.close();
    tempFile.close();

    if (accountFound)
    {
        ofstream originalFile(bankName + ".csv", ios::trunc);
        ifstream tempInputFile("temp.csv");

        if (!originalFile.is_open() || !tempInputFile.is_open())
        {
            cerr << "Error: Could not finalize the password update.\n";
            return;
        }

        while (getline(tempInputFile, line))
        {
            originalFile << line << '\n';
        }

        originalFile.close();
        tempInputFile.close();

        ofstream clearTempFile("temp.csv", ios::trunc);
        clearTempFile.close();

        cout << "Password updated successfully.\n";
    }
    else
    {
        cout << "Error: Account not found.\n";
        return;
    }
}