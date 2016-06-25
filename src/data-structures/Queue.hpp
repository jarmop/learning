//
//  Queue.hpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 18/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#ifndef Queue_hpp
#define Queue_hpp

#include <stdio.h>

#include "LinkedList.hpp"

class Queue {
    LinkedList * list;
public:
    Queue();
    void push(int data);
    int pop();
};

#endif /* Queue_hpp */
