#include <iostream>
using namespace std;

int ack(int m, int n) {
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return ack(m - 1, 1);
    } else {
        return ack(m - 1, ack(m, n - 1));
    }
}
int main() {
    int m, n;
    cout << "Enter m and n: ";
    cin >> m >> n;
  if (m < 0 || n < 0) {
        cout << "輸入錯誤：m 和 n 必須是 >= 0 的整數。" << endl;
        return 1;
}
    cout << "Ackermann(" << m << ", " << n << ") = " << ack(m, n) << endl;
    return 0;
}
