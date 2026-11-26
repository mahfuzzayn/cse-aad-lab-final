#include <bits/stdc++.h>
using namespace std;

int n, r;
vector<int> a, cur;

void comb(int start) {
    if ((int)cur.size() == r) {
        for (int i = 0; i < r; i++) cout << cur[i] << " \n"[i == r - 1];
        return;
    }
    for (int i = start; i < n; i++) {
        cur.push_back(a[i]);
        comb(i + 1);
        cur.pop_back();
    }
}

int main() {
    cin >> n >> r;
    if (n <= 0 || r < 0 || r > n) return 0;
    a.resize(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a.begin(), a.end());
    comb(0);
    return 0;
}
