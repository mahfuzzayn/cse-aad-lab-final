#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m, src;
    cin >> n >> m >> src;
    vector<vector<pair<int, int>>> g(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
    const long long INF = LLONG_MAX;
    vector<long long> dist(n + 1, INF);
    if (n >= 1 && src >= 1 && src <= n) {
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        dist[src] = 0;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;
            for (auto [v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }
    for (int i = 1; i <= n; i++)
        cout << i << ":" << (dist[i] == INF ? -1 : dist[i]) << " \n"[i == n];
    return 0;
}
