#include <iostream>
using namespace std;

template <typename T>
class TreeNode
{
public:
    T data;
    TreeNode *left;
    TreeNode *right;

    TreeNode(T val) : data(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree
{
public:
    TreeNode<T> *root;

    BinaryTree() : root(nullptr) {}

    void insert(T val)
    {
        root = insertRec(root, val);
    }

    TreeNode<T> *insertRec(TreeNode<T> *node, T val)
    {
        if (node == nullptr)
        {
            return new TreeNode<T>(val);
        }
        if (val < node->data)
        {
            node->left = insertRec(node->left, val);
        }
        else
        {
            node->right = insertRec(node->right, val);
        }
        return node;
    }

    void inorderTraversal(TreeNode<T> *node)
    {
        if (node)
        {
            inorderTraversal(node->left);
            cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

    void display()
    {
        inorderTraversal(root);
        cout << endl;
    }
};
