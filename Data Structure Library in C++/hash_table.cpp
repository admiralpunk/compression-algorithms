#include <iostream>
#include <vector>
#include <list>
using namespace std;

template <typename T>
class HashTable
{
public:
    vector<list<T>> table;
    int size;

    HashTable(int size) : size(size)
    {
        table.resize(size);
    }

    int hashFunction(T key)
    {
        return key % size;
    }

    void insert(T key)
    {
        int index = hashFunction(key);
        table[index].push_back(key);
    }

    bool search(T key)
    {
        int index = hashFunction(key);
        for (auto &item : table[index])
        {
            if (item == key)
            {
                return true;
            }
        }
        return false;
    }

    void display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << i << ": ";
            for (auto &item : table[i])
            {
                cout << item << " ";
            }
            cout << endl;
        }
    }
};
