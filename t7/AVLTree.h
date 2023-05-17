//
// Created by Evgeniya Zemlyanaya on 03.05.2023.
//

#ifndef FILP_MO201_AVLTREE_H
#define FILP_MO201_AVLTREE_H

#include<iostream>
#include <memory>

using namespace std;

// https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm

template<class K, class V>
class AVLTree {
    struct Node {
        K key;
        V value;
        int height;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(K key, V value, int height) {
            this->key = key;
            this->value = value;
            this->height = height;
        }

        Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right, K key, V value, int height) {
            this->left = left;
            this->right = right;

            this->key = key;
            this->value = value;
            this->height = height;
        }
    };

    struct Trunk
    {
        std::shared_ptr<Trunk> prev;
        string str;

        Trunk(std::shared_ptr<Trunk> prev, string str)
        {
            this->prev = prev;
            this->str = str;
        }
    };

    void showTrunks(std::shared_ptr<Trunk> p)
    {
        if (p == nullptr) {
            return;
        }

        showTrunks(p->prev);
        cout << p->str;
    }

    void printTree(std::shared_ptr<Node> root, std::shared_ptr<Trunk> prev, bool isLeft)
    {
        if (root == nullptr) {
            return;
        }

        string prev_str = "       ";
        auto trunk = std::make_shared<Trunk>(prev, prev_str);

        printTree(root->right, trunk, true);

        if (!prev) {
            trunk->str = "------";
        }
        else if (isLeft)
        {
            trunk->str = " .------";
            prev_str = "       |";
        }
        else {
            trunk->str = " `------";
            prev->str = prev_str;
        }

        showTrunks(trunk);
        cout << "(" << root->key << ")" << endl; // << " [value: " << root->value << "]" << endl;

        if (prev) {
            prev->str = prev_str;
        }
        trunk->str = "       |";

        printTree(root->left, trunk, false);
    }

    int getHeight(std::shared_ptr<Node> node) {
        return node == nullptr ? 0 : node->height;
    }

    int balancedHeight(std::shared_ptr<Node> node) {
        return std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    int getBalance(std::shared_ptr<Node> node) {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    std::shared_ptr<Node> rotateRight(std::shared_ptr<Node> node) {
        auto newParent = std::make_shared<Node>(node->left->key, node->left->value, 1);
        auto newRight = std::make_shared<Node>(node->key, node->value, 1);

        newParent->right = newRight;
        newParent->left = node->left->left;

        newRight->left = node->left->right;
        newRight->right = node->right;

        newRight->height = balancedHeight(newRight);
        newParent->height = balancedHeight(newParent);
        return newParent;
    }

    std::shared_ptr<Node> rotateLeft(std::shared_ptr<Node> node) {
        auto newParent = std::make_shared<Node>(node->right->key, node->right->value, 1);
        auto newLeft = std::make_shared<Node>(node->key, node->value, 1);

        newParent->left = newLeft;
        newParent->right = node->right->right;

        newLeft->right = node->right->left;
        newLeft->left = node->left;

        newLeft->height = balancedHeight(newLeft);
        newParent->height = balancedHeight(newParent);
        return newParent;
    }

    std::shared_ptr<Node> add(std::shared_ptr<Node> node, K key, V value) {
        if (node == nullptr) {
            return std::make_shared<Node>(key, value, 1);
        }

        if (node->value == value && node->key == key) {
            return node;
        }

        if (node->key == key) {
            return std::make_shared<Node>(node->left, node->right, key, value, node->height);
        }

        std::shared_ptr<Node> newNode = std::make_shared<Node>(node->key, node->value, node->height);
        if (node->key > key) {
            newNode->left = add(node->left, key, value);
            newNode->right = node->right;
        }
        else {
            newNode->left = node->left;
            newNode->right = add(node->right, key, value);
        }
        node = newNode;
        node->height = balancedHeight(node);

        // -------------------балансировка дерева------------------
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rotateRight(node);

        if (balance < -1 && key > node->right->key)
            return rotateLeft(node);

        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    std::shared_ptr<Node> remove(std::shared_ptr<Node> node, K key) {
        auto removed = removeUnbalanced(node, key);

        // -------------------балансировка дерева------------------
        int balance = getBalance(removed);

        if (balance > 1 && getBalance(removed->left) >= 0)
            return rotateRight(removed);

        if (balance < -1 && getBalance(removed->right) <= 0)
            return rotateLeft(removed);

        if (balance > 1 && getBalance(removed->left) < 0) {
            removed->left = rotateLeft(node->left);
            return rotateRight(removed);
        }
        if (balance < -1 && getBalance(removed->right) > 0) {
            removed->right = rotateRight(removed->right);
            return rotateLeft(removed);
        }
        return removed;
    }

    std::shared_ptr<Node> removeUnbalanced(std::shared_ptr<Node> node, K key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key > key) {
            return std::make_shared<Node>(remove(node->left, key), node->right, node->key, node->value, node->height);
        }

        if (node->key < key) {
            return std::make_shared<Node>(node->left, remove(node->right, key), node->key, node->value, node->height);
        }

        if (node->key == key) {
            if (node->left == nullptr && node->right == nullptr) {
                return nullptr;
            }
            else if (node->left == nullptr) {
                return node->right;
            }
            else if (node->right == nullptr) {
                return node->left;
            }
            else {
                auto deleted = findRightestNode(node->left);
                auto deletedTree = remove(node, deleted->key);
                deletedTree->key = deleted->key;
                return deletedTree;
            }
        }


    }

    std::shared_ptr<Node> findRightestNode(std::shared_ptr<Node> node) {
        if (node->right == nullptr) return node;

        return findRightestNode(node->right);
    }

public:

    std::shared_ptr<Node> root;

    AVLTree() {}

    AVLTree(K key, V value) {
        root = std::make_shared<Node>(key, value, 1);
    }

    AVLTree(std::shared_ptr<Node> node) {
        root = node;
    }

    AVLTree(std::initializer_list<std::pair<K, V>> list) {
        for (auto el : list) {
            root = add(root, el.first, el.second);
        }
    }

    AVLTree addNode(K key, V value) {
        return AVLTree(add(root, key, value));
    }

    AVLTree removeNode(K key) {
        return AVLTree(remove(root, key));
    }

    void printTree(std::shared_ptr<Node> node) {
        printTree(this->root, nullptr, false);
    }
};


#endif //FILP_MO201_AVLTREE_H
