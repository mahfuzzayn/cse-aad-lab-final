#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    double W;
    cin >> n >> W;
    vector<pair<double, double>> a(n);
    for (int i = 0; i < n; i++) cin >> a[i].first >> a[i].second;
    sort(a.begin(), a.end(), [](const pair<double, double> &x, const pair<double, double> &y) {
        double rx = x.first > 0 ? x.second / x.first : 1e18;
        double ry = y.first > 0 ? y.second / y.first : 1e18;
        return rx > ry;
    });
    double total = 0, cap = W;
    for (int i = 0; i < n && cap > 0; i++) {
        double w = a[i].first, v = a[i].second;
        if (w <= cap) {
            total += v;
            cap -= w;
        } else {
            total += v * (cap / w);
            cap = 0;
        }
    }
    cout << fixed << setprecision(2) << total << endl;
    return 0;
}
