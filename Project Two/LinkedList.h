#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
#include <iostream>
template<class Data> class LinkedList{
   public:
      // Constructors
      LinkedList(){ headPtr = nullptr; tailPtr = nullptr; };
      // Destructor
      ~LinkedList();
      // Mutators and Accessors
      Node<Data>* GetHeadPtr() const{ return headPtr; }
      void SetHeadPtr(Node<Data>* dataPtr) { headPtr = dataPtr; }
      Node<Data>* GetTailPtr() const{ return tailPtr; }
      void SetTailPtr(Node<Data>* dataPtr) { tailPtr = dataPtr; }
      //Function
      void Prepend(Node<Data>* nodePtr);
      void Append(Node<Data>* nodePtr);
      void InsertAt(int position, Node<Data>* nodePtr);
      void RemoveAt(int position);
      Node<Data>* PopAt(int position);
      Node<Data>* Search(Data& data);
      void InsertionSort();
      void RecursiveRemove(Node<Data>* nodePtr);
      //Overloading Output
      friend std::ostream& operator<<(std::ostream& stream, const LinkedList<Data>& linkedList){
         linkedList.RecursivePrint(stream, linkedList.headPtr);
         return stream;
      }
      void RecursivePrint(std::ostream& stream, Node<Data>* nodePtr) const;
   private:
      Node<Data>* headPtr;
      Node<Data>* tailPtr;
      // Helper Function
      int FindInsertionPosition(Node<Data>* data);
      Node<Data>* RetrieveAt(int position);
};
template<class Data> LinkedList<Data>::~LinkedList(){
   // Free all the nodes in Linked List
   // std::cout << "Deleting Nodes" << std::endl;
   RecursiveRemove(this->headPtr);
}
template<class Data> void LinkedList<Data>::RecursiveRemove(Node<Data>* nodePtr){
   if(nodePtr != nullptr){
      // Recursively traverse to the end, then delete from the back up
      RecursiveRemove(nodePtr->GetNextPtr());
      delete nodePtr;
   }
}
template<class Data> void LinkedList<Data>::Prepend(Node<Data>* nodePtr){
   if(this->GetHeadPtr() == nullptr){
      // If list is empty, the new node is head and tail
      this->SetHeadPtr(nodePtr);
      this->SetTailPtr(nodePtr);
   }
   else{
      // Set node's next to head's next
      nodePtr->SetNextPtr(this->headPtr);
      // Set the node to be the head
      this->SetHeadPtr(nodePtr);
   }
}
template<class Data> void LinkedList<Data>::Append(Node<Data>* nodePtr){
   if(this->GetHeadPtr() == nullptr){
      // If list is empty, the new node is head and tail
      this->SetHeadPtr(nodePtr);
      this->SetTailPtr(nodePtr);
   }
   else{
      // Set the tail's next to the node
      this->GetTailPtr()->SetNextPtr(nodePtr);
      // Set the node to be the tail
      this->SetTailPtr(nodePtr);
   }
}
template<class Data> Node<Data>* LinkedList<Data>::RetrieveAt(int position){
   Node<Data>* temp = this->GetHeadPtr();
   // Traverse through the linked list, stopping when reaching position
   for(int i = 0; i < position; i++) {
      if(temp->GetNextPtr() != nullptr) {
         temp = temp->GetNextPtr();
      }
   }
   return temp;
}
template<class Data> void LinkedList<Data>::InsertAt(int position, Node<Data>* nodePtr){
   // If insert at head, prepend the node
   if(position == 0) this->Prepend(nodePtr);
   else{
      // Get the node before the position
      Node<Data>* temp = RetrieveAt(position-1);
      // If insert after tail, append the node
      if(temp == this->GetTailPtr()) this->Append(nodePtr);
      else{
         // Create the node
         // Connect the new node to the next node
         nodePtr->SetNextPtr(temp->GetNextPtr());
         // Connect the previous node to the new node
         temp->SetNextPtr(nodePtr);
      }
   }
}
template<class Data> void LinkedList<Data>::RemoveAt(int position){
   if(position == 0){
      // Store the next of head
      Node<Data>* temp = this->GetHeadPtr()->GetNextPtr();
      // Deleting head
      delete this->GetHeadPtr();
      // Make the new head the next of head
      this->SetHeadPtr(temp);
   }
   else{
      // Get the previous node
      Node<Data>* temp = RetrieveAt(position - 1);
      if(temp->GetNextPtr() == this->GetTailPtr()){
         // Delete the tail
         delete this->GetTailPtr();
         // Set the previous node's next to null
         temp->SetNextPtr(nullptr);
         // Set the tail as the previous node
         this->SetTailPtr(temp);
      }
      else{
         // Get the next node of current
         Node<Data>* temp2 = temp->GetNextPtr()->GetNextPtr();
         // Delete the current
         delete temp->GetNextPtr();
         // Connect the previous node to the next node
         temp->SetNextPtr(temp2);
      }
   }
}
template<class Data> Node<Data>* LinkedList<Data>::PopAt(int position){
   if(position == 0){
      // Store the head pointer
      Node<Data>* temp = this->GetHeadPtr();
      // Make the new head the next of head
      this->SetHeadPtr(this->GetHeadPtr()->GetNextPtr());
      // Return former head
      temp->SetNextPtr(nullptr);
      return temp;
   }
   else{
      // Get the previous node
      Node<Data>* temp = RetrieveAt(position - 1);
      if(temp->GetNextPtr() == this->GetTailPtr()){
         // Delete the tail
         Node<Data>* temp2 = this->GetTailPtr();
         // Set the previous node's next to null
         temp->SetNextPtr(nullptr);
         // Set the tail as the previous node
         this->SetTailPtr(temp);
         // Return former tail
         temp2->SetNextPtr(nullptr);
         return temp2;
      }
      else{
         // Save the current node
         Node<Data>* temp2 = temp->GetNextPtr();
         // Connect the previous node to the next node
         temp->SetNextPtr(temp2->GetNextPtr());
         // Return current node
         temp2->SetNextPtr(nullptr);
         return temp2;
      }
   }
}
template<class Data> void LinkedList<Data>::InsertionSort(){
   int positionCurrent = 1;
   // Start at the index 1
   Node<Data>* current = this->GetHeadPtr()->GetNextPtr();
   while(current != nullptr){
      Node<Data>* next = current->GetNextPtr();
      // Find the position that is less than or equal to node
      int position = FindInsertionPosition(current);
      // Pop the current node
      current = PopAt(positionCurrent);
      // And move it to the position found
      if(position == 0) Prepend(current);
      else InsertAt(position, current);
      // Move to the next node
      current = next;
      positionCurrent++;
   }
}
template<class Data> int LinkedList<Data>::FindInsertionPosition(Node<Data>* data){
   int position = 0;
   Node<Data>* current = this->GetHeadPtr();
   // Move forward until reaching a node that is larger or equal the current
   while(current != nullptr && *data > *current){
      position++;
      current = current->GetNextPtr();
   }
   // Return the position before the larger value
   return position;
}
template<class Data> Node<Data>* LinkedList<Data>::Search(Data& data){
   Node<Data>* temp = this->GetHeadPtr();
   // Traverse through the list until the end or the value found
   while(temp != nullptr){
      if(temp->GetData() == data) return temp;
      temp = temp->GetNextPtr();
   }
   return nullptr;
}
template<class Data> void LinkedList<Data>::RecursivePrint(std::ostream& stream, Node<Data>* nodePtr) const{
   if(nodePtr != nullptr){
      // Print the data
      stream << *nodePtr << std::endl;
      // Recursively go to the next node
      RecursivePrint(stream, nodePtr->GetNextPtr());
   }
}
#endif // LINKEDLIST_H
