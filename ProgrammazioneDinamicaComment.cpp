#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int fastestWay(vector<int> a1, vector<int> a2, vector<int> t1, vector<int> t2, int e1, int e2, int x1, int x2) {
    int n = a1.size();          // Numero di stazioni
    vector<int> f1(n), f2(n);   // Tempi minimi per arrivare alle stazioni
    vector<int> l1(n), l2(n);   // Tracciamento percorso

    // Tempo per iniziare dalla catena 1 e 2
    f1[0] = e1 + a1[0];
    f2[0] = e2 + a2[0];

    // Calcolo dinamico per ogni stazione successiva
    for (int j = 1; j < n; j++) {

        // Calcolo tempo minimo per catena 1
        if (f1[j - 1] + a1[j] <= f2[j - 1] + t2[j - 1] + a1[j]) {
            f1[j] = f1[j - 1] + a1[j];
            l1[j] = 1;
        } else {
            f1[j] = f2[j - 1] + t2[j - 1] + a1[j];
            l1[j] = 2;
        }

        // Calcolo tempo minimo per catena 2
        if (f2[j - 1] + a2[j] <= f1[j - 1] + t1[j - 1] + a2[j]) {
            f2[j] = f2[j - 1] + a2[j];
            l2[j] = 2;
        } else {
            f2[j] = f1[j - 1] + t1[j - 1] + a2[j];
            l2[j] = 1;
        }
    }

    // Aggiunta tempi di uscita
    if (f1[n - 1] + x1 <= f2[n - 1] + x2) {
        cout << "Percorso più veloce termina nella catena 1\n";
        return f1[n - 1] + x1;
    } else {
        cout << "Percorso più veloce termina nella catena 2\n";
        return f2[n - 1] + x2;
    }
}

/*      Main FastestWay
int main() {
    vector a1 = {7, 9, 3, 4, 8, 4};
    vector a2 = {8, 5, 6, 4, 5, 7};
    vector t1 = {2, 3, 1, 3, 4}; // Passaggi da catena 1 a 2
    vector t2 = {2, 1, 2, 2, 1}; // Passaggi da catena 2 a 1
    int e1 = 2, e2 = 4;
    int x1 = 3, x2 = 2;

    int tempo = fastestWay(a1, a2, t1, t2, e1, e2, x1, x2);
    cout << "Tempo minimo totale: " << tempo << endl;

    return 0;
}
*/

int knapsack01(int W, vector<int> peso, vector<int> valore) {
    int n = peso.size();

    // dp[i][w] = valore massimo usando i oggetti con capacità w
    vector dp(n + 1, vector(W + 1, 0));

    // Riempimento tabella bottom-up
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (peso[i - 1] <= w) {

                // Prendo o non prendo l'oggetto
                dp[i][w] = max(valore[i - 1] + dp[i - 1][w - peso[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w]; // Non posso prenderlo
            }
        }
    }

    return dp[n][W]; // Valore massimo ottenibile
}

/*      Main Zaino-01
int main() {
    vector peso = {2, 3, 4, 5};
    vector valore = {3, 4, 5, 6};
    int capacità = 5;

    cout << "Valore massimo (0-1): " << knapsack01(capacità, peso, valore) << endl;
    return 0;
}
*/

// Struttura per oggetti con valore, peso e valore/peso
struct Oggetto {
    int valore, peso;
    double rapporto;
};

// Ordinamento decrescente per rapporto valore/peso
bool cmp(Oggetto a, Oggetto b) {
    return a.rapporto > b.rapporto;
}

double fractionalKnapsack(int W, vector<int> peso, vector<int> valore) {
    int n = peso.size();
    vector<Oggetto> oggetti(n);

    // Calcolo rapporto valore/peso per ogni oggetto
    for (int i = 0; i < n; i++) {
        oggetti[i] = {valore[i], peso[i], static_cast<double>(valore[i]) / peso[i]};
    }

    // Ordina oggetti per efficienza
    ranges::sort(oggetti, cmp);

    double valoreTotale = 0.0;

    for (int i = 0; i < n && W > 0; i++) {
        if (oggetti[i].peso <= W) {

            // Prendo l'intero oggetto
            W -= oggetti[i].peso;
            valoreTotale += oggetti[i].valore;
        } else {

            // Prendo solo una frazione
            valoreTotale += oggetti[i].rapporto * W;
            W = 0;
        }
    }

    return valoreTotale;
}

/*      Main Zaino Frazionario
int main() {
    vector peso = {10, 20, 30};
    vector valore = {60, 100, 120};
    int capacità = 50;

    cout << "Valore massimo (frazionario): " << fractionalKnapsack(capacità, peso, valore) << endl;
    return 0;
}
*/
