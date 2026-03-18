# 🏦 Bank Management System — C++

A console-based Bank Management System built in **C++** demonstrating
advanced concepts of **Functional & Procedural Programming**.

---

## 🎬 Video Walkthrough

[![Bank Management System in C++](https://img.youtube.com/vi/LGK9aZJ4soo/0.jpg)](https://youtu.be/LGK9aZJ4soo)

> Click the image above to watch the full project walkthrough on YouTube.

## 🎯 Learning Objectives

- Multi-level menu navigation
- File I/O for persistent data storage
- Bitwise operators for permission management
- XOR encryption for password security
- Recursive Merge Sort for sorting clients and users
- Login and authentication system
- Forward declarations
- Static variables
- String manipulation and parsing

---

## 🕹️ Features

### 👥 Client Management
- 📋 List all clients
- ➕ Add new client
- ❌ Delete client with confirmation
- ✏️ Update client info
- 🔍 Find client by account number
- 🔤 Sort clients by Name A-Z
- 🔢 Sort clients by Account Number

### 💰 Transactions
- 💵 Deposit
- 💸 Withdraw with balance validation
- 📊 Show total balances

### 👤 User Management
- 📋 List all users
- ➕ Add new user with unique username
- ❌ Delete user
- ✏️ Update user
- 🔍 Find user
- 🔤 Sort users by Username A-Z
- 🔑 Change Admin credentials

### 🔐 Security
- XOR encrypted passwords in file
- Role-based permission system
- Login authentication
- Passwords hidden in display

---

## 🔐 Permission System

Uses **Bitwise AND** to check permissions:
```cpp
enum enMainMenuPermissions
{
    eAll          = -1,  // Full access
    pListClients  = 1,   // 0000001
    pAddNewClient = 2,   // 0000010
    pDeleteClient = 4,   // 0000100
    pUpdateClient = 8,   // 0001000
    pFindClient   = 16,  // 0010000
    pTransactions = 32,  // 0100000
    pManageUsers  = 64   // 1000000
};
```

---

## 🏗️ Project Architecture
```
main()
 └── Login()
      └── ShowMainMenu()
           ├── Client CRUD
           ├── SortClientsByName()      ← Recursive
           ├── SortClientsByAccNum()    ← Recursive
           ├── ShowTransactionsMenu()
           │    ├── Deposit()
           │    └── Withdraw()
           └── ShowManageUsersMenu()
                ├── User CRUD
                ├── SortUsersByUsername() ← Recursive
                └── ChangeAdminCredentials()
```

---

## 💡 Key Concepts

| Concept | Where in Code |
|---|---|
| Enums | `enMainMenuOptions`, `enMainMenuPermissions` |
| Structs | `stClient`, `stUser` |
| Vectors | `vClients`, `vUsers` |
| File I/O | `SaveClientsToFile()`, `LoadClientsFromFile()` |
| Bitwise AND | `CheckAccessPermission()` |
| XOR Encryption | `XOREncryptDecrypt()` |
| Recursion | `SortClientsByName()`, `SortUsersByUsername()` |
| Static Variables | `ChangeAdminCredentials()` |
| Forward Declarations | `ShowMainMenu()`, `Login()` |
| String Methods | `SplitString()`, `substr()`, `stoi()` |

---

## 🔧 Build & Run

**Open in Visual Studio**
```
double click: bank-management-cpp.slnx
Ctrl + F5
```

**Default Admin Credentials**
```
Username: Admin
Password: 1234
```

---

## 📁 File Structure
```
bank-management-cpp/
├── .gitignore
├── README.md
├── LICENSE
├── bank-management-cpp.slnx
└── bank-management-cpp/
    ├── bank-management-cpp.cpp
    ├── bank-management-cpp.vcxproj
    └── bank-management-cpp.vcxproj.filters
```

> 📝 `Clients.txt` and `Users.txt` are auto-generated at runtime.
> They are excluded from version control via `.gitignore`.

---

## 👤 Author

Built as a learning exercise in C++ Procedural Programming.

---

## 📄 License

MIT License