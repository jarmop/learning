//
//  Queue.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 18/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include "Queue.hpp"

Queue::Queue() {
    this->list = new LinkedList;
}

void Queue::push(int data) {
    this->list->insertLast(data);
}

int Queue::pop() {
    int data = this->list->getFirst();
    this->list->removeFirst();
    return data;
}