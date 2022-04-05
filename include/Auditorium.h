#ifndef AUDITORIUM_H
#define AUDITORIUM_H
#include <fstream>
#include "Seat.h"
#include "Node.h"

//NAME:  Eric Cao
//NETID: EXC190021

class Auditorium
{
   public:
      Auditorium(string file, int totalC, int totalR);
      void Display(int totalR, int totalC);
      void Check(int bA, int bS, int bC, int bT, int rW, char cW, int totalR, int totalC, int &begRow, char &begCol, bool &buy, bool offer);
      void BestAvailable(int totalR, int totalC, int bT, int bA, int bC, int bS, int &begRow, char &begCol, bool &buy);
      float ScanRow(Node<Seat*> *travel, int counter, int bT, int &r, char &c, int mid);
      void Write(int totalR, int theaterNum);
      void Report(int totalR, int &totalA, int &totalC, int &totalS, int &totalT, float &totalSales, int &totalOpen, int &totalReserved);
      char Delete(int rW, char cW);
      virtual ~Auditorium();

      Node<Seat*> *first = nullptr;
      int rows = 0;
      int cols = 0;

      Node<Seat*> *getFirst(){ return first;} //accessor
      void setFirst(Node<Seat*> *point){ first = point;} //mutator

      int getRows() { return rows; }
      void setRows(int x) { rows = x; }
      int getCols() { return cols; }
      void setCols(int x) { cols = x; }



   protected:

   private:
};

#endif // AUDITORIUM_H
