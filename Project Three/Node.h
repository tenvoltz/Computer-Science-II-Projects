#ifndef NODE_H
#define NODE_H
template<typename T> class BinarySearchTree;

template<typename T> class Node
{
   friend class BinarySearchTree<T>;
   public:
      // Constructors
      Node<T>() {}
      Node<T>(T _data){data = _data;}
      ~Node<T>() {
         //std::cout << "Deleting " << data << std::endl;
      }
   private:
      T data;
      Node<T>* left = nullptr;
      Node<T>* right = nullptr;
};

#endif // NODE_H
