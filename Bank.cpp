#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;


fstream MyFile;
const string UsersFile = "SystemUsers.txt";
const string ClientsFile = "SystemClients.txt";
enum enClientMenuChoices
{
	enShowClientList = 1,
	enAddNewClient = 2,
	enDeleteClient = 3,
	enUpdateClientInfo = 4,
	enFindClient = 5,
	enTransactions = 6,
	enMangeUsers = 7,
	enLogout = 8,
	enExit = 9
};
enum enUsersMenuChoices
{
	enListUsers = 1,
	enAddNewUser = 2,
	enDeleteUser = 3,
	enUpdateUserInfo = 4,
	enFindUser = 5,
	MainMenu = 6
};
enum enTransactions
{
	enDeposit = 1,
	enWithdraw = 2,
	enTotalBalances = 3,
	enMainMenu = 4
};
struct stUsersInfo
{
	string UserName;
	string Password;
	string PermissionsCode;
};
struct stClientInfo
{
	string AccNumber;
	string PinCode;
	string ClientName;
	string Phone;
	string Balance;
	bool MarkForDelete = false;
};
// Some Prototype because we called this functions sometimes before its defintion
void Login(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers);
void BankSystem(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers, string UserName = "Admin");
void MangeUsers(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers, string UserName);
bool FindClientWithAccNumber(string AccNumber, vector<stClientInfo>& MyClients);

bool ContinueOrNot()
{
	char status;
	cin >> status;

	return status == 'y' || status == 'Y';
}
void ShowMainMenuScreen()
{
	cout << "===================================\n";
	cout << "\t Main Menu Screen\n";
	cout << "===================================\n";
	cout << "[1]Show Client List\n";
	cout << "[2]Add New Client\n";
	cout << "[3]Delete Client\n";
	cout << "[4]Update Client Info\n";
	cout << "[5]Find Client\n";
	cout << "[6]Transactions\n";
	cout << "[7]MangeUsers\n";
	cout << "[8]Logout\n";
	cout << "[9]Exit\n\n";
}
stClientInfo ReadclientData(vector<stClientInfo>& MyClients)
{
	stClientInfo client;

	cout << "Please enter account number : ";
	getline(cin >> ws, client.AccNumber);

	while (FindClientWithAccNumber(client.AccNumber, MyClients))
	{
		cout << "\nClient with [" << client.AccNumber << "] is already existed, please enter another account number : ";
		getline(cin, client.AccNumber);

	}
	cout << "Please enter pin code : ";
	getline(cin, client.PinCode);
	cout << "Please enter your name : ";
	getline(cin, client.ClientName);
	cout << "Please enter phone number : ";
	getline(cin, client.Phone);
	cout << "Please enter balance : ";
	getline(cin, client.Balance);
	return client;
}
void ShowClientData(string AccountNumber, vector<stClientInfo>& MyClients)
{
	for (stClientInfo& CurrentClient : MyClients)
	{
		if (CurrentClient.AccNumber == AccountNumber)
		{
			cout << "--------------------------------------\n";
			cout << "Account Number : " << CurrentClient.AccNumber << endl;
			cout << "Pin code : " << CurrentClient.PinCode << endl;
			cout << "Name : " << CurrentClient.ClientName << endl;
			cout << "Phone : " << CurrentClient.Phone << endl;
			cout << "Account Balance : " << CurrentClient.Balance << endl;
			cout << "--------------------------------------\n";
		}
	}
}
string ConvertClientsDataToLine(stClientInfo client)
{
	string result = "";
	result += client.AccNumber + "#//#";
	result += client.PinCode + "#//#";
	result += client.ClientName + "#//#";
	result += client.Phone + "#//#";
	result += client.Balance;
	return result;
}

vector<string> SplitString(string str, string Delimiter = "#//#")
{
	vector<string> result;
	string temp = "";
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] != '#')
		{
			temp += str[i];
		}
		else
		{
			result.push_back(temp);
			temp = "";
			i += 3;
		}
	}
	result.push_back(temp);
	return result;
}
vector<stClientInfo> LoadClientsFileToVector()
{
	vector<stClientInfo> result;
	stClientInfo temp;
	string line;
	MyFile.open(ClientsFile, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, line))
		{
			vector<string> SplitedWords = SplitString(line);;
			temp.AccNumber = SplitedWords[0];
			temp.PinCode = SplitedWords[1];
			temp.ClientName = SplitedWords[2];
			temp.Phone = SplitedWords[3];
			temp.Balance = SplitedWords[4];
			result.push_back(temp);
		}
		MyFile.close();
	}
	return result;
}
vector<stUsersInfo> LoadUsersFileToVector()
{
	vector<stUsersInfo> result;
	stUsersInfo temp;
	string line;
	MyFile.open(UsersFile, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, line))
		{
			vector<string> SplitedWords = SplitString(line);
			temp.UserName = SplitedWords[0];
			temp.Password = SplitedWords[1];
			temp.PermissionsCode = SplitedWords[2];
			result.push_back(temp);
		}
		MyFile.close();
	}
	return result;
}
void ShowClientList(vector<stClientInfo>& MyClients)
{

	cout << "\t\t\tClients List (" << MyClients.size() << ") Client(S).\n";
	cout << "-------------------------------------------------------------------------\n";
	cout << "|Account Number   | Pin Code  |  Client Name      | Phone        |Balance\n";
	cout << "-------------------------------------------------------------------------\n";
	for (stClientInfo CurrentClient : MyClients)
	{
		cout << "|" << setw(17) << left << CurrentClient.AccNumber;
		cout << "|" << setw(11) << left << CurrentClient.PinCode;
		cout << "|" << setw(19) << left << CurrentClient.ClientName;
		cout << "|" << setw(14) << left << CurrentClient.Phone;
		cout << "|" << setw(7) << left << CurrentClient.Balance;
		cout << endl;
	}
}
void LoadClientsToFile(vector<stClientInfo>& MyClients)
{
	for (int i = 0; i < MyClients.size(); i++)
	{
		if (MyClients[i].MarkForDelete == true)
		{
			continue;
		}
		else
		{
			string line = ConvertClientsDataToLine(MyClients[i]);
			MyFile << line << endl;
		}
	}
}
void AddNewClient(vector<stClientInfo>& MyClients)
{
	cout << "==================================\n";
	cout << "\t Add New Client Screen\n";
	cout << "==================================\n\n";
	stClientInfo NewClint = ReadclientData(MyClients);
	MyClients.push_back(NewClint);
	MyFile.open(ClientsFile, ios::out);

	if (MyFile.is_open())
	{
		LoadClientsToFile(MyClients);
		MyFile.close();
	}
	cout << "\nClient Added Sucssefult\n";
}
bool FindClientWithAccNumber(string AccNumber, vector<stClientInfo>& MyClients)
{
	for (stClientInfo CurrentClient : MyClients)
	{
		if (CurrentClient.AccNumber == AccNumber)
		{
			return true;
		}
	}
	return false;
}
void MarkClientForDelete(string AccountNumber, vector<stClientInfo>& MyClients)
{
	for (stClientInfo& CurrentClient : MyClients)
	{
		if (CurrentClient.AccNumber == AccountNumber)
		{
			CurrentClient.MarkForDelete = true;
		}
	}
}
void DeleteClient(vector<stClientInfo>& MyClients)
{
	cout << "==================================\n";
	cout << "\t Delete Client Screen\n";
	cout << "==================================\n\n";
	string AccountNumber;
	cout << "Please Enter Account Number : ";
	getline(cin >> ws, AccountNumber);
	if (FindClientWithAccNumber(AccountNumber, MyClients))
	{
		cout << "\nThe Following are the client Details :- \n\n";
		ShowClientData(AccountNumber, MyClients);
		cout << "\nAre you sure to delete this client (y / n) : ";
		if (ContinueOrNot())
		{
			MyFile.open(ClientsFile, ios::out);
			if (MyFile.is_open())
			{
				vector<stClientInfo> ::iterator it = MyClients.begin();
				while (it != MyClients.end())
				{
					if ((*it).AccNumber == AccountNumber)
					{
						MyClients.erase(it);
						LoadClientsToFile(MyClients);
						cout << "Client Deleted sucssefuly.\n";
						break;
					}
					else
					{
						++it;
					}
				}
				MyFile.close();
			}
		}
	}
	else
	{
		cout << "Account not found.\n";
	}
}
void UpdateClientInfo(vector<stClientInfo>& MyClients)
{
	cout << "==================================\n";
	cout << "\t Update Client Info Screen\n";
	cout << "==================================\n";
	string AccountNumber;
	cout << "PLease enter Client Account Name : ";
	getline(cin >> ws, AccountNumber);
	if (FindClientWithAccNumber(AccountNumber, MyClients))
	{
		cout << "\nThe following are the client details :-\n";
		ShowClientData(AccountNumber, MyClients);
		cout << "Are you sure you wnat to update this client (y / n) : ";
		if (ContinueOrNot())
		{
			for (stClientInfo& CurrentClient : MyClients)
			{
				if (AccountNumber == CurrentClient.AccNumber)
				{
					cout << "Please enter Pin code : ";
					getline(cin >> ws, CurrentClient.PinCode);
					cout << "Please enter name : ";
					getline(cin, CurrentClient.ClientName);
					cout << "Please enter phone : ";
					getline(cin, CurrentClient.Phone);
					cout << "Please enter account balance : ";
					getline(cin, CurrentClient.Balance);
					break;
				}
			}
			cout << "Client Updated Sucssfuly.\n";
		}
	}
	else
	{
		cout << "Client is not found.\n";
	}
}
void FindClient(vector<stClientInfo>& MyClients)
{

	cout << "==================================\n";
	cout << "\t Find Client Screen\n";
	cout << "==================================\n";
	string Accnumber;
	cout << "Please enter your Account Number : ";
	cin >> Accnumber;
	if (FindClientWithAccNumber(Accnumber, MyClients))
	{
		cout << "\nThe following are the client details :-\n";

		ShowClientData(Accnumber, MyClients);
	}
	else
	{
		cout << "Client is not found\n";
	}
}
short ReadChoice(short from, short to)
{
	cout << "Please Enter your Choice ( " << from << " : " << to << " ) : ";
	short Choice;
	cin >> Choice;
	return Choice;
}
void BackToMainMenu(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers, string UserName)
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	BankSystem(MyClients, MyUsers, UserName);
}
void ShowTransactionsMenuScreen()
{
	cout << "=======================================\n";
	cout << "\t TranAction Menu Screen\n";
	cout << "=======================================\n";
	cout << "[1] Deposit.\n";
	cout << "[2] WithDraw.\n";
	cout << "[3] Total Balances.\n";
	cout << "[4] Main Menu.\n";
}
void ShowDepositScreen()
{
	cout << "------------------\n";
	cout << "  Deposit Screen\n";
	cout << "------------------\n";
}
void DepositClient(vector<stClientInfo>& MyClients)
{
	ShowDepositScreen();
	string AccNumber;
	cout << "Please account number : ";
	getline(cin >> ws, AccNumber);
	while (!FindClientWithAccNumber(AccNumber, MyClients))
	{
		cout << "Client is not found, please enter a valid client : ";
		getline(cin, AccNumber);
	}
	ShowClientData(AccNumber, MyClients);

	long long deposit;
	cout << "Please enter deposit amount :";
	cin >> deposit;
	cout << "Are you sure to performe this trasaction (y / n) : ";
	if (ContinueOrNot())
	{
		for (stClientInfo& CurrentClient : MyClients)
		{
			if (CurrentClient.AccNumber == AccNumber)
			{
				long long newBlance = stoll(CurrentClient.Balance) + deposit;
				CurrentClient.Balance = to_string(newBlance);
				break;
			}
		}
	}
	MyFile.open(ClientsFile, ios::out);
	if (MyFile.is_open())
	{
		LoadClientsToFile(MyClients);
		MyFile.close();
	}
}
void Transactions(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers);
void BackToTransctionsScreen(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers)
{
	cout << "\n\nPress any key to go back to Transactions Menue...\n";
	system("pause>0");
	Transactions(MyClients, MyUsers);
}
void ShowWithDrawScreen()
{
	cout << "------------------\n";
	cout << "  Deposit Screen\n";
	cout << "------------------\n";
}
void WithdrawClient(vector<stClientInfo>& MyClients)
{
	ShowWithDrawScreen();
	string AccNumber;
	cout << "Please account number : ";
	getline(cin >> ws, AccNumber);
	while (!FindClientWithAccNumber(AccNumber, MyClients))
	{
		cout << "Client is not found, please enter a valid client : ";
		getline(cin, AccNumber);
	}
	ShowClientData(AccNumber, MyClients);

	long long withdraw;
	cout << "Please enter withdraw amount :";
	cin >> withdraw;
	for (stClientInfo& CurrentClient : MyClients)
	{
		if (CurrentClient.AccNumber == AccNumber)
		{
			while (stoll(CurrentClient.Balance) < withdraw)
			{
				cout << "\nYou Have Only [" << CurrentClient.Balance << "], You Can't Take [" << withdraw << "].\n";
				cout << "PLease Enter a Valid Number : ";
				cin >> withdraw;
			}
		}
	}
	cout << "Are you sure to performe this trasaction (y / n) : ";
	if (ContinueOrNot())
	{
		for (stClientInfo& CurrentClient : MyClients)
		{
			if (CurrentClient.AccNumber == AccNumber)
			{
				long long newBlance = stoll(CurrentClient.Balance) - withdraw;
				CurrentClient.Balance = to_string(newBlance);
				break;
			}
		}
	}
	MyFile.open(ClientsFile, ios::out);
	if (MyFile.is_open())
	{
		LoadClientsToFile(MyClients);
		MyFile.close();
	}

}
long long CountTotalBalances(vector<stClientInfo>& MyClients)
{
	long long total = 0;
	for (stClientInfo& CurrentClient : MyClients)
	{
		total += stoll(CurrentClient.Balance);
	}
	return total;
}
void TotalBalances(vector<stClientInfo>& MyClients)
{
	cout << "\t\tBalacnes  List (" << MyClients.size() << ") Client(S).\n";
	cout << "--------------------------------------------------\n";
	cout << "|Account Number   |  Client Name      ||Balance\n";
	cout << "--------------------------------------------------\n";
	for (stClientInfo CurrentClient : MyClients)
	{
		cout << "|" << setw(17) << left << CurrentClient.AccNumber;
		cout << "|" << setw(19) << left << CurrentClient.ClientName;
		cout << "|" << setw(7) << left << CurrentClient.Balance;
		cout << endl;
	}
	cout << "\t" << "Total Balances is : $" << CountTotalBalances(MyClients);
}
void Transactions(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers)
{
	cout << "==================================\n";
	cout << "\t Transactions Screen\n";
	cout << "==================================\n";
	system("cls");

	ShowTransactionsMenuScreen();
	short choice = ReadChoice(1, 4);
	switch (choice)
	{
	case enTransactions::enDeposit:
		system("cls");
		DepositClient(MyClients);
		BackToTransctionsScreen(MyClients, MyUsers);
		break;
	case enTransactions::enWithdraw:
		system("cls");
		WithdrawClient(MyClients);
		BackToTransctionsScreen(MyClients, MyUsers);
		break;
	case enTransactions::enTotalBalances:
		system("cls");
		TotalBalances(MyClients);
		BackToTransctionsScreen(MyClients, MyUsers);
	case enTransactions::enMainMenu:
		system("cls");
		BankSystem(MyClients, MyUsers);
		break;
	}
}
vector<int> ReadPermission(string UserName, vector<stUsersInfo>& MyUsers)
{
	vector<int> result;
	for (stUsersInfo& CurrentUser : MyUsers)
	{
		if (CurrentUser.UserName == UserName)
		{
			string temp = "";
			for (char ch : CurrentUser.PermissionsCode)
			{
				if (ch != '-')
				{
					temp += ch;
				}
				else
				{
					result.push_back(stoi(temp));
					temp = "";
				}
			}
			result.push_back(stoi(temp));
			break;
		}
	}
	return result;
}
bool isHavePermission(vector<int> Permissions, int choice)
{
	for (int i = 0; i < Permissions.size(); i++)
	{
		if (choice == Permissions[i])return true;
	}
	return false;
}
void ShowAccessDenied()
{
	cout << "\n\n====================================\n";
	cout << "Access Denied,\n";
	cout << "You dont Have Permission To Do this,\n";
	cout << "Please Contact Your Admin.\n";
	cout << "====================================\n";
}
void ShowMangeUsersScreen()
{
	cout << "===================================\n";
	cout << "\t Mange Users Screen\n";
	cout << "===================================\n";
	cout << "[1]Show Users List\n";
	cout << "[2]Add New User\n";
	cout << "[3]Delete User\n";
	cout << "[4]Update User Info\n";
	cout << "[5]Find User\n";
	cout << "[6]Main Menu\n";

}
void ShowListUsers(vector<stUsersInfo>& MyUsers)
{
	MyFile.open(UsersFile, ios::in);

	if (MyFile.is_open())
	{
		cout << "\t\t\tClients List (" << MyUsers.size() << ") Client(S).\n";
		cout << "-----------------------------------------------------------------------\n";
		cout << "|User Name     | Password     |          Permissions \n";
		cout << "-----------------------------------------------------------------------\n";
		for (stUsersInfo CurrentUser : MyUsers)
		{
			cout << "|" << setw(14) << left << CurrentUser.UserName;
			cout << "|" << setw(14) << left << CurrentUser.Password;
			cout << "|" << setw(40) << left << CurrentUser.PermissionsCode;
			cout << endl;
		}
		MyFile.close();
	}
}
string ReadPermissions()
{
	string result = "";
	cout << "\n\nDo you want to Give access to :\n\n";
	cout << "Show Client List (y / n) : ";
	if (ContinueOrNot())
	{
		result += "1-";
	}
	cout << "Add New Client (y / n) : ";
	if (ContinueOrNot())
	{
		result += "2-";
	}
	cout << "Delete Client (y / n) : ";
	if (ContinueOrNot())
	{
		result += "3-";
	}
	cout << "Update Client (y / n) : ";
	if (ContinueOrNot())
	{
		result += "4-";
	}
	cout << "Find Client (y / n) : ";
	if (ContinueOrNot())
	{
		result += "5-";
	}
	cout << "Transactions (y / n) : ";
	if (ContinueOrNot())
	{
		result += "6-";
	}
	cout << "Mange Users (y / n) : ";
	if (ContinueOrNot())
	{
		result += "7-";
	}
	return result.substr(0, result.length() - 1);
}
string ConvertUsersDataToLine(stUsersInfo User)
{
	string result = "";
	result += User.UserName + "#//#";
	result += User.Password + "#//#";
	result += User.PermissionsCode;
	return result;
}
void LoadUsersToFile(vector<stUsersInfo>& MyUsers)
{
	MyFile.open(UsersFile, ios::out); // write mode

	if (MyFile.is_open())
	{
		for (int i = 0; i < MyUsers.size(); i++)
		{
			string line = ConvertUsersDataToLine(MyUsers[i]);
			MyFile << line << endl;
		}
		MyFile.close();
	}

}
stUsersInfo ReadUserData();

void AddNewUser(vector<stUsersInfo>& MyUsers)
{
	cout << "==================================\n";
	cout << "\t Add New User Screen\n";
	cout << "==================================\n";
	do
	{
		stUsersInfo NewUser = ReadUserData();
		bool FullAccess;
		cout << "\n\nDo you want to Give full Access (y / n) :";
		ContinueOrNot() ? FullAccess = true : FullAccess = false;
		FullAccess ? NewUser.PermissionsCode = "1-2-3-4-5-6-7" : NewUser.PermissionsCode = ReadPermissions();
		MyUsers.push_back(NewUser);
		cout << "User Added Successfult, Do you want to Add Another User (y / n)  : ";
		LoadUsersToFile(MyUsers);
	} while (ContinueOrNot());

}
bool FindUser(vector<stUsersInfo>& MyUsers, string UserName)
{
	for (stUsersInfo CurrentUser : MyUsers)
	{
		if (CurrentUser.UserName == UserName) return true;
	}
	return false;
}
void ShowUserData(vector<stUsersInfo>& MyUsers, string UserName)
{
	for (stUsersInfo CurrentUser : MyUsers)
	{
		if (CurrentUser.UserName == UserName)
		{
			cout << "The Following are the User Details :\n";
			cout << "=================================\n";
			cout << "User Name : " << CurrentUser.UserName << endl;
			cout << "Password :" << CurrentUser.Password << endl;
			cout << "Permissions Code :" << CurrentUser.PermissionsCode << endl;
			cout << "=================================\n";
			break;
		}
	}
}
void DeleteUsers(vector<stUsersInfo>& MyUsers)
{
	cout << "==================================\n";
	cout << "\t Delete User Screen\n";
	cout << "==================================\n";
	string MyUserName;
	cout << "Please enter User Name : ";
	getline(cin >> ws, MyUserName);
	while (!FindUser(MyUsers, MyUserName))
	{
		cout << "User [" << MyUserName << "] is not found.\n\n";
		cout << "PLease enter a valid user.\n\n";
		cout << "Please enter User Name : ";
		getline(cin, MyUserName);
	}
	if (MyUserName == "Admin")
	{
		cout << "You Can't Delete Admin From Users.";
		return;
	}
	ShowUserData(MyUsers, MyUserName);
	cout << "\n\nAre you sure you want to delete this user (y / n) : ";
	if (ContinueOrNot())
	{
		vector<stUsersInfo>::iterator it ;
		for (it = MyUsers.begin(); it != MyUsers.end(); ++it)
		{
			if (it->UserName == MyUserName)
			{
				MyUsers.erase(it);
				LoadUsersToFile(MyUsers);
				break;
			}		
		}

	}

}
void UpdateUserInfo(vector<stUsersInfo>& MyUsers)
{
	cout << "==================================\n";
	cout << "\t Update User Info Screen\n";
	cout << "==================================\n";
	string UserName;
	cout << "Please enter User Name : ";
	getline(cin >> ws, UserName);
	while (!FindUser(MyUsers, UserName))
	{
		cout << "User [" << UserName << "] is not found.\n\n";
		cout << "PLease enter a valid user.\n\n";
		cout << "Please enter User Name : ";
		getline(cin, UserName);
	}
	ShowUserData(MyUsers, UserName);
	if (UserName == "Admin")
	{
		cout << "You Can't Update Admin's info.";
		return;
	}
	cout << "Are you sure you Want to Update User info (y / n) : ";
	if (ContinueOrNot())
	{
		for (stUsersInfo& UpdatedUser : MyUsers)
		{
			if (UpdatedUser.UserName == UserName)
			{
				cout << "\n\nEnter user password : ";
				getline(cin >> ws, UpdatedUser.Password);
				bool FullAccess;
				cout << "\n\nDo you want to Give full Access (y / n) :";
				ContinueOrNot() ? FullAccess = true : FullAccess = false;
				FullAccess ? UpdatedUser.PermissionsCode = "1-2-3-4-5-6-7" : UpdatedUser.PermissionsCode = ReadPermissions();
				LoadUsersToFile(MyUsers);
				break;
			}
		}
	}
}
void FindUserInfo(vector<stUsersInfo>& MyUsers)
{
	cout << "==================================\n";
	cout << "\t Find User Screen\n";
	cout << "==================================\n";
	string UserName;
	cout << "Please enter User Name : ";
	getline(cin >> ws, UserName);
	while (!FindUser(MyUsers, UserName))
	{
		cout << "User [" << UserName << "] is not found.\n\n";
		cout << "PLease enter a valid user.\n\n";
		cout << "Please enter User Name : ";
		getline(cin, UserName);
	}
	ShowUserData(MyUsers, UserName);
}
void BackToMangeUsersScreen(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers, string UserName)
{
	cout << "\nPlease enter any key to go to Mange Users Menu...";
	system("pause>0");
	MangeUsers(MyClients, MyUsers, UserName);

}
void MangeUsers(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers, string UserName)
{
	system("cls");
	ShowMangeUsersScreen();
	int choice = ReadChoice(1, 6);
	switch (choice)
	{
	case enUsersMenuChoices::enListUsers:
		system("cls");
		ShowListUsers(MyUsers);
		BackToMangeUsersScreen(MyClients, MyUsers, UserName);
		break;

	case enUsersMenuChoices::enAddNewUser:
		system("cls");
		AddNewUser(MyUsers);
		BackToMangeUsersScreen(MyClients, MyUsers, UserName);
		break;
	case enUsersMenuChoices::enDeleteUser:
		system("cls");
		DeleteUsers(MyUsers);
		BackToMangeUsersScreen(MyClients, MyUsers, UserName);
		break;

	case enUsersMenuChoices::enUpdateUserInfo:
		system("cls");
		UpdateUserInfo(MyUsers);
		BackToMangeUsersScreen(MyClients, MyUsers, UserName);
		break;

	case enUsersMenuChoices::enFindUser:
		system("cls");
		FindUserInfo(MyUsers);
		BackToMangeUsersScreen(MyClients, MyUsers, UserName);
		break;

	case enUsersMenuChoices::MainMenu:
		system("cls");
		BankSystem(MyClients, MyUsers, UserName);
		break;
	}


}
void BankSystem(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers, string UserName)
{
	system("cls");
	vector<int> Permissions = ReadPermission(UserName, MyUsers);

	ShowMainMenuScreen();
	short choice = ReadChoice(1, 9);
	switch (choice)
	{
	case enClientMenuChoices::enShowClientList:
		if (isHavePermission(Permissions, choice))
		{
			system("cls");
			ShowClientList(MyClients);
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}
		else
		{
			ShowAccessDenied();
			BackToMainMenu(MyClients, MyUsers, UserName);
		}
	case enClientMenuChoices::enAddNewClient:
		if (isHavePermission(Permissions, choice))
		{
			system("cls");
			AddNewClient(MyClients);
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}
		else
		{
			ShowAccessDenied();
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}

	case enClientMenuChoices::enDeleteClient:
		if (isHavePermission(Permissions, choice))
		{
			system("cls");
			DeleteClient(MyClients);
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}
		else
		{
			ShowAccessDenied();
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}

	case enClientMenuChoices::enUpdateClientInfo:
		if (isHavePermission(Permissions, choice))
		{
			system("cls");
			UpdateClientInfo(MyClients);
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}
		else
		{
			ShowAccessDenied();
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}

	case enClientMenuChoices::enFindClient:
		if (isHavePermission(Permissions, choice))
		{
			system("cls");
			FindClient(MyClients);
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}
		else
		{
			ShowAccessDenied();
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}

	case enClientMenuChoices::enTransactions:
		if (isHavePermission(Permissions, choice))
		{
			system("cls");
			Transactions(MyClients, MyUsers);
			break;
		}
		else
		{
			ShowAccessDenied();
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}
	case enClientMenuChoices::enMangeUsers:
		if (isHavePermission(Permissions, choice))
		{
			system("cls");
			MangeUsers(MyClients, MyUsers, UserName);
			break;
		}
		else
		{
			ShowAccessDenied();
			BackToMainMenu(MyClients, MyUsers, UserName);
			break;
		}
	case enClientMenuChoices::enLogout:

		system("cls");
		Login(MyClients, MyUsers);
		break;

	case enClientMenuChoices::enExit:
		cout << "Thank You For Using Our System\n";
		return;
	}
}
void ShowLoginScreen()
{
	cout << "===============================\n";
	cout << "\tLogin Screen\n";
	cout << "===============================\n";
}
stUsersInfo ReadUserData()
{
	stUsersInfo User;
	cout << "Enter UserName :";
	getline(cin >> ws, User.UserName);
	cout << "Enter Password :";
	getline(cin, User.Password);

	return User;
}
bool isUserExisted(vector<stUsersInfo>& MyUsers, stUsersInfo User)
{
	for (stUsersInfo& CurrentUser : MyUsers)
	{
		if (CurrentUser.UserName == User.UserName)
		{
			if (CurrentUser.Password == User.Password) return true;
		}
	}
	return false;
}
void Login(vector<stClientInfo>& MyClients, vector<stUsersInfo>& MyUsers)
{
	ShowLoginScreen();
	stUsersInfo User = ReadUserData();
	while (!isUserExisted(MyUsers, User))
	{
		cin.clear();
		cout << "\nInvaild UserName or Password, Please try again\n\n";
		User = ReadUserData();
	}
	BankSystem(MyClients, MyUsers, User.UserName);
}
int main()
{
	vector<stClientInfo> MyClients = LoadClientsFileToVector();
	vector<stUsersInfo> MyUsers = LoadUsersFileToVector();
	Login(MyClients, MyUsers);
}
