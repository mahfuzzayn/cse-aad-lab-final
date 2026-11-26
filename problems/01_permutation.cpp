#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> a, cur;
vector<bool> taken;

void perm() {
    if ((int)cur.size() == n) {
        for (int i = 0; i < n; i++) cout << cur[i] << " \n"[i == n - 1];
        return;
    }
    for (int i = 0; i < n; i++) {
        if (taken[i]) continue;
        taken[i] = true;
        cur.push_back(a[i]);
        perm();
        cur.pop_back();
        taken[i] = false;
    }
}

int main() {
    cin >> n;
    if (n <= 0) return 0;
    a.resize(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a.begin(), a.end());
    taken.assign(n, false);
    perm();
    return 0;
}
