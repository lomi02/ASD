#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Classe template che rappresenta un singolo elemento (Item) all'interno della tabella.
template<typename T>
class Item {
public:
    T key;          // Chiave identificativa dell'elemento.
    T value;        // Valore associato alla chiave.
    bool deleted;   // Flag per la gestione della cancellazione logica (lazy deletion).

    // Costruttore: inizializza chiave e valore e imposta lo stato di cancellazione a false.
    Item(T key, T value) : key(key), value(value), deleted(false) {}
};

// Classe template che gestisce una Hash Table con indirizzamento aperto.
template<typename T>
class HashTable {
public:
    int m; // Dimensione della tabella (numero di slot).
    vector<Item<T> *> table; // Vettore di puntatori a Item che costituisce la tabella.

    // Costruttore: inizializza la dimensione m e alloca lo spazio necessario impostando a null.
    HashTable(int m) : m(m), table(m, nullptr) {}

    // Funzione di Hash: Ispezione Lineare.
    int hash(T key, int i) {
        return (key + i) % m;
    }

    // Funzione di Hash: Ispezione Quadratica.
    int hash_quad(T key, int i) {
        return (key + i + i * i) % m;
    }

    // Funzione di Hash: Doppio Hashing.
    int hash_doppio(int key, int i) {
        int h1 = key % m;
        int h2 = 1 + key % (m - 1);
        return (h1 + i * h2) % m;
    }

    /**
     * Insert:
     * Inserisce un elemento nella tabella cercando il primo slot libero tramite probing.
     */
    void insert(Item<T> *item) {
        int i = 0;

        // Ciclo di scansione fino al raggiungimento della dimensione massima m.
        while (i != m) {
            int j = hash(item->key, i);

            // Controllo di sicurezza sulla dimensione del vettore (ridondante se m è corretto).
            if (table.size() <= j)
                table.resize(j + 1, nullptr);

            // Passo 1: Se lo slot è vuoto, inserisce l'elemento e termina.
            if (table[j] == nullptr) {
                table[j] = item;
                return;
            }

            // Passo 2: Altrimenti incrementa il contatore dei tentativi (collisione).
            i++;
        }

        // Se il ciclo termina senza aver trovato un posto, la tabella è piena.
        cout << "Errore, overflow" << endl;
    }

    /**
     * Search:
     * Cerca un elemento data una chiave e restituisce il puntatore all'Item se trovato.
     */
    Item<T> *search(T key) {
        int i = 0;
        int j = hash(key, 0);

        // Passo 1: Scansione finché lo slot non è vuoto o non si è esaurita la tabella.
        // Uno slot nullo indica che la chiave non è mai stata inserita (fine della catena).
        while (table[j] != nullptr && i != m) {

            // Passo 2: Verifica se la chiave coincide e se l'elemento non è marcato come cancellato.
            if (!table[j]->deleted && table[j]->key == key)
                return table[j];

            // Passo 3: Proseguimento della scansione.
            i++;
            j = hash(key, i);
        }

        // Se non trovato, restituisce null.
        return nullptr;
    }

    /**
     * Remove:
     * Esegue la cancellazione logica di un elemento marcandolo come 'deleted'.
     */
    void remove(T key) {
        int i = 0;
        int j = hash(key, 0);

        // Passo 1: Ricerca dell'elemento all'interno della tabella.
        while (table[j] != nullptr && i != m) {

            // Passo 2: Se la chiave corrisponde, si attiva il flag di cancellazione.
            // L'elemento resta in memoria per non interrompere le catene di ricerca.
            if (table[j]->key == key) {
                table[j]->deleted = true;
            }
            i++;
            j = hash(key, i);
        }
    }
};

int main() {

    // Apertura del file di input contenente i dati della tabella.
    ifstream input("inputH.txt");
    if (!input.is_open()) {
        cerr << "Errore durante la lettura del file di input" << endl;
        return 1;
    }

    int hashSize = 0;
    // Legge la dimensione della tabella dalla prima riga del file.
    input >> hashSize;
    HashTable<int> H(hashSize);

    // Ciclo per leggere e inserire ogni elemento (chiave-valore) nella tabella.
    for (int i = 0; i < hashSize; i++) {
        int key, value;
        input >> key >> value;
        auto itemx = new Item<int>(key, value);
        H.insert(itemx);
    }
    input.close();

    // Apertura del file di output per riportare i risultati delle operazioni.
    ofstream output("outputH.txt");
    if (!output.is_open()) {
        cerr << "Errore nella creazione del file di output." << endl;
        return 1;
    }

    // Esecuzione di una ricerca di test (chiave 2).
    Item<int> *result = H.search(2);
    if (result != nullptr)
        output << "Trovato elemento, chiave: " << result->key << ", valore: " << result->value << endl;
    else
        output << "Elemento non trovato" << endl;
    output << endl;

    // Esecuzione di una cancellazione di test (chiave 8) e successiva verifica.
    H.remove(8);
    Item<int> *removedResult = H.search(8);
    if (removedResult != nullptr)
        output << "Trovato elemento dopo la cancellazione, chiave: " << removedResult->key << ", valore: " << removedResult->value << endl;
    else
        output << "Elemento non trovato dopo la cancellazione" << endl;
    output << endl;

    output.close();
    cout << "File di output creato con successo!" << endl;

    return 0;
}
