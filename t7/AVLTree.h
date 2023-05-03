//
// Created by Evgeniya Zemlyanaya on 03.05.2023.
//

#ifndef FILP_MO201_AVLTREE_H
#define FILP_MO201_AVLTREE_H

#include<iostream>
#include <memory>

// https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm

template<class K, class V> struct AVLTree {
    struct Node {
        K key;
        V value;
        std::size_t height;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(K key, V value, std::size_t height) {
            this->key = key;
            this->value = value;
            this->height = height;
        }

        Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right, K key, V value, std::size_t height) {
            this->left = left;
            this->right = right;

            this->key = key;
            this->value = value;

            this->height = height;
        }
    };

    std::shared_ptr<Node> root;

    AVLTree() {}

    AVLTree(K key, V value) {
        root = std::make_shared<Node>(key, value);
    }

    AVLTree(std::shared_ptr<Node> node) {
        root = node;
    }

    AVLTree(std::initializer_list<std::pair<K, V>> list) {
        for (auto el : list) {
            root = add(root, el.first, el.second);
        }
    }

    std::shared_ptr<Node> add(std::shared_ptr<Node> node, K key, V value) {
        if (node == nullptr) {
            return std::make_shared<Node>(key, value);
        }

        if (node->value == value && node->key == key) {
            return node;
        }

        if (node->key == key) {
            return std::make_shared<Node>(node->left, node->right, key, value, node->height);
        }

        auto newNode = std::make_shared<Node>(key, value, node->height);

        if (node->key > key) {
            newNode->left = add(node->left, key, value);
            newNode->right = node->right;
        }
        else {
            newNode->left = node->left;
            newNode->right = add(node->right, key, value);
        }

        node = newNode;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // start balancing avl tree
        std::size_t balance = getHeight(node->left) - getHeight(node->right);
        if (balance > 1 && key < node->left->key) {
            // rotate right
            auto newParent = std::make_shared<Node>(node->left->key, node->left->value, 1);
            auto newRight = std::make_shared<Node>(node->key, node->value, 1);

            newRight->right = node->right;
            newRight->left = node->left->right;
            newRight->height = balancedHeight(newRight);

            newParent->right = newRight;
            newParent->left = node->left->left;
            newParent->height = balancedHeight(newParent);

            return newParent;

        }
        if (balance > 1 && key > node->left->key) {

        }

        if (balance < -1 && key < node->right->key) {

        }

        if (balance < -1 && key > node->right->key) {
            // rotate left
            auto newParent = std::make_shared<Node>(node->right->key, node->right->value, 1);
            auto newLeft = std::make_shared<Node>(node->key, node->value, 1);

            newLeft->left = node->left;
            newLeft->right = node->right->left;
            newLeft->height = balancedHeight(newLeft);

            newParent->left = newLeft;
            newParent->right = node->right->right;
            newParent->height = balancedHeight(newParent);

            return newParent;
        }
    }

    std::size_t getHeight(std::shared_ptr<Node> node) {
        return node == nullptr ? 0 : node->height;
    }

    std::size_t balancedHeight(std::shared_ptr<Node> node) {
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    std::shared_ptr<Node> remove(std::shared_ptr<Node> node, K key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key > key) {
            return std::make_shared<Node>(remove(node, key), node->right, node->key, node->value);
        }

        if (node->key < key) {
            return std::make_shared<Node>(node->left, remove(node, key), node->key, node->value);
        }

        if (node->key == key) {
            return nullptr;
        }
    }

    AVLTree addNode(K key, V value) {
        return AVLTree(add(root, key, value));
    }

    AVLTree removeNode(K key) {
        return AVLTree(remove(root, key));
    }

    void printTree(std::shared_ptr<Node> node) {
        if (node == nullptr) {
            std::cout << "EON" << std::endl;
            return;
        }

        std::cout << node->key << " - " << node->value << std::endl;

        this->printTree(node->left);
        this->printTree(node->right);
    }
};


#endif //FILP_MO201_AVLTREE_H
