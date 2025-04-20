# 🏦 Bank Management System

## 📌 Description

This is a simple Bank Management System written in C++, file based and console application that allows administrators and users to manage client accounts. The system supports features such as:

 Adding, deleting, updating, and finding clients.

 Handling transactions (deposit, withdraw, and total balance calculation).

 Managing users with different permissions.

 Secure login system.

## 📌 Features

 Client Management: Add, delete, update, and search for clients.

 Transaction System: Deposit, withdraw, and check total balances.

 User Management: Add, delete, update, and list system users with different access levels.

 File-Based Storage: Stores clients and users' data in text files.
 User Interface Preview
## 📌 Login Menu

users must log in with their credentials. The login menu looks like this:
~~~
===============================
         Login Screen      
===============================
👤 Enter Username: ________
🔑 Enter Password: ________
-------------------------------
~~~

## 📌 Example Users (from SystemUsers.txt)
~~~
Username: Admin  |  Password: 1234  |  Permissions: Full access  
Username: User1  |  Password: 1111  |  Permissions: Full access  
Username: User4  |  Password: 4447  |  Permissions: Limited access  
~~~
## 📌 Main Menu

Once logged in, users are greeted with the Main Menu, which looks like this:
~~~
===================================
          Main Menu Screen
===================================
[1] Show Client List  
[2] Add New Client  
[3] Delete Client  
[4] Update Client Info  
[5] Find Client  
[6] Transactions  
[7] Manage Users  
[8] Logout  
[9] Exit  
===================================
~~~

## 📌 Installation & Usage

The system requires two files for storing data:

SystemUsers.txt: Stores system user credentials and permissions.

SystemClients.txt: Stores client account details.

Clone this repository or download the source files.

Open Bank.cpp in your C++ development environment.

Compile and run the program.

## 📌 Project Structure

  Bank.cpp                # Main source code
  
  SystemClients.txt       # simulation to a Database of clients
  
  SystemUsers.txt         # simulation to a Database of users
  
  README.md               # Project documentation
  

 ## 📌 Example Clients (from SystemClients.txt)
 ~~~
 Account: A150  |  PIN: 4966  |  Name: Moamen   |  Phone: 01033162134  |  Balance: $10,111
 Account: A151  |  PIN: 1234  |  Name: Moustafa |  Phone: 01055555646  |  Balance: $6,000
 Account: A152  |  PIN: 1234  |  Name: Mohamed  |  Phone: 01771149059  |  Balance: $12,000
 Account: A153  |  PIN: 1234  |  Name: Ahemd    |  Phone: 01011665514  |  Balance: $8,000
 Account: A155  |  PIN: 1234  |  Name: Ali      |  Phone: 01281677743  |  Balance: $305,000
~~~
## ⚠️ Notes
You can't delete the admin user from the file system using the program

All phone numbers used in the example clients are Fake and do not belong to real individuals
