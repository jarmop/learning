//
//  LinkedList.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 15/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include "LinkedList.hpp"

void LinkedList::insertFirst(int key, int data) {
    struct node * node = new struct node;
    node->key = key;
    node->data = data;
    if (!this->head) {
        this->head = node;
        this->tail = node;
    } else {
        node->next = this->head;
        this->head = node;
    }
};

struct node * LinkedList::getFirst() {
    return this->head;
}
