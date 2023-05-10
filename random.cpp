#include <iostream>
using namespace std;

template<class K, class V> struct Tree {
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

    std::shared_ptr<Node> root;

    Tree() {}

    Tree(K key, V value) {
        root = std::make_shared<Node>(key, value, 1);
    }

    Tree(std::shared_ptr<Node> node) {
        root = node;
    }

    Tree(std::initializer_list<std::pair<K, V>> list) {
        for (auto el : list) {
            root = add(root, el.first, el.second);
        }
    }

    int getHeight(std::shared_ptr<Node> node) {
        return node == nullptr
               ? 0
               : node->height;
    }

    int findHeight(std::shared_ptr<Node> node) {
        return std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    std::shared_ptr<Node> rightRotate(std::shared_ptr<Node> node) {
        auto newParent = std::make_shared<Node>(node->left->key, node->left->value, 1);
        auto newRight = std::make_shared<Node>(node->key, node->value, 1);

        newParent->right = newRight;
        newParent->left = node->left->left;

        newRight->left = node->left->right;
        newRight->right = node->right;

        newRight->height = findHeight(newRight);
        newParent->height = findHeight(newParent);
        return newParent;
    }

    std::shared_ptr<Node> leftRotate(std::shared_ptr<Node> node) {
        auto newParent = std::make_shared<Node>(node->right->key, node->right->value, 1);
        auto newLeft = std::make_shared<Node>(node->key, node->value, 1);

        newParent->left = newLeft;
        newParent->right = node->right->right;

        newLeft->right = node->right->left;
        newLeft->left = node->left;

        newLeft->height = findHeight(newLeft);
        newParent->height = findHeight(newParent);
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
        node->height = findHeight(node);

        // -------------------балансировка дерева------------------
        int balance = getHeight(node->left) - getHeight(node->right);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
        // -------------------балансировка дерева------------------

    }

    std::shared_ptr<Node> remove(std::shared_ptr<Node> node, K key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->key > key) {
            return std::make_shared<Node>(remove(node->left, key), node->right, node->key, node->value);
        }

        if (node->key < key) {
            return std::make_shared<Node>(node->left, remove(node->right, key), node->key, node->value);
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
                auto deleted = deleteRightestNode(node->left);
                auto newTree = deleted.first;
                auto deletedItem = deleted.second;

                return std::make_shared<Node>(newTree, node->right, deletedItem->key, deletedItem->value);
            }
        }


    }

    // tree, item
    std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> deleteRightestNode(std::shared_ptr<Node> node) {
        if (node->right == nullptr) return std::make_pair(node->left, node);

        auto right = deleteRightestNode(node->right);
        return std::make_pair(std::make_shared<Node>(node->left, right.first, node->key, node->value), right.second);
    }

    Tree addNode(K key, V value) {
        return Tree(add(root, key, value));
    }

    /*Tree removeNode(K key) {
        return Tree(remove(root, key));
    }*/

    void printTree(std::shared_ptr<Node> node) {
        printTree(this->root, nullptr, false);
    }
};


template<class K, class V>
size_t findTreeHeight(Tree<K, V> tree) {
    auto root = tree.root;

    if (root == nullptr)
        return 0;
    return max(findTreeHeight(Tree<K, V>(root->left)), findTreeHeight(Tree<K, V>(root->right))) + 1;
}


int main()
{
    Tree<int, int> tree = Tree<int, int>();

    for (int i = 0; i < 10000; i++) {
        tree = tree.addNode(i, 0);
    }

    cout << findTreeHeight(tree);
    //tree.printTree(tree.root);

    std::cout << std::endl;

    /*tree = tree.removeNode(3);
    tree.printTree(tree.root);*/

    return 0;
}