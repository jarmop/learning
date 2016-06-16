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

struct node {
    int key;
    int data;
    struct node * next;
};

class LinkedList {    
    struct node * head;
    struct node * tail;
public:
    void insertFirst(int key, int data);
    void insertLast(int key, int data);
    struct node * getFirst();
    struct node * getLast();
};

#endif /* LinkedList_hpp */
