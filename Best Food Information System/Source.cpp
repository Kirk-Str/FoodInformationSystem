

/*
########################################
#  BEST FOOD INFORMATION SYSTEM        #
#  Created by Kamalraj on 2016-01-29   #
#  HND/COM/70/33                       #
#  Procedural Programming              #
########################################
*/


//Preprocesors 'Include' Directives
//##########################################################

#include "stdio.h" // Basic Input and Output
#include <iostream> // Advance Basic Input and Output
#include <Windows.h> // Windpws API for Console window manipulations
#include <conio.h> //Manipulator
#include <string> // for direct string usage
#include <stdlib.h>
#include <fstream> // File reader
#include <iomanip> // Console window Manipulation
#include <ctime> //System Timer
#include <vector>

using namespace std;

//##########################################################


//Constants 'DEFINE' Directives
//##########################################################


//Other Themes
//#define BASECOLOR 138
//#define INVERTED_BASECOLOR 168

//Default UI theme
//#define BASECOLOR 7
//#define INVERTED_BASECOLOR 112
//#define I_BASECOLOR 112

//Current Theme 
#define BASECOLOR 23
#define INVERTED_BASECOLOR 121
#define I_BASECOLOR 27


#define USERS_DB "Database\\users.txt"
#define MENU_DB "Database\\menu.txt"
#define CATERING_DB "Database\\catering.txt"
#define SERVICES_DB "Database\\services.txt"
#define ORDERLIST_DB "Database\\orderlist.txt"
#define TEMP_ORDERLIST_DB "Database\\temporderlist.txt"
//
//#define BASECOLOR 31
//#define INVERTED_BASECOLOR 176
//#define I_BASECOLOR 30

//##########################################################


//Function Prototypes
//##########################################################

void home();
void setConsoleSize(int, int);
void setConsoleSizeEx(int, int, int, int);
int getCenterCoordinatesForText(string);
int getNextCoordinatesForText(string, int);
vector<string> splitString(string, string);
void customerLogin();
void guestLogin();
void gotoxy(int, int);
void clear();
void setColor(int);

void CustomerOptions();
void AdminOptions();

void FoodsNBeverages();
void FoodsNBeveragesInformation(string);
void Catering();
void Services();
void Help();
void CreateUser();
void ViewAllOrderLists();
void OrderDetail(string);
void Logoff();

void saveDB();
void deleteAndUpdateOrders(string);

void AppHeader(string);
void headerMenu();
void footerMenu();
void footer();
void footerCetering();
void headerService();
void footerServices();
void footerOptionList();
void footerAdminOptions();
void headerViewOrders();
void footerViewOrdersDetails();
void footeroptionNavigation();

//##########################################################

//Definition of user roles in enumeration
enum UserRole {
	Administrator = 'A',
	Customer = 'M',
	Guest = 'G'
};

//void GetFileName();
//void GetExeFileName();
//FILE *menu;

//Gobal Variables
//##########################################################

//Window Handlers & Buffers
CONSOLE_SCREEN_BUFFER_INFO csbi;
HWND hConx = GetConsoleWindow();
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//Registered Users
string users[10][4];

//For Food Menu List
string menuList[100][2];
string cateringList[100][2];

//Order lists
string foodOrders[100][2];
string cateringOrders[100][2];

string username = "";
string password = "";
string currentRole = "";

int menuIndex = 0;

bool appExit = false;

int keyState = 0;
SHORT KEY_DOWN = 0x8000;  // 'If the most significant bit is set, the key is down
SHORT KEY_PRESSED = 0x1;
//##########################################################


int main(void)
{

	setConsoleSize(80, 25);

	home();
}

//Home menu navigation
void home()
{

	menuIndex = 0;

	while (true)
	{
		setColor(2);
		setColor(BASECOLOR);
		clear();

		string appName = "BEST FOOD INFORMATION SYSTEM";
		string loginbtn = "CUSTOMER LOGIN";
		string guestbtn = "GUEST";

		gotoxy(getCenterCoordinatesForText(appName), 2);
		cout << appName << endl;

		gotoxy(getCenterCoordinatesForText(loginbtn + " || " + guestbtn), 5);

		//Updating Screen Text State Based On Key Selection
		if (menuIndex == 0)
		{
			setColor(INVERTED_BASECOLOR);
			cout << loginbtn;
			setColor(BASECOLOR);
			cout << " || ";
			cout << guestbtn << endl;
		}
		else if (menuIndex == 1)
		{
			setColor(BASECOLOR);
			cout << loginbtn;
			cout << " || ";
			setColor(INVERTED_BASECOLOR);
			cout << guestbtn << endl;
		}

		//Getting Key Events
		while (true)
		{
			WORD Check = GetAsyncKeyState(VK_RETURN);
			if (GetAsyncKeyState(VK_RIGHT) != 0)
			{
				menuIndex += 1;
				if (menuIndex == 2)
				{
					menuIndex = 0;
				}
				break;
			}
			else if (GetAsyncKeyState(VK_LEFT) != 0)
			{
				menuIndex -= 1;
				if (menuIndex == -1)
				{
					menuIndex = 1;
				}
				break;
			}
			else if ((Check & 0x8000) == 0x8000 && keyState == 0)
			{
				keyState = 1;
				switch (menuIndex)
				{
					case 0:
					{
						customerLogin();
						break;
					}

					case 1:
					{
						guestLogin();
						break;
					}
				}
			}

			if (Check == 0)
			{
				keyState = 0;
			}
			//Sleep(200);
		}

		Sleep(400);
	}
}



/* Authentication Section for Guest, Registered Members and for Administrators
   This would read the database to match the credentials with input username and password.
   Opens up appropriate navigation for Admin, Registered Members, Guests.
*/
//##########################################################
void guestLogin()
{
	setColor(BASECOLOR);
	clear();

	username = "";

	string screen_lable = "GUEST LOGIN";
	gotoxy(getCenterCoordinatesForText(screen_lable), 2);
	cout << screen_lable << endl;

	gotoxy(getCenterCoordinatesForText(screen_lable), 3);
	for (int i = 0; i < screen_lable.length(); i++)
	{
		cout << "=";
	}


	gstnamelbl:
	screen_lable = "PLEASE ENTER YOUR NAME (as a guest):";
	gotoxy(getCenterCoordinatesForText(screen_lable), 5);
	cout << screen_lable;
	
	gotoxy(getCenterCoordinatesForText(screen_lable), 7);

	
	cin.clear();
	getline(std::cin, username);

	if (username == "")
	{
		goto gstnamelbl;
	}

	if (username.length() != 0)
	{
		currentRole = Guest;
		CustomerOptions();
	}

}


//Customer Login Section
void customerLogin()
{

	username = "";
	password = "";
	
	cin.ignore(INT_MAX, '\n');

	setColor(BASECOLOR);
	clear();

	string screen_lable = "CUSTOMER LOGIN";

	gotoxy(getCenterCoordinatesForText(screen_lable), 2);

	cout << screen_lable << endl;

	gotoxy(getCenterCoordinatesForText(screen_lable), 3);

	for (int i = 0; i < screen_lable.length(); i++)
	{
		cout << "=";
	}

	userlbl:
	screen_lable = "PLEASE ENTER YOUR NAME: ";
	gotoxy(getCenterCoordinatesForText(screen_lable), 5);
	cout << screen_lable;
		
	gotoxy(getCenterCoordinatesForText(screen_lable), 7);
	//cin >> username;
	getline(std::cin, username);

	if (username == "")
	{
		goto userlbl;
	}


	passlbl:
	screen_lable = "PLEASE ENTER YOUR PASSWORD: ";
	gotoxy(getCenterCoordinatesForText(screen_lable), 9);
	cout << screen_lable;

	
	gotoxy(getCenterCoordinatesForText(screen_lable), 11);
	//cin >> password;
	getline(std::cin, password);


	if (password == "")
	{
		goto passlbl;
	}

	//Reading Users from Database
	ifstream usr(USERS_DB);
	string userAttrLine;
	int lineIndex = 0;

	if (usr.is_open())
	{
		while (getline(usr, userAttrLine))
		{
			vector<string> vline = splitString(userAttrLine, ";");

			users[lineIndex][0] = userAttrLine;
			for (int i = 1; i <= vline.size(); i++)
			{
				users[lineIndex][i] = vline[(i - 1)];
			}

			lineIndex += 1;
		}
		usr.close();
	}


	//This repetitive algorithm checks for correct username & password
	for (int i = 0; i <= 10 ; i++)
	{
		if (users[i][2] == username && users[i][3] == password)
		{
			if (users[i][1] == "A")
			{
				currentRole = Administrator;
				AdminOptions();
			}
			else
			{
				currentRole = Customer;
				CustomerOptions();
			}

		}
		
		// In case of failure, It repeats again with the login insertion option
		if (i == 10)
		{

			setColor(BASECOLOR);
			clear();

			screen_lable = "CUSTOMER LOGIN";
			gotoxy(getCenterCoordinatesForText(screen_lable), 2);
			cout << screen_lable;

			screen_lable = "Wrong Username & Password";
			gotoxy(getCenterCoordinatesForText(screen_lable), 10);
			cout << screen_lable;

			Sleep(2000);
			customerLogin();

		}
	}



	while (true)
	{

		if (GetAsyncKeyState(VK_ESCAPE) != 0)
		{
			home();
			break;
		}

	}
}

//##########################################################
#pragma region Navigations

/* 
	
	Registered customer option navigation panel unit that shows available options to all registered users 
	This option may change the available options for the user depends upon the user role

*/

void CustomerOptions()
{
	menuIndex = 0;

	while (true)
	{
		setColor(BASECOLOR);
		clear();

		string menuBtn = "MENUS";
		string catering = "CATERING";
		string services = "SERVICES";
		string helpBtn = "HELP";
		string logoutBtn = "LOGOUT";

		footeroptionNavigation();

		if (currentRole == "G")
		{
			AppHeader("GUEST NAME : " + username);
		}
		else
		{
			AppHeader("MEMBER NAME : " + username);
		}

		setColor(24);
		
		string lblMessage1 = "Please proceed meal Orders or Catering orders from above options";
		string lblMessage2 = "And by pressing 'ESC' return back here to save.";
		string lblMessage3 = "To save orders & catering, Press 'C'";

		gotoxy(getCenterCoordinatesForText(lblMessage1), 10);
		cout << lblMessage1;
		gotoxy(getCenterCoordinatesForText(lblMessage2), 11);
		cout << lblMessage2;
		gotoxy(getCenterCoordinatesForText(lblMessage3), 12);
		cout << lblMessage3;

		gotoxy(3, 6);
		
		setColor(BASECOLOR);
		//Updating Screen Text State Based On Key Selection
		if (menuIndex == 0)
		{
			setColor(INVERTED_BASECOLOR);
			cout << menuBtn;
			setColor(BASECOLOR);
			cout << " || ";
			cout << catering;
			cout << " || ";

			//This will be ignored when user role guest activated, 
			//So that services will not popup to guest role users
			if (currentRole != "G")
			{
				cout << services;
				cout << " || ";
			}

			cout << helpBtn;
			cout << " || ";
			cout << logoutBtn;
		}
		else if (menuIndex == 1)
		{
			setColor(BASECOLOR);
			cout << menuBtn;
			cout << " || ";
			setColor(INVERTED_BASECOLOR);
			cout << catering;
			setColor(BASECOLOR);
			cout << " || ";

			//This will be ignored when user role guest activated, 
			//So that services will not popup to guest role users
			if (currentRole != "G")
			{
				cout << services;
				cout << " || ";
			}

			cout << helpBtn;
			cout << " || ";
			cout << logoutBtn;
		}
		//This will be ignored when user role guest activated, 
		//So that services will not popup to guest role users
		else if (menuIndex == 2 && currentRole != "G")
		{
			setColor(BASECOLOR);
			cout << menuBtn;
			cout << " || ";
			cout << catering;
			cout << " || ";
			setColor(INVERTED_BASECOLOR);
			cout << services;
			setColor(BASECOLOR);
			cout << " || ";
			cout << helpBtn;
			cout << " || ";
			cout << logoutBtn;
		}
		else if (menuIndex == 3)
		{
			setColor(BASECOLOR);
			cout << menuBtn;
			cout << " || ";
			cout << catering;
			cout << " || ";

			//This will be ignored when user role guest activated, 
			//So that services will not popup to guest role users
			if (currentRole != "G")
			{
				cout << services;
				cout << " || ";
			}

			setColor(INVERTED_BASECOLOR);
			cout << helpBtn;
			setColor(BASECOLOR);
			cout << " || ";
			cout << logoutBtn;
		}
		else if (menuIndex == 4)
		{
			setColor(BASECOLOR);
			cout << menuBtn;
			cout << " || ";
			cout << catering;
			cout << " || ";

			//This will be ignored when user role guest activated, 
			//So that services will not popup to guest role users
			if (currentRole != "G")
			{
				cout << services;
				cout << " || ";
			}

			cout << helpBtn;
			cout << " || ";
			setColor(INVERTED_BASECOLOR);
			cout << logoutBtn;
		}

		//Getting Key Events
		while (true)
		{
			WORD CheckReturn = GetAsyncKeyState(VK_RETURN);
			WORD CheckKeyC= GetAsyncKeyState(67);

			if (GetAsyncKeyState(VK_RIGHT) != 0)
			{
				if (currentRole != "G")
				{
					menuIndex += 1;
				}else
				{
					if (menuIndex == 1)
					{
						menuIndex = 3;
					}
					else
					{
						menuIndex += 1;
					}
				}
				
				if (menuIndex == 5)
				{
					menuIndex = 4;
				}
				break;
			}
			else if (GetAsyncKeyState(VK_LEFT) != 0)
			{

				if (currentRole != "G")
				{
					menuIndex -= 1;
				}
				else
				{
					if (menuIndex == 3)
					{
						menuIndex = 1;
					}
					else
					{
						menuIndex -= 1;
					}
				}

				
				if (menuIndex == -1)
				{
					menuIndex = 0;
				}
				break;
			}
			//Saving Order to DB
			else if ((CheckKeyC & 0x8000) == 0x8000 && keyState == 0)
			{

				keyState = 1;
				setColor(I_BASECOLOR);

				string orderConfirmationLable = "ORDER HAS BEEN CONFIRMED & SENT!";
				gotoxy(getCenterCoordinatesForText(orderConfirmationLable), 14);
				cout << orderConfirmationLable << endl;

				orderConfirmationLable = "Thank You for visiting us :)";
				gotoxy(getCenterCoordinatesForText(orderConfirmationLable), 16);
				cout << orderConfirmationLable << endl;

				saveDB();

				Sleep(3000);

				break;
			}
			else if ((CheckReturn & 0x8000) == 0x8000 && keyState == 0)
			{
				keyState = 1;
				switch (menuIndex)
				{
				case 0:
				{
					FoodsNBeverages();
				} break;
				case 1:
				{
					Catering();
				} break;
				case 2:
				{
					Services();
				} break;
				case 3:
				{
					Help();
				} break;
				case 4:
				{
					Logoff();
				} break;
				}
			}

			if (CheckReturn == 0 || CheckKeyC == 0)
			{
				keyState = 0;
			}

			Sleep(200);
		}

		Sleep(200);

	}
}


/* 

	Admin option navigation panel unit that shows available options to all admins 
	This will be called only when admin user logs into the system

*/
void AdminOptions()
{
	menuIndex = 0;

	while (true)
	{
		setColor(BASECOLOR);
		clear();

		string createUserBtn = "CREATE USERS";
		string viewOrders = "VIEW ORDERS & REQUESTS";
		string logoutBtn = "LOGOUT";

		footerAdminOptions();
		AppHeader("Admin Options");
		gotoxy(3, 6);

		//Updating Screen Text State Based On Key Selection
		if (menuIndex == 0)
		{
			setColor(INVERTED_BASECOLOR);
			cout << createUserBtn;
			setColor(BASECOLOR);
			cout << " || ";
			cout << viewOrders;
			cout << " || ";
			cout << logoutBtn;
		}
		else if (menuIndex == 1)
		{
			setColor(BASECOLOR);
			cout << createUserBtn;
			cout << " || ";
			setColor(INVERTED_BASECOLOR);
			cout << viewOrders;
			setColor(BASECOLOR);
			cout << " || ";
			cout << logoutBtn;
		}

		else if (menuIndex == 2)
		{
			setColor(BASECOLOR);
			cout << createUserBtn;
			cout << " || ";
			cout << viewOrders;
			cout << " || ";
			setColor(INVERTED_BASECOLOR);
			cout << logoutBtn;
		}

		//Getting Key Events
		while (true)
		{
			WORD Check = GetAsyncKeyState(VK_RETURN);
			WORD CheckKeyC = GetAsyncKeyState(67);
			if (GetAsyncKeyState(VK_RIGHT) != 0)
			{
				menuIndex += 1;
				if (menuIndex == 3)
				{
					menuIndex = 2;
				}
				break;
			}
			else if (GetAsyncKeyState(VK_LEFT) != 0)
			{

				menuIndex -= 1;
				if (menuIndex == -1)
				{
					menuIndex = 0;
				}
				break;
			}
			//Confirming Order to DB
			else if ((CheckKeyC & 0x8000) == 0x8000 && keyState == 0)
			{

				keyState = 1;
				setColor(I_BASECOLOR);

				string orderConfirmationLable = "ORDER HAS BEEN CONFIRMED & UNDERTAKEN!";
				gotoxy(getCenterCoordinatesForText(orderConfirmationLable), 10);
				cout << orderConfirmationLable << endl;

				saveDB();

				Sleep(3000);

				break;
			}
			else if ((Check & 0x8000) == 0x8000 && keyState == 0)
			{
				keyState = 1;
				switch (menuIndex)
				{
				case 0:
				{
					CreateUser();
				} break;
				case 1:
				{
					ViewAllOrderLists();
				} break;
				case 2:
				{
					Logoff();
				} break;
				}
			}

			if (Check == 0 && CheckKeyC == 0)
			{
				keyState = 0;
			}

			Sleep(200);
		}

		Sleep(200);

	}
}
#pragma endregion

#pragma region Customer functions

//Foods & Beverage Ordering Section
void FoodsNBeverages()
{

	char NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	setColor(2);
	setConsoleSizeEx(80, 100, 80, 25);

	string line;
	int lineIndex = 1;
	ifstream fin(MENU_DB);

	if (fin.is_open())
	{
		while (getline(fin, line))
		{
			menuList[lineIndex][0] = line;
			menuList[lineIndex][1] = "0";
			lineIndex += 1;
		}
		fin.close();
	}

	menuIndex = 1;
	menuList[0][1] = "";

	int max_list_view = 12;
	int start_point = 1;
	int end_point = 12;
	int _print_Line = 8;

	while (true)
	{
		setColor(BASECOLOR);
		clear();

		AppHeader("Foods & Drinks");
		footerMenu();

		setColor(BASECOLOR);
		gotoxy(3, 6);
		cout << "Food Menus";
		gotoxy(73, 6);
		cout << "Qty.";
		gotoxy(3, 7);
		for (int i = 0; i < 73; i++)
		{
			cout << "_";
		}

		int selection_state = 0;
		int _print_Line = 9;
		//menuIndex = 0;

		for (int i = start_point; i <= end_point; i++)
		{
			gotoxy(5, _print_Line);

			if (i == menuIndex)
			{
				setColor(INVERTED_BASECOLOR);
				vector<string> vline = splitString(menuList[i][0], ";");
				cout << i << ". " << vline[0];
				gotoxy(73, _print_Line);
				if (menuList[i][1] != "0")
					cout << menuList[i][1];
			}
			else
			{
				setColor(BASECOLOR);
				vector<string> vline = splitString(menuList[i][0], ";");
				cout << i << ". " << vline[0];
				gotoxy(73, _print_Line);
				if (menuList[i][1] != "0")
					cout << menuList[i][1];
			}

			_print_Line += 1;

			if (i == end_point)
			{
				break;
			}
		}



		//Getting Key Events
		while (true)
		{
			WORD Check = GetAsyncKeyState(VK_RETURN);
			WORD CheckEscape = GetAsyncKeyState(VK_ESCAPE);
			if (GetAsyncKeyState(VK_UP) != 0)
			{
				menuIndex -= 1;
				if (menuIndex == 0)
				{
					menuIndex = 1;
				}

				if (start_point != 1)
				{
					start_point -= 1;
					end_point -= 1;
				}

				break;
			}
			else if (GetAsyncKeyState(VK_DOWN) != 0)
			{

				if (max_list_view <= menuIndex && menuIndex != lineIndex)
				{
					start_point += 1;
					end_point += 1;
				}

				if (menuIndex != lineIndex)
				{
					menuIndex += 1;
				}

				break;
			}
			else if ((CheckEscape & 0x8000) == 0x8000 && keyState == 0)
			{
				keyState = 1;
				CustomerOptions();
				break;
			}
			else if (GetAsyncKeyState(VK_ADD) != 0)
			{
				int no = stoi(menuList[menuIndex][1]);

				if (no < 100)
				{
					menuList[menuIndex][1] = to_string(no + 1);
				}
				break;
			}
			else if (GetAsyncKeyState(VK_SUBTRACT) != 0)
			{
				int no = stoi(menuList[menuIndex][1]);
				if (no > 0)
				{
					menuList[menuIndex][1] = to_string(no - 1);
				}
				break;
			}
			else if ((Check & 0x8000) == 0x8000 && keyState == 0)
			{
				keyState = 1;
				FoodsNBeveragesInformation(menuList[menuIndex][0]);
				break;
			}

			if (Check == 0)
			{
				keyState = 0;
			}
		}

		Sleep(200);

	}

}

//Displaying full information about Food & Beverage Information
void FoodsNBeveragesInformation(string information)
{
	int lineIndex = 1;

	setColor(BASECOLOR);
	clear();

	vector<string> vline = splitString(information, ";");
	string foodName = vline[0];
	string foodIngredients = vline[1];

	AppHeader("Food & Bevarage information");
	footer();

	gotoxy(3, 6);
	cout << "Item Name: " << foodName;

	setColor(BASECOLOR);
	gotoxy(3, 9);
	cout << foodIngredients;

	//Exiting Option
	while (true)
	{
		WORD Check = GetAsyncKeyState(VK_ESCAPE);
		if ((Check & 0x8000) == 0x8000 && keyState == 0)
		{
			keyState = 1;
			FoodsNBeverages();
			break;
		}
		if (Check == 0)
		{
			keyState = 0;
		}
	}

}

//Catering Request Section
void Catering()
{

	char NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	setColor(BASECOLOR);
	setConsoleSizeEx(80, 100, 80, 25);

	string line;
	int lineIndex = 0;
	ifstream fin(CATERING_DB);

	if (fin.is_open())
	{
		while (getline(fin, line))
		{
			cateringList[lineIndex][0] = line;
			cateringList[lineIndex][1] = "";
			lineIndex += 1;
		}
		fin.close();
	}

	menuIndex = 1;
	cateringList[0][1] = "";

	int max_list_view = 12;
	int start_point = 1;
	int end_point = 12;
	int _print_Line = 8;

	while (true)
	{
		setColor(BASECOLOR);
		clear();

		AppHeader("Catering Services");
		footerCetering();

		setColor(BASECOLOR);
		gotoxy(3, 6);
		cout << "Services";
		gotoxy(70, 6);
		cout << "Status";
		gotoxy(3, 7);
		for (int i = 0; i < 73; i++)
		{
			cout << "_";
		}

		int selection_state = 0;
		int _print_Line = 9;
		//menuIndex = 0;

		for (int i = start_point; i <= end_point; i++)
		{
			gotoxy(5, _print_Line);

			if (i == menuIndex)
			{
				setColor(INVERTED_BASECOLOR);
				cout << i << ". " << cateringList[i][0];
				gotoxy(64, _print_Line);
				cout << cateringList[i][1];
			}
			else
			{
				setColor(BASECOLOR);
				cout << i << ". " << cateringList[i][0];
				gotoxy(64, _print_Line);
				cout << cateringList[i][1];
			}

			_print_Line += 1;

			if (i == end_point)
			{
				break;
			}
		}



		//Getting Key Events
		while (true)
		{
			if (GetAsyncKeyState(VK_UP) != 0)
			{
				menuIndex -= 1;
				if (menuIndex == 0)
				{
					menuIndex = 1;
				}

				if (start_point != 1)
				{
					start_point -= 1;
					end_point -= 1;
				}

				break;
			}
			else if (GetAsyncKeyState(VK_DOWN) != 0)
			{

				if (max_list_view <= menuIndex && menuIndex != lineIndex)
				{
					start_point += 1;
					end_point += 1;
				}

				if (menuIndex != lineIndex)
				{
					menuIndex += 1;
				}

				break;
			}
			else if (GetAsyncKeyState(VK_ESCAPE) != 0)
			{
				CustomerOptions();
				break;
			}
			else if (GetAsyncKeyState(VK_ADD) != 0)
			{

				cateringList[menuIndex][1] = "REQUEST SENT";

				break;
			}
			else if (GetAsyncKeyState(VK_SUBTRACT) != 0)
			{

				cateringList[menuIndex][1] = "";

				break;
			}
		}

		Sleep(200);

	}

}

//Services & Special Offers Section
void Services()
{

	char NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	setColor(BASECOLOR);
	setConsoleSizeEx(80, 100, 80, 25);

	string menuList[100][2];
	string line;
	int lineIndex = 0;
	ifstream fin(SERVICES_DB);

	if (fin.is_open())
	{
		while (getline(fin, line))
		{
			menuList[lineIndex][0] = line;
			menuList[lineIndex][1] = "";
			lineIndex += 1;
		}
		fin.close();
	}

	menuIndex = 1;
	menuList[0][1] = "";

	int max_list_view = 12;
	int start_point = 1;
	int end_point = 12;
	int _print_Line = 8;

	while (true)
	{
		setColor(BASECOLOR);
		clear();

		AppHeader("Services");
		footerServices();

		setColor(BASECOLOR);
		gotoxy(3, 6);
		cout << "Special Offers";
		/*gotoxy(70, 6);
		cout << "";*/
		gotoxy(3, 7);
		for (int i = 0; i < 73; i++)
		{
			cout << "_";
		}

		int selection_state = 0;
		int _print_Line = 9;
		//menuIndex = 0;

		for (int i = start_point; i <= end_point; i++)
		{
			gotoxy(5, _print_Line);

			if (i == menuIndex)
			{
				setColor(INVERTED_BASECOLOR);
				cout << i << ". " << menuList[i][0];
				/*gotoxy(64, _print_Line);
				cout << menuList[i][1];*/
			}
			else
			{
				setColor(BASECOLOR);
				cout << i << ". " << menuList[i][0];
				/*gotoxy(64, _print_Line);
				cout << menuList[i][1];*/
			}

			_print_Line += 1;

			if (i == end_point)
			{
				break;
			}
		}



		//Getting Key Events
		while (true)
		{
			if (GetAsyncKeyState(VK_UP) != 0)
			{
				menuIndex -= 1;
				if (menuIndex == 0)
				{
					menuIndex = 1;
				}

				if (start_point != 1)
				{
					start_point -= 1;
					end_point -= 1;
				}

				break;
			}
			else if (GetAsyncKeyState(VK_DOWN) != 0)
			{

				if (max_list_view <= menuIndex && menuIndex != lineIndex)
				{
					start_point += 1;
					end_point += 1;
				}

				if (menuIndex != lineIndex)
				{
					menuIndex += 1;
				}

				break;
			}
			else if (GetAsyncKeyState(VK_ESCAPE) != 0)
			{
				CustomerOptions();
				break;
			}
		}

		Sleep(200);

	}

}
#pragma endregion

#pragma region Admin functions

//View all food & beverage orders and catering requests
void ViewAllOrderLists()
{
	headerViewOrders();

	string ordersAndRequestsLine;
	string OrdersAndRequestDB[100][6];
	string FoodOrders[100];
	string requests[100];
	int lineIndex = 1;

	ifstream fin(ORDERLIST_DB);

	if (fin.is_open())
	{
		while (getline(fin, ordersAndRequestsLine))
		{
			vector<string> vline = splitString(ordersAndRequestsLine, ";");
			
			OrdersAndRequestDB[lineIndex][0] = ordersAndRequestsLine;
			for (int i = 1; i <= vline.size(); i++)
			{
				OrdersAndRequestDB[lineIndex][i] = vline[(i-1)];
			}
			
			lineIndex += 1;
		}
		fin.close();
	}

	setColor(2);

	string line;

	menuIndex = 1;
	menuList[0][1] = "";

	int max_list_view = 12;
	int start_point = 1;
	int end_point = 12;
	int _print_Line = 8;

	while (true)
	{
		setColor(BASECOLOR);
		clear();

		headerViewOrders();
		
		setColor(BASECOLOR);
		gotoxy(3, 6);
		cout << "Orders of members";
		gotoxy(40, 6);
		cout << "Date & Time";
		gotoxy(63, 6);
		cout << "Customer Type";

		gotoxy(3, 7);
		for (int i = 0; i < 73; i++)
		{
			cout << "_";
		}

		int selection_state = 0;
		int _print_Line = 9;
		//menuIndex = 0;

		for (int i = start_point; i <= end_point; i++)
		{

			if (i == menuIndex)
			{
				setColor(INVERTED_BASECOLOR);
				gotoxy(5, _print_Line);
				cout << i << "." << OrdersAndRequestDB[i][1];
				gotoxy(40, _print_Line);
				cout << OrdersAndRequestDB[i][3];
				gotoxy(63, _print_Line);
				cout << (OrdersAndRequestDB[i][2] == "G" ? "Guest" : OrdersAndRequestDB[i][2] == "M" ? "Member" : "");
	
			}
			else
			{
				setColor(BASECOLOR);
				gotoxy(5, _print_Line);
				cout << i << "." << OrdersAndRequestDB[i][1];
				gotoxy(40, _print_Line);
				cout << OrdersAndRequestDB[i][3];
				gotoxy(63, _print_Line);
				cout << (OrdersAndRequestDB[i][2] == "G" ? "Guest" : OrdersAndRequestDB[i][2] == "M" ? "Member" : "");
			}

			_print_Line += 1;

			if (i == end_point)
			{
				break;
			}
		}



		//Getting Key Events
		while (true)
		{
			WORD Check = GetAsyncKeyState(VK_RETURN);
			if (GetAsyncKeyState(VK_UP) != 0)
			{
				menuIndex -= 1;
				if (menuIndex == 0)
				{
					menuIndex = 1;
				}

				if (start_point != 1)
				{
					start_point -= 1;
					end_point -= 1;
				}

				break;
			}
			else if (GetAsyncKeyState(VK_DOWN) != 0)
			{

				if (max_list_view <= menuIndex && menuIndex != lineIndex)
				{
					start_point += 1;
					end_point += 1;
				}

				if (menuIndex != lineIndex)
				{
					menuIndex += 1;
				}

				break;
			}
			else if (GetAsyncKeyState(VK_ESCAPE) != 0)
			{
				AdminOptions();
				break;
			}
			else if ((Check & 0x8000) == 0x8000 && keyState == 0)
			{

				keyState = 1;

				vector<string> foodLine = splitString(OrdersAndRequestDB[menuIndex][4], ",");
				for (int i = 0; i < foodLine.size(); i++)
				{
					vector<string> xline = splitString(foodLine[i], ":");
					foodOrders[i][0] = xline[0];
					foodOrders[i][1] = xline[1];
				}

				vector<string> cateringLine = splitString(OrdersAndRequestDB[menuIndex][5], ",");
				for (int i = 0; i < cateringLine.size(); i++)
				{
					vector<string> xline = splitString(cateringLine[i], ":");
					cateringOrders[i][0] = xline[0];
					cateringOrders[i][1] = xline[1];
				}

				OrderDetail(OrdersAndRequestDB[menuIndex][0]);
				break;
			}

			if (Check == 0)
			{
				keyState = 0;
			}

		
		}

		Sleep(200);

	}

}

void OrderDetail(string orderLine)
{
	int lineIndex = 1;

	setColor(BASECOLOR);

	string line;

	menuIndex = 1;
	menuList[0][1] = "";

	int max_list_view = 12;
	int start_point = 1;
	int end_point = 12;
	int _print_Line = 8;

	//while (true)
	{
		setColor(BASECOLOR);
		clear();

		headerViewOrders();
		footerViewOrdersDetails();

		setColor(BASECOLOR);
		gotoxy(3, 6);
		cout << "Foods & Beverages / Requests";
		gotoxy(65, 6);
		cout << "Qty./Status";

		gotoxy(3, 7);
		for (int i = 0; i < 73; i++)
		{
			cout << "_";
		}

		int selection_state = 0;
		int _print_Line = 9;
		//menuIndex = 0;

		setColor(BASECOLOR);
		for (int i = 0; i < 100; i++)
		{
			if (foodOrders[i][0] != "")
			{
				gotoxy(5, _print_Line);
				cout << (i+1) << "." << foodOrders[i][0];
				gotoxy(70, _print_Line);
				cout << foodOrders[i][1];
				_print_Line++;
			}
			
		}

		while (true)
		{
			if (GetAsyncKeyState(VK_ESCAPE) != 0)
			{
				ViewAllOrderLists();
				break;
			}
			else if (GetAsyncKeyState(67) != 0)
			{
				deleteAndUpdateOrders(orderLine);

				setColor(I_BASECOLOR);
				string screen_lable = "Order has been acknowledged !";
				gotoxy(getCenterCoordinatesForText(screen_lable), 13);
				cout << screen_lable;
				Sleep(2000);

				ViewAllOrderLists();

				break;
			}

			Sleep(200);

		}

	}
}


//User Creation Section
void CreateUser()
{
	menuIndex = 0;

	while (true)
	{
		setColor(BASECOLOR);
		clear();

		string appName = "CREATE USER ACCOUNT";
		string adminBtn = "Admin";
		string MemberBtn = "Member";
		string username = "";
		string password = "";

		gotoxy(getCenterCoordinatesForText(appName), 2);
		cout << appName << endl;

		gotoxy(getCenterCoordinatesForText(adminBtn + " || " + MemberBtn), 5);

		//Updating Screen Text State Based On Key Selection
		if (menuIndex == 0)
		{
			setColor(INVERTED_BASECOLOR);
			cout << adminBtn;
			setColor(BASECOLOR);
			cout << " || ";
			cout << MemberBtn << endl;
		}
		else if (menuIndex == 1)
		{
			setColor(BASECOLOR);
			cout << adminBtn;
			cout << " || ";
			setColor(INVERTED_BASECOLOR);
			cout << MemberBtn << endl;
		}

		//Getting Key Events
		while (true)
		{
			WORD Check = GetAsyncKeyState(VK_RETURN);
			if (GetAsyncKeyState(VK_RIGHT) != 0)
			{
				menuIndex += 1;
				if (menuIndex == 2)
				{
					menuIndex = 0;
				}
				break;
			}
			else if (GetAsyncKeyState(VK_LEFT) != 0)
			{
				menuIndex -= 1;
				if (menuIndex == -1)
				{
					menuIndex = 1;
				}
				break;
			}
			else if ((Check & 0x8000) == 0x8000 && keyState == 0)
			{
				keyState = 1;

				string _username;
				string _password;

				_usrlbl:
				string screen_lable = "USERNAME:";
				gotoxy(20, 7);
				cout << screen_lable << endl;
				gotoxy(getNextCoordinatesForText(screen_lable, 20)+1, 7);
				getline(std::cin, _username);

				if (_username == "")
				{
					goto _usrlbl;
				}

				_passlbl:
				screen_lable = "PASSWORD:";
				gotoxy(20, 9);
				cout << screen_lable << endl;
				gotoxy(getNextCoordinatesForText(screen_lable, 20)+1, 9);
				getline(std::cin, _password);

				if (_password == "")
				{
					goto _passlbl;
				}

				string userType;

				if (menuIndex == 0) 
				{
					userType = Administrator;
				}
				else
				{
					userType = Customer;
				}

				string credentials = userType + ";" + _username + ";" + _password;
						
				ofstream usrFile;
				usrFile.open(USERS_DB, ios::app);
				usrFile << credentials;
				usrFile << "\n";
				usrFile.close();

				setColor(I_BASECOLOR);
				screen_lable = "User account has been created !";
				gotoxy(getCenterCoordinatesForText(screen_lable), 13);
				cout << screen_lable;
				Sleep(2000);

				AdminOptions();
				break;

			}

			if (Check == 0)
			{
				keyState = 0;
			}

		}

		Sleep(400);
	}
}

#pragma endregion

#pragma region Common Functions
//All Common Functions 

void deleteAndUpdateOrders(string orderID)
{

	string readingLine;
	ifstream fin(ORDERLIST_DB);
	ofstream tfile(TEMP_ORDERLIST_DB, ios::app);

	if (fin.is_open())
	{
		while (getline(fin, readingLine))
		{

			if (readingLine != orderID)
			{
				tfile << readingLine;
				tfile << "\n";
			}

		}

		
	}

	fin.close();
	tfile.close();

	remove(ORDERLIST_DB);
	rename(TEMP_ORDERLIST_DB, ORDERLIST_DB);

}

void AppHeader(string menuName)
{
	
	setColor(BASECOLOR);
	string screen_lable = "||| BEST FOOD INFOMATION SYSTEM |||";
	gotoxy(3, 1);
	cout << screen_lable;

	gotoxy(3, 3);
	cout << menuName;

	setColor(I_BASECOLOR);
	gotoxy(3, 4);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}

	gotoxy(3, 7);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}

}


//Displaying Help
void Help()
{

	setColor(BASECOLOR);
	clear();

	AppHeader("Help - Navigation of Options");

	gotoxy(3, 9);
	cout << "Selection ""Go Up"" : Press Up Arrow";

	gotoxy(3, 10);
	cout << "Selection ""Go Down"" : Press Down Arrow";

	gotoxy(3, 11);
	cout << "Menu Navigation - Go Left : Press Left Arrow";

	gotoxy(3, 12);
	cout << "Menu Navigation - Go Right : Press Right Arrow";

	gotoxy(3, 13);
	cout << "Add Qty : +";

	gotoxy(3, 14);
	cout << "Remove Qty : -";

	gotoxy(3, 15);
	cout << "Confirm Order / Aknowledge Order : C";

	gotoxy(3, 16);
	cout << "Go Back : 'ESC'";

	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Go Back : Press 'ESC'";

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE) != 0)
		{
			CustomerOptions();
			break;
		}
	}
}

void headerMenu()
{
	setColor(30);
	gotoxy(3, 1);
	cout << "Best Food Information System";
	gotoxy(3, 2);
	cout << "Menus and Beverages";
}

void footerMenu()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Go Back : Press 'ESC' || Add Qty : Press +  ||  Remove Qty : Press - || Menu Details : Press 'ENTER'";
}

void headerService()
{
	setColor(30);
	gotoxy(3, 1);
	cout << "Best Food Information System";
	gotoxy(3, 2);
	cout << "Services";
}

void footerCetering()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Go Back : Press 'ESC' || Send Inquery : Press +  ||  Undo Inquery : Press - ";
	gotoxy(3, 23);
	setColor(24);
	cout << "*Upon request, Best Foot staff will contact you within 24h";
}

void footerServices()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Go Back : Press 'ESC'";
	gotoxy(3, 23);
	setColor(24);
	cout << "*All offers apply only to registered Customers";
}

void footer()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Go Back : Press 'ESC'";
	//gotoxy(3, 23);
	//setColor(24);
	//cout << "*All offers apply only to registered Customers";
}

void headerViewOrders()
{
	setColor(30);
	gotoxy(3, 1);
	cout << "Best Food Information System";
	gotoxy(3, 2);
	cout << "Food Orders & Catering Requests";
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
}

void footerViewOrdersDetails()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Acknowledge Order List : Press 'C'";
}

void footerOptionList()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Confirm Order : Press 'C'";
}

void footerAdminOptions()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
}

void footeroptionNavigation()
{
	setColor(BASECOLOR);
	gotoxy(3, 21);
	for (int i = 0; i < 73; i++)
	{
		cout << "_";
	}
	gotoxy(3, 22);
	cout << "Go Right : Press Right Arrow || Go Left : Press Left Arrow";
}


void Logoff()
{
	string label = "Successfully logged off !";
	setColor(BASECOLOR);
	clear();
	gotoxy(getCenterCoordinatesForText(label), 4);
	cout << label;
	Sleep(2000);
	home();
}

void clear()
{
	system("cls");
}

void setColor(int color)
{
	SetConsoleTextAttribute(hConsole, color);
}

//Gets right location x from the text
int getCenterCoordinatesForText(string text)
{
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	COORD currentConsoleSize;
	currentConsoleSize.X = csbi.srWindow.Right;
	currentConsoleSize.Y = csbi.srWindow.Bottom;
	return (currentConsoleSize.X - text.length()) / 2;
}

int getNextCoordinatesForText(string text, int start)
{
	return start + text.length();
}

void gotoxy(int x, int y)
{
	COORD cursorLocation;
	cursorLocation.X = x;
	cursorLocation.Y = y;
	SetConsoleCursorPosition(hConsole, cursorLocation);
}

void setConsoleSize(int width, int height)
{

	COORD coord;
	coord.X = width;
	coord.Y = height;

	SMALL_RECT rect;
	rect.Top = 0;
	rect.Right = width - 1;
	rect.Bottom = height - 1;
	rect.Left = 0;

	SetConsoleWindowInfo(hConsole, TRUE, &rect);
	SetConsoleScreenBufferSize(hConsole, coord);
}

void setConsoleSizeEx(int width, int height, int xBuffer, int yBuffer)
{

	COORD coord;
	coord.X = width;
	coord.Y = height;

	SMALL_RECT rect;
	rect.Top = 0;
	rect.Right = xBuffer - 1;
	rect.Bottom = yBuffer - 1;
	rect.Left = 0;

	SetConsoleWindowInfo(hConsole, TRUE, &rect);
	SetConsoleScreenBufferSize(hConsole, coord);

}



void saveDB()//Append Data
{
	ofstream file;
	file.open(ORDERLIST_DB, ios::app);

	SYSTEMTIME lt;
	GetLocalTime(&lt);

	string timestamp = to_string(lt.wYear) + "/" + to_string(lt.wMonth) + "/" + to_string(lt.wDay) + " " + to_string(lt.wHour) + ":" + to_string(lt.wMinute);
	string foodOrder = "";
	string cateringRequest = "";

	//Preparing ordered foods & beverage data
	for (int y = 0; y < 100; y++)
	{
		vector<string> vline = splitString(menuList[y][0], ";");
		if (vline.size() > 0)
		{
			string itemName = vline[0];
			string itemQty = menuList[y][1];
			if (itemName != "" && itemQty != "0")
			{
				foodOrder = foodOrder + itemName;
				foodOrder = foodOrder + ":" + itemQty + ",";
			}
		}
	}

	//Preparing catering request data
	for (int y = 0; y < 100; y++)
	{
		if (cateringList[y][1] != "")
		{
			cateringRequest = cateringRequest + cateringList[y][0];
			cateringRequest = cateringRequest + ":" + cateringList[y][1] + ",";
		}
	}

	string line = username + ";" + currentRole + ";" + timestamp + ";" + foodOrder + ";" + cateringRequest + "";
	file << line + "\n";

	file.close();
}

//Delimitter Function for semicolon seperated data
vector<string> splitString(string input, string delimiter)
{
	vector<string> output;
	size_t start = 0;
	size_t end = 0;

	while (start != string::npos && end != string::npos)
	{
		start = input.find_first_not_of(delimiter, end);

		if (start != string::npos)
		{
			end = input.find_first_of(delimiter, start);

			if (end != string::npos)
			{
				output.push_back(input.substr(start, end - start));
			}
			else
			{
				output.push_back(input.substr(start));
			}
		}
	}

	return output;

}

#pragma endregion 


