#include <algorithm>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////
template <typename T>
class Heap {
private:
    T arr[10000];   // 用陣列取代 vector
    int sz;         // 目前元素數量
    bool isMin;     // true = MinHeap, false = MaxHeap

    bool cmp(const T& a, const T& b) const {
        return isMin ? a < b : a > b;
    }

    void up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (cmp(arr[i], arr[p])) {
                swap(arr[i], arr[p]);
                i = p;
            } else break;
        }
    }

    void down(int i) {
        while (2 * i + 1 < sz) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            int t = l;

            if (r < sz && cmp(arr[r], arr[l])) t = r;

            if (cmp(arr[t], arr[i])) {
                swap(arr[i], arr[t]);
                i = t;
            } else break;
        }
    }

public:
    Heap(bool type = true) {
        isMin = type;
        sz = 0;
    }

    bool empty() const {
        return sz == 0;
    }

    T top() const {
        if (empty()) {
            cout << "Heap is empty!\n";
            return T();
        }
        return arr[0];
    }

    void push(T x) {
        arr[sz++] = x;
        up(sz - 1);
    }

    void pop() {
        if (empty()) {
            cout << "Heap is empty!\n";
            return;
        }
        swap(arr[0], arr[sz - 1]);
        sz--;
        if (!empty()) down(0);
    }

    void build(int n) {
        T x;
        for (int i = 0; i < n; i++) {
            cin >> x;
            push(x);
        }
    }

    void printLevels() const {
        int i = 0, level = 0;
        while (i < sz) {
            int cnt = 1 << level;
            cout << "Level " << level << ": ";
            for (int j = 0; j < cnt && i < sz; j++) {
                cout << arr[i++] << " ";
            }
            cout << endl;
            level++;
        }
    }
};
//////////////////////////////////////////////////////////////

int main() {
    int n;
    cout << "輸入測資數量: ";
    cin >> n;

    Heap<int> h1(true);
    cout << "輸入 MinHeap:\n";
    h1.build(n);

    cout << "\nMinHeap:\n";
    h1.printLevels();
    cout << "最小值: " << h1.top() << endl;
    h1.pop();
    cout << "刪除後:\n";
    h1.printLevels();

    Heap<int> h2(false);
    cout << "\n輸入 MaxHeap:\n";
    h2.build(n);

    cout << "\nMaxHeap:\n";
    h2.printLevels();
    cout << "最大值: " << h2.top() << endl;
    h2.pop();
    cout << "刪除後:\n";
    h2.printLevels();

    return 0;
}
