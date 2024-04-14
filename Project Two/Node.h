#ifndef NODE_H
#define NODE_H
#include <iostream>
template<typename Data> class LinkedList;

template<class Data> class Node
{
   friend class LinkedList<Data>;
   public:
      // Constructors
      Node(Data _data){data = _data; nextPtr = nullptr;}
      // Mutators and Accessors
      Data& GetData() { return data; }
      void SetData(Data _data) { data = _data; }
      Node* GetNextPtr() const{ return nextPtr; }
      void SetNextPtr(Node* _nextPtr) { nextPtr = _nextPtr; }
      //Overloading Output
      friend std::ostream& operator<<(std::ostream& stream, const Node<Data>& node){
         stream << node.data;
         return stream;
      }
      //Overloading Comparison
      bool operator<(const Node<Data>& other) const{return data < other.data;}
      bool operator>(const Node<Data>& other) const{return data > other.data;}
      bool operator==(const Node<Data>& other) const{return data == other.data;}
   private:
      Data data;
      Node* nextPtr;
};
#endif // NODE_H
