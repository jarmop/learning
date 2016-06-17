//
//  LinkedList.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 15/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include "LinkedList.hpp"

void LinkedList::insertFirst(int data) {
    struct node * node = new struct node;
    node->data = data;
    if (!this->head) {
        this->head = node;
        this->tail = node;
    } else {
        node->next = this->head;
        this->head = node;
    }
};

void LinkedList::insertLast(int data) {
    struct node * node = new struct node;
    node->data = data;
    if (!this->tail) {
        this->head = node;
        this->tail = node;
    } else {
        this->tail->next = node;
    }
};

struct node * LinkedList::getFirst() {
    return this->head;
}

struct node * LinkedList::getLast() {
    return this->tail;
}
