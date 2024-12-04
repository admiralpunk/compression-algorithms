#include <iostream>
using namespace std;

template <typename T>
class Node
{
public:
    T data;
    Node *next;

    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class LinkedList
{
public:
    Node<T> *head;

    LinkedList() : head(nullptr) {}

    void insertFront(T val)
    {
        Node<T> *newNode = new Node<T>(val);
        newNode->next = head;
        head = newNode;
    }

    void insertEnd(T val)
    {
        Node<T> *newNode = new Node<T>(val);
        if (!head)
        {
            head = newNode;
            return;
        }
        Node<T> *temp = head;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    void deleteNode(T val)
    {
        if (!head)
            return;
        if (head->data == val)
        {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node<T> *temp = head;
        while (temp->next && temp->next->data != val)
        {
            temp = temp->next;
        }
        if (temp->next)
        {
            Node<T> *delNode = temp->next;
            temp->next = temp->next->next;
            delete delNode;
        }
    }

    void display()
    {
        Node<T> *temp = head;
        while (temp)
        {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }

    ~LinkedList()
    {
        while (head)
        {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
        }
    }
};
