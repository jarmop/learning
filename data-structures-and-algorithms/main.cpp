//
//  main.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 14/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include <iostream>
#include "LinkedList.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(LinkedListTest, insertFirst) {
    LinkedList *linkedList = new LinkedList;
    linkedList->insertFirst(1, 2);
    linkedList->insertFirst(2, 5);
    struct node * last = linkedList->getLast();
    
    ASSERT_TRUE(last);
    ASSERT_EQ(last->key, 1);
    ASSERT_EQ(last->data, 2);
}

TEST(LinkedListTest, insertLast) {
    LinkedList *linkedList = new LinkedList;
    linkedList->insertLast(1, 2);
    linkedList->insertLast(2, 5);
    struct node * first = linkedList->getFirst();
    
    ASSERT_TRUE(first);
    ASSERT_EQ(first->key, 1);
    ASSERT_EQ(first->data, 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



