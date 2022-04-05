#ifndef NODE_H
#define NODE_H
using namespace std;
//NAME:  Eric Cao
//NETID: EXC190021

template <typename T>
class Node
{
   public:
      Node *up;
      Node *down;
      Node *right;
      Node *left;
      T payload;
      Node() //default constructor
      {
         up - nullptr;
         down = nullptr;
         right = nullptr;
         left = nullptr;
      }

      //overloaded constructor
      Node(T data)
      {
         payload = data;
      }
     // virtual ~Node();

      //Accessor Methods
      Node* getUp(){ return up;}
      Node* getDown(){ return down; }
      Node* getRight(){ return right; }
      Node* getLeft(){ return left; }
      T getPayload() { return payload; }

      //Mutator Methods
      Node* setUp(Node* u) { up = u; }
      Node* setDown(Node* d) { down = d; }
      Node* setRight(Node* r) { right = r; }
      Node* setLeft(Node* l) { left = l; }
      T setPayload(T data) { payload = data; }


   protected:

   private:
};

#endif // NODE_H
