//
//  main.cpp
//  data-structures-and-algorithms
//
//  Created by Jarmo Pihlajaniemi on 14/06/16.
//  Copyright © 2016 Jarmo Pihlajaniemi. All rights reserved.
//

#include <iostream>
#include "LinkedList.hpp"
#include "gtest/gtest.h"

using namespace std;


int main(int argc, const char * argv[]) {
    LinkedList *linkedList = new LinkedList();
    
    cout << linkedList->getTest() << endl;
    cout << "test" << endl;
    
    return 0;
}



