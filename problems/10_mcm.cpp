#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> p;
vector<vector<long long>> m;
vector<vector<int>> s;

void printOrder(int i, int j) {
    if (i == j) {
        cout << "A" << i;
        return;
    }
    cout << "(";
    printOrder(i, s[i][j]);
    cout << "*";
    printOrder(s[i][j] + 1, j);
    cout << ")";
}

int main() {
    cin >> n;
    if (n <= 0) {
        cout << 0 << endl;
        return 0;
    }
    p.resize(n + 1);
    for (int i = 0; i <= n; i++) cin >> p[i];
    m.assign(n + 1, vector<long long>(n + 1, 0));
    s.assign(n + 1, vector<int>(n + 1, 0));
    for (int len = 2; len <= n; len++) {
        for (int i = 1; i + len - 1 <= n; i++) {
            int j = i + len - 1;
            m[i][j] = LLONG_MAX;
            for (int k = i; k < j; k++) {
                long long cost = m[i][k] + m[k + 1][j] + (long long)p[i - 1] * p[k] * p[j];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = k;
                }
            }
        }
    }
    cout << m[1][n] << endl;
    printOrder(1, n);
    cout << endl;
    return 0;
}
