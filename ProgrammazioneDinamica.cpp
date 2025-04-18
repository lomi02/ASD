#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int fastestWay(vector<int> a1, vector<int> a2, vector<int> t1, vector<int> t2, int e1, int e2, int x1, int x2) {
    int n = a1.size();
    vector<int> f1(n), f2(n);
    vector<int> l1(n), l2(n);

    f1[0] = e1 + a1[0];
    f2[0] = e2 + a2[0];

    for (int j = 1; j < n; j++) {
        if (f1[j - 1] + a1[j] <= f2[j - 1] + t2[j - 1] + a1[j]) {
            f1[j] = f1[j - 1] + a1[j];
            l1[j] = 1;
        } else {
            f1[j] = f2[j - 1] + t2[j - 1] + a1[j];
            l1[j] = 2;
        }

        if (f2[j - 1] + a2[j] <= f1[j - 1] + t1[j - 1] + a2[j]) {
            f2[j] = f2[j - 1] + a2[j];
            l2[j] = 2;
        } else {
            f2[j] = f1[j - 1] + t1[j - 1] + a2[j];
            l2[j] = 1;
        }
    }

    if (f1[n - 1] + x1 <= f2[n - 1] + x2) {
        cout << "Percorso più veloce termina nella catena 1\n";
        return f1[n - 1] + x1;
    } else {
        cout << "Percorso più veloce termina nella catena 2\n";
        return f2[n - 1] + x2;
    }
}

int knapsack01(int W, vector<int> peso, vector<int> valore) {
    int n = peso.size();

    vector dp(n + 1, vector(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (peso[i - 1] <= w) {
                dp[i][w] = max(valore[i - 1] + dp[i - 1][w - peso[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][W];
}

struct Oggetto {
    int valore, peso;
    double rapporto;
};

bool cmp(Oggetto a, Oggetto b) {
    return a.rapporto > b.rapporto;
}

double fractionalKnapsack(int W, vector<int> peso, vector<int> valore) {
    int n = peso.size();
    vector<Oggetto> oggetti(n);

    for (int i = 0; i < n; i++) {
        oggetti[i] = {valore[i], peso[i], static_cast<double>(valore[i]) / peso[i]};
    }

    ranges::sort(oggetti, cmp);

    double valoreTotale = 0.0;

    for (int i = 0; i < n && W > 0; i++) {
        if (oggetti[i].peso <= W) {
            W -= oggetti[i].peso;
            valoreTotale += oggetti[i].valore;
        } else {
            valoreTotale += oggetti[i].rapporto * W;
            W = 0;
        }
    }

    return valoreTotale;
}
