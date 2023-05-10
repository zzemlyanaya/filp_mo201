//
// Created by Evgeniya Zemlyanaya on 05.04.2023.
//

#ifndef FILP_MO201_BINARYTREE_H
#define FILP_MO201_BINARYTREE_H

#include<new>
#include <memory>

using namespace std;

template<class K, class V> struct BinaryTree {
    struct Node {
        K key;
        V value;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(K key, V value) {
            this->key = key;
            this->value = value;
        }

        Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right, K key, V value) {
            this->left = left;
            this->right = right;

            this->key = key;
            this->value = value;
        }
    };

    std::shared_ptr<Node> root;

    BinaryTree() {}

    BinaryTree(K key, V value) {
        root = std::make_shared<Node>(key, value);
    }

    BinaryTree(std::shared_ptr<Node> node) {
        root = node;
    }

    BinaryTree(std::initializer_list<std::pair<K, V>> list) {
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
            return std::make_shared<Node>(key, value);
        }

        if (node->key > key) {
            std::shared_ptr<Node> newNode = std::make_shared<Node>(node->key, node->value);

            newNode->left = add(node->left, key, value);
            newNode->right = node->right;

            return newNode;
        }
        else {
            std::shared_ptr<Node> newNode = std::make_shared<Node>(node->key, node->value);

            newNode->left = node->left;
            newNode->right = add(node->right, key, value);

            return newNode;
        }
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

    BinaryTree addNode(K key, V value) {
        return BinaryTree(add(root, key, value));
    }

    BinaryTree removeNode(K key) {
        return BinaryTree(remove(root, key));
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

#endif //FILP_MO201_BINARYTREE_H
