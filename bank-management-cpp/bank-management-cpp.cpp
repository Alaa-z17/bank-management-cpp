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
    eChangeAdminCreds = 7,
    eUsersMainMenu = 8
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
// XOR Encryption
// =====================

const char ENCRYPTION_KEY = 'B'; // يمكن تغييره لأي حرف

/**
 * @brief Encrypts or Decrypts a string using XOR cipher.
 *
 * @param Text The string to encrypt or decrypt.
 * @return Encrypted or decrypted string.
 *
 * @details
 * XOR is self-inverse — applying it twice returns the original:
 * Encrypt(Encrypt("1234")) == "1234"
 * This means the same function handles both encrypt and decrypt.
 *
 * @example
 * XOREncryptDecrypt("1234") → encrypted string
 * XOREncryptDecrypt(encrypted) → "1234"
 */
string XOREncryptDecrypt(string Text)
{
    string Result = Text;
    for (int i = 0; i < Result.length(); i++)
    {
        Result[i] = Text[i] ^ ENCRYPTION_KEY;
    }
    return Result;
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
    User.Password = XOREncryptDecrypt(vData[1]);
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
    Line += XOREncryptDecrypt(User.Password) + Separator;
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
        AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(InitialUser));
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
    cout << "Password    : " << string(User.Password.length(), '*') << "\n";
    cout << "Permissions : " << User.Permissions << "\n";
    cout << "-----------------------------------\n";
}

void PrintUserRecordLine(stUser User)
{
    cout << "| " << left << setw(15) << User.UserName;
    cout << "| " << left << setw(10) << string(User.Password.length(), '*');
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
// =====================
// Transaction Functions
// =====================

void PrintClientBalanceLine(stClient Client)
{
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(12) << Client.AccountBalance;
}

void ShowTotalBalances()
{
    vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);

    cout << "\n\t\t\tBalances List (" << vClients.size() << ") Client(s).\n";
    cout << string(75, '_') << "\n\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n" << string(75, '_') << "\n\n";

    double TotalBalances = 0;

    if (vClients.empty())
        cout << "\t\t\tNo Clients Available!\n";
    else
        for (stClient& C : vClients)
        {
            PrintClientBalanceLine(C);
            TotalBalances += C.AccountBalance;
            cout << "\n";
        }

    cout << "\n" << string(75, '_') << "\n";
    cout << "\t\t\tTotal Balances = " << TotalBalances << "\n";
}

bool DepositToClient(string AccountNumber, double Amount,
    vector<stClient>& vClients)
{
    char Answer = 'N';
    cout << "\nAre you sure you want to perform this transaction? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsToFile(ClientsFileName, vClients);
                cout << "\nDone! New Balance: " << C.AccountBalance << "\n";
                return true;
            }
        }
    }
    return false;
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen\n";
    cout << "-----------------------------------\n";

    vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);
    stClient Client;

    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient [" << AccountNumber << "] not found!\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nEnter Deposit Amount: ";
    cin >> Amount;

    DepositToClient(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "-----------------------------------\n";

    vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);
    stClient Client;

    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient [" << AccountNumber << "] not found!\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nEnter Withdraw Amount: ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount exceeds balance!"
            << "\nMax withdrawal: " << Client.AccountBalance
            << "\nEnter amount (0 to cancel): ";
        cin >> Amount;
    }

    if (Amount > 0)
        DepositToClient(AccountNumber, Amount * -1, vClients);
}
// =====================
// Permission System
// =====================

bool CheckAccessPermission(enMainMenuPermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuPermissions::eAll)
        return true;

    return (Permission & CurrentUser.Permissions) == Permission;
}

void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied!\n";
    cout << "You don't have permission to do this.\n";
    cout << "Please contact your Admin.\n";
    cout << "------------------------------------\n";
}

bool LoadUserInfo(string Username, string Password)
{
    return FindUserByUsernameAndPassword(Username,
        Password, CurrentUser);
}

// =====================
// Login
// =====================

void Login()
{
    bool LoginFailed = false;
    string Username, Password;

    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\t   Login Screen\n";
        cout << "---------------------------------\n";

        if (LoginFailed)
            cout << "Invalid Username or Password!\n\n";

        cout << "Enter Username: ";
        cin >> Username;

        cout << "Enter Password: ";
        cin >> Password;

        LoginFailed = !LoadUserInfo(Username, Password);

    } while (LoginFailed);

    ShowMainMenu();
}
// =====================
// Sort Functions
// =====================

void MergeClientsByName(vector<stClient>& vClients,
    int Left, int Mid, int Right)
{
    int LeftSize = Mid - Left + 1;
    int RightSize = Right - Mid;

    vector<stClient> vLeft(LeftSize);
    vector<stClient> vRight(RightSize);

    for (int i = 0; i < LeftSize; i++)
        vLeft[i] = vClients[Left + i];
    for (int i = 0; i < RightSize; i++)
        vRight[i] = vClients[Mid + 1 + i];

    int i = 0, j = 0, k = Left;
    while (i < LeftSize && j < RightSize)
    {
        if (vLeft[i].Name <= vRight[j].Name)
            vClients[k++] = vLeft[i++];
        else
            vClients[k++] = vRight[j++];
    }
    while (i < LeftSize)  vClients[k++] = vLeft[i++];
    while (j < RightSize) vClients[k++] = vRight[j++];
}

/**
 * @brief Sorts clients alphabetically by Name.
 * Recursive — calls itself on left and right halves.
 * Base case: Left >= Right (single element, already sorted).
 */
void SortClientsByName(vector<stClient>& vClients,
    int Left, int Right)
{
    if (Left >= Right) return;

    int Mid = Left + (Right - Left) / 2;

    SortClientsByName(vClients, Left, Mid);
    SortClientsByName(vClients, Mid + 1, Right);
    MergeClientsByName(vClients, Left, Mid, Right);
}

void MergeClientsByAccountNumber(vector<stClient>& vClients,
    int Left, int Mid, int Right)
{
    int LeftSize = Mid - Left + 1;
    int RightSize = Right - Mid;

    vector<stClient> vLeft(LeftSize);
    vector<stClient> vRight(RightSize);

    for (int i = 0; i < LeftSize; i++)
        vLeft[i] = vClients[Left + i];
    for (int i = 0; i < RightSize; i++)
        vRight[i] = vClients[Mid + 1 + i];

    int i = 0, j = 0, k = Left;
    while (i < LeftSize && j < RightSize)
    {
        if (vLeft[i].AccountNumber <= vRight[j].AccountNumber)
            vClients[k++] = vLeft[i++];
        else
            vClients[k++] = vRight[j++];
    }
    while (i < LeftSize)  vClients[k++] = vLeft[i++];
    while (j < RightSize) vClients[k++] = vRight[j++];
}

/**
 * @brief Sorts clients by Account Number.
 * Recursive — calls itself on left and right halves.
 * Base case: Left >= Right (single element, already sorted).
 */
void SortClientsByAccountNumber(vector<stClient>& vClients,
    int Left, int Right)
{
    if (Left >= Right) return;

    int Mid = Left + (Right - Left) / 2;

    SortClientsByAccountNumber(vClients, Left, Mid);
    SortClientsByAccountNumber(vClients, Mid + 1, Right);
    MergeClientsByAccountNumber(vClients, Left, Mid, Right);
}

void MergeUsersByUsername(vector<stUser>& vUsers,
    int Left, int Mid, int Right)
{
    int LeftSize = Mid - Left + 1;
    int RightSize = Right - Mid;

    vector<stUser> vLeft(LeftSize);
    vector<stUser> vRight(RightSize);

    for (int i = 0; i < LeftSize; i++)
        vLeft[i] = vUsers[Left + i];
    for (int i = 0; i < RightSize; i++)
        vRight[i] = vUsers[Mid + 1 + i];

    int i = 0, j = 0, k = Left;
    while (i < LeftSize && j < RightSize)
    {
        if (vLeft[i].UserName <= vRight[j].UserName)
            vUsers[k++] = vLeft[i++];
        else
            vUsers[k++] = vRight[j++];
    }
    while (i < LeftSize)  vUsers[k++] = vLeft[i++];
    while (j < RightSize) vUsers[k++] = vRight[j++];
}

/**
 * @brief Sorts users alphabetically by Username.
 * Recursive — calls itself on left and right halves.
 * Base case: Left >= Right (single element, already sorted).
 */
void SortUsersByUsername(vector<stUser>& vUsers,
    int Left, int Right)
{
    if (Left >= Right) return;

    int Mid = Left + (Right - Left) / 2;

    SortUsersByUsername(vUsers, Left, Mid);
    SortUsersByUsername(vUsers, Mid + 1, Right);
    MergeUsersByUsername(vUsers, Left, Mid, Right);
}

void ChangeAdminCredentials()
{
    //here fixed a logical problem
	static string currentUserName = CurrentUser.UserName, currentPassword = CurrentUser.Password;

    if (CurrentUser.UserName != currentUserName && CurrentUser.Password != currentPassword)
    {
        cout <<"\nWrong Credentials !"
             << "\nOnly Admin can change these credentials!\n";
        return;
    }

    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);
    string Answer = "N";

    cout << "\n-----------------------------------\n";
    cout << "\tChange Admin Credentials\n";
    cout << "-----------------------------------\n";
    cout << "Current Username : " << CurrentUser.UserName << "\n";
    cout << "Current Password : " << CurrentUser.Password << "\n";
    cout << "-----------------------------------\n";

    do 
    {
        cout << "\nAre you sure you want to change credentials? Y/N? ";
		getline(cin >> ws, Answer);

    } while (toupper(Answer[0]) != 'Y' && toupper(Answer[0]) != 'N');

    if (toupper(Answer[0]) == 'N')
        return;
    
    string NewUsername = "";
    string NewPassword = "";

    cout << "\nEnter New Username: ";
    getline(cin >> ws, NewUsername);

    cout << "Enter New Password: ";
    getline(cin, NewPassword);

    for (stUser& U : vUsers)
    {
        if (U.UserName == CurrentUser.UserName && U.Password == CurrentUser.Password)
        {
            U.UserName = NewUsername;
            U.Password = NewPassword;
            CurrentUser = U;

			currentUserName = NewUsername;
			currentPassword = NewPassword;
            break;
        }
    }

    SaveUsersToFile(UsersFileName, vUsers);
    cout << "\nCredentials Updated Successfully!\n";
    cout << "New Username: " << CurrentUser.UserName << "\n";
}

// =====================
// Navigation Functions
// =====================

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenu();
}

void GoBackToTransactionsMenu()
{
    cout << "\n\nPress any key to go back to Transactions Menu...";
    system("pause>0");
    ShowTransactionsMenu();
}

void GoBackToManageUsersMenu()
{
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ShowManageUsersMenu();
}

// =====================
// Transactions Menu
// =====================

void PerformTransactionsMenuOption(
    enTransactionsMenuOptions Choice)
{
    switch (Choice)
    {
    case enTransactionsMenuOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenu();
        break;

    case enTransactionsMenuOptions::eWithdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenu();
        break;

    case enTransactionsMenuOptions::eShowTotalBalance:
        system("cls");
        ShowTotalBalances();
        GoBackToTransactionsMenu();
        break;

    case enTransactionsMenuOptions::eShowMainMenu:
        ShowMainMenu();
        break;
    }
}

void ShowTransactionsMenu()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pTransactions))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menu\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit\n";
    cout << "\t[2] Withdraw\n";
    cout << "\t[3] Total Balances\n";
    cout << "\t[4] Main Menu\n";
    cout << "===========================================\n";
   
    short Choice = 0;
    do
    {
        cout << "Choose [1-4]: ";
        cin >> Choice;

	} while (Choice < 1 || Choice > 4);
 
    PerformTransactionsMenuOption((enTransactionsMenuOptions)Choice);
}

// =====================
// Manage Users Menu
// =====================

void PerformManageUsersMenuOption(enManageUsersMenuOptions Choice)
{
    switch (Choice)
    {
    case enManageUsersMenuOptions::eListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenu();
        break;

    case enManageUsersMenuOptions::eAddNewUser:
        system("cls");
        AddNewUsers();
        GoBackToManageUsersMenu();
        break;

    case enManageUsersMenuOptions::eDeleteUser:
    {
        system("cls");
        vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);
        DeleteUserByUsername(ReadUsername(), vUsers);
        GoBackToManageUsersMenu();
        break;
    }

    case enManageUsersMenuOptions::eUpdateUser:
    {
        system("cls");
        vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);
        UpdateUserByUsername(ReadUsername(), vUsers);
        GoBackToManageUsersMenu();
        break;
    }

    case enManageUsersMenuOptions::eFindUser:
    {
        system("cls");
        vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);
        stUser User;
        string Username = ReadUsername();
        if (FindUserByUsername(Username, vUsers, User))
            PrintUserCard(User);
        else
            cout << "\nUser [" << Username << "] Not Found!\n";
        GoBackToManageUsersMenu();
        break;
    }

    case enManageUsersMenuOptions::eSortUsers:
    {
        system("cls");
        vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);
        if (vUsers.empty())
        {
            cout << "\nNo Users to Sort!\n";
        }
        else
        {
            SortUsersByUsername(vUsers, 0, (int)vUsers.size() - 1);
            SaveUsersToFile(UsersFileName, vUsers);
            cout << "\nUsers Sorted by Username A-Z Successfully!\n";
        }
        GoBackToManageUsersMenu();
        break;
    }
    case enManageUsersMenuOptions::eChangeAdminCreds:
        system("cls");
        ChangeAdminCredentials();
        GoBackToManageUsersMenu();
        break;
    case enManageUsersMenuOptions::eUsersMainMenu:
        ShowMainMenu();
        break;
    }
}

void ShowManageUsersMenu()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenu();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menu\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users\n";
    cout << "\t[2] Add New User\n";
    cout << "\t[3] Delete User\n";
    cout << "\t[4] Update User\n";
    cout << "\t[5] Find User\n";
    cout << "\t[6] Sort Users A-Z\n";
    cout << "\t[7] Change Admin Credentials\n";
    cout << "\t[8] Main Menu\n";
    cout << "===========================================\n";

    short Choice = 0;
    do
    {
        cout << "Choose [1-8]: ";
        cin >> Choice;
	} while (Choice < 1 || Choice > 8);
   
    PerformManageUsersMenuOption((enManageUsersMenuOptions)Choice);
}
// =====================
// Main Menu
// =====================

void PerformMainMenuOption(enMainMenuOptions Choice)
{
    switch (Choice)
    {
    case enMainMenuOptions::eListClients:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eAddNewClient:
        if (!CheckAccessPermission(enMainMenuPermissions::pAddNewClient))
        {
            ShowAccessDeniedMessage();
            GoBackToMainMenu();
            return;
        }
        system("cls");
        AddNewClients();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::eDeleteClient:
    {
        if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
        {
            ShowAccessDeniedMessage();
            GoBackToMainMenu();
            return;
        }
        system("cls");
        vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);
        DeleteClientByAccountNumber(ReadClientAccountNumber(), vClients);
        GoBackToMainMenu();
        break;
    }

    case enMainMenuOptions::eUpdateClient:
    {
        if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClients))
        {
            ShowAccessDeniedMessage();
            GoBackToMainMenu();
            return;
        }
        system("cls");
        vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);
        UpdateClientByAccountNumber(ReadClientAccountNumber(), vClients);
        GoBackToMainMenu();
        break;
    }

    case enMainMenuOptions::eFindClient:
    {
        if (!CheckAccessPermission(enMainMenuPermissions::pFindClient))
        {
            ShowAccessDeniedMessage();
            GoBackToMainMenu();
            return;
        }
        system("cls");
        vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);
        stClient Client;
        string AccountNumber = ReadClientAccountNumber();
        if (FindClientByAccountNumber(AccountNumber, vClients, Client))
            PrintClientCard(Client);
        else
            cout << "\nClient [" << AccountNumber << "] Not Found!\n";
        GoBackToMainMenu();
        break;
    }

    case enMainMenuOptions::eShowTransactions:
        system("cls");
        ShowTransactionsMenu();
        break;

    case enMainMenuOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenu();
        break;

    case enMainMenuOptions::eSortClientsByName:
    {
        system("cls");
        vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);
        if (vClients.empty())
        {
            cout << "\nNo Clients to Sort!\n";
        }
        else
        {
            SortClientsByName(vClients, 0, (int)vClients.size() - 1);
            SaveClientsToFile(ClientsFileName, vClients);
            cout << "\nClients Sorted by Name A-Z Successfully!\n";
        }
        GoBackToMainMenu();
        break;
    }

    case enMainMenuOptions::eSortClientsByAccNum:
    {
        system("cls");
        vector<stClient> vClients = LoadClientsFromFile(ClientsFileName);
        if (vClients.empty())
        {
            cout << "\nNo Clients to Sort!\n";
        }
        else
        {
            SortClientsByAccountNumber(vClients, 0, (int)vClients.size() - 1);
            SaveClientsToFile(ClientsFileName, vClients);
            cout << "\nClients Sorted by Account Number Successfully!\n";
        }
        GoBackToMainMenu();
        break;
    }

    case enMainMenuOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t  Bank Management System\n";
    cout << "\t  Welcome, " << CurrentUser.UserName << "\n";
    cout << "===========================================\n";
    cout << "\t[1]  List Clients\n";
    cout << "\t[2]  Add New Client\n";
    cout << "\t[3]  Delete Client\n";
    cout << "\t[4]  Update Client\n";
    cout << "\t[5]  Find Client\n";
    cout << "\t[6]  Transactions\n";
    cout << "\t[7]  Manage Users\n";
    cout << "\t[8]  Sort Clients by Name A-Z\n";
    cout << "\t[9]  Sort Clients by Account Number\n";
    cout << "\t[10] Logout\n";
    cout << "===========================================\n";

    short Choice = 0;
    do
    {
        cout << "Choose [1-10]: ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 10);

    PerformMainMenuOption((enMainMenuOptions)Choice);
}

int main()
{
    Login();
    return 0;
}
