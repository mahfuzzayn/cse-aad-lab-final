#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    if (n <= 0) return 0;
    vector<char> sym(n);
    vector<long long> freq(n);
    for (int i = 0; i < n; i++) cin >> sym[i] >> freq[i];
    vector<long long> w(2 * n, 0);
    vector<int> l(2 * n, -1), r(2 * n, -1);
    for (int i = 0; i < n; i++) w[i] = freq[i];
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    for (int i = 0; i < n; i++) pq.push({freq[i], i});
    int cnt = n;
    while (pq.size() > 1) {
        auto [fu, u] = pq.top(); pq.pop();
        auto [fv, v] = pq.top(); pq.pop();
        w[cnt] = fu + fv;
        l[cnt] = u;
        r[cnt] = v;
        pq.push({w[cnt], cnt});
        cnt++;
    }
    int root = pq.top().second;
    vector<string> code(n);
    queue<pair<int, string>> q;
    q.push({root, ""});
    while (!q.empty()) {
        auto [u, c] = q.front();
        q.pop();
        if (u < n) {
            code[u] = c.empty() ? "0" : c;
            continue;
        }
        q.push({l[u], c + "0"});
        q.push({r[u], c + "1"});
    }
    long long total = 0;
    for (int i = 0; i < n; i++) {
        cout << sym[i] << " " << code[i] << endl;
        total += freq[i] * (long long)code[i].size();
    }
    cout << total << endl;
    return 0;
}
