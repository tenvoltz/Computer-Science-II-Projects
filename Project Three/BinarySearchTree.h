#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include <iostream>
#include <vector>
#include "Node.h"
template<typename T> class BinarySearchTree{
   public:
      // Constructors
      BinarySearchTree<T>() {}
      ~BinarySearchTree<T>(){RecursiveClear(nullptr, root);}
      // Function
      bool IsEmpty(){return root == nullptr;}
      void Insert(T data){RecursiveInsert(root, data);}
      void Remove(T data){RecursiveRemove(nullptr, root, data);}
      T PopMin();
      T PopMax();
      bool Search(T data){return RecursiveSearch(root, data);}
      void Clear(){RecursiveClear(nullptr, root);}
      std::vector<T> GetVector();
      // Overloading Output
      friend std::ostream& operator<<(std::ostream& stream, const BinarySearchTree& bst){
         bst.RecursivePrint(stream, bst.root, "", "");
         return stream;
      }
   private:
      Node<T>* root = nullptr;
      // Helper Function
      void RecursiveInsert(Node<T>* node, T& data);
      void RecursiveRemove(Node<T>* parent, Node<T>* node, T& data);
      Node<T>* Detach(Node<T>* parent, Node<T>* node);
      Node<T>* RecursiveSearchLeft(Node<T>* &parent, Node<T>* node);
      Node<T>* RecursiveSearchRight(Node<T>* &parent, Node<T>* node);
      bool RecursiveSearch(Node<T>* node, T data);
      void RecursiveClear(Node<T>* parent, Node<T>* node);
      void RecursivePrint(std::ostream& stream, Node<T>* node, std::string prefix, std::string branch) const;
      void RecursiveGetVector(Node<T>* node, std::vector<T>& values);
};

template<typename T> void BinarySearchTree<T>::RecursiveInsert(Node<T>* node, T& data){
   // If root is empty, add the new node to root
   if(root == nullptr){root = new Node<T>(data);}
   else if(data < node->data){
      // If the left of current node is empty, add the new node as left child
      if(node->left == nullptr) node->left = new Node<T>(data);
      // The left is not empty and can be continue to be traversed
      else RecursiveInsert(node->left, data);
   }
   else if(data == node->data || data > node->data){
      // If the right of current node is empty, add the new node as right child
      if(node->right == nullptr) node->right = new Node<T>(data);
      // The right is not empty and can be continue to be traversed
      else RecursiveInsert(node->right, data);
   }
}

template<typename T> void BinarySearchTree<T>::RecursiveRemove(Node<T>* parent, Node<T>* node, T& data){
   // Cannot found the node
   if(node == nullptr){return;}
   // Continue to traverse left subtree
   if(data < node->data){ RecursiveRemove(node, node->left, data);}
   // Continue to traverse right subtree
   else if(data > node->data){ RecursiveRemove(node, node->right, data);}
   else if(data == node->data){
      // Found the target node, detaching it from its parent
      Node<T>* removed = Detach(parent, node);
      // Free the memory
      delete removed;
   }
}

template<typename T> Node<T>* BinarySearchTree<T>::Detach(Node<T>* parent, Node<T>* node){
   // If node has two children
   if(node->left != nullptr && node->right != nullptr){
      Node<T>* successorParent = node;
      Node<T>* successor = RecursiveSearchRight(successorParent, node->left);
      node->data = successor->data;
      return Detach(successorParent, successor);
   }
   // If node has one or two children
   else{
      // If detaching is at root
      if(parent == nullptr) root = node->left != nullptr ? node->left : node->right;
      // If detaching is from the left side
      else if(parent->left == node) parent->left = node->left != nullptr ? node->left : node->right;
      // If detaching from the right side
      else if(parent->right == node) parent->right = node->left != nullptr ? node->left : node->right;
      // Detach the target node and return it
      node->left = nullptr;
      node->right = nullptr;
      return node;
   }
}

template<typename T> T BinarySearchTree<T>::PopMin(){
   // Get the rightmost node in the tree
   Node<T>* parent = nullptr;
   Node<T>* node = RecursiveSearchLeft(parent, root);
   // Detach the node, deleting it while returning the value
   Node<T>* popped = Detach(parent, node);
   T data = popped->data;
   delete popped;
   return data;
}
template<typename T> T BinarySearchTree<T>::PopMax(){
   // Get the rightmost node in the tree
   Node<T>* parent = nullptr;
   Node<T>* node = RecursiveSearchRight(parent, root);
   // Detach the node, deleting it while returning the value
   Node<T>* popped = Detach(parent, node);
   T data = popped->data;
   delete popped;
   return data;
}
template<typename T> Node<T>* BinarySearchTree<T>::RecursiveSearchLeft(Node<T>* &parent, Node<T>* node){
   // Stop if the node is a root, and the tree is empty
   if(node == nullptr) return nullptr;
   // Stop if the node is a leave
   if(node->left == nullptr) return node;
   // Recursive search to the leftmost node
   return RecursiveSearchLeft(parent = node, node->left);
}
template<typename T> Node<T>* BinarySearchTree<T>::RecursiveSearchRight(Node<T>* &parent, Node<T>* node){
   // Stop if the node is a root, and the tree is empty
   if(node == nullptr) return nullptr;
   // Stop if the node is a leave
   if(node->right == nullptr) return node;
   // Recursive search to the rightmost node
   return RecursiveSearchRight(parent = node, node->right);
}

template<typename T> bool BinarySearchTree<T>::RecursiveSearch(Node<T>* node, T data){
   // Cannot found the node
   if(node == nullptr){return false;}
   // Continue to traverse left subtree
   else if(data < node->data){ return RecursiveSearch(node->left, data);}
   // Continue to traverse right subtree
   else if(data > node->data){ return RecursiveSearch(node->right, data);}
   // Found the node
   else if(data == node->data){ return true;}
}

template<typename T> void BinarySearchTree<T>::RecursiveClear(Node<T>* parent, Node<T>* node){
   if(node == nullptr) return;
   // Recurse through all the child first before deleting the parent
   RecursiveClear(node, node->left);
   RecursiveClear(node, node->right);
   // Cut off tie with parent
   if(parent == nullptr){root = nullptr;}
   else if(parent->left == node){parent->left = nullptr;}
   else if(parent->right == node){parent->right = nullptr;}
   // Deleting the node
   delete node;
}

template<typename T> std::vector<T> BinarySearchTree<T>::GetVector(){
   std::vector<T> results;
   // Call helper function
   RecursiveGetVector(root, results);
   return results;
}
template<typename T> void BinarySearchTree<T>::RecursiveGetVector(Node<T>* node, std::vector<T>& values){
   // Return if outside of tree
   if(node == nullptr) return;
   // In-order traversal to list out the item in increasing order
   RecursiveGetVector(node->left, values);
   values.push_back(node->data);
   RecursiveGetVector(node->right, values);
}

template<typename T> void BinarySearchTree<T>::RecursivePrint(std::ostream& stream, Node<T>* node, std::string prefix, std::string branch) const{
   if(node == nullptr) return;
   // Pretty print the tree
   RecursivePrint(stream, node->right, branch + "|--- ", branch + "     ");
   stream << prefix << node->data << "\n";
   RecursivePrint(stream, node->left,  branch + "|--- ", branch + "     ");
}
#endif // BINARYSEARCHTREE_H
