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
    ASSERT_EQ(linkedList->getLast(), 1);
}

TEST(LinkedListTest, insertLast) {
    LinkedList *linkedList = new LinkedList;
    linkedList->insertLast(1);
    linkedList->insertLast(2);
    ASSERT_EQ(linkedList->getFirst(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



