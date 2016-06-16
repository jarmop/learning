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

TEST(LinkedListTest, add) {
    LinkedList *linkedList = new LinkedList();
    
    ASSERT_EQ(5, 5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



