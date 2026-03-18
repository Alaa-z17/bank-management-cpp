#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;

// =====================
// Enums
// =====================

enum enTransactionsMenuOptions
{
    eDeposit = 1,
    eWithdraw = 2,
    eShowTotalBalance = 3,
    eShowMainMenu = 4
};

enum enManageUsersMenuOptions
{
    eListUsers = 1,
    eAddNewUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eSortUsers = 6,
    eUsersMainMenu = 7
};

enum enMainMenuOptions
{
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eShowTransactions = 6,
    eManageUsers = 7,
    eSortClientsByName = 8,
    eSortClientsByAccNum = 9,
    eLogout = 10
};

enum enMainMenuPermissions
{
    eAll = -1,
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClients = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64
};

// =====================
// Structs
// =====================

struct stClient
{
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

struct stUser
{
    string UserName = "";
    string Password = "";
    int Permissions = 0;
    bool MarkForDelete = false;
};

// =====================
// Global Variables
// =====================

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
stUser CurrentUser;

// =====================
// Forward Declarations
// =====================

void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu();
bool CheckAccessPermission(enMainMenuPermissions Permission);
void Login();
void AddDataLineToFile(string FileName, string stDataLine);

// =====================
// String Utility Functions
// =====================

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    string::size_type pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
            vString.push_back(sWord);
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
        vString.push_back(S1);

    return vString;
}

// =====================
// Convert Functions
// =====================

stClient ConvertLineToClientRecord(string Line,
    string Separator = "#//#")
{
    stClient Client;
    vector<string> vData = SplitString(Line, Separator);

    Client.AccountNumber = vData[0];
    Client.PinCode = vData[1];
    Client.Name = vData[2];
    Client.Phone = vData[3];
    Client.AccountBalance = stod(vData[4]);

    return Client;
}

stUser ConvertLineToUserRecord(string Line,
    string Separator = "#//#")
{
    stUser User;
    vector<string> vData = SplitString(Line, Separator);

    User.UserName = vData[0];
    User.Password = vData[1];
    User.Permissions = stoi(vData[2]);

    return User;
}

string ConvertClientRecordToLine(stClient Client,
    string Separator = "#//#")
{
    string Line = "";
    Line += Client.AccountNumber + Separator;
    Line += Client.PinCode + Separator;
    Line += Client.Name + Separator;
    Line += Client.Phone + Separator;
    Line += to_string(Client.AccountBalance);
    return Line;
}

string ConvertUserRecordToLine(stUser User,
    string Separator = "#//#")
{
    string Line = "";
    Line += User.UserName + Separator;
    Line += User.Password + Separator;
    Line += to_string(User.Permissions);
    return Line;
}

// =====================
// File Functions
// =====================

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

vector<stClient> LoadClientsFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (Line != "")
                vClients.push_back(ConvertLineToClientRecord(Line));
        }
        MyFile.close();
    }
    return vClients;
}

vector<stUser> LoadUsersFromFile(string FileName)
{
    vector<stUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (Line != "")
                vUsers.push_back(ConvertLineToUserRecord(Line));
        }
        MyFile.close();
    }
    return vUsers;
}

void SaveClientsToFile(string FileName, vector<stClient>& vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClient& C : vClients)
        {
            if (!C.MarkForDelete)
                MyFile << ConvertClientRecordToLine(C) << endl;
        }
        MyFile.close();
    }
}

void SaveUsersToFile(string FileName, vector<stUser>& vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (stUser& U : vUsers)
        {
            if (!U.MarkForDelete)
                MyFile << ConvertUserRecordToLine(U) << endl;
        }
        MyFile.close();
    }
}

int main()
{
    return 0;
}