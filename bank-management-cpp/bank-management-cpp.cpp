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
// =====================
// Client Helper Functions
// =====================

bool ClientExistsByAccountNumber(string AccountNumber,
    string FileName)
{
    vector<stClient> vClients = LoadClientsFromFile(FileName);
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
            return true;
    }
    return false;
}

bool FindClientByAccountNumber(string AccountNumber,
    vector<stClient>& vClients, stClient& Client)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void PrintClientCard(stClient Client)
{
    cout << "\n-----------------------------------\n";
    cout << "Account Number : " << Client.AccountNumber << "\n";
    cout << "Pin Code       : " << Client.PinCode << "\n";
    cout << "Name           : " << Client.Name << "\n";
    cout << "Phone          : " << Client.Phone << "\n";
    cout << "Balance        : " << Client.AccountBalance << "\n";
    cout << "-----------------------------------\n";
}

void PrintClientRecordLine(stClient Client)
{
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(10) << Client.PinCode;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(12) << Client.Phone;
    cout << "| " << left << setw(12) << Client.AccountBalance;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nEnter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

stClient ReadNewClient()
{
    stClient Client;

    cout << "Enter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber,
        ClientsFileName))
    {
        cout << "\nAccount [" << Client.AccountNumber
            << "] already exists, Enter another: ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter Pin Code: ";
    getline(cin, Client.PinCode);

    cout << "Enter Name: ";
    getline(cin, Client.Name);

    cout << "Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "Enter Pin Code: ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name: ";
    getline(cin, Client.Name);

    cout << "Enter Phone: ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

bool MarkClientForDelete(string AccountNumber,
    vector<stClient>& vClients)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void AddNewClient()
{
    stClient Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName,ConvertClientRecordToLine(Client));
    cout << "\nClient Added Successfully!\n";
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "\nAdding New Client:\n";
        AddNewClient();
        cout << "\nAdd more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(string AccountNumber,
    vector<stClient>& vClients)
{
    stClient Client;
    char Answer = 'N';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\nAre you sure you want to delete? Y/N? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            MarkClientForDelete(AccountNumber, vClients);
            SaveClientsToFile(ClientsFileName, vClients);
            vClients = LoadClientsFromFile(ClientsFileName);
            cout << "\nClient Deleted Successfully!\n";
            return true;
        }
    }
    else
        cout << "\nClient [" << AccountNumber << "] Not Found!\n";

    return false;
}

bool UpdateClientByAccountNumber(string AccountNumber,
    vector<stClient>& vClients)
{
    stClient Client;
    char Answer = 'N';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\nAre you sure you want to update? Y/N? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsToFile(ClientsFileName, vClients);
            cout << "\nClient Updated Successfully!\n";
            return true;
        }
    }
    else
        cout << "\nClient [" << AccountNumber << "] Not Found!\n";

    return false;
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pListClients))
    {
        cout << "\nAccess Denied!\n";
        return;
    }

    vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);

    cout << "\n\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << string(95, '_') << "\n\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n" << string(95, '_') << "\n\n";

    if (vClients.empty())
        cout << "\t\t\tNo Clients Available!\n";
    else
        for (stClient& C : vClients)
        {
            PrintClientRecordLine(C);
            cout << "\n";
        }

    cout << "\n" << string(95, '_') << "\n";
}
// =====================
// User Helper Functions
// =====================

bool UserExistsByUsername(string Username, string FileName)
{
    vector<stUser> vUsers = LoadUsersFromFile(FileName);
    for (stUser& U : vUsers)
    {
        if (U.UserName == Username)
            return true;
    }
    return false;
}

bool FindUserByUsername(string Username,
    vector<stUser>& vUsers, stUser& User)
{
    for (stUser& U : vUsers)
    {
        if (U.UserName == Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool FindUserByUsernameAndPassword(string Username,
    string Password, stUser& User)
{
    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

    if (vUsers.empty())
    {
        stUser InitialUser;
        InitialUser.UserName = "Admin";
        InitialUser.Password = "1234";
        InitialUser.Permissions = -1;
        AddDataLineToFile(UsersFileName,
            ConvertUserRecordToLine(InitialUser));
        vUsers = LoadUsersFromFile(UsersFileName);
    }

    for (stUser& U : vUsers)
    {
        if (U.UserName == Username && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

void PrintUserCard(stUser User)
{
    cout << "\n-----------------------------------\n";
    cout << "Username    : " << User.UserName << "\n";
    cout << "Password    : " << User.Password << "\n";
    cout << "Permissions : " << User.Permissions << "\n";
    cout << "-----------------------------------\n";
}

void PrintUserRecordLine(stUser User)
{
    cout << "| " << left << setw(15) << User.UserName;
    cout << "| " << left << setw(10) << User.Password;
    cout << "| " << left << setw(40) << User.Permissions;
}

string ReadUsername()
{
    string Username = "";
    cout << "\nEnter Username: ";
    cin >> Username;
    return Username;
}

int ReadPermissionsToSet()
{
    int Permissions = 0;
    char Answer = 'N';

    cout << "\nDo you want to give full access? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        return -1;

    cout << "\nShow Client List? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pListClients;

    cout << "\nAdd New Client? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pAddNewClient;

    cout << "\nDelete Client? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pDeleteClient;

    cout << "\nUpdate Client? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pUpdateClients;

    cout << "\nFind Client? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pFindClient;

    cout << "\nTransactions? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pTransactions;

    cout << "\nManage Users? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pManageUsers;

    if (Permissions == 127)
        Permissions = -1;

    return Permissions;
}

stUser ReadNewUser()
{
    stUser User;

    cout << "Enter Username: ";
    getline(cin >> ws, User.UserName);

    while (UserExistsByUsername(User.UserName, UsersFileName))
    {
        cout << "\nUser [" << User.UserName
            << "] already exists, Enter another: ";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password: ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

stUser ChangeUserRecord(string Username)
{
    stUser User;
    User.UserName = Username;

    cout << "Enter Password: ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

bool MarkUserForDelete(string Username,
    vector<stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.UserName == Username)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void AddNewUser()
{
    stUser User = ReadNewUser();
    AddDataLineToFile(UsersFileName,
        ConvertUserRecordToLine(User));
    cout << "\nUser Added Successfully!\n";
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        cout << "\nAdding New User:\n";
        AddNewUser();
        cout << "\nAdd more users? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool DeleteUserByUsername(string Username,
    vector<stUser>& vUsers)
{
    if (Username == "Admin")
    {
        cout << "\nCannot delete Admin user!\n";
        return false;
    }

    stUser User;
    char Answer = 'N';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\nAre you sure you want to delete? Y/N? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            MarkUserForDelete(Username, vUsers);
            SaveUsersToFile(UsersFileName, vUsers);
            vUsers = LoadUsersFromFile(UsersFileName);
            cout << "\nUser Deleted Successfully!\n";
            return true;
        }
    }
    else
        cout << "\nUser [" << Username << "] Not Found!\n";

    return false;
}

bool UpdateUserByUsername(string Username,
    vector<stUser>& vUsers)
{
    stUser User;
    char Answer = 'N';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\nAre you sure you want to update? Y/N? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (stUser& U : vUsers)
            {
                if (U.UserName == Username)
                {
                    U = ChangeUserRecord(Username);
                    break;
                }
            }
            SaveUsersToFile(UsersFileName, vUsers);
            cout << "\nUser Updated Successfully!\n";
            return true;
        }
    }
    else
        cout << "\nUser [" << Username << "] Not Found!\n";

    return false;
}

void ShowAllUsersScreen()
{
    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

    cout << "\n\t\t\tUsers List (" << vUsers.size() << ") User(s).\n";
    cout << string(75, '_') << "\n\n";
    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n" << string(75, '_') << "\n\n";

    if (vUsers.empty())
        cout << "\t\t\tNo Users Available!\n";
    else
        for (stUser& U : vUsers)
        {
            PrintUserRecordLine(U);
            cout << "\n";
        }

    cout << "\n" << string(75, '_') << "\n";
}
int main()
{
    return 0;
}