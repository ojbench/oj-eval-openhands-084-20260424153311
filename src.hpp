//
// Created by wzj on 2021/12/3.
// Modified by Yifei Ma on 2023/10/25.
//

#ifndef FINAL_SRC_HPP
#define FINAL_SRC_HPP

#include <string>
#include <vector>

// If the two following commands bother you, you may comment them and write
// std::vector and std::string instead.
using std::string;
using std::vector;

struct LinkedHashMap;
struct Node {
  Node* next = nullptr;
  Node* timePre = nullptr;
  Node* timeNext = nullptr;

  int key = 0;
  string value;
};

struct Data {
  int key;
  string value;
};

vector<Data> traverseByTime(const LinkedHashMap& obj);
vector<Data> traverseByIndex(const LinkedHashMap& obj);

struct LinkedHashMap {
  // You can modify beg, cur and related parts in the constructor as you wish.
  // 你可以根据自己写链表的习惯修改或删除头尾节点并修改构造函数。

  int len;
  Node** array;
  Node* beg;
  Node* cur;
  // TODO: define a function pointer named "forEachMethod"
  // to be pointed to one of the functions "traverseByTime" or "traverseByDict".
  // 定义一个名为 forEachMethod 的函数指针，用于指向 travelByTime 或
  // travelByDict 函数。
  vector<Data> (*forEachMethod)(const LinkedHashMap&);

  void init(int len_, bool forEachByTime) {
    len = len_;
    beg = new Node();
    cur = beg;
    array = new Node*[len];
    for (int i = 0; i < len; i++) {
      array[i] = nullptr;
    }
    if (forEachByTime)
      forEachMethod = traverseByTime;
    else
      forEachMethod = traverseByIndex;
  }

  void clearMemory() {
    // Traverse through the time-based linked list and delete all nodes
    Node* current = beg->timeNext;
    while (current != nullptr) {
      Node* temp = current;
      current = current->timeNext;
      delete temp;
    }
    delete beg;
    delete[] array;
  }

  void insert(int key, string value) {
    // Create a new node
    Node* newNode = new Node();
    newNode->key = key;
    newNode->value = value;
    
    // Insert at the head of array[key] linked list
    newNode->next = array[key];
    array[key] = newNode;
    
    // Insert at the tail of the time-based doubly linked list
    newNode->timePre = cur;
    newNode->timeNext = nullptr;
    cur->timeNext = newNode;
    cur = newNode;
  }

  void remove(int key, string value) {
    // Remove from array[key] linked list
    Node* prev = nullptr;
    Node* current = array[key];
    
    while (current != nullptr) {
      if (current->value == value) {
        // Remove from hash chain
        if (prev == nullptr) {
          array[key] = current->next;
        } else {
          prev->next = current->next;
        }
        
        // Remove from time-based doubly linked list
        if (current->timePre != nullptr) {
          current->timePre->timeNext = current->timeNext;
        }
        if (current->timeNext != nullptr) {
          current->timeNext->timePre = current->timePre;
        } else {
          // This was the last node (cur)
          cur = current->timePre;
        }
        
        Node* temp = current;
        current = current->next;
        delete temp;
      } else {
        prev = current;
        current = current->next;
      }
    }
  }

  vector<string> ask(int key) const {
    vector<string> result;
    Node* current = array[key];
    while (current != nullptr) {
      result.push_back(current->value);
      current = current->next;
    }
    return result;
  }

  vector<Data> forEach() const { return forEachMethod(*this); }
};

vector<Data> traverseByTime(const LinkedHashMap& obj) {
  vector<Data> result;
  Node* current = obj.beg->timeNext;
  while (current != nullptr) {
    Data data;
    data.key = current->key;
    data.value = current->value;
    result.push_back(data);
    current = current->timeNext;
  }
  return result;
}

vector<Data> traverseByIndex(const LinkedHashMap& obj) {
  vector<Data> result;
  for (int i = 0; i < obj.len; i++) {
    Node* current = obj.array[i];
    while (current != nullptr) {
      Data data;
      data.key = current->key;
      data.value = current->value;
      result.push_back(data);
      current = current->next;
    }
  }
  return result;
}

#endif  // FINAL_SRC_HPP
