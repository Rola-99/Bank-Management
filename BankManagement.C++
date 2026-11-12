#pragma warning(disable : 4996)
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cctype>
using namespace std;

struct sClient {
    string AccountNum;
    string PinCode;
    string Name;
    string Phone;
    double AccBalance;
    bool MarkForDelete = false;
};

struct sUser {
    string Name;
    string Pssword;
    int Permission;
    bool MarkForUserDelete = false;
};
sUser CurrentUser;

enum enPermission {
    eAll = -1, pShowClientList = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdatClient = 8,
    pFindClient = 16, pTransactionClient = 32, pManageUsers = 64
};

enum enMyProgram {
    ShowAllScreen = 1, AddClientsToProg = 2, DeleteClientFromPro = 3, UpdateClientFromPro = 4,
    FindClientInPro = 5, enTransactions = 6, ManageUsers = 7, Logout = 8
};

enum enTransactionMenue { enDposit = 1, enWithdraw = 2, enTotalBalance = 3, enMainMenue = 4 };

enum enManageMenue { enListUsers = 1, AddNewUser = 2, DeleteUser = 3, UpdateUser = 4, FindUser = 5, MainMenue = 6 };

const string sClientsFileName = "Clients.txt";
const string sUsersFileName = "Users.txt";

void GoBackToMainMenue();
void ShowMainMenu();
void ShowManageUsersMenueScreen();
void LogIn();
void ShowTransactionMenueScreen();

void AccessDenied();
bool CheckAccessPermission(enPermission Permission) {

    if (CurrentUser.Permission == enPermission::eAll)
        return true;
   
    if ((Permission & CurrentUser.Permission) == Permission)
        return true;
    else
        return false;
    
}

void Finish() {
    cout << "------------------------------------------\n";
    cout << "\t\tFinish our program\n";
    cout << "------------------------------------------\n";
    LogIn();
}
///////////////////////////////////////////////////////////////////////////////////////////
//Show All Client
vector <string> SpiltString(string Text, string Delim) {
    vector <string> Tokens;

    short pos = 0;
    string sWord;

    while ((pos = Text.find(Delim)) != std::string::npos) {
        sWord = Text.substr(0, pos);
        if (sWord != "") {
            Tokens.push_back(sWord);
        }
        Text.erase(0, pos + Delim.length());
    }
    if (Text != "") {
        Tokens.push_back(Text);
    }
    return Tokens;
}
sClient ConvertClientLineToRecord(string LineRecord, string seperatr = "#//#") {

    sClient Data;
    vector <string> vClientsData;

    vClientsData = SpiltString(LineRecord, seperatr);

    Data.AccountNum = vClientsData[0];
    Data.PinCode = vClientsData[1];
    Data.Name = vClientsData[2];
    Data.Phone = vClientsData[3];
    Data.AccBalance = stod(vClientsData[4]);

    return Data;

}



vector <sClient> LoadClientDataFromFile(string FileName) {

    vector < sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line)) {

            Client = ConvertClientLineToRecord(Line);
            vClients.push_back(Client);

        }
        MyFile.close();
    }
    return vClients;
}
void PrintClientRecord(sClient Client) {
    cout << "| " << setw(20) << left << Client.AccountNum;
    cout << "| " << setw(25) << left << Client.PinCode;
    cout << "| " << setw(20) << left << Client.Name;
    cout << "| " << setw(25) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccBalance;
}

void ShowAllClientsScreen() {

    if (!CheckAccessPermission(enPermission::pShowClientList))
    {
        AccessDenied();
        return;
    }
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);

    cout << "\n\t\t\t\t\tBooking List (" << vClients.size() << ") Book(s).";

    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

    cout << left << setw(22) << "Account Number";
    cout << left << setw(26) << "Pin Code";
    cout << left << setw(22) << "Name";
    cout << left << setw(26) << "Phone";
    cout << left << setw(14) << "Account Balance";
    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

    if (vClients.size() == 0) {
        cout << "\t\t\tNo Clients Available in the program.";
    }
    else
    {
        for (sClient Client : vClients)
        {
            PrintClientRecord(Client);
            cout << endl;
        }
    }
    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

}


/// ///////////////////////////////////////////////////////////////////////////////////////
//Add Client
string ConvertRecordToLine(sClient Person1, string Seperator = "#//#") {

    string stNewLineAllData = "";

    stNewLineAllData += Person1.AccountNum + Seperator;
    stNewLineAllData += Person1.PinCode + Seperator;
    stNewLineAllData += Person1.Name + Seperator;
    stNewLineAllData += Person1.Phone + Seperator;
    stNewLineAllData += to_string(Person1.AccBalance);
    return stNewLineAllData;
}
bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertClientLineToRecord(Line);
            if (Client.AccountNum == AccountNumber) {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}
sClient ReadClientData() {
    sClient Person1;

    cout << "Enter Account Number: ";
    getline(cin >> ws, Person1.AccountNum);

    while (ClientExistsByAccountNumber(Person1.AccountNum, sClientsFileName)) {
        cout << "\nClient with [" << Person1.AccountNum << "] already exists, Enter another Account number:";
        getline(cin >> ws, Person1.AccountNum);
    }

    cout << "Enter Pin Code: ";
    getline(cin, Person1.PinCode);

    cout << "Enter Name? ";
    getline(cin, Person1.Name);

    cout << "Enter Phone? ";
    getline(cin, Person1.Phone);

    cout << "Enter Account Balance? ";
    cin >> Person1.AccBalance;

    return Person1;
}

void AddDataLineToFile(string FileName, string DataLine) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open()) {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}
void AddNewClient() {
    sClient OneClient;
    OneClient = ReadClientData();
    AddDataLineToFile(sClientsFileName, ConvertRecordToLine(OneClient));
}
void AddClients() {

    if (!CheckAccessPermission(enPermission::pAddNewClient))
    {
        AccessDenied();
        return;
    }
    cout << "------------------------------------------------\n";
    cout << "\t\tScreen Add New Client\n";
    cout << "------------------------------------------------\n";
    char AddMore = 'Y';
    do {
        system("cls");
        cout << "Adding New Clients:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more client?";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

/////////////////////////////////////////////////////////////////////////////////////////////

//Find Client
string ReadAccountNum() {
    string Acc = "";
    cout << "Please enter account Number ? ";
    cin >> Acc;
    return Acc;
}
bool FindClientByAccountNum(string AccountNumber, vector <sClient> vClients, sClient& Client1) {
    vClients = LoadClientDataFromFile(sClientsFileName);

    for (sClient c : vClients) {
        if (c.AccountNum == AccountNumber) {
            Client1 = c;
            return true;
        }
    }
    return false;
}
void PrintClientCard(sClient Client1) {

    cout << "Account Number: " << Client1.AccountNum << endl;
    cout << "Pin Code: " << Client1.PinCode << endl;
    cout << "Name: " << Client1.Name << endl;
    cout << "Phone: " << Client1.Phone << endl;
    cout << "Account Balance: " << Client1.AccBalance << endl;
}
void FindAccountNum() {
    string AccountNumber = ReadAccountNum();
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
    sClient Client1;
    if (FindClientByAccountNum(AccountNumber, vClients, Client1)) {
        PrintClientCard(Client1);
    }
    else
    {
        cout << "\nClient with this Account number[" << AccountNumber << "] is not found!";
    }

}
void enFindAccountNum() {
    if (!CheckAccessPermission(enPermission::pFindClient))
    {
        AccessDenied();
        return;
    }
    cout << "---------------------------------------------\n";
    cout << "\t\tFind Account Number\n";
    cout << "---------------------------------------------\n";
    FindAccountNum();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//Delete Account
bool MarkClientForDeleteByAccountNumber(string AccountNum, vector <sClient>& vClients) {
    for (sClient& C : vClients) {
        if (C.AccountNum == AccountNum) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient> vClients) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open()) {
        for (sClient C : vClients) {
            if (C.MarkForDelete == false) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}
bool DeleteClientByAccountNumber(string AccountNum, vector <sClient>& vClients) {

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNum(AccountNum, vClients, Client)) {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to delete this client? Y/N ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            MarkClientForDeleteByAccountNumber(AccountNum, vClients);
            SaveClientsDataToFile(sClientsFileName, vClients);

            //Refresh Clients
            vClients = LoadClientDataFromFile(sClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }

    else
    {
        cout << "\nThis Account " << AccountNum << " isn't found";
        return false;
    }
}

void ScreenDelete() {
    if (!CheckAccessPermission(enPermission::pDeleteClient))
    {
        AccessDenied();
        return;
    }
    cout << "------------------------------------------------\n";
    cout << "\t\tScreen Delete Client\n";
    cout << "------------------------------------------------\n";
    string AccountNum = ReadAccountNum();
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
    DeleteClientByAccountNumber(AccountNum, vClients);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//Update Client
sClient ChangeClientRecord(string AccountNumber) {
    sClient Client;

    Client.AccountNum = AccountNumber;

    cout << "\n\nEnter Pin Code? ";
    getline(cin >> ws, Client.PinCode);

    cout << "\nEnter Name? ";
    getline(cin, Client.Name);

    cout << "\nEnter Phone? ";
    getline(cin, Client.Phone);

    cout << "\nEnter Account balance? ";
    cin >> Client.AccBalance;

    return Client;

}
bool UpdateClientByAccountNumber(string AccountNum, vector <sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNum(AccountNum, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure want to edit this account? Y/N?";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y')
        {
            for (sClient& C : vClients)
            {
                C = ChangeClientRecord(AccountNum);
                break;
            }
        }
        SaveClientsDataToFile(sClientsFileName, vClients);

        cout << "\n\nClient appdated successfully.\n";
        return true;
    }
    else
    {
        cout << "This account " << AccountNum << " isn't found.\n";
        return false;
    }

}

void UpAccount() {
    if (!CheckAccessPermission(enPermission::pUpdatClient))
    {
        AccessDenied();
        return;
    }
    cout << "------------------------------------------------\n";
    cout << "\t\tScreen Update Client\n";
    cout << "------------------------------------------------\n";

    string AccountNum = ReadAccountNum();
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
    UpdateClientByAccountNumber(AccountNum, vClients);
}


////////////////////////////////////////////////////////////////////////////////////////////////
//Account Deposit 
bool DepositBalanceToClientByAccountNum(string AccountNumber, double NewBalance, vector <sClient> vClients) {
    char Sure = 'y';
    cout << "Are you sure you want perform this tranaction?";
    cin >> Sure;

    if (Sure == 'y' || Sure == 'Y') {

        for (sClient& C : vClients) {
            if (C.AccountNum == AccountNumber) {
                C.AccBalance += NewBalance;
                SaveClientsDataToFile(sClientsFileName, vClients);
                cout << "Done Successfully Balance Updated. Your Accout: " << C.AccBalance << endl;

                return true;
            }
        }
        return false;
    }
}

void DepositScreen() {
    cout << "---------------------------------------------\n";
    cout << "\t\tDeposit Screen\n";
    cout << "---------------------------------------------\n";

    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
    sClient Client1;
    string AccountNumber = ReadAccountNum();



    while (!FindClientByAccountNum(AccountNumber, vClients, Client1)) {
        cout << "\nClients with[ " << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadAccountNum();
    }

    PrintClientCard(Client1);

    double NewBalance = 0;
    cout << "\nPlease enter deposit amount?";
    cin >> NewBalance;
    DepositBalanceToClientByAccountNum(AccountNumber, NewBalance, vClients);

}

//////////////////////////////////////////////////////////////////////////////////////////
// Witdraw Screen
bool WithdrawBalanceToClientByAccountNum(string AccountNumber, double NewBalance, vector <sClient> vClients) {
    char Sure = 'y';
    cout << "Are you sure you want perform this tranaction?";
    cin >> Sure;

    if (Sure == 'y' || Sure == 'Y') {

        for (sClient& C : vClients) {
            if (C.AccountNum == AccountNumber) {
                C.AccBalance -= NewBalance;
                SaveClientsDataToFile(sClientsFileName, vClients);
                cout << "Done Successfully Balance Updated. Your Accout: " << C.AccBalance << endl;

                return true;
            }
        }
        return false;
    }
}
void WithdrawScreen() {
    cout << "---------------------------------------------\n";
    cout << "\t\tWithDraw Screen\n";
    cout << "---------------------------------------------\n";
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
    sClient Client1;
    string AccountNumber = ReadAccountNum();

    while (!FindClientByAccountNum(AccountNumber, vClients, Client1)) {
        cout << "\nClients with[ " << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadAccountNum();
    }
    PrintClientCard(Client1);

    double NewBalance = 0;
    cout << "\nPlease enter withdraw amount?";
    cin >> NewBalance;
    WithdrawBalanceToClientByAccountNum(AccountNumber, NewBalance, vClients);

}

/////////////////////////////////////////////////////////////////////////////////////////
// Total Balances

void PrintTransactionRecord(sClient Client) {
    cout << "| " << setw(20) << left << Client.AccountNum;

    cout << "| " << setw(20) << left << Client.Name;

    cout << "| " << setw(12) << left << Client.AccBalance;
}

void ShowAllTeransactionScreen() {
    double TotalBalance = 0;
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);

    cout << "\n\t\t\t\t\tBooking List (" << vClients.size() << ") Book(s).";

    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

    cout << left << setw(22) << "Account Number";

    cout << left << setw(22) << "Name";

    cout << left << setw(14) << "Account Balance";
    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

    if (vClients.size() == 0) {
        cout << "\t\t\tNo Clients Available in the program.";
    }
    else
    {
        for (sClient Client : vClients)
        {
            PrintTransactionRecord(Client);
            cout << endl;
        }
    }
    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;
    for (sClient c : vClients) {
        TotalBalance += c.AccBalance;
    }
    cout << "\t\t\t\t\t\t\tTotal Balance = " << TotalBalance;

}

///////////////////////////////////////////////////////////////////////////////////////
//Transaction Menue
short ReadTransactionMenueOption() {
    short Choose = 0;
    cout << "\t\t\t\tChoose what do you want to do [1 to 4] ?";
    cin >> Choose;
    return Choose;
}



void GoTransactionScreen() {

    cout << "\nPress any key to back Main Transactions Screen...";
    system("pause>0");
    ShowTransactionMenueScreen();
}

void PerformTranactionsMenueOption(enTransactionMenue TransactionMenue) {
    switch (TransactionMenue) {
    case enTransactionMenue::enDposit: {
        system("cls");
        DepositScreen();
        GoTransactionScreen();
        break;
    }
    case enTransactionMenue::enWithdraw: {
        system("cls");
        WithdrawScreen();
        GoTransactionScreen();
        break;
    }
    case enTransactionMenue::enTotalBalance: {
        system("cls");
        ShowAllTeransactionScreen();
        GoTransactionScreen();
        break;
    }
    case enTransactionMenue::enMainMenue: {
        system("cls");
        ShowMainMenu();
        break;
    }
    }
}

void ShowTransactionMenueScreen() {
    if (!CheckAccessPermission(enPermission::pTransactionClient))
    {
        AccessDenied();
        return;
    }
    system("cls");
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\tTransactions Menue Screen\n";
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\t[1] Deposit.\n";
    cout << "\t\t\t\t\t\t[2] Withdraw.\n";
    cout << "\t\t\t\t\t\t[3] Total Balance.\n";
    cout << "\t\t\t\t\t\t[4] Main Menue.\n";

    cout << "\t\t\t\t--------------------------------------------------\n";
    PerformTranactionsMenueOption(enTransactionMenue(ReadTransactionMenueOption()));
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Go Back to Manage Menue
void GoBackToManageMenue() {
    cout << "Press any Key to back The Main Menue...";
    system("pause>0");
    ShowManageUsersMenueScreen();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Show Users List
sUser ConvertUserLineToRecord(string LineRecord, string seperatr = "#//#") {

    sUser User;
    vector <string> vUserData;

    vUserData = SpiltString(LineRecord, seperatr);

    User.Name = vUserData[0];
    User.Pssword = vUserData[1];
    User.Permission = stoi(vUserData[2]);

    return User;

}

vector <sUser> LoadUserDataFromFile(string FileName) {

    vector <sUser> vUser;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line)) {

            User = ConvertUserLineToRecord(Line);
            vUser.push_back(User);

        }
        MyFile.close();
    }
    return vUser;
}

void PrintUserRecord(sUser User) {
    cout << "| " << setw(20) << left << User.Name;
    cout << "| " << setw(25) << left << User.Pssword;
    cout << "| " << setw(20) << left << User.Permission;

}

void ShowAllUsersScreen() {
    vector <sUser> vUser = LoadUserDataFromFile(sUsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUser.size() << ") User(s).";

    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

    cout << left << setw(22) << "Name";
    cout << left << setw(26) << "Password";
    cout << left << setw(22) << "Permissions";

    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

    if (vUser.size() == 0) {
        cout << "\t\t\tNo Clients Available in the program.";
    }
    else
    {
        for (sUser User : vUser)
        {
            PrintUserRecord(User);
            cout << endl;
        }
    }
    cout << "\n-----------------------------------------------------------";
    cout << "-------------------------------------------------------------\n" << endl;

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Add New User
string ConvertUserRecordToLine(sUser User1, string Seperator = "#//#") {

    string stNewLineAllData = "";

    stNewLineAllData += User1.Name + Seperator;
    stNewLineAllData += User1.Pssword + Seperator;
    stNewLineAllData += to_string(User1.Permission);

    return stNewLineAllData;
}

bool UserExistsByUserName(string UserName, string FileName) {
    vector <sUser> vUser;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            if (User.Name == UserName) {
                MyFile.close();
                return true;
            }
            vUser.push_back(User);
        }
        MyFile.close();
    }
    return false;
}

int ReadPermissionsToSet() {
    int Permissions = 0;
    char answer = 'n';

    cout << "Do you want give full access? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        return -1;
    }
    cout << "\nDo you want give access to: ";

    cout << "\nShow clients list? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permissions += enPermission::pShowClientList;
    }


    cout << "Do you want give add New clients? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permissions += enPermission::pAddNewClient;
    }

    cout << "Do you want give Delete client? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permissions += enPermission::pDeleteClient;
    }

    cout << "Do you want give Update client? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permissions += enPermission::pUpdatClient;
    }

    cout << "Do you want give Find client? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permissions += enPermission::pFindClient;
    }

    cout << "Do you want give Transaction client? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permissions += enPermission::pTransactionClient;
    }

    cout << "Do you want give Manage client? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permissions += enPermission::pManageUsers;
    }

    return Permissions;
}

sUser ReadUserData() {
    sUser User1;

    cout << "Enter User Name: ";
    getline(cin >> ws, User1.Name);

    while (UserExistsByUserName(User1.Name, sUsersFileName)) {
        cout << "\nUser with [" << User1.Name << "] already exists, Enter another User Name:";
        getline(cin >> ws, User1.Name);
    }

    cout << "Enter Password: ";
    getline(cin, User1.Pssword);

    User1.Permission = ReadPermissionsToSet();


    return User1;
}

void AddingNewUser() {

    sUser OneUser;
    OneUser = ReadUserData();
    AddDataLineToFile(sUsersFileName, ConvertUserRecordToLine(OneUser));
}

void AddUsers() {
    cout << "------------------------------------------------\n";
    cout << "\t\tScreen Add New User\n";
    cout << "------------------------------------------------\n";
    char AddMore = 'Y';
    do {
        system("cls");
        cout << "Adding New Users:\n\n";

        AddingNewUser();

        cout << "\nUser Added Successfully, do you want to add more User?";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//Find User
string ReadUserName() {
    string UserNam = "";
    cout << "Please enter User Name? ";
    getline(cin >> ws, UserNam);
    return UserNam;
}

bool FindUserByUserName(string UserName, vector <sUser> vUser, sUser& User1) {
    vUser = LoadUserDataFromFile(sUsersFileName);

    for (sUser U : vUser) {
        if (U.Name == UserName) {
            User1 = U;
            return true;
        }
    }
    return false;
}

void PrintUserCard(sUser User1) {

    cout << "User Name: " << User1.Name<< endl;
    cout << "Pssword: " << User1.Pssword << endl;
    cout << "Permission: " << User1.Permission << endl;
   
}

void FindUserName() {
    string UserName = ReadUserName();
    vector <sUser> vUser = LoadUserDataFromFile(sUsersFileName);
    sUser User1;
    if (FindUserByUserName(UserName, vUser, User1)) {
        PrintUserCard(User1);
    }
    else
    {
        cout << "\nUser with this User Name[" << UserName << "] is not found!";
    }

}

void enFindUserName() {
    cout << "---------------------------------------------\n";
    cout << "\t\tFind User Nume\n";
    cout << "---------------------------------------------\n";
    FindUserName();
}

// /////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Delete User
bool MarkUserForDeleteByUserName(string UserName, vector <sUser>& vUser) {
    for (sUser& U : vUser) {
        if (U.Name == UserName) {
            U.MarkForUserDelete = true;
            return true;
        }
    }
    return false;
}

vector <sUser> SaveUsersDataToFile(string FileName, vector <sUser> vUser) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open()) {
        for (sUser U : vUser) {
            if (U.MarkForUserDelete == false) {
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUser;
}

bool DeleteUserByUserName(string UserName, vector <sUser>& vUser) {

    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUser, User)) {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want to delete this User? Y/N ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            MarkUserForDeleteByUserName(UserName, vUser);
            SaveUsersDataToFile(sUsersFileName, vUser);

            //Refresh Clients
            vUser = LoadUserDataFromFile(sUsersFileName);

            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }

    else
    {
        cout << "\nThis User " << UserName << " isn't found";
        return false;
    }
}

void ScreenUserDelete() {
    cout << "------------------------------------------------\n";
    cout << "\t\tScreen Delete User\n";
    cout << "------------------------------------------------\n";
    string UserName = ReadUserName();
    if (UserName == "Admin") {
        cout << "You can't delete this user.\n";
    }
    else
    {
        vector <sUser> vUser = LoadUserDataFromFile(sUsersFileName);
        DeleteUserByUserName(UserName, vUser);
    }
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// Update User
sUser ChangeUserRecord(string UserName) {
    sUser User;

    User.Name = UserName;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, User.Pssword);

    User.Permission = ReadPermissionsToSet();
    return User;
}

bool UpdateUserByUserName(string UserName, vector <sUser>& vUser) {
    sUser User1;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUser, User1))
    {
        PrintUserCard(User1);

        cout << "\n\nAre you sure want to edit this User? Y/N?";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y')
        {
            for (sUser& U : vUser)
            {
                if (U.Name == UserName) {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }
        }
        SaveUsersDataToFile(sUsersFileName, vUser);

        cout << "\n\nUser appdated successfully.\n";
        return true;
    }
    else
    {
        cout << "This account " << UserName << " isn't found.\n";
        return false;
    }

}

void UpdaUser() {
    cout << "------------------------------------------------\n";
    cout << "\t\tScreen Update User\n";
    cout << "------------------------------------------------\n";

    string UserName = ReadUserName();
    vector <sUser> vUser = LoadUserDataFromFile(sUsersFileName);
    UpdateUserByUserName(UserName, vUser);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// LogIn
bool FindUserByUserNameAndUserPassword(string UserName, string Password, sUser& User) {

    vector <sUser> vUser = LoadUserDataFromFile(sUsersFileName);

    for (sUser U : vUser) {
        if (U.Name == UserName && U.Pssword == Password) {
            User = U;
            return true;
        }
    }
    return false;
}
bool LoadUserInfo(string UserName, string Password) {

    if (FindUserByUserNameAndUserPassword(UserName, Password, CurrentUser)) {
        return true;
    }
    return false;
}
void LogIn() {

    bool LoginFaild = false;
    string UserName, Pssword;

    do {
        cout << "\n------------------------------------------------\n";
        cout << "\t\t Login Screen\n";
        cout << "------------------------------------------------\n";

        cout << "Enter User Name?";
        getline(cin >> ws, UserName);

     
        cout << "Enter User Password?";
        getline(cin >> ws, Pssword);


        if (!LoginFaild) {
            cout << "Invalaid Username/Password";
        }

        LoginFaild = !LoadUserInfo(UserName, Pssword);

        


    } while (LoginFaild);
    ShowMainMenu();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//Manage Meinue

void AccessDenied() {
    cout << "-----------------------------------------------\n";
    cout << "Access Denied," << endl;
    cout << "You don't have permission to do this," << endl;
    cout << "Plese contact with your admin." << endl;
    cout << "-----------------------------------------------\n";
    GoBackToMainMenue();
}

short ReadManageOption() {
    short Choose;
    cout << "\t\t\t\tChoose what do you want to do [1 to 6] ?";
    cin >> Choose;
    return Choose;
}

void PerformManageMenue(enManageMenue ManageMenue) {
    switch (ManageMenue) {
    case enManageMenue::enListUsers: {
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageMenue();
        break;
    }
    case enManageMenue::AddNewUser: {
        system("cls");
        AddUsers();
        GoBackToManageMenue();
        break;
    }
    case enManageMenue::DeleteUser: {
        system("cls");
        ScreenUserDelete();
        GoBackToManageMenue();
        break;
    }
    case enManageMenue::UpdateUser: {
        system("cls");
        UpdaUser();
        GoBackToManageMenue();
        break;
    }
    case enManageMenue::FindUser: {
        system("cls");
        enFindUserName();
        GoBackToManageMenue();
        break;
    }
    case enManageMenue::MainMenue: {
        system("cls");
        ShowMainMenu();
        break;
    }
    }
}

void ShowManageUsersMenueScreen() {

    if (!CheckAccessPermission(enPermission::pManageUsers))
    {
        AccessDenied();
        return;
    }
    system("cls");
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\tManage Users Menue Screen\n";
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\t[1] List Users.\n";
    cout << "\t\t\t\t\t\t[2] Add New User.\n";
    cout << "\t\t\t\t\t\t[3] Delete User.\n";
    cout << "\t\t\t\t\t\t[4] Update User.\n";
    cout << "\t\t\t\t\t\t[5] Find User.\n";
    cout << "\t\t\t\t\t\t[6] Main Menue.\n";

    cout << "\t\t\t\t--------------------------------------------------\n";
    PerformManageMenue(enManageMenue(ReadManageOption()));
}
///////////////////////////////////////////////////////////////////////////////////////////////////

short ReadMainMenueOption() {
    short Choose = 0;
    cout << "\t\t\t\tChoose what do you want to do [1 to 8] ?";
    cin >> Choose;
    return Choose;
}

void PerformMainMenueOption(enMyProgram MyProgramOption) {
    switch (MyProgramOption) {
    case enMyProgram::ShowAllScreen: {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMyProgram::AddClientsToProg: {
        system("cls");
        AddClients();
        GoBackToMainMenue();
        break;
    }
    case enMyProgram::DeleteClientFromPro: {
        system("cls");
        ScreenDelete();
        GoBackToMainMenue();
        break;
    }
    case enMyProgram::UpdateClientFromPro: {
        system("cls");
        UpAccount();
        GoBackToMainMenue();
        break;
    }
    case enMyProgram::FindClientInPro: {
        system("cls");
        enFindAccountNum();
        GoBackToMainMenue();
        break;
    }
    case enMyProgram::enTransactions: {
        system("cls");
        ShowTransactionMenueScreen();
        ReadTransactionMenueOption();
        break;
    }
    case enMyProgram::ManageUsers: {
        system("cls");
        ShowManageUsersMenueScreen();
        break;
    }
    case enMyProgram::Logout: {
        system("cls");
        Finish();
        break;
    }

    }
}

void GoBackToMainMenue() {
    cout << "Press any Key to back The Main Menue...";
    system("pause>0");
    ShowMainMenu();

}
void ShowMainMenu() {

    system("cls");
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\tMain Menue Screen\n";
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\t[1] Show Client List.\n";
    cout << "\t\t\t\t\t\t[2] Add New Client.\n";
    cout << "\t\t\t\t\t\t[3] Delete Client.\n";
    cout << "\t\t\t\t\t\t[4] Update Client Info.\n";
    cout << "\t\t\t\t\t\t[5] Find Client.\n";
    cout << "\t\t\t\t\t\t[6] Transactions.\n";
    cout << "\t\t\t\t\t\t[7] ManageUser.\n";
    cout << "\t\t\t\t\t\t[8] Logout.\n";
    cout << "\t\t\t\t--------------------------------------------------\n";

    PerformMainMenueOption((enMyProgram)ReadMainMenueOption());
}


int main() {

    srand((unsigned)time(NULL));

    LogIn();

    system("pause>0");
    return 0;
}
