#ifndef SEAT_H
#define SEAT_H
//NAME:  Eric Cao
//NETID: EXC190021

class Seat
{
   public:
      int row;
      char seat; //column
      char tType; //ticket type

      Seat(); //default constructor
      Seat(int r, char s, char t); //overloaded constructor
      virtual ~Seat(); //destructor

      //accessors
      int getRow() { return row; }
      char getSeat() { return seat; }
      char getType() { return tType; }

      //mutators
      void setRow(int r) { row = r; }
      void setSeat(char s) { seat = s; }
      void setType(char t) { tType = t; }

   protected:

   private:
};

#endif // SEAT_H
