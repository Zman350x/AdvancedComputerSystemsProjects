#ifndef __BST_H__
#define __BST_H__

#include <cstdint>
#include <iostream>

#define VAL_BUF_SIZE 32

namespace Bst
{
    struct Node
    {
        uint32_t key;
        char value[VAL_BUF_SIZE];
        Node *left;
        Node *right;
    };

    Node *createNode(uint32_t key, const char *const value)
    {
        Node *node = new Node();
        node->key = key;
        node->left = nullptr;
        node->right = nullptr;

        if (value == nullptr)
        {
            node->value[0] = '\0';
            return node;
        }

        for (int i = 0; i < VAL_BUF_SIZE; ++i)
        {
            node->value[i] = value[i];

            if (value[i] == '\0')
                break;
        }

        return node;
    }

    Node *insert(Node *root, uint32_t key, const char *const value)
    {
        if (root == nullptr)
            return createNode(key, value);

        if (key < root->key)
            root->left = insert(root->left, key, value);
        else if (key > root->key)
            root->right = insert(root->right, key, value);
        else
        {
            for (int i = 0; i < VAL_BUF_SIZE; ++i)
            {
                root->value[i] = value[i];

                if (value[i] == '\0')
                    break;
            }
        }

        return root;
    }


    Node *find(Node *root, uint32_t key)
    {
        if (root == nullptr || root->key == key)
            return root;

        if (key < root->key)
            return find(root->left, key);

        return find(root->right, key);
    }

    Node *minKey(Node *node)
    {
        Node *current = node;

        while (current && current->left != nullptr)
            current = current->left;

        return current;
    }

    Node *erase(Node *root, uint32_t key)
    {
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = erase(root->left, key);
        else if (key > root->key)
            root->right = erase(root->right, key);
        else
        {
            if (root->left == nullptr)
            {
                Node *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                Node *temp = root->left;
                delete root;
                return temp;
            }

            Node *temp = minKey(root->right);

            root->key = temp->key;

            for (int i = 0; i < VAL_BUF_SIZE; ++i)
                root->value[i] = temp->value[i];

            root->right = erase(root->right, temp->key);
        }

        return root;
    }

    void printTree(Node* root)
    {
        if (root != nullptr)
        {
            printTree(root->left);
            std::cout << root->key << ": " << root->value << '\n';
            printTree(root->right);
        }
    }
}

#endif
