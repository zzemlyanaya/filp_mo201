//
// Created by Evgeniya Zemlyanaya on 03.05.2023.
//

#include "Treap.h"

int main()
{
    Tree<int, int> tree = Tree<int, int>();

    tree = tree.addNode(3, 1, 8);
    tree = tree.addNode(2, 1, 7);
    tree = tree.addNode(1, 1, 9);
    tree = tree.addNode(0, 1, 6);
    tree = tree.addNode(-1, 1, 8);
    tree = tree.addNode(-3, 1, 5);
    tree = tree.addNode(4, 1, 5);

    tree.PrintTree();
    std::cout << std::endl << tree.checkCorrect(tree.root) << std::endl;

    tree = tree.removeNode(0);

    tree.PrintTree();
    std::cout << std::endl << tree.checkCorrect(tree.root) << std::endl;

    return 0;
}
