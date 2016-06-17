//
//  Stack.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 17/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include "Stack.hpp"

Stack::Stack(void) {
    this->list = new LinkedList;
}

void Stack::push(int data) {
    this->list->insertFirst(data);
}

int Stack::pop() {
    int data = this->list->getFirst();
    this->list->removeFirst();
    
    return data;
}