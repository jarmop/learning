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
    struct node * first = linkedList->getFirst();
    
    ASSERT_TRUE(first);
    ASSERT_EQ(first->key, 1);
    ASSERT_EQ(first->data, 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



