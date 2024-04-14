#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>

// Entry of a hash map with template key and template value
template<typename K, typename V> struct Entry{
   K key; V value;
   Entry() : key(), value(){}
   Entry(K _key, V _value) : key(_key), value(_value){}
   ~Entry(){}//{std::cout << "Delete " << *this << std::endl;}
   template<typename _K, typename _V>
   friend std::ostream& operator<<(std::ostream& stream, const Entry<_K, _V>& entry);
};
template<typename _K, typename _V>
std::ostream& operator<<(std::ostream& stream, const Entry<_K, _V>& entry){
   stream << "Key: " << std::setw(10) << std::left << entry.key;
   stream << "Value:" << std::setw(10) << std::left << entry.value;
   return stream;
}

// State of a hash map's bucket
enum State{NOENTRY = 1, REMOVED = 2, FILLED = 4, EMPTY = NOENTRY | REMOVED};
// Bucket of a hash map with its state and template entry
template<typename K, typename V> struct Bucket{
   State state;
   Entry<K, V>* entry;
   Bucket() : state(State::NOENTRY), entry(nullptr){} // A default bucket is empty
   Bucket(Entry<K, V>* _entry) : state(State::FILLED), entry(_entry){}
   ~Bucket(){delete entry;}
   template<typename _K, typename _V>
   friend std::ostream& operator<<(std::ostream& stream, const Bucket<_K, _V>& bucket);
};
template<typename _K, typename _V>
std::ostream& operator<<(std::ostream& stream, const Bucket<_K, _V>& bucket){
   if(bucket.state == State::FILLED){
      if(bucket.entry == nullptr) return stream << "Bucket:  FILLED | NULL";
      else return stream << "Bucket:  FILLED | " << *(bucket.entry);
   }
   else if(bucket.state == State::REMOVED) stream << "Bucket: REMOVED |";
   else if(bucket.state == State::NOENTRY) stream << "Bucket:   EMPTY |";
   return stream;
}

template<typename Derived, typename K, typename V> class BaseHashMap
{
   public:
      // Constructors
      BaseHashMap(){Resize(11);}
      ~BaseHashMap(){Clear();}
      // Capacity
      bool IsEmpty() const {return m_size == 0;}
      int GetSize() const {return m_size;}
      // Modifiers
      Bucket<K, V>& Insert(K key, V value);
      Bucket<K, V>& Find(const K& key);
      Entry<K, V> Remove(const K& Key);
      void Clear();
      // Accessors
      bool Contain(const K& key);
      V& At(const K& key);
      V& operator[](const K& key);
      std::set<Entry<K, V>> EntrySet();
      std::set<K> KeySet();
      std::set<V> ValueSet();
      template<typename _Derived, typename _K, typename _V>
      friend std::ostream& operator<<(std::ostream& stream, const BaseHashMap<_Derived, _K, _V>& baseHashMap);
      // Hashing
      double GetLoadFactor() const {return (double) m_size / m_maxSize;}
      void SetMaxLoadFactor(double value){ m_maxLoadFactor = value; if(IsAboveThreshold()) Rehash();}
      double GetMaxLoadFactor() const{return m_maxLoadFactor;}
      bool IsAboveThreshold() const { return GetLoadFactor() >= GetMaxLoadFactor();}
      int GetHashCode(const K& key){ return static_cast<Derived*>(this)->Hashing(key); }
      int PrimaryHashing(const K& key);
      int SecondaryHashing(const K& key);
      void Rehash();

   protected:
      // Member Variables
      int m_size = 0;
      int m_maxSize = 11;
      double m_maxLoadFactor = 0.5;
      std::vector<Bucket<K, V>> m_buckets;
      // Prime Helper
      static int m_previousPrime;
      static int GetNextPrimeAbove(int num);
      static bool IsPrime(int num);
      // Non-public Function
      void Resize(int amount);
};

template<typename Derived, typename K, typename V>
int BaseHashMap<Derived, K, V>::m_previousPrime = 7;

// Modifiers -----------------------------------------
template<typename Derived, typename K, typename V>
Bucket<K, V>& BaseHashMap<Derived, K, V>::Insert(K key, V value){
   // Checking if the hash map need a rehash
   if(IsAboveThreshold()) Rehash();
   // Double-hashing until reaching an empty bucket
   int primaryIndex = PrimaryHashing(key);
   int secondaryIndex = SecondaryHashing(key);
   int probeAmount = 0;
   while(m_buckets.at(primaryIndex).state == State::FILLED &&
         probeAmount < m_maxSize){
      primaryIndex += secondaryIndex;
      primaryIndex %= m_maxSize;
      probeAmount++;
   }
   // Filled the bucket with entry and return the bucket
   m_size++;
   m_buckets.at(primaryIndex).state = State::FILLED;
   m_buckets.at(primaryIndex).entry = new Entry<K, V>(key, value);
   return m_buckets.at(primaryIndex);
}
template<typename Derived, typename K, typename V>
Bucket<K, V>& BaseHashMap<Derived, K, V>::Find(const K& key){
   // Double-hashing until finding the right bucket
   int primaryIndex = PrimaryHashing(key);
   int secondaryIndex = SecondaryHashing(key);
   int probeAmount = 0;
   // Stop when reaching an empty-since-start bucket
   while (m_buckets.at(primaryIndex).state != State::NOENTRY &&
          probeAmount < m_maxSize){
      if(m_buckets.at(primaryIndex).state == State::FILLED &&
         m_buckets.at(primaryIndex).entry->key == key) break;
      primaryIndex += secondaryIndex;
      primaryIndex %= m_maxSize;
      probeAmount++;
   }
   return m_buckets.at(primaryIndex);
}
template<typename Derived, typename K, typename V>
Entry<K, V> BaseHashMap<Derived, K, V>::Remove(const K& key){
   // Find the bucket with the specified key
   Bucket<K, V>& result = Find(key);
   // If the bucket is the right bucket
   if(result.entry && result.entry->key == key){
      // Empty the bucket out
      Entry<K, V> tempCopy = *result.entry;
      result.state = State::REMOVED;
      delete result.entry;
      result.entry = nullptr;
      m_size--;
      // Return the entry;
      return tempCopy;
   }
   // Return a default entry if the key does not exist
   else return Entry<K, V>();
}
template<typename Derived, typename K, typename V>
void BaseHashMap<Derived, K, V>::Clear(){
   // Iterate though all the bucket and empty them out
   for(int i = 0; i < m_maxSize; i++){
      m_buckets.at(i).state = State::NOENTRY;
      delete m_buckets.at(i).entry;
      m_buckets.at(i).entry = nullptr;
   }
}
// Accessors -----------------------------------------
template<typename Derived, typename K, typename V>
bool BaseHashMap<Derived, K, V>::Contain(const K& key){
   // Find the bucket with the specified key
   Bucket<K, V>& result = Find(key);
   // Return true if the bucket is the right one, false otherwise
   return result.entry ? result.entry->key == key : false;
}

template<typename Derived, typename K, typename V>
V& BaseHashMap<Derived, K, V>::At(const K& key){
   // Find the bucket with the specified key
   Bucket<K, V>& result = Find(key);
   // If the bucket is the right bucket, return the value in the entry
   if(result.entry && result.entry->key == key){
      return result.entry->value;
   }
   // Throws an exception if the key does not exist
   else throw std::out_of_range("The key does not exist");
}
template<typename Derived, typename K, typename V>
V& BaseHashMap<Derived, K, V>::operator[](const K& key){
   // Find the bucket with the specified key
   Bucket<K, V>& result = Find(key);
   // If the bucket is the right bucket, return the value in the entry
   if(result.entry && result.entry->key == key){
      return result.entry->value;
   }
   // Create a new bucket with the key and add it to hash map
   else{ return Insert(key, V()).entry->value; }
}
template<typename Derived, typename K, typename V>
std::set<Entry<K, V>> BaseHashMap<Derived, K, V>::EntrySet(){
   // Return all the entry in a set
   std::set<Entry<K, V>> entrySet;
   for(int i = 0; i < m_maxSize; i++){
      if(m_buckets.at(i).state == State::FILLED)
         entrySet.insert(*m_buckets.at(i).entry);
   }
   return entrySet;
}
template<typename Derived, typename K, typename V>
std::set<K> BaseHashMap<Derived, K, V>::KeySet(){
   // Return all the key in a set
   std::set<K> keySet;
   for(int i = 0; i < m_maxSize; i++){
      if(m_buckets.at(i).state == State::FILLED)
         keySet.insert(m_buckets.at(i).entry->key);
   }
   return keySet;
}
template<typename Derived, typename K, typename V>
std::set<V> BaseHashMap<Derived, K, V>::ValueSet(){
   // Return all the value in a set
   std::set<V> valueSet;
   for(int i = 0; i < m_maxSize; i++){
      if(m_buckets.at(i).state == State::FILLED)
         valueSet.insert(m_buckets.at(i).entry->value);
   }
   return valueSet;
}
template<typename _Derived, typename _K, typename _V>
std::ostream& operator<<(std::ostream& stream, const BaseHashMap<_Derived, _K, _V>& baseHashMap){
   // Print all the bucket
   for(int i = 0; i < baseHashMap.m_maxSize; i++){
      stream << baseHashMap.m_buckets.at(i) << std::endl;
   }
   return stream;
}

// Hashing
template<typename Derived, typename K, typename V>
int BaseHashMap<Derived, K, V>::PrimaryHashing(const K& key){
   // Primary hash function = code % size
   // Return a hash code within the range of the hash map's capacity
   int hashcode = GetHashCode(key) % this->m_maxSize;
   return hashcode + (hashcode < 0 ? this->m_maxSize : 0);
}
template<typename Derived, typename K, typename V>
int BaseHashMap<Derived, K, V>::SecondaryHashing(const K& key){
   // Secondary hash function = prime - code % size
   // Return a hash code within the range of the hash map's capacity
   int hashcode = GetHashCode(key) % this->m_maxSize;
   hashcode += (hashcode < 0 ? this->m_maxSize : 0);
   return m_previousPrime - hashcode % m_previousPrime;
}
template<typename Derived, typename K, typename V>
void BaseHashMap<Derived, K, V>::Rehash(){
   // Create empty bucket
   std::vector<Bucket<K, V>> buckets;
   // Swap the buckets
   m_buckets.swap(buckets);
   m_previousPrime = m_maxSize;
   m_size = 0;
   // Resize the capacity of buckets
   Resize(GetNextPrimeAbove(m_maxSize * 2));
   // Transfer all bucket from current hash map to the new hash map
   for(int i = 0; i < m_previousPrime; i++){
      if(buckets.at(i).state == State::FILLED)
         Insert(buckets.at(i).entry->key,
                buckets.at(i).entry->value);
   }
}
// Prime Helper ----------------------------------------
template<typename Derived, typename K, typename V>
int BaseHashMap<Derived, K, V>::GetNextPrimeAbove(int num){
   // Iterate one-by-one, checking if it is prime
   while(!IsPrime(++num)){}
   return num;
}
template<typename Derived, typename K, typename V>
bool BaseHashMap<Derived, K, V>::IsPrime(int number){
    // Check for prime that is one away from multiple of 6
    if (number == 2 || number == 3)
        return true;
    if (number % 2 == 0 || number % 3 == 0)
        return false;
    for(int divisor = 6; !(divisor * divisor > number); divisor += 6){
        if (number % (divisor - 1) == 0)
            return false;
        if (number % (divisor + 1) == 0)
            return false;
    }
    return true;
}
// Non-public Function ---------------------------------
template<typename Derived, typename K, typename V>
void BaseHashMap<Derived, K, V>::Resize(int amount){
   // Resize the capacity for bucket
   m_maxSize = amount;
   m_buckets.resize(amount);
}

// Curiously Recurring Template Pattern - Specializing template for getting the right hash code
template <typename K, typename V> class HashMap : public BaseHashMap<HashMap<K, V>, K, V>{
   public:
      HashMap() : BaseHashMap<HashMap<K, V>, K, V>(){}
      // Hash code = address of Object
      int Hashing(const K& key){ return reinterpret_cast<std::uintptr_t>(&key); }
};
template <typename V> class HashMap<int, V>: public BaseHashMap<HashMap<int, V>, int, V> {
   public:
      HashMap() : BaseHashMap<HashMap<int, V>, int, V>(){}
      // Hash code = the value of the integer
      int Hashing(int key){ return key; }
};
template <typename V> class HashMap<double, V>: public BaseHashMap<HashMap<double, V>, double, V> {
   public:
      HashMap() : BaseHashMap<HashMap<double, V>, double, V>(){}
      // Hash code = upper 32-bit XOR lower 32-bit
      int Hashing(double key){ return (int) key ^ (int)(((long long) key) >> 32); }
};
template <typename V> class HashMap<std::string, V>: public BaseHashMap<HashMap<std::string, V>, std::string, V> {
   public:
      HashMap() : BaseHashMap<HashMap<std::string, V>, std::string, V>(){}
      int Hashing(const std::string& key);
   private:
};
template <typename V> int HashMap<std::string, V>::Hashing(const std::string& key){
   // Hash code = Sum of (ASCII * prime 31 ^ index)
   int hashcode = 0;
   for(int ASCII : key){ hashcode = (hashcode + ASCII) * 31; }
   return hashcode;
}


#endif // HASHMAP_H
