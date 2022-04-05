#include "Seat.h"
//NAME:  Eric Cao
//NETID: EXC190021

Seat::Seat()
{
   row = 0;
   seat = ' ';
   tType = ' ';
}
Seat::Seat(int r, char s, char t)
{
   row = r;
   seat = s;
   tType = t;
}

Seat::~Seat()
{
   //dtor
}
