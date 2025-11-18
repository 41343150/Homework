#include <iostream>
using namespace std;

void ps(int S[], int n, int sub[], int sz, int id) {
    if (id == n) {
        cout << "{";
        for (int i = 0; i < sz; ++i) {
            cout << sub[i];
            if (i != sz - 1) cout << ",";
        }
        cout << "}" << endl;
        return;
    }
    ps(S, n, sub, sz, id + 1);
    sub[sz] = S[id];
    ps(S, n, sub, sz + 1, id + 1);
}

int main() {
    int S[] = {1, 2, 3};
    int n = 3;
    int sub[3];
    ps(S, n, sub, 0, 0);
    return 0;
}
