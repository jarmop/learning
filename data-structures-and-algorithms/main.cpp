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
    linkedList->insertFirst(1);
    linkedList->insertFirst(2);
    struct node * last = linkedList->getLast();
    
    ASSERT_TRUE(last);
    ASSERT_EQ(last->data, 1);
}

TEST(LinkedListTest, insertLast) {
    LinkedList *linkedList = new LinkedList;
    linkedList->insertLast(1);
    linkedList->insertLast(2);
    struct node * first = linkedList->getFirst();
    
    ASSERT_TRUE(first);
    ASSERT_EQ(first->data, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



