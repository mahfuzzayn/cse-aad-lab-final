#include <bits/stdc++.h>
using namespace std;

int n, cnt = 0;
vector<int> queen;
vector<bool> colUsed, diag1, diag2;
vector<int> firstSol;
bool found = false;

void solve(int row) {
    if (row == n) {
        cnt++;
        if (!found) {
            firstSol = queen;
            found = true;
        }
        return;
    }
    for (int c = 0; c < n; c++) {
        if (colUsed[c] || diag1[row + c] || diag2[row - c + n - 1]) continue;
        queen[row] = c;
        colUsed[c] = diag1[row + c] = diag2[row - c + n - 1] = true;
        solve(row + 1);
        colUsed[c] = diag1[row + c] = diag2[row - c + n - 1] = false;
    }
}

int main() {
    cin >> n;
    if (n <= 0) {
        cout << 0 << endl;
        return 0;
    }
    queen.assign(n, -1);
    colUsed.assign(n, false);
    diag1.assign(2 * n - 1, false);
    diag2.assign(2 * n - 1, false);
    solve(0);
    cout << cnt << endl;
    if (found) {
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) cout << (firstSol[r] == c ? 'Q' : '.');
            cout << "\n";
        }
    }
    return 0;
}
