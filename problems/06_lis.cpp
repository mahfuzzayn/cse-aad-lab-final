#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    if (n <= 0) {
        cout << 0 << endl;
        return 0;
    }
    vector<int> a(n), dp(n, 1), par(n, -1);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                par[i] = j;
            }
        }
    }
    int best = 0;
    for (int i = 1; i < n; i++)
        if (dp[i] > dp[best]) best = i;
    vector<int> seq;
    for (int i = best; i != -1; i = par[i]) seq.push_back(a[i]);
    reverse(seq.begin(), seq.end());
    cout << dp[best] << endl;
    for (int i = 0; i < (int)seq.size(); i++) cout << seq[i] << " \n"[i + 1 == (int)seq.size()];
    return 0;
}
