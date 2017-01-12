#include <iostream>
#include "SlabPool.h"
#include "gtest/gtest.h"

using namespace std;

class Int {
  public:
    Int *poolNext;
    Slab<Int> *slab;

  public:
    int data;
};

TEST(initTest, initTestCase) {
  SlabPool<Int> slabPool(100, 2);
}

TEST(allocTest, allocTestCase) {
  const int num = 500;
  Int **objs = new Int*[num];
  bool *isFree = new bool[num];

  for(int i = 0; i < num; i++) {
    isFree[i] = true;
  }

  SlabPool<Int> sp(20, 1);

  for(int count = 0; count < 5000 * num; count++) {
    int index = rand() % num;
    if(isFree[index]) {
      objs[index] = sp.alloc();
      isFree[index] = false;
    } else {
      sp.release(objs[index]);
      isFree[index] = true;
    }
  }

  for(int i = 0; i < num; i++) {
    if(!isFree[i]) {
      sp.release(objs[i]);
    }
  }

  /*
  cout << "allocCnt: " << sp.allocCnt << endl;
  cout << "releaseCnt: " << sp.releaseCnt << endl;
  cout << "sysNewCnt: " << sp.sysNewCnt << endl;
  cout << "sysFreeCnt: " << sp.sysFreeCnt << endl;
  */

  delete[] objs;
}

TEST(sysAllocTest, sysAllocTestCase) {
  const int num = 500;
  Int **objs = new Int*[num];
  bool *isFree = new bool[num];

  for(int i = 0; i < num; i++) {
    isFree[i] = true;
  }

  SlabPool<Int> sp(20, 1);

  for(int count = 0; count < 5000 * num; count++) {
    int index = rand() % num;
    if(isFree[index]) {
      objs[index] = new Int();
      isFree[index] = false;
    } else {
      delete objs[index];
      isFree[index] = true;
    }
  }

  for(int i = 0; i < num; i++) {
    if(!isFree[i]) {
      delete objs[i];
    }
  }

  delete[] objs;
}

/*
TEST(listInsertFirstTest, InsertFirestTestCase) {
  MiniList<Int> list;
  Int obj;
  Int obj1;

  list.insertFirst(&obj);
  EXPECT_EQ(obj.next, (void *)0);
  EXPECT_EQ(list.last(), &obj);
  EXPECT_EQ(obj.prev, (void *)0);
  EXPECT_EQ(list.first(), &obj);
  EXPECT_EQ(list.size(), 1);

  list.insertFirst(&obj1);
  EXPECT_EQ(obj1.next, &obj);
  EXPECT_EQ(obj.prev, &obj1);
  EXPECT_EQ(obj1.prev, (void *)0);
  EXPECT_EQ(list.first(), &obj1);
  EXPECT_EQ(list.size, 2); 
}

TEST(listInsertLastTest, InsertLastTestCase) {
  IntList list;
  Int obj;
  Int obj1;

  listInit(&list);  
  listInsertLast(&list, &obj);
  EXPECT_EQ(obj.prev, (void *)0); 
  EXPECT_EQ(list.head, &obj); 
  EXPECT_EQ(obj.next, (void *)0); 
  EXPECT_EQ(obj.owner, &list); 
  EXPECT_EQ(list.tail, &obj); 
  EXPECT_EQ(list.size, 1); 

  listInsertLast(&list, &obj1);
  EXPECT_EQ(obj1.prev, &obj); 
  EXPECT_EQ(obj.next, &obj1); 
  EXPECT_EQ(obj1.next, (void *)0); 
  EXPECT_EQ(obj1.owner, &list); 
  EXPECT_EQ(list.tail, &obj1); 
  EXPECT_EQ(list.size, 2); 
}
*/
// To do: add more unit test
