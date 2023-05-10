//
// Created by Evgeniya Zemlyanaya on 29.03.2023.
//

#include <iostream>
#include <map>
#include <mutex>
#include "BinaryTree.h"

using namespace std;


int main() {
    BinaryTree<int, int> tree = BinaryTree<int, int>();
    tree = tree.addNode(1, 2);
    tree = tree.addNode(4, 1);
    tree = tree.addNode(0, 2);
    tree.printTree(tree.root);
    return 0;
}