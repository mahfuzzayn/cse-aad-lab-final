#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m, src;
    cin >> n >> m >> src;
    vector<tuple<int, int, int>> e(m);
    for (auto &[u, v, w] : e) cin >> u >> v >> w;
    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(n + 1, INF);
    if (n >= 1 && src >= 1 && src <= n) {
        dist[src] = 0;
        for (int i = 1; i <= n - 1; i++) {
            bool changed = false;
            for (auto &[u, v, w] : e) {
                if (dist[u] == INF) continue;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    changed = true;
                }
            }
            if (!changed) break;
        }
        for (auto &[u, v, w] : e) {
            if (dist[u] == INF) continue;
            if (dist[u] + w < dist[v]) {
                cout << -1 << endl;
                return 0;
            }
        }
    }
    for (int i = 1; i <= n; i++)
        cout << i << ":" << (dist[i] == INF ? -1 : dist[i]) << " \n"[i == n];
    return 0;
}
