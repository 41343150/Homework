#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

//////////////////////////////////////////////////////////////////
// Heap 類別模板，可建構 MinHeap 或 MaxHeap
template <typename T>
class Heap {
private:
    vector<T> data;       // 存放堆元素
    bool isMinHeap;       // true = MinHeap, false = MaxHeap

    // 比較函式，決定父子節點交換方向
    bool compare(const T& a, const T& b) const {
        return isMinHeap ? a < b : a > b;
    }

    // 往上調整堆
    void siftUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (compare(data[idx], data[parent])) {
                swap(data[idx], data[parent]);
                idx = parent;
            } else break;
        }
    }

    // 往下調整堆
    void siftDown(int idx) {
        int n = data.size();
        while (2 * idx + 1 < n) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int swapIdx = left;

            if (right < n && compare(data[right], data[left])) swapIdx = right;

            if (compare(data[swapIdx], data[idx])) {
                swap(data[idx], data[swapIdx]);
                idx = swapIdx;
            } else break;
        }
    }

public:
    Heap(bool minHeap = true) : isMinHeap(minHeap) {}

    bool empty() const { return data.empty(); }

    const T& top() const {
        if (empty()) throw runtime_error("Heap is empty!");
        return data[0];
    }

    void push(const T& val) {
        data.push_back(val);
        siftUp(data.size() - 1);
    }

    void pop() {
        if (empty()) throw runtime_error("Heap is empty!");
        swap(data[0], data.back());
        data.pop_back();
        if (!empty()) siftDown(0);
    }

    void build(int n) {
        T x;
        for (int i = 0; i < n; i++) {
            cin >> x;
            push(x);
        }
    }

    void printLevels() const {
        int n = data.size();
        int idx = 0, level = 0;
        while (idx < n) {
            int nodes = 1 << level;
            cout << "Level " << level << ": ";
            for (int i = 0; i < nodes && idx < n; i++) {
                cout << data[idx++] << " ";
            }
            cout << endl;
            level++;
        }
    }
};
//////////////////////////////////////////////////////////////////

int main() {
    int n;
    cout << "請輸入測資數量: ";
    cin >> n;

    // MinHeap
    Heap<int> minHeap(true);
    cout << "輸入 " << n << " 個數字 (MinHeap):\n";
    minHeap.build(n);

    cout << "\nMinHeap:\n";
    minHeap.printLevels();
    cout << "最小值: " << minHeap.top() << endl;
    minHeap.pop();
    cout << "刪除最小值後:\n";
    minHeap.printLevels();

    // MaxHeap
    Heap<int> maxHeap(false);
    cout << "\n輸入 " << n << " 個數字 (MaxHeap):\n";
    maxHeap.build(n);

    cout << "\nMaxHeap:\n";
    maxHeap.printLevels();
    cout << "最大值: " << maxHeap.top() << endl;
    maxHeap.pop();
    cout << "刪除最大值後:\n";
    maxHeap.printLevels();

    cout << "\n41343124.洋芋 =>防委標記\n";
    return 0;
}
