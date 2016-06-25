//
//  main.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 14/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include <iostream>

#include "gtest/gtest.h"

#include "LinkedList.hpp"
#include "Stack.hpp"
#include "Queue.hpp"

using namespace std;

TEST(LinkedListTest, insertFirst) {
    LinkedList * linkedList = new LinkedList;
    linkedList->insertFirst(1);
    linkedList->insertFirst(2);
    ASSERT_EQ(linkedList->getFirst(), 2);
}

TEST(LinkedListTest, insertLast) {
    LinkedList* linkedList = new LinkedList;
    linkedList->insertFirst(1);
    linkedList->insertLast(2);
    ASSERT_EQ(linkedList->getLast(), 2);
}

TEST(StackTest, pushAndPop) {
    Stack* stack = new Stack;
    stack->push(1);
    stack->push(2);
    ASSERT_EQ(stack->pop(), 2);
    ASSERT_EQ(stack->pop(), 1);
}

TEST(QueueTest, pushAndPop) {
    Queue* queue = new Queue;
    queue->push(1);
    queue->push(2);
    ASSERT_EQ(queue->pop(), 1);
    ASSERT_EQ(queue->pop(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



