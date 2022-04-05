#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Auditorium.h"
#include "Node.h"
#include "Seat.h"
#include <unordered_map>
#include <string>
using namespace std;

void GetUsers(unordered_map<string, vector<string> > &hashtable); //Gets usernames and passwords from user file
void CreateTheater(int &totalC, int &totalR, int &theaterNum); //Takes in data for all three of the theaters to create them
void StartingPoint(unordered_map<string, vector<string> > &hashtable, Auditorium &a1, Auditorium &a2, Auditorium &a3); //Login Menu
void MainMenu(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3); //Main Menu for user to choose actions
void Reserve(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3, int &bA, int &bC, int &bS, int &bT, bool offer, bool newOrder, int orderNum, int theaterNum); //Method to reserve seats
void Purchase(int &bought_a, int &bought_s, int &bought_c, int &bought_t, int &rowWant, char &colWant, int totalR, int totalC); //Prompts user for inputs to purchase tickets
void ViewOrder(unordered_map<string, vector<string> > &hashtable, string username); //Allows user to view the orders made so far
void DisplayReceipt(unordered_map<string, vector<string> > &hashtable, string username); //Displays the user's current receipt
void UpdateOrder(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3, int &bA, int &bC, int &bS, int &bT); //Allows user to update a specific order
void AddTickets(unordered_map<string, vector<string> > &hashtable, string username, int &bA, int &bC, int &bS, int &bT, int begRow, char begCol, int orderNum);
void DeleteTickets(unordered_map<string, vector<string> > &hashtable, string username, int orderNum, int rowWant, char colWant, int theaterNum, Auditorium &a1, Auditorium &a2, Auditorium &a3, string selected); //User selects one seat and unreserves it
void AdminMainMenu(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3); //The main menu for the admin
void CancelOrder(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3, int orderNum); //Deletes all of a user's orders
int main()
{
   int c1 = 0; //total columns of theater 1
   int r1 = 0; //total rows of theater 1
   int c2 = 0; //total columns of theater 2
   int r2 = 0; //total rows of theater 2
   int c3 = 0; //total columns of theater 3
   int r3 = 0; //total rows of theater 3
   string file1 = "A1.txt";
   string file2 = "A2.txt";
   string file3 = "A3.txt";
   int theaterNum = 1;

   //creates the three auditoriums
   CreateTheater(c1, r1, theaterNum);
   Auditorium a1(file1, c1, r1);
   a1.setRows(r1);
   a1.setCols(c1);

   CreateTheater(c2, r2, theaterNum);
   Auditorium a2(file2, c2, r2);
   a2.setRows(r2);
   a2.setCols(c2);

   CreateTheater(c3, r3, theaterNum);
   Auditorium a3(file3, c3, r3);
   a3.setRows(r3);
   a3.setCols(c3);

   unordered_map<string, vector<string> > hashtable;
   GetUsers(hashtable);
   //for(auto x : hashtable) //remove later, tests if user and password have been inputted
   // cout << x.first << " " << (x.second).at(0) << endl;
   StartingPoint(hashtable, a1, a2, a3);

   return 0;
}

void GetUsers(unordered_map<string, vector<string> > &hashtable)
{
   ifstream inFS;
   string line;
   inFS.open("userdb.dat"); //opens the user file
   while(inFS.good()) //while not at the end of the file
   {
      getline(inFS, line);
      string str = ""; //will be temporary substring variable
      string username = "";
      string password = "";
      char letter;

      for(unsigned long x = 0; x <= line.length(); x++) //goes through each individual character of the line
      {
         letter = line[x]; //grabs a letter from the line
         if(letter == ' ') //if encounters a space then store first substring as username
         {
            username = str;
            str = ""; //resets str
         }
         else if(x == line.length()) { password = str; } //at the end of the line, second substring is password
         else { str = str + letter; } //adds a letter to the substring
      }

      vector<string> data;
      data.push_back(password); //adds password to top of the vector
      hashtable[username] = data; //adds components to hashmap
   }
   inFS.close(); //closes the file

}
void CreateTheater(int &totalC, int &totalR, int &theaterNum)
{
   ifstream inFS;
   string file = "";
   if(theaterNum == 1) { file = "A1.txt"; } //based on theaterNum, choose which auditorium to build
   else if(theaterNum == 2) { file = "A2.txt"; }
   else{ file = "A3.txt"; }
   inFS.open(file); //opens the file
   while(inFS.good())
   {
      string str = "";
      getline(inFS, str); //str is a line from the text file
      if(str.length() > 0)
      {
         totalC = (int)str.length(); //total columns is the length of the line (one for each char)
         totalR++; //total number of rows go up if there is a line to "get"
      }
   }
   inFS.close(); //closest he file
   theaterNum++; //goes to next theater

}
void StartingPoint(unordered_map<string, vector<string> > &hashtable, Auditorium &a1, Auditorium &a2, Auditorium &a3)
{
   string username;
   string password;
   bool valPassword = true;
   int incorrect = 0;
   //Prompts user for a login
   cout << "Enter in username" << endl;
   cin >> username;
   if(username == "admin") //for admin main menu
   {
      while(valPassword)
      {
         cout << "Enter in the password" << endl;
         cin >> password;
         if(hashtable[username].at(0) == password) //if password matches
         {
            incorrect = 0;
            valPassword = false;
            AdminMainMenu(hashtable, username, a1, a2, a3); //Go to admin main menu
         }
         else //if fail return to login screen
         {
            incorrect++;
            if(incorrect == 3)
            {
               cout << "Please try relogging." << endl;
               valPassword = false;
               StartingPoint(hashtable, a1, a2, a3);
            }
            cout << "Invalid password. Try again:" << endl;
            cout << "Incorrect tries: " << incorrect << " out of 3." << endl << endl;
         }

      }
   }
   else //for regular users
   {
      while(valPassword)
      {
         cout << "Enter in the password" << endl;
         cin >> password;
         if(hashtable[username].at(0) == password) //if password matches
         {
            incorrect = 0;
            valPassword = false;
            MainMenu(hashtable, username, a1, a2, a3); //Go to main menu
         }
         else
         {
            incorrect++;
            if(incorrect == 3)
            {
               cout << "Please try relogging." << endl;
               valPassword = false;
               StartingPoint(hashtable, a1, a2, a3);
            }
            cout << "Invalid password. Try again: " << endl;
            cout << "Incorrect tries: " << incorrect << " out of 3." << endl << endl;
         }

      }
   }


}

void MainMenu(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3)
{
   //Selected number of tickets wanted
   int bA = 0; //adult tickets bought
   int bC = 0; //children tickets bought
   int bS = 0; //senior tickets bought
   int bT = 0; //total tickets bought


   int response; //users input
   bool validMain = false; //input validation variables
   bool goodToConvert = true;
   string temp = "";
   char tester;

   while(!validMain) //input validation for main menu
   {
      goodToConvert = true;
      cout << endl << "Main Menu" << endl
      << "1. Reserve Seats" << endl
      << "2. View Orders" << endl
      << "3. Update Orders" << endl
      << "4. Display Receipt" << endl
      << "5. Log Out" << endl
      << "-------------------------------------" << endl;

      cin >> temp;
      tester = temp[0];
      if(tester < 48 || tester > 53)
      {
         goodToConvert = false;
      }
      if(goodToConvert) { response = stoi(temp); } //convert and test if input are digits 1-5
      else { response = -1; } //will make below invalid input

      if(response >= 1 && response <= 5) { validMain = true; }
      else { cout << "Invalid Input" << endl; }
   }

   if(response == 1) //if 1, go reserve seats
   {
      bool offer = true;
      bool newOrder = true;
      int orderNum = 0;
      int theaterNum = 1;
      Reserve(hashtable, username, a1, a2, a3, bA, bC, bS, bT, offer, newOrder, orderNum, theaterNum);
      MainMenu(hashtable, username, a1, a2, a3);
   }
   else if(response == 2) //if 2, display the users orders
   {
      ViewOrder(hashtable, username);
      MainMenu(hashtable, username, a1, a2, a3);
   }
   else if (response == 3) //if 3, allow the user to update the order
   {
      UpdateOrder(hashtable, username, a1, a2, a3, bA, bC, bS, bT);
      MainMenu(hashtable, username, a1, a2, a3);
   }
   else if(response == 4) //if 4, display the user's receipt
   {
      DisplayReceipt(hashtable, username);
      MainMenu(hashtable, username, a1, a2, a3);
   }
   else if(response == 5) //if 5, log out and return to log in
   {
      StartingPoint(hashtable, a1, a2, a3);
   }

}

void Reserve(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3, int &bA, int &bC, int &bS, int &bT, bool offer, bool newOrder, int orderNum, int theaterNum)
{
   if(newOrder)
   {
      cout << "Select an auditorium" << endl
      << "1. Auditorium 1" << endl
      << "2. Auditorium 2" << endl
      << "3. Auditorium 3" << endl
      << "-------------------------------------" << endl;
   }

   int rowWant = 0; //variables to see starting row and end seat. also see if the user can actually purchase the tickets
   char colWant = ' ';
   bool buy = true;
   int response;

   if(!newOrder) { response = theaterNum; } //for updating orders
   else { cin >> response; }

   if(response == 1) //for auditorium 1
   {
      a1.Display(a1.getRows(), a1.getCols()); //displays the seating chart with #'s and .'s
      Purchase(bA, bS, bC, bT, rowWant, colWant, a1.getRows(), a1.getCols()); //requests user for number of seats
      int begRow = rowWant;
      char begCol = colWant;
      a1.Check(bA, bS, bC, bT, rowWant, colWant, a1.getRows(), a1.getCols(), begRow, begCol, buy, offer); //check if the seats are available or if there are best available
      string order = "Auditorium 1, ";
      if(buy && newOrder) //creates a string of the order
      {
         for(int x = 0; x < bT; x++)
         {
            order = order + to_string(begRow+1) + begCol;
            if(x+1 != bT) { order = order + ","; }
            begCol = (char) (begCol + 1);
         }
         order = order + "\n" + to_string(bA) + " adult, " + to_string(bC) + " child, " + to_string(bS) + " senior";
         hashtable[username].push_back(order); //add the order to a vector of orders
      }
      else if(buy && !newOrder)
      {
         AddTickets(hashtable, username, bA, bC, bS, bT, rowWant, colWant, orderNum); //for updating orders
      }
   }
    else if(response == 2) //for auditorium 2
   {
      a2.Display(a2.getRows(), a2.getCols()); //displays the seating chart with #'s and .'s
      Purchase(bA, bS, bC, bT, rowWant, colWant, a2.getRows(), a2.getCols()); //requests user for number of seats
      int begRow = rowWant;
      char begCol = colWant;
      a2.Check(bA, bS, bC, bT, rowWant, colWant, a2.getRows(), a2.getCols(), begRow, begCol, buy, offer); //check if the seats are available or if there are best available
      string order = "Auditorium 2, ";
      if(buy && newOrder) //creates a string of the order and adds it to a vector of orders
      {
         for(int x = 0; x < bT; x++)
         {
            order = order + to_string(begRow+1) + begCol;
            if(x+1 != bT) { order = order + ","; }
            begCol = (char) (begCol + 1);
         }
         order = order + "\n" + to_string(bA) + " adult, " + to_string(bC) + " child, " + to_string(bS) + " senior";
         hashtable[username].push_back(order);
      }
      else if (buy && !newOrder)
      {
         AddTickets(hashtable, username, bA, bC, bS, bT, rowWant, colWant, orderNum); //for updating orders
      }
   }
   else if(response == 3) //for auditorium 3
   {
      a3.Display(a3.getRows(), a3.getCols()); //displays the seating charts of #'s and .'s
      Purchase(bA, bS, bC, bT, rowWant, colWant, a3.getRows(), a3.getCols()); //requests user for number of seats they want
      int begRow = rowWant;
      char begCol = colWant;
      a3.Check(bA, bS, bC, bT, rowWant, colWant, a3.getRows(), a3.getCols(), begRow, begCol, buy, offer); //checks if seats or best available is available
      string order = "Auditorium 3, ";
      if(buy && newOrder) //creates a string of the order and adds it to a vector of orders
      {
         for(int x = 0; x < bT; x++)
         {
            order = order + to_string(begRow+1) + begCol;
            if(x+1 != bT) { order = order + ","; }
            begCol = (char) (begCol + 1);
         }
         order = order + "\n" + to_string(bA) + " adult, " + to_string(bC) + " child, " + to_string(bS) + " senior";
         hashtable[username].push_back(order);
      }
      else if (buy && !newOrder)
      {
         AddTickets(hashtable, username, bA, bC, bS, bT, rowWant, colWant, orderNum); //for updating orders
      }
   }
}

void Purchase(int &bought_a, int &bought_s, int &bought_c, int &bought_t, int &rowWant, char &colWant, int totalR, int totalC)
{
   //bools for input validation
   bool rowVal = false;
   bool colVal = false;
   bool aT = false;
   bool cT = false;
   bool sT = false;
   string temp;
   char tester;
   bool goodToConvert = true;

   //Prompts user for the starting seat they want
   while(!rowVal)
   {
      goodToConvert = true; //resets goodToConvert in case of failure
      cout << "Enter in the desired row number: ";
      cin >> temp;
      for(unsigned long x = 0; x < temp.length(); x++) //search through each character of the input
      {
         tester = temp[x];
         if(tester < 48 || tester > 57)
         {
            goodToConvert = false;
            break;
         }
      }
      if(goodToConvert) { rowWant = stoi(temp); } //convert and test if input are digits
      else { rowWant = -1; } //will make below invalid input

      if(rowWant > 0 && rowWant <= totalR) { rowVal = true; }
      else { cout << "Invalid Input" << endl; }

   }
   rowWant--; //indexing starts at 0

   while(!colVal)
   {
      goodToConvert = true; //resets goodToConvert in case of failure
      cout << "Enter in a starting seat letter: ";
      cin >> temp;
      for(unsigned long x = 0; x < temp.length(); x++) //search through each character of the input
      {
         tester = temp[x];
         if(tester < 65 || tester > 90)
         {
            goodToConvert = false;
            break;
         }
      }

      if(goodToConvert) { colWant = temp[0]; } //convert and test if input are digits
      else { colWant = 63; } //will make below invalid input

      if((int)colWant > 64 && (int)colWant <= (totalC+65)) { colVal = true; }
      else { cout << "Invalid Input" << endl; }
   }

   //Prompts user for number of tickets and their type
   while(!aT)
   {
      goodToConvert = true; //resets goodToConvert in case of failure
      cout << "How many adult tickets would you like to purchase: ";
      cin >> temp;
      for(unsigned long x = 0; x < temp.length(); x++) //search through each character of the input
      {
         tester = temp[x];
         if(tester < 48 || tester > 57)
         {
            goodToConvert = false;
            break;
         }
      }
      if(goodToConvert) { bought_a = stoi(temp); } //convert and test if input are digits
      else { bought_a = -1; } //will make below invalid input

      if(bought_a >= 0 && bought_a < totalC) { aT = true; }
      else { cout << "Invalid Input" << endl; }
   }

   while(!cT)
   {
      goodToConvert = true; //resets goodToConvert in case of failure
      cout << "How many children tickets would you like to purchase: ";
      cin >> temp;

      for(unsigned long x = 0; x < temp.length(); x++) //search through each character of the input
      {
         tester = temp[x];
         if(tester < 48 || tester > 57)
         {
            goodToConvert = false;
            break;
         }
      }
      if(goodToConvert) { bought_c = stoi(temp); } //convert and test if input are digits
      else { bought_c = -1; } //will make below invalid input

      if(bought_c >= 0 && bought_c < totalC) { cT = true; }
      else { cout << "Invalid Input" << endl; }
   }

   while(!sT)
   {
      goodToConvert = true; //resets goodToConvert in case of failure
      cout << "How many senior tickets would you like to purchase: ";
      cin >> temp;
      for(unsigned long x = 0; x < temp.length(); x++) //search through each character of the input
      {
         tester = temp[x];
         if(tester < 48 || tester > 57)
         {
            goodToConvert = false;
            break;
         }
      }
      if(goodToConvert) { bought_s = stoi(temp); } //convert and test if input are digits
      else { bought_s = -1; } //will make below invalid input

      if(bought_s >= 0 && bought_s < totalC) { sT = true; }
      else { cout << "Invalid Input" << endl; }
   }

   //Calculates total number of requested tickets
   bought_t = bought_a + bought_c + bought_s;
}
void ViewOrder(unordered_map<string, vector<string> > &hashtable, string username)
{
   if(hashtable[username].size() > 1) //if there are orders (stored in the vector from 1 to n)
   {
      for(unsigned long x = 1; x < hashtable[username].size(); x++) //print out the order
      {
         cout << hashtable[username].at(x) << endl;
      }
   }
   else //if there arent any orders
   {
      cout << "No orders " << endl;
   }

}
void DisplayReceipt(unordered_map<string, vector<string> > &hashtable, string username)
{
   if(hashtable[username].size() > 1) //if there are orders
   {
      float totalSales = 0;
      for(unsigned long y = 1; y < hashtable[username].size(); y++) //go through each order individually
      {
         string firsthalf = ""; //Auditorium X, [orders]
         string secondhalf = ""; //a adult, b child, c senior
         char letter = ' ';
         string str = "";
         int aTickets = 0;
         int cTickets = 0;
         int sTickets = 0;
         float sales = 0;
         int checkpoint = 0;
         for(unsigned long x = 0; x <= hashtable[username].at(y).length(); x++) //splits the order into two
         {
            letter = hashtable[username].at(y)[x];
            if(letter == '\n')
            {
               firsthalf = str;
               str = "";
            }
            else if(x == hashtable[username].at(y).length())
            {
               secondhalf = str;
               str = "";
            }
            else
            {
               str = str + letter;
            }
         }
         cout << firsthalf << endl << secondhalf << endl; //prints out the order
         str = ""; //resets str
         for(unsigned long x = 0; x <= secondhalf.length(); x++) //substrings the second half of the order
         {
            letter = secondhalf[x];
            if(letter == ' ' && checkpoint == 0) //grabs the number of adult tickets
            {
               aTickets = stoi(str);
               str = "";
               checkpoint ++;
            }
            else if(letter == ' ' && checkpoint == 1) //skips "adult"
            {
               str = "";
               checkpoint ++;
            }
            else if(letter == ' ' && checkpoint == 2) //grabs the number of children tickets
            {
               cTickets = stoi(str);
               str = "";
               checkpoint ++;
            }
            else if(letter == ' ' && checkpoint == 3) //skips "child"
            {
               str = "";
               checkpoint ++;
            }
            else if(letter == ' ' && checkpoint == 4) //grabs the number of senior tickets
            {
               sTickets = stoi(str);
               str = "";
               checkpoint++;
            }
            else { str = str + letter; }
         }
         sales = (float)((float)aTickets*10 + (float)cTickets*5 + (float)sTickets*7.50f); //calculates the total price for that order
         totalSales += sales; //calculates the total sales
         cout << "Order Total: $" << fixed << setprecision(2) << sales << endl << endl;
      }
      cout << "Customer Total: $" << fixed << setprecision(2) << totalSales << endl;
   }
   else //if there are no orders
   {
      cout << "No orders. " << endl;
      cout << "Customer Total: $0.00" << endl;
   }
}

void UpdateOrder(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3, int &bA, int &bC, int &bS, int &bT)
{
   if(hashtable[username].size() > 1) //validates that there are orders to update
   {
      bool validUpdate = false;
      bool goodToConvert = true;
      string temp = "";
      char tester;
      int response;

      while(!validUpdate) //validates that the user inputs in a proper order.
      {
         goodToConvert = true;
         for(unsigned long x = 1; x < hashtable[username].size(); x++)
         {
            cout << "Order " << x << ": " << endl << hashtable[username].at(x) << endl << endl;
         }
         cout << "Select an order to update" << endl;
         cin >> temp;
         for(unsigned long x = 0; x < temp.length(); x++) //search through each character of the input
         {
            tester = temp[x];
            if(tester < 48 || tester > 57)
            {
               goodToConvert = false;
               break;
            }
         }
         if(goodToConvert) { response = stoi(temp); } //convert and test if input are digits
         else { response = -1; } //will make below invalid input

         if(response >= 1&& response <= (int)hashtable[username].size()-1) { validUpdate = true; }
         else { cout << "Invalid Input" << endl; }
      }
      int orderNum = response;
      validUpdate = false;
      while(!validUpdate) //validates that the user inputs in 1-3
      {
         goodToConvert = true;
         cout << "How would you like to update your order?" << endl
         << "1. Add tickets to order" << endl
         << "2. Delete tickets from order" << endl
         << "3. Cancel order" << endl
         << "-------------------------------------" << endl;
         cin >> temp;
         for(unsigned long x = 0; x < temp.length(); x++) //search through each character of the input
         {
            tester = temp[x];
            if(tester < 48 || tester > 57)
            {
               goodToConvert = false;
               break;
            }
         }
         if(goodToConvert) { response = stoi(temp); } //convert and test if input are digits
         else { response = -1; } //will make below invalid input

         if(response >= 1&& response <= 3) { validUpdate = true; }
         else { cout << "Invalid Input" << temp << endl; }
      }
      if(response == 1) //if choose to add tickets
      {
         bool offer = false;
         bool newOrder = false;
         char letter = ' ';
         int checkpoint = 0;
         int theaterNum = 1;
         string str = "";
         for(unsigned long x = 0; x <= hashtable[username].at(orderNum).length(); x++) //grabs the order
         {
            letter = hashtable[username].at(orderNum)[x];
            if(letter == ' ' && checkpoint == 0)
            {
               checkpoint ++;
               str = "";
            }
            else if(letter == ',' && checkpoint == 1) //grabs the auditorium number
            {
               theaterNum = stoi(str);
               break;
            }
            else { str = str + letter; }
         }
         Reserve(hashtable, username, a1, a2, a3, bA, bC, bS, bT, offer, newOrder, orderNum, theaterNum); //reserves seats
      }
      else if(response == 2) //if user wants to delete seats
      {
         cout << "Enter in the row of the seat you wish to delete." << endl;
         int delRow = 0;
         cin >> delRow;
         cout << "Enter in the column of the seat you wish to delete." << endl;
         char delCol = ' ';
         cin >> delCol;
         string selected = to_string(delRow) + delCol; // combine the row and column together like 4+A = 4A

         char letter = ' ';
         int checkpoint = 0;
         int theaterNum = 1;
         string str = "";
         bool deleteSeat = false;
         for(unsigned long x = 0; x <= hashtable[username].at(orderNum).length(); x++) //grabs the order
         {
            letter = hashtable[username].at(orderNum)[x];
            if(letter == ' ' && checkpoint == 0) //clears "Auditorium"
            {
               checkpoint ++;
               str = "";
            }
            else if(letter == ',' && checkpoint == 1) //gets the theater number
            {
               theaterNum = stoi(str);
               str = "";
               checkpoint ++;
            }
            else if(letter == ' ' && checkpoint == 2) //clears the space
            {
               str = "";
               checkpoint++;
            }
            else if(letter == ',' && checkpoint == 3) //checks each order
            {
               if(selected == str)
               {
                  deleteSeat = true; //if selected seat is there
                  break;
               }
               else { str = ""; }
            }
            else if(letter == '\n') //reaches the end of the orders
            {
               if(selected == str)
               {
                  deleteSeat = true;
                  break;
               }
               cout << "The seat selected does not exist. " << endl; //if seat does not exist
               break;
            }
            else{ str = str + letter; }
         }
         if(deleteSeat)
         {
            DeleteTickets(hashtable, username, orderNum, delRow, delCol, theaterNum, a1, a2, a3, selected); //if seat is detected delete it
         }
      }
      else if(response == 3) //cancels order if option 3 is chosen
      {
         CancelOrder(hashtable, username, a1, a2, a3, orderNum);
      }

   }
   else
   {
      cout << "No orders to update" << endl;
   }
}

void AddTickets(unordered_map<string, vector<string> > &hashtable, string username, int &bA, int &bC, int &bS, int &bT, int begRow, char begCol, int orderNum)
{
   string firsthalf = "";
   string secondhalf = "";
   char letter = ' ';
   string str = "";
   int aTickets = 0;
   int cTickets = 0;
   int sTickets = 0;
   int checkpoint = 0;
   for(unsigned long x = 0; x <= hashtable[username].at(orderNum).length(); x++) //splits the order into two
   {
      letter = hashtable[username].at(orderNum)[x];
      if(letter == '\n')
      {
         firsthalf = str;
         str = "";
      }
      else if(x == hashtable[username].at(orderNum).length())
      {
         secondhalf = str;
         str = "";
      }
      else
      {
         str = str + letter;
      }
   }
   str = ""; //resets str
   for(unsigned long x = 0; x <= secondhalf.length(); x++) //goes through the second half
   {
      letter = secondhalf[x];
      if(letter == ' ' && checkpoint == 0) //grabs adult, children, and senior tickets
      {
         aTickets = stoi(str);
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 1)
      {
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 2)
      {
         cTickets = stoi(str);
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 3)
      {
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 4)
      {
         sTickets = stoi(str);
         str = "";
         checkpoint++;
      }
      else { str = str + letter; }
   }
   string order = "";
   for(int x = 0; x < bT; x++)
   {
      order = order + to_string(begRow+1) + begCol;
      if(x+1 != bT) { order += ","; }
      begCol = (char) (begCol + 1);
   }
   //adds in the new order in alphabetical order
   checkpoint = 0;
   string updatedOrder = "";
   str = "";
   vector<string> seatingOrder; //current order
   for(unsigned long x = 0; x <= firsthalf.length(); x++) //adds in the new seats through the user of vectors
   {
      letter = firsthalf[x];
      if(letter == ' ' && checkpoint == 0)
      {
         updatedOrder = updatedOrder + str + " ";
         str = "";
         checkpoint++;
      }
      else if(letter == ' '  && checkpoint == 1)
      {
         updatedOrder = updatedOrder + str + " ";
         str = "";
         checkpoint++;
      }
      else if(letter == ',' && checkpoint == 2)
      {
         seatingOrder.push_back(str);
         str = "";
      }
      else if(x == firsthalf.length())
      {
         seatingOrder.push_back(str);
      }
      else
      {
         str = str + letter;
      }
   }
   bool addIn = true;
   firsthalf = updatedOrder;
   for(unsigned long x = 0; x < seatingOrder.size(); x++) //adds in the substrings from the vectors
   {
      if(seatingOrder.at(x)[0] > order[0] && addIn)
      {
         firsthalf = firsthalf + order + ",";
         addIn = false;
      }
      else if(seatingOrder.at(x)[0] == order[0] && seatingOrder.at(x)[1] > order[1] && addIn)
      {
         firsthalf = firsthalf + order + ",";
         addIn = false;
      }
      firsthalf = firsthalf + seatingOrder.at(x);
      if(x+1 != seatingOrder.size()) { firsthalf = firsthalf + ","; }
   }
   if(addIn) { firsthalf = firsthalf + "," + order; } //if not add in yet, add it in to the end of the order

   //updates the new tickets based on whats bought
   aTickets += bA;
   sTickets += bS;
   cTickets += bC;
   secondhalf = to_string(aTickets) + " adult, " + to_string(cTickets) + " child, " + to_string(sTickets) + " senior";
   string finalOrder = firsthalf + "\n" + secondhalf;
   hashtable[username].at(orderNum) = "";
   hashtable[username].at(orderNum) = finalOrder; //adds it to the user's vectors
}

void DeleteTickets(unordered_map<string, vector<string> > &hashtable, string username, int orderNum, int rowWant, char colWant, int theaterNum, Auditorium &a1, Auditorium &a2, Auditorium &a3, string selected)
{
   char temp = 'X';
   if(theaterNum == 1) //delete the seat based on the respective theater
   {
      temp = a1.Delete(rowWant, colWant);
   }
   else if(theaterNum == 2)
   {
      temp = a2.Delete(rowWant, colWant);
   }
   else
   {
      temp = a3.Delete(rowWant, colWant);
   }

   string firsthalf = "";
   string secondhalf = "";
   string tempFirst = "";
   char letter = ' ';
   string str = "";
   int aTickets = 0;
   int cTickets = 0;
   int sTickets = 0;
   int checkpoint = 0;
   for(unsigned long x = 0; x <= hashtable[username].at(orderNum).length(); x++) //splits the order into two
   {
      letter = hashtable[username].at(orderNum)[x];
      if(letter == '\n')
      {
         firsthalf = str;
         str = "";
      }
      else if(x == hashtable[username].at(orderNum).length())
      {
         secondhalf = str;
         str = "";
      }
      else
      {
         str = str + letter;
      }
   }
   str = ""; //resets str
   for(unsigned long x = 0; x <= firsthalf.length(); x++) //splits the order into two
   {
      letter = firsthalf[x];
         if(letter == ',' && checkpoint == 0)
         {
            tempFirst = tempFirst + str + ",";
            checkpoint ++;
            str = "";
         }
         else if(letter == ' ' && checkpoint == 1)
         {
            tempFirst = tempFirst + " ";
            str = "";
            checkpoint++;
         }
         else if(letter == ',' && checkpoint == 2) //adds in the seats that are not the deleted one
         {
            if(selected != str)
            {
               tempFirst = tempFirst + str;
               if((x+1) != tempFirst.length())
               {
                  tempFirst = tempFirst + ",";
               }
               str = "";
            }
            else { str = ""; }
         }
         else if(x == firsthalf.length())
         {
            if(selected != str)
            {
               tempFirst = tempFirst + str;
            }
            if(tempFirst[tempFirst.length()-1] == ',')
            {
               string copyTempFirst;
               for(unsigned long y = 0; y < tempFirst.length()-1; y++)
               {
                  copyTempFirst = copyTempFirst + tempFirst[y];
               }
               tempFirst = copyTempFirst;
            }
            break;
         }
         else{ str = str + letter; }
   }
   str = ""; //resets str
   checkpoint = 0;
   for(unsigned long x = 0; x <= secondhalf.length(); x++) //grabs adult, children, and senior tickets
   {
      letter = secondhalf[x];
      if(letter == ' ' && checkpoint == 0)
      {
         aTickets = stoi(str);
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 1)
      {
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 2)
      {
         cTickets = stoi(str);
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 3)
      {
         str = "";
         checkpoint ++;
      }
      else if(letter == ' ' && checkpoint == 4)
      {
         sTickets = stoi(str);
         str = "";
         checkpoint++;
      }
      else { str = str + letter; }
   }

   if(temp == 'A') //substracts the deleted seat from the order
   {
      aTickets--;
   }
   else if(temp == 'C')
   {
      cTickets--;
   }
   else if(temp == 'S')
   {
      sTickets--;
   }

   if(aTickets == 0 && cTickets == 0 && sTickets == 0) //if there are no tickets left, delete the order from the vector
   {
      hashtable[username].erase(hashtable[username].begin() + orderNum);
   }
   else //else just update the order
   {
      secondhalf = to_string(aTickets) + " adult, " + to_string(cTickets) + " child, " + to_string(sTickets) + " senior";
      string updatedOrder = tempFirst + "\n" + secondhalf;
      hashtable[username].at(orderNum) = "";
      hashtable[username].at(orderNum) = updatedOrder;
   }
}

void CancelOrder(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3, int orderNum)
{
   int checkpoint = 0;
   char letter = ' ';
   int theaterNum = 1;
   string str = "";
   vector<string> seatingOrder; //current order
   string firsthalf = hashtable[username].at(orderNum);
   for(unsigned long x = 0; x <= firsthalf.length(); x++) //grabs the auditorium number and seats
   {
         letter = firsthalf[x];
         if(letter == ' ' && checkpoint == 0)
         {
            str = "";
            checkpoint++;
         }
         else if(letter == ','  && checkpoint == 1)
         {
            theaterNum = stoi(str);
            str = "";
            checkpoint++;
         }
         else if(letter == ' ' && checkpoint == 2)
         {
            str = "";
            checkpoint++;
         }
         else if(letter == ',' && checkpoint == 3)
         {
            seatingOrder.push_back(str);
            str = "";
         }
         else if(letter == '\n')
         {
            seatingOrder.push_back(str);
            for(unsigned long z = 0; z < seatingOrder.size(); z++) //goes through vector containing the seats
            {
               int rW = seatingOrder.at(z)[0]-48;
               char cW = seatingOrder.at(z)[1];
               if(theaterNum == 1) //deletes the order
               {
                  a1.Delete(rW, cW);
               }
               else if(theaterNum == 2)
               {
                  a2.Delete(rW, cW);
               }
               else if(theaterNum == 3)
               {
                  a3.Delete(rW, cW);
               }
            }
         }
         else
         {
            str = str + letter;
         }
   }
   hashtable[username].erase(hashtable[username].begin() + orderNum); //erases the order from the username vector
}

void AdminMainMenu(unordered_map<string, vector<string> > &hashtable, string username, Auditorium &a1, Auditorium &a2, Auditorium &a3)
{
   int response;
   bool validMain = false;
   bool goodToConvert = true;
   string temp = "";
   char tester;

   while(!validMain) //validates input for 1-3
   {
      goodToConvert = true;
      cout << endl << "Admin Main Menu" << endl
      << "1. Print Report" << endl
      << "2. Logout" << endl
      << "3. Exit" << endl
      << "-------------------------------------" << endl;

      cin >> temp;
      tester = temp[0];
      if(tester < 48 || tester > 53)
      {
         goodToConvert = false;
      }
      if(goodToConvert) { response = stoi(temp); } //convert and test if input are digits
      else { response = -1; } //will make below invalid input

      if(response >= 1 && response <= 3) { validMain = true; }
      else { cout << "Invalid input. Try again:" << endl; }
   }
   if(response == 1) //if the user selects option 1, print out a report of all the theaters
   {
      int totalA = 0;
      int totalC = 0;
      int totalS = 0;
      int totalT = 0;
      int totalOpen = 0;
      int totalReserved = 0;
      float totalSales = 0;
      cout << "Auditorium 1\t";
      a1.Report(a1.getRows(), totalA, totalC, totalS, totalT, totalSales, totalOpen, totalReserved);
      cout << "Auditorium 2\t";
      a2.Report(a2.getRows(), totalA, totalC, totalS, totalT, totalSales, totalOpen, totalReserved);
      cout << "Auditorium 3\t";
      a3.Report(a3.getRows(), totalA, totalC, totalS, totalT, totalSales, totalOpen, totalReserved);
      cout << "Total\t\t" << totalOpen << "\t" << totalReserved << "\t" << totalA << "\t"
      << totalC << "\t" << totalS << "\t$" << fixed << setprecision(2) << totalSales << endl;
      AdminMainMenu(hashtable, username, a1, a2, a3);
   }
   else if(response == 2) //if log out return to starting point
   {
      StartingPoint(hashtable, a1, a2, a3);
   }
   else if (response == 3) //if 3, end the program and write the reports
   {
      int theaterNum = 1;
      a1.Write(a1.getRows(), theaterNum);
      theaterNum = 2;
      a2.Write(a1.getRows(), theaterNum);
      theaterNum = 3;
      a3.Write(a1.getRows(), theaterNum);
   }
}
