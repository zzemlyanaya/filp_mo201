//
// Created by Evgeniya Zemlyanaya on 03.05.2023.
//

#ifndef FILP_MO201_TREAP_H
#define FILP_MO201_TREAP_H

#include <iostream>
#include <memory>
#include <string>

template<class K, class V> struct Tree {
    struct Node {
        K key;
        V value;
        std::size_t priority;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(K key, V value, std::size_t priority) {
            this->key = key;
            this->value = value;
            this->priority = priority;
        }

        Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right, K key, V value, std::size_t priority) {
            this->left = left;
            this->right = right;

            this->key = key;
            this->value = value;

            this->priority = priority;
        }
    };

    struct Trunk {
        std::shared_ptr<Trunk> prev;
        std::string str;

        Trunk(std::shared_ptr<Trunk> prev, std::string str)
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
        std::cout << p->str;
    }

    void printTree(std::shared_ptr<Node> root, std::shared_ptr<Trunk> prev, bool isLeft)
    {
        if (root == nullptr) {
            return;
        }

        std::string prev_str = "       ";
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
        std::cout << " (" << root->key << " : " << root->priority << ")" << std::endl; // << " [value: " << root->value << "]" << endl;

        if (prev) {
            prev->str = prev_str;
        }
        trunk->str = "       |";

        printTree(root->left, trunk, false);
    }

    std::shared_ptr<Node> root;

    Tree() {}

    Tree(K key, V value) {
        root = std::make_shared<Node>(key, value);
    }

    Tree(std::shared_ptr<Node> node) {
        root = node;
    }

    Tree(std::initializer_list<std::pair<K, V>> list) {
        for (auto el : list) {
            root = add(root, el.first, el.second);
        }
    }

    std::shared_ptr<Node> add(std::shared_ptr<Node> node, K key, V value, std::size_t priority) {
        if (node == nullptr) {
            return std::make_shared<Node>(key, value, priority);
        }

        if (node->value == value && node->key == key) {
            return node;
        }

        if (node->key == key) {
            return std::make_shared<Node>(key, value, priority);
        }

        if (node->key > key) {
            auto added = add(node->left, key, value, priority);

            if (node->priority > added->priority) {
                std::shared_ptr<Node> newParent = std::make_shared<Node>(added->key, added->value, added->priority);
                std::shared_ptr<Node> newRight = std::make_shared<Node>(node->key, node->value, node->priority);

                newParent->right = newRight;
                newParent->left = added->left;

                newRight->left = added->right;
                newRight->right = node->right;

                return newParent;
            }
            else {
                std::shared_ptr<Node> newNode = std::make_shared<Node>(node->key, node->value, node->priority);
                newNode->left = added;
                newNode->right = node->right;

                return newNode;
            }

        }
        else {
            auto added = add(node->right, key, value, priority);

            if (node->priority > added->priority) {
                std::shared_ptr<Node> newParent = std::make_shared<Node>(added->key, added->value, added->priority);
                std::shared_ptr<Node> newLeft = std::make_shared<Node>(node->key, node->value, node->priority);

                newParent->right = added->right;
                newParent->left = newLeft;

                newLeft->left = node->left;
                newLeft->right = added->left;

                return newParent;
            }
            else {
                std::shared_ptr<Node> newNode = std::make_shared<Node>(node->key, node->value, node->priority);
                newNode->left = node->left;
                newNode->right = added;

                return newNode;
            }

        }


    }

    std::shared_ptr<Node> remove(std::shared_ptr<Node> node, K key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key > key) {
            return std::make_shared<Node>(remove(node->left, key), node->right, node->key, node->value, node->priority);
        }

        if (node->key < key) {
            return std::make_shared<Node>(node->left, remove(node->right, key), node->key, node->value, node->priority);
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
                if (node->left->priority < node->right->priority) {
                    // rotate right
                    std::shared_ptr<Node> newParent = std::make_shared<Node>(node->left->key, node->left->value, node->left->priority);
                    std::shared_ptr<Node> newRight = std::make_shared<Node>(node->key, node->value, node->priority);

                    newRight->right = node->right;
                    newRight->left = node->left->right;

                    newRight = remove(newRight, newRight->key);

                    newParent->left = node->left->left;
                    newParent->right = newRight;

                    return newParent;
                }
                else {
                    // rotate left
                    std::shared_ptr<Node> newParent = std::make_shared<Node>(node->right->key, node->right->value, node->right->priority);
                    std::shared_ptr<Node> newLeft = std::make_shared<Node>(node->key, node->value, node->priority);

                    newLeft->left = node->left;
                    newLeft->right = node->right->left;

                    newLeft = remove(newLeft, newLeft->key);

                    newParent->right = node->right->right;
                    newParent->left = newLeft;

                    return newParent;
                }
            }
        }


    }


    // tree, item
    std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> deleteRightestNode(std::shared_ptr<Node> node) {
        if (node->right == nullptr) return std::make_pair(node->left, node);

        auto right = deleteRightestNode(node->right);
        return std::make_pair(std::make_shared<Node>(node->left, right.first, node->key, node->value), right.second);
    }

    Tree addNode(K key, V value, std::size_t priority) {
        return Tree(add(root, key, value, priority));
    }

    Tree removeNode(K key) {
        return Tree(remove(root, key));
    }

    void PrintTree() {
        printTree(this->root, nullptr, false);
    }

    bool checkCorrect(std::shared_ptr<Node> node) {
        if (node == nullptr) return true;

        if (node->left && node->key < node->left->key) {
            printf("INCORRECT KEYS LEFT %d < %d", node->key, node->left->key);
            return false;
        }
        if (node->left && node->priority > node->left->priority) {
            printf("INCORRECT PRORITY LEFT %d > %d", node->priority, node->left->priority);
            return false;
        }
        if (node->right && node->priority > node->right->priority) {
            printf("INCORRECT PRIORITY RIGHT %d > %d", node->priority, node->right->priority);
            return false;
        }
        if (node->right && node->key > node->right->key) {
            printf("INCORRECT KEYS RIGHT %d > %d", node->key, node->right->key);
            return false;
        }

        return checkCorrect(node->left) && checkCorrect(node->right);
    }
};

#endif //FILP_MO201_TREAP_H
