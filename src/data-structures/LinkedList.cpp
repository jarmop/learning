//
//  LinkedList.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 15/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include "LinkedList.hpp"

void LinkedList::insertFirst(int data) {
    Node* node = new Node;
    node->data = data;
    if (this->size == 0) {
        this->head = node;
        this->tail = node;
    } else {
        node->next = this->head;
        this->head = node;
    }
    this->size++;
};

void LinkedList::insertLast(int data) {
    Node* node = new Node;
    node->data = data;
    if (this->size == 0) {
        this->head = node;
        this->tail = node;
    } else {
        this->tail->next = node;
        this->tail = node;
    }
    this->size++;
};

void LinkedList::removeFirst() {
    if (this->size > 0) {
        Node * oldHead = this->head;
        this->head = oldHead->next;
        delete oldHead;
    }
}

int LinkedList::getFirst() {
    return this->head->data;
}

int LinkedList::getLast() {
    return this->tail->data;
}
