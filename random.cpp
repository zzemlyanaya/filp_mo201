#include <iostream>

using namespace std;

template<class K, class V>
class AVLTree {
    struct Node {
        K key;
        V value;

        shared_ptr<Node> left;
        shared_ptr<Node> right;

        Node(K _key, V _value) {
            key = _key;
            value = _value;
        }
    };

    shared_ptr<Node> root;

    AVLTree(shared_ptr<Node> r) {
        root = r;
    }

    shared_ptr<Node> Add(shared_ptr<Node> node, K key, V value) {
        if (node == nullptr)
            return make_shared<Node>(key, value);

        if (node->key == key && node->value == value)
            return node;

        if (node->key == key)
            return make_shared<Node>(key, value);

        if (node->key > key) {
            auto newOldNode = make_shared<Node>(node -> key, node -> value);
            newOldNode->left = Add(node->left, key, value);
            newOldNode->right = node->right;
            return newOldNode;
        }


        auto newOldNode = make_shared<Node>(node->key, node->value);
        newOldNode->right = Add(node->right, key, value);
        newOldNode->left = node->left;
        return newOldNode;

    }

public:
    AVLTree(){}

    AVLTree(K key, V value) {
        root = make_shared<Node>(key, value);
    }

    AVLTree Add(K key, V value) {
        return AVLTree(Add(root, key, value));
    }

    AVLTree Remove(K key) {
        // TODO
    }

    void PrintTree() {
        // TODO
    }
};

int main() {
    AVLTree<int, int> tree = AVLTree<int, int>().Add(1, 2);
    return 0;
}