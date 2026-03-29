# 41343150
作業四

## 解題說明
###  Min / Max Heap
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
| 測試案例     | 輸入參數           | 輸出結果                    |
| ------------ | ---------------- | -------------------------- |
| 多項式 A     | 3 項              | Ax^4 + Bx^2 + Cx^0         |
| 多項式 B     | 2 項              | Dx^3 + Ex^1                |
| A + B        | -                | Ax^4 + Dx^3 + Bx^2 + Ex^1 + Cx^0 |
| A - B        | -                | Ax^4 - Dx^3 + Bx^2 - Ex^1 + Cx^0 |
| A * B        | -                | (相乘後的結果，保持降冪)       |
| A(x)        | x = 2            | 計算結果                    |

## 編譯執行指令
- 編譯程式
g++ -std=c++17 -o heap.exe your_file.cpp

- 執行程式
./heap.exe
# 申論及開發報告（重點畫意版）

## Heap 實作心得與設計

- **核心觀察**：MinHeap 與 MaxHeap 差別只在「比較規則」，整體結構一致 → 抽象化與程式重用非常重要  
- **維護堆積性質**：`heap_up` 與 `heap_down` 是核心操作  
  - 插入時向上調整 (`heap_up`)  
  - 刪除時向下調整 (`heap_down`)  
- **完全二元樹表示法**：使用 `vector` + 索引計算父子節點  
  - 父節點： `(i-1)/2`  
  - 左子節點： `2*i + 1`  
  - 右子節點： `2*i + 2`  
- **安全性處理**：使用 `runtime_error` 例外避免空堆操作崩潰  

## Heap Sort

- **排序步驟**
  1. 建立 MaxHeap  
  2. 將根（最大值）與尾元素交換  
  3. 縮小範圍，對新根進行 `heap_down`  
  4. 重複直到排序完成  

- **特性**
  - 時間複雜度：O(n log n)  
  - 空間複雜度：O(1)（原地排序）  

- **優勢與用途**
  - 快速建立堆（O(n)）  
  - 可做 Heap Sort  
  - 適合優先佇列  
  - 常用於圖論演算法（Dijkstra、Prim）  
  - Top-K 與即時資料分析（Median）  

- **進階設計思路**
  - 使用 `vector` 管理記憶體  
  - 增加輸入驗證與例外處理  
  - 用模板支援多種型別  
  - 將 MinHeap / MaxHeap 整合成單一通用類別  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
## 解題說明
###  Binary Search Tree（BST）
本題分為兩部分：
1.高度分析  

- 從空 BST 開始，插入 n 個隨機數
- 計算樹的高度 height，並計算比值 height / log₂(n)
- 驗證比值是否接近常數（約 2）
2.刪除節點
  
- 實作函式刪除指定 key 的節點
- 分析時間複雜度
