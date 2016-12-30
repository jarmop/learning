//
//  LinkedList.hpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 15/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#ifndef LinkedList_hpp
#define LinkedList_hpp

#include <stdio.h>
#include "Node.hpp"

class LinkedList {
    unsigned int size = 0;
    Node * head;
    Node * tail;
public:
    void insertFirst(int data);
    void insertLast(int data);
    void removeFirst();
    int getFirst();
    int getLast();
};

#endif /* LinkedList_hpp */
