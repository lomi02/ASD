#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;

// Classe per rappresentare una Hash Table
class HashTable {

    // Una lista di liste (catene) per gestire collisioni
    vector<list<pair<int, string> > > table;
    int size; // Numero di bucket

    // Funzione hash per mappare una chiave a un indice
    int hashFunction(int key) {
        return key % size;
    }

public:

    // Costruttore
    HashTable(int s) {
        size = s;
        table.resize(size);
    }

    // Inserimento di una coppia (chiave, valore)
    void insert(int key, const string &value) {
        int index = hashFunction(key);
        table[index].push_back(make_pair(key, value));
    }

    // Ricerca di un valore per una data chiave
    string search(int key) {
        int index = hashFunction(key);
        for (auto &pair: table[index]) {
            if (pair.first == key) {
                return pair.second; // Se la chiave è trovata
            }
        }
        return "Not found"; // Se la chiave non esiste
    }

    // Rimozione di una coppia (chiave, valore)
    void remove(int key) {
        int index = hashFunction(key);
        auto &chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == key) {
                chain.erase(it);
                return;
            }
        }
    }

    // Stampa della Hash Table
    void print() {
        for (int i = 0; i < size; ++i) {
            cout << "Bucket " << i << ": ";
            for (auto &pair: table[i]) {
                cout << "{" << pair.first << ": " << pair.second << "} ";
            }
            cout << endl;
        }
    }
};

// Main per testare la HashTable
int main() {
    HashTable ht(10);

    ht.insert(1, "Apple");
    ht.insert(2, "Banana");
    ht.insert(12, "Orange");
    ht.insert(22, "Grapes");

    cout << "Search for key 1: " << ht.search(1) << endl;
    cout << "Search for key 2: " << ht.search(2) << endl;
    cout << "Search for key 12: " << ht.search(12) << endl;

    ht.remove(2);
    cout << "After removing key 2, search for key 2: " << ht.search(2) << endl;

    cout << "Current Hash Table:" << endl;
    ht.print();

    return 0;
}
