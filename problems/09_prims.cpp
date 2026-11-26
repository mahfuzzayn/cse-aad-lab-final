#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, int>>> g(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    if (n <= 0) {
        cout << 0 << endl;
        return 0;
    }
    vector<bool> vis(n + 1, false);
    vector<int> par(n + 1, -1);
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    pq.push({0, 1, -1});
    long long total = 0;
    vector<tuple<int, int, int>> mst;
    while (!pq.empty() && (int)mst.size() < n) {
        auto [w, u, p] = pq.top();
        pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        total += w;
        if (p != -1) mst.push_back({p, u, w});
        for (auto [v, w2] : g[u])
            if (!vis[v]) pq.push({w2, v, u});
    }
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        if (vis[i]) cnt++;
    if (cnt < n) {
        cout << -1 << endl;
        return 0;
    }
    for (auto [u, v, w] : mst) cout << u << " " << v << " " << w << endl;
    cout << total << endl;
    return 0;
}
