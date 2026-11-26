#include <bits/stdc++.h>
using namespace std;

vector<int> par;

int find(int x) {
    return par[x] == x ? x : par[x] = find(par[x]);
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, int>> edges(m);
    for (auto &[w, u, v] : edges) cin >> u >> v >> w;
    sort(edges.begin(), edges.end());
    par.resize(n + 1);
    for (int i = 0; i <= n; i++) par[i] = i;
    long long total = 0;
    vector<tuple<int, int, int>> mst;
    for (auto &[w, u, v] : edges) {
        int a = find(u), b = find(v);
        if (a == b) continue;
        par[a] = b;
        total += w;
        mst.push_back({u, v, w});
        if ((int)mst.size() == n - 1) break;
    }
    if (n >= 1 && (int)mst.size() != n - 1) {
        cout << -1 << endl;
        return 0;
    }
    for (auto [u, v, w] : mst) cout << u << " " << v << " " << w << endl;
    cout << total << endl;
    return 0;
}
