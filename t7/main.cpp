//
// Created by Evgeniya Zemlyanaya on 03.05.2023.
//

#include "AVLTree.h"

using namespace std;

template<class K, class V>
size_t findTreeHeight(AVLTree<K, V> tree) {
    auto root = tree.root;

    if (root == nullptr)
        return 0;
    return max(findTreeHeight(AVLTree<K, V>(root->left)), findTreeHeight(AVLTree<K, V>(root->right))) + 1;
}

int main()
{
    AVLTree<int, int> tree = AVLTree<int, int>();

    for (int i = 0; i < 10000; i++) {
        tree = tree.addNode(i, 0);
    }
    cout << findTreeHeight(tree);
//    tree.printTree(tree.root);

    std::cout << std::endl;

    for (int i = 0; i < 10000; i+=2) {
        tree = tree.removeNode(i);
    }
    tree.printTree(tree.root);
    cout << findTreeHeight(tree);

    std::cout << std::endl;

    return 0;
}