#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template<typename T>
class Item {
public:
    T key, value;
    bool deleted;

    Item(T key, T value) : key(key), value(value), deleted(false) {}
};

template<typename T>
class HashTable {
public:
    int m;
    vector<Item<T> *> table;

    HashTable(int m) : m(m), table(m, nullptr) {}

    int hash(T key, int i) {
        return (key + i) % m;
    }

    void insert(Item<T> *item) {
        int i = 0;
        while (i != m) {
            int j = hash(item->key, i);
            if (table[j] == nullptr) {
                table[j] = item;
                return;
            }
            i++;
        }
        cout << "Errore, overflow" << endl;
    }

    Item<T> *search(T key) {
        int i = 0;
        int j = hash(key, 0);
        while (table[j] != nullptr && i != m) {
            if (!table[j]->deleted && table[j]->key == key)
                return table[j];
            i++;
            j = hash(key, i);
        }
        return nullptr;
    }

    void remove(T key) {
        int i = 0;
        int j = hash(key, 0);
        while (table[j] != nullptr && i != m) {
            if (table[j]->key == key)
                table[j]->deleted = true;
            i++;
            j = hash(key, i);
        }
    }
};

int main() {
    ifstream input("inputH.txt");
    if (!input.is_open()) {
        cerr << "Errore";
        return 1;
    }

    int hashSize = 0;
    input >> hashSize;
    HashTable<int> H(hashSize);

    for (int i = 0; i < hashSize; i++) {
        int key, value;
        input >> key >> value;
        auto itemx = new Item<int>(key, value);
        H.insert(itemx);
    }
    input.close();

    ofstream output("outputH.txt");
    if (!output.is_open()) {
        cerr << "Errore";
        return 1;
    }

    Item<int> *result = H.search(2);
    if (result != nullptr)
        output << "Trovato elemento, chiave: " << result->key << ", valore: " << result->value << endl;
    else
        output << "Elemento non trovato" << endl;
    output << endl;

    H.remove(8);
    Item<int> *removedResult = H.search(8);
    if (removedResult != nullptr)
        output << "Trovato elemento dopo la cancellazione, chiave: " << removedResult->key << ", valore; " << removedResult->value << endl;
    else
        output << "Elemento non trovato dopo la cancellazione" << endl;
    output << endl;

    output.close();

    cout << "File creato con successo!" << endl;
    return 0;
}
