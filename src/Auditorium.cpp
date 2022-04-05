#include "Auditorium.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Seat.h"

//NAME:  Eric Cao
//NETID: EXC190021

Auditorium::Auditorium(string file, int totalC, int totalR)
{
   string temp; //temporary string value
   int r = 1; //row starts at one
   ifstream inFS;
   inFS.open(file); //opens the file
   while(inFS.good()) //keep it open until it reaches the end
   {
      char c = 'A'; //column letter starts with A
      getline(inFS, temp);
      for(unsigned x = 0; x < temp.length(); x++)
      {
         char t = temp[x]; //type letter based on file
         Node<Seat*> *newNode = new Node<Seat*>(new Seat(r, c, t)); //Creates a new node with first seat inside
         newNode->right = NULL; //right of it points to null
         Node<Seat*> *trav = first; //traversing node
         if(first == NULL) //if header node points to null
         {
            first = newNode; //header node points to this
         }
         else
         {
            while(trav->right != NULL) //while right of trav not null
            {
               trav = trav->right; //keep making trav go right
            }
            trav->right = newNode; //then right of trav becomes the new node
         }
         c = (char)(c+1); //next node contains next column
      }
      r++; //after x amount of columns increase row
   }
   inFS.close(); //closes the file

   //double linking the nodes
   Node<Seat*> *after = first->right; //node after the first
   Node<Seat*> *before = first;
   while(after != nullptr) //while the sequential node isnt null
   {
      after->left = before; //left of after is before
      after = after->right; //shift before and after to the right
      before = before->right;
   }

   //linking the nodes up and down QUADRA LINK
   Node<Seat*> *topNode = first;
   Node<Seat*> *botNode = first;
   for(int x = 0; x < totalC; x++) //bottom node will point to the first node of next row
   {
      botNode = botNode->right;
   }
   while(botNode != nullptr) //while bottom node isn't null
   {
      topNode->down = botNode; //make topnode point down to bottom node
      botNode->up = topNode; //bottom node points up to top node
      topNode = topNode->right; //both of them shift right
      botNode = botNode->right;
   }

   //stopping right and left to point out of control
   Node<Seat*> *firstOfRow = first;
   for(int x = 0; x < totalR; x++)
   {
      Node<Seat*> *travelRight = firstOfRow; //starts at the first node of every row
      firstOfRow->left = nullptr;
      for(int x = 0; x < totalC-1; x++) //goes to the end of that row
      {
         travelRight = travelRight->right;
      }
      travelRight->right = nullptr; //making the right of it point to nullptr
      firstOfRow = firstOfRow->down; //first of row goes to next row until it reaches the end
   }


}

void Auditorium::Display(int totalR, int totalC)
{
   char seatLetters = 'A'; //prints out the column labels
   cout << "   "; //spacing for column label
   for(int x = 0; x < totalC; x++)
   {
      cout << seatLetters;
      seatLetters = (char)(seatLetters+1);
   }
   cout << endl;

   int rowNumbers = 1;
   Node<Seat*> *firstOfRow = first;
   for(int x = 0; x < totalR; x++)
   {
      if(rowNumbers > 9){ cout << rowNumbers << " "; } //prints the row number
      else{ cout << rowNumbers << "  "; }


      Node<Seat*> *travelRight = firstOfRow; //travel node starts at the first of every row
      while(travelRight != nullptr) //if it's not null
      {
         if(travelRight->getPayload()->getType() != '.') //and does not contain a free seat
         {
            cout << "#"; //print a #
         }
         else { cout << "."; } //else print a free seat "."
         travelRight = travelRight->right; //make it goto the right until its nullptr
      }
      cout << endl; //skips line for a new row
      firstOfRow = firstOfRow->down; //goes down to next row
      rowNumbers++; //row label increases
   }

   cout << endl << endl;
}
void Auditorium::Check(int bA, int bS, int bC, int bT, int rW, char cW, int totalR, int totalC, int &begRow, char &begCol, bool &buy, bool offer)
 {
    bool reservable = true; //bool to see if seats can be reserved

    Node<Seat*> *trav = first;
    for(int x = 0; x < rW; x++) //makes trav go go down to desired row
    {
       trav = trav->down;
    }
    int gotoRight = int(cW-65);
    for(int x = 0; x < gotoRight; x++) //trav will goto the right based on desired column
    {
       trav = trav->right;
    }
    Node<Seat*> *temp = trav; //creates a temporary node
    for(int x = 0; x < bT; x++) //if runs into any reserved seat
    {
      if(temp->getPayload()->getType() == 'A' ||
         temp->getPayload()->getType() == 'S' ||
         temp->getPayload()->getType() == 'C')
      {
         cout << "no seats available" << endl;
         reservable = false; //seats can no longer be reserved
         if(offer) { BestAvailable(totalR, totalC, bT, bA, bC, bS, begRow, begCol, buy); }
         else { buy = false; }
         break;
      }
      temp = temp->right; //keep making temp go right
    }
    if(reservable)
    {
       for(int x = 0; x < bA; x++) //changes . to A
       {
          trav->getPayload()->setType('A');
          trav = trav->right;
       }
       for(int x = 0; x < bC; x++) //then to C
       {
          trav->getPayload()->setType('C');
          trav = trav->right;
       }
       for(int x = 0; x < bS; x++) //and finally S
       {
          trav->getPayload()->setType('S');
          trav= trav->right;
       }
    }
 }

void Auditorium::Write(int totalR, int theaterNum)
{
   ofstream outFS;
   if(theaterNum == 1) //opens up text file based on the auditorium number
   {
      outFS.open("A1Final.txt");
   }
   else if(theaterNum == 2)
   {
      outFS.open("A2Final.txt");
   }
   else
   {
      outFS.open("A3Final.txt");
   }

   Node<Seat*> *cur = first; //traversing node
   Node<Seat*> *firstOfRow = first;
   for(int x = 0; x < totalR; x++)
   {
      while(cur != nullptr) //while cur is not null
      {
        outFS << cur->getPayload()->getType(); //write out cur to the file
        cur = cur->right; //make it goto the right
      }
      outFS << endl; //skip a line for a new row
      firstOfRow = firstOfRow->down; //row goes down to next row
      cur = firstOfRow;
   }
   outFS.close(); //closes the text file
}

void Auditorium::Report(int totalR, int &totalA, int &totalC, int &totalS, int &totalT, float &totalSales, int &totalOpen, int &totalReserved)
{
   //Instantiating variables for the report
   int seats = 0;
   int tTick = 0;
   float aTick = 0;
   float cTick = 0;
   float sTick = 0;
   float sales = 0.00f;

   Node<Seat*> *cur = first; //Traversing node
   Node<Seat*> *firstOfRow = first;
   for(int x = 0; x < totalR; x++)
   {
      while(cur != nullptr) //while its not null
      {
         if(cur->getPayload()->getType() == 'A') //increase number of ticket based on what cur is
         {
            aTick++;
            tTick++;
            seats++;
         }
         else if(cur->getPayload()->getType() == 'C')
         {
            cTick++;
            tTick++;
            seats++;
         }
         else if(cur->getPayload()->getType() == 'S')
         {
            sTick++;
            tTick++;
            seats++;
         }
         else
         {
            seats++; //non taken seat will be counted as a seat
         }
         cur = cur->right; //cur node will be the node to the right
      }
      firstOfRow = firstOfRow->down; //fOR will goto the next row
      cur = firstOfRow;

   }
   int reservedSeats = (int)(aTick + cTick + sTick);
   int openSeats = int(seats - reservedSeats);
   sales = float((aTick*10) + (cTick*5) + (sTick*7.50f));
   cout << openSeats << "\t" << reservedSeats << "\t" <<
   fixed << setprecision(0) << aTick << "\t"
   << fixed << setprecision(0) << cTick << "\t"
   << fixed << setprecision(0) << sTick <<
   "\t$" << fixed << setprecision(2) << sales << endl;
   totalA += (int)aTick;
   totalC += (int)cTick;
   totalS += (int)sTick;
   totalT += (int)(aTick + cTick + sTick);
   totalSales += sales;
   totalOpen += openSeats;
   totalReserved += reservedSeats;



}

void Auditorium::BestAvailable(int totalR, int totalC, int bT, int bA, int bC, int bS,  int &begRow, char &begCol, bool &buy)
{
   int center = totalC/2; //index of middle column
   int mid = totalR/2; //index of middle row
   int counter = 0; //counter variable for free seats

   //middle for odd numbers
   if(totalR%2 == 0) { mid--; }
   if(totalC%2 == 0) { center--; }

   //Pointers for rows
   Node<Seat*> *midRow = first;

   for(int x = 0; x < mid; x++) { midRow = midRow->down; } //points to the middle row

   Node<Seat*> *travel = midRow;
   for(int x = 0; x < center; x++) //gets travel pointer to goto the middle
   {
      travel = travel->right;
   }

   //distance variables for scans
   float topDist = 0;
   float botDist = 0;
   float midDist = 0;

   //will be the starting row and column seat
   int topR = 0;
   char topC = ' ';
   int midR = 0;
   char midC = ' ';
   int botR = 0;
   char botC = ' ';

   bool topWin = false;
   bool botWin = false;
   bool midWin = false;
   int winningR = 0;
   char winningC = ' ';

   Node<Seat*> *comp1Row = travel->up;
   Node<Seat*> *comp2Row = travel->down;

   while(comp1Row != nullptr && comp2Row != nullptr)
   {
      midDist = ScanRow(travel, counter, bT, midR, midC, mid); //Getting middle row's distance
      topDist = ScanRow(comp1Row, counter, bT, topR, topC, mid); //getting above row distance
      botDist = ScanRow(comp2Row, counter, bT, botR, botC, mid); //getting bottom row distance

      if(midDist < topDist && midDist < botDist) //if the middle row has the least distance
      {
         comp1Row = comp1Row->up; //move the other comparisons
         comp2Row = comp2Row->down;
         midWin = true;
         botWin = false;
         topWin = false;
      }
      else if(topDist < midDist && topDist < botDist) //if the above row has the least distance
      {
         comp2Row = comp2Row->down; //move the bottom row down
         midWin = false;
         botWin = false;
         topWin = true;
      }
      else if(botDist < midDist && botDist < topDist) //if the bottom row has the least distance
      {
         if(comp1Row->getPayload()->getRow() == 1) { comp1Row = nullptr; }
         else{ comp1Row = comp1Row->up; }  //move the above row more
         midWin = false;
         botWin = true;
         topWin = false;
      }
      else if(botDist < midDist && topDist < midDist && topDist == botDist) //if there is a tie between bottom and top
      {
         comp2Row = comp2Row->down; //upper row wins, move the bottom row down
         midWin = false;
         botWin = false;
         topWin = true;
      }
      else if(botDist == midDist && topDist == midDist) //if its a 3 way tie
      {
         comp1Row = comp1Row->up; //move the other comparisons, mid wins
         comp2Row = comp2Row->down;
         midWin = true;
         botWin = false;
         topWin = false;
      }
      else if(midDist == topDist && midDist < botDist) //tie between mid and top
      {
         comp1Row = comp1Row->up; //move the other comparisons, mid wins still
         comp2Row = comp2Row->down;
         midWin = true;
         botWin = false;
         topWin = false;
      }
      else if(midDist == botDist && midDist < topDist) //tie between mid and bot
      {
         comp1Row = comp1Row->up; //move the other comparisons, mid wins still
         comp2Row = comp2Row->down;
         midWin = true;
         botWin = false;
         topWin = false;
      }
      else if(botDist == 1000 && topDist == 1000 && midDist == 1000) //if no best seats available
      {
         comp1Row = comp1Row->up; //move all the other comparisons
         comp2Row = comp2Row->down;
         midWin = false; //all of them lose
         botWin = false;
         topWin = false;
      }
   }
   if(botDist == 1000 && topDist == 1000 && midDist == 1000) //if no best seats available
   {
      midWin = false; //all of them lose
      botWin = false;
      topWin = false;
      buy = false;
   }

   //Based on winner, determines which will be the beginning seat to be reserved
   if(midWin)
   {
      winningR = midR;
      winningC = midC;
   }
   else if(topWin)
   {
      winningR = topR;
      winningC = topC;
   }
   else if(botWin)
   {
      winningR = botR;
      winningC = botC;
   }
   if(midWin || topWin || botWin) //if there are best seats available
   {
      Node<Seat*> *begSeat = first;
      for(int x = 0; x < winningR-1; x++) //move begSeat to the starting seat location
      {
         begSeat = begSeat->down;
      }
      for(int x = 0; x < (int)(winningC - 65); x++)
      {
         begSeat = begSeat->right;
      }

      Node<Seat*> *endSeat = begSeat;
      for(int x = 0; x < bT-1; x++) //moves end seat to ending seat
      {
         endSeat = endSeat->right;
      }
      //Prompt if want to reserve best seats
      cout << "Best Seats Available: " << begSeat->getPayload()->getRow() << begSeat->getPayload()->getSeat()
      << " - " << endSeat->getPayload()->getRow() << endSeat->getPayload()->getSeat() << endl;
      cout << "Would you like to reserve them? (Y/N)" << endl;

      string response;
      cin >> response;
      if(response == "Y") //if yes reserve
      {
         begRow = begSeat->getPayload()->getRow() - 1;
         begCol = begSeat->getPayload()->getSeat();
         for(int x = 0; x < bA; x++)
         {
            begSeat->getPayload()->setType('A'); //starting with S
            begSeat = begSeat->right;
         }
         for(int x = 0; x < bC; x++)
         {
            begSeat->getPayload()->setType('C'); //then to C
            begSeat = begSeat->right;
         }
         for(int x = 0; x < bS; x++)
         {
            begSeat->getPayload()->setType('S'); //and finally A (which is first)
            begSeat = begSeat->right;
         }
      }
      else { buy = false; }
   }

}

float Auditorium::ScanRow(Node<Seat*> *travel, int counter, int bT, int &r, char &c, int mid)
{
   //calculating best available for middle row
   bool middleGood = true; //prevent any further scans

   if(travel->getPayload()->getType() == '.') //if center is a free seat counter goes up
   {
      counter++;
   }
   else{ middleGood = false; }
   Node<Seat*> *goingLeft = travel; //pointer that will go left as travel goes right
   Node<Seat*> *centerPointer = travel;

   float distance = 0;
   bool goRight = true; //becomes false if the pointers cant go right anymore (runs into road block or null)
   bool goLeft = true;
   while(travel != nullptr && counter != bT && middleGood)
   {
      travel = travel->right;
      if(travel->getPayload()->getType() == '.' && goRight) //if center is a free seat counter goes up
      {
         counter++;
      }
      else if(travel->getPayload()->getType() != '.' && goRight) //scans to the right, if not free stop
      {
         goRight = false;
      }
      if(counter == bT)
      {
         break;
      }
      goingLeft = goingLeft->left;
      if(goingLeft->getPayload()->getType() == '.' && goLeft) //if center is a free seat counter goes up
      {
         counter++;
      }
      else if(goingLeft->getPayload()->getType() != '.' && goLeft) //scans to the left, if not free stop
      {
         goLeft = false;
      }
      if(counter == bT)
      {
         break;
      }
      if(!goLeft && !goRight) //if can't go right or left anymore
      {
         middleGood = false; //other rows will have to be scanned
         break;
      }
   }
   if(!middleGood)
   {
      int rightCount = 0;
      int leftCount = 0;
      travel = centerPointer; //resets travel and goingLeft
      goingLeft = centerPointer;
      goRight = true;
      goLeft = true;
      while(travel != nullptr && goingLeft != nullptr)
      {
         if(travel == nullptr) { goRight = false; }
         if(travel->getPayload()->getType() == '.' && goRight) //independent right search
         {
            rightCount++;
            if(rightCount != bT){ travel = travel->right; } //move travel to the right if needed
         }
         else
         {
            rightCount = 0; //else reset counter and make it move right
            travel = travel->right;
         }
         if(rightCount == bT) //RESERVING SEATS FROM RIGHT TO CENTER
         {
             //Sets in contested row and column
             r = travel->getPayload()->getRow();
             c = travel->getPayload()->getSeat();
             Node<Seat*> *temp = travel; //temporary node, points to ending seat
             for(int x = 0; x < bT; x++) {temp = temp->left;}
             char endS = temp->getPayload()->getSeat();
             float avg = float(endS+c)/2;
             temp = temp->right; //realligns temp properly
             c = temp->getPayload()->getSeat(); //c is the contesting column

             //Calculating distance
             float a = (float)(mid - r + 1);
             float b = (((float)(avg - centerPointer->getPayload()->getSeat())));
             distance = (float)sqrt((a*a) + (b*b));

            return distance; //returns that it worked
         }
         if(goingLeft == nullptr) { goLeft = false; }
         if(goingLeft->getPayload()->getType() == '.' && goLeft)
         {
            leftCount++;
            if(leftCount != bT) { goingLeft = goingLeft->left; } //moves to the left if needed
         }
         else
         {
            leftCount = 0; //resets left counter and pointer keeps going left
            goingLeft = goingLeft->left;
         }
         if(leftCount == bT)
         {
             //Sets in contested row and column
             r = goingLeft->getPayload()->getRow();
             c = goingLeft->getPayload()->getSeat();
             Node<Seat*> *temp = goingLeft; //temporary node, points to ending seat
             for(int x = 0; x < bT-1; x++) {temp = temp->right;}
             char endS = temp->getPayload()->getSeat();
             float avg = float(endS+c)/2;

             //Calculating distance
             float a = (float)(mid - r + 1);
             float b = (((float)(centerPointer->getPayload()->getSeat())) - avg);
             distance = (float)sqrt((a*a) + (b*b));

            return distance; //returns that it worked
         }

         //For edge case, left always reaches end first

           if(goingLeft == nullptr && rightCount != bT && leftCount != bT && travel != nullptr) //if left has reached the end
           {
              if(travel->getPayload()->getType() == '.' && goRight) //check the last right column
               {
                  rightCount++;
               }
               //if last right column does the job start reserving
               if(rightCount == bT) //RESERVING SEATS FROM RIGHT TO CENTER
               {
                   //Sets in contested row and column
                   r = travel->getPayload()->getRow();
                   c = travel->getPayload()->getSeat();
                   Node<Seat*> *temp = travel; //temporary node, points to ending seat
                   for(int x = 0; x < bT; x++) {temp = temp->left;}
                   char endS = temp->getPayload()->getSeat();
                   float avg = float(endS+c)/2;
                   temp = temp->right; //realligns temp properly
                   c = temp->getPayload()->getSeat(); //c is the contesting column

                   //Calculating distance
                   float a = (float)(mid - r + 1);
                   float b = (((float)(avg - centerPointer->getPayload()->getSeat())));
                   distance = (float)sqrt((a*a) + (b*b));

                  return distance; //returns that it worked
               }
           }
      }
      return 1000; //returns that it failed
   }
   else
   {
       //Sets in contested row and column
       r = goingLeft->getPayload()->getRow();
       c = goingLeft->getPayload()->getSeat();
       Node<Seat*> *temp = goingLeft; //temporary node, points to ending seat
       for(int x = 0; x < bT; x++) {temp = temp->right;}
       char endS = temp->getPayload()->getSeat();
       float avg = float(endS+c)/2;

       //Calculating distance
       float a = (float)(mid - r +1);
       float b = (((float)(centerPointer->getPayload()->getSeat())) - avg);
       distance = (float)sqrt((a*a) + (b*b));

       return distance; //returns that it worked
   }

   return 1000; //failed return
}
char Auditorium::Delete(int rW, char cW)
{
   Node<Seat*> *trav = first;
    for(int x = 0; x < rW-1; x++) //makes trav go go down to desired row
    {
       trav = trav->down;
    }
    int gotoRight = int(cW-65);
    for(int x = 0; x < gotoRight; x++) //trav will goto the right based on desired column
    {
       trav = trav->right;
    }
    char currentType = trav->getPayload()->getType();
    trav->getPayload()->setType('.');
    return currentType;
}
Auditorium::~Auditorium()
{
   Node<Seat*> *rowPointer = first->down; //pointer that starts second row
   while(rowPointer != nullptr)
   {
      while(first != nullptr)
      {
         Node<Seat*> *colPointer = first->right; //pointer that starts second column
         delete first;
         first = colPointer;
      }
      //once the first row is deleted move onto the next
      first = rowPointer; //first is now first of next row
      rowPointer = rowPointer->down; //RP points to next row
   }
   //repeats one more time because first still points to last row while RP is null
   while(first != nullptr)
      {
         Node<Seat*> *colPointer = first->right; //pointer that starts second column
         delete first;
         first = colPointer;
      }
}
