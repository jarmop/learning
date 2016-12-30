//
//  Stack.hpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 17/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <stdio.h>
#include "LinkedList.hpp"

class Stack {
    LinkedList * list;
public:
    Stack();
    void push(int data);
    int pop();
};

#endif /* Stack_hpp */
