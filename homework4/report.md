# 41343150
作業四

## 解題說明
### (1) Min / Max Heap
定義 MinPQ 抽象類別（包含 insert、extractMin 等操作），  
後再 MinHeap 繼承並用 Binary Heap，各操作時間複雜度與 MaxHeap 對應相同。  
再設計 MaxHeap 並與 MinHeap 做對照，觀察兩者在操作上的時間複雜度是否一樣。

## 解題策略
- 用 vector 存完全二元樹
- 透過索引模擬樹：
  - parent = (i - 1) / 2
  - left = 2*i + 1
  - right = 2*i + 2
- 用比較函式決定是 MinHeap 還是 MaxHeap
  - heap_up 用來維持插入後的堆積性質
  - heap_down 用來維持刪除後的堆積性質
## 程式實作
### Heap 類別
```cpp
template <typename T>
class Heap {
private:
    vector<T> data;       // 存放堆元素
    bool isMinHeap;       // true = MinHeap, false = MaxHeap

    // 比較函式決定父子節點交換方向
    bool compare(const T& a, const T& b) const {
        return isMinHeap ? a < b : a > b;
    }

    // 往上調整
    void siftUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (compare(data[idx], data[parent])) {
                swap(data[idx], data[parent]);
                idx = parent;
            } else break;
        }
    }

    // 往下調整
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
```
### 公開介面
```cpp
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
```
### main()
```cpp
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

    return 0;
}
```
# 效能分析

## 節點比較與上/下浮操作

- **比較函式 `C(a, b)`**  
  判斷父子節點大小，單次 `<` 或 `>` 比較 → O(1)
- **上浮 `heap_up(i)` / 下沉 `heap_down(i)`**  
  沿父或子方向逐層調整堆性質  
  - 最壞情況需遍歷堆高度 → O(log n)  
  - 平均情況只需少量交換 → 接近 O(1)
## 插入與刪除元素

- **Push()**：插入元素 + 上浮  
  - `push_back()` → O(1)  
  - `heap_up()` → O(log n)  
  - **總複雜度**：O(log n)
- **Pop()**：刪除堆頂 + 下沉  
  - 交換 + `pop_back()` → O(1)  
  - `heap_down()` → O(log n)  
  - **總複雜度**：O(log n)
- **Top() / Empty()**：直接取堆頂或判空 → O(1)
## 批量建堆與印出

- **建堆 `in_p(n)`**：連續 Push n 個元素 → O(n log n)  
- **印出層級 `printTree()`**：掃描陣列印出每層節點 → O(n)
## 整體流程分析

- **MinHeap 範例**：
  1. 輸入 n 個元素 → O(n log n)  
  2. 印出堆 → O(n)  
  3. 讀取堆頂 → O(1)  
  4. 刪除堆頂 → O(log n)  
  5. 再印一次 → O(n)  

- **MaxHeap 範例**：流程與 MinHeap 相同，僅比較方向不同 → 總複雜度 ≈ O(n log n)

## 測試與驗證
| 測試案例     | 輸入參數                | 預期輸出                               | 實際輸出                             |
| -------- | ------------------- | ---------------------------------- | -------------------------------- |
| 第一個多項式 A | (7,4), (2,2), (6,0) | 7x⁴ + 2x² + 6x⁰                    | 7x⁴ + 2x² + 6x⁰                  |
| 第二個多項式 B | (3,3), (4,1)        | 3x³ + 4x¹                          | 3x³ + 4x¹                        |
| A + B    | -                   | 7x⁴ + 3x³ + 2x² + 4x¹ + 6x⁰        | 7x⁴ + 3x³ + 2x² + 4x¹ + 6x⁰      |
| A - B    | -                   | 7x⁴ + -3x³ + 2x² + -4x¹ + 6x⁰      | 7x⁴ + -3x³ + 2x² + -4x¹ + 6x⁰    |
| A * B    | -                   | 21x⁷ + 28x⁵ + 14x⁴ + 12x³ + 24x¹   | 21x⁷ + 28x⁵ + 14x⁴ + 12x³ + 24x¹ |
| A(2)     | x = 2               | 7*16 + 2*4 + 6 = 128 + 8 + 6 = 142 | 142                              |
### 測試輸入
輸入 A 的項數: 3
輸入第 1 項 (係數 指數): 7 4
輸入第 2 項 (係數 指數): 2 2
輸入第 3 項 (係數 指數): 6 0

輸入 B 的項數: 2
輸入第 1 項 (係數 指數): 3 3
輸入第 2 項 (係數 指數): 4 1

輸入 x 為多少: 2
### 測試輸出
A = 7x^4 + 2x^2 + 6x^0
B = 3x^3 + 4x^1
A + B = 7x^4 + 3x^3 + 2x^2 + 4x^1 + 6x^0
A - B = 7x^4 + -3x^3 + 2x^2 + -4x^1 + 6x^0
A * B = 21x^7 + 28x^5 + 14x^4 + 12x^3 + 24x^1
A(2) = 142

## 編譯執行指令
g++ -std=c++17 -o polynomial.exe your_file.cpp
.\polynomial.exe

## 申論及開發報告

這程式主要功能包括建立多項式、加、減、乘法，及在特定值下的求值。程式可以自動合併相同次方的項目，並保持多項式按指數從大到小排列，這樣輸出會清楚且計算正確。
經由多重測試案例的驗證，本程式都能夠產出預期的輸出。
在記憶體管理的方面，程式用了鏈結串列有效的避免頻繁配置與釋放節點所造成的效能消耗，這樣不僅提升了運算效率，更是降低了記憶體碎片化的風險。
本程式採用物件導向的設計思維，架構清晰且模組化，有利於後續的維護與功能擴充。透過迭代器提供的標準化鏈結串列遍歷方式，多項式運算的邏輯得以簡化並易於擴展。
