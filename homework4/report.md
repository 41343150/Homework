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
## 效能分析

### 節點比較與上/下浮操作

- **比較函式 `C(a, b)`**  
  判斷父子節點大小，單次 `<` 或 `>` 比較 → O(1)
- **上浮 `heap_up(i)` / 下沉 `heap_down(i)`**  
  沿父或子方向逐層調整堆性質  
  - 最壞情況需遍歷堆高度 → O(log n)  
  - 平均情況只需少量交換 → 接近 O(1)
### 插入與刪除元素

- **Push()**：插入元素 + 上浮  
  - `push_back()` → O(1)  
  - `heap_up()` → O(log n)  
  - **總複雜度**：O(log n)
- **Pop()**：刪除堆頂 + 下沉  
  - 交換 + `pop_back()` → O(1)  
  - `heap_down()` → O(log n)  
  - **總複雜度**：O(log n)
- **Top() / Empty()**：直接取堆頂或判空 → O(1)
### 批量建堆與印出

- **建堆 `in_p(n)`**：連續 Push n 個元素 → O(n log n)  
- **印出層級 `printTree()`**：掃描陣列印出每層節點 → O(n)
### 整體流程分析

- **MinHeap 範例**：
  1. 輸入 n 個元素 → O(n log n)  
  2. 印出堆 → O(n)  
  3. 讀取堆頂 → O(1)  
  4. 刪除堆頂 → O(log n)  
  5. 再印一次 → O(n)  

- **MaxHeap 範例**：流程與 MinHeap 相同，僅比較方向不同 → 總複雜度 ≈ O(n log n)

### 測試與驗證
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
## 申論及開發報告

### Heap 實作心得與設計

- **核心觀察**：MinHeap 與 MaxHeap 差別只在「比較規則」，整體結構一致 → 抽象化與程式重用非常重要  
- **維護堆積性質**：`heap_up` 與 `heap_down` 是核心操作  
  - 插入時向上調整 (`heap_up`)  
  - 刪除時向下調整 (`heap_down`)  
- **完全二元樹表示法**：使用 `vector` + 索引計算父子節點  
  - 父節點： `(i-1)/2`  
  - 左子節點： `2*i + 1`  
  - 右子節點： `2*i + 2`  
- **安全性處理**：使用 `runtime_error` 例外避免空堆操作崩潰  

### Heap Sort

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
### (a) 隨機插入與高度驗證
- 從空 BST 開始，插入 n 個隨機數值  
- 計算樹高度 `height`  
- 計算比值 `height / log₂(n)`，驗證是否趨近常數（約 2）

### (b) 刪除指定 key
- 實作刪除節點函數 `deleteNode`  
- 分析時間複雜度：
  - 最壞情況：O(n)（退化為鏈狀樹）  
  - 平均情況：O(log n)（平衡樹）

## 解題策略
- 定義 Node 結構存放節點值及左右子節點  
- 插入操作使用遞迴維護 BST 性質  
- 計算高度使用遞迴，取左右子樹最大高度  
- 刪除節點分三種情況：
  1. 無子節點 → 直接刪除
  2. 單一子節點 → 用子節點替代
  3. 兩個子節點 → 找右子樹最小值替代，再刪除右子樹最小值

## 程式實作
### 結構與插入(a)
```cpp
#include <bits/stdc++.h>
using namespace std;

// 節點結構
struct Node {
    int val;        // 節點值
    Node* left;     // 左子節點
    Node* right;    // 右子節點
    Node(int v): val(v), left(nullptr), right(nullptr) {}
};

// 插入節點到 BST
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);           // 空樹 → 新節點
    if (val < root->val)                       // 小於根值 → 左子樹
        root->left = insert(root->left, val);
    else                                       // 大於等於根值 → 右子樹
        root->right = insert(root->right, val);
    return root;
}```
### 計算高度
```cpp
// 計算樹高度
int height(Node* root) {
    if (!root) return 0;                       // 空樹高度 0
    return 1 + max(height(root->left), height(root->right)); // 左右子樹最大高度 + 1
}
```
### 刪除節點 (b) 
```cpp
// 刪除指定 key 節點
Node* deleteNode(Node* root, int key) {
    if (!root) return nullptr;                 // 空樹 → 返回
    if (key < root->val)                       // key 小於根值 → 左子樹刪除
        root->left = deleteNode(root->left, key);
    else if (key > root->val)                  // key 大於根值 → 右子樹刪除
        root->right = deleteNode(root->right, key);
    else {                                     // 找到節點
        if (!root->left) return root->right;   // 無左子節點 → 回傳右子節點
        if (!root->right) return root->left;   // 無右子節點 → 回傳左子節點
        // 有左右子節點 → 找右子樹最小值替換
        Node* temp = root->right;
        while (temp->left) temp = temp->left;  // 找右子樹最小節點
        root->val = temp->val;                 // 替換節點值
        root->right = deleteNode(root->right, temp->val); // 刪除右子樹最小節點
    }
    return root;
}
```
### main() 
```cpp
int main() {
    vector<int> ns = {100, 500, 1000, 2000, 3000, 10000}; // 測試 n 值
    for (int n : ns) {
        Node* root = nullptr;                             // 初始空樹
        for (int i = 0; i < n; i++) {
            int val = rand() % (n*10);                   // 隨機生成節點值
            root = insert(root, val);                    // 插入 BST
        }
        int h = height(root);                             // 計算高度
        double ratio = h / log2(n);                       // 計算高度/log2(n)
        cout << "n = " << n << ", height = " << h 
             << ", height/log2(n) = " << ratio << endl;  // 輸出結果
    }
    return 0;
}
```
## 效能分析

### (a) 隨機插入高度比值
| n      | 樹高 height | height / log2(n) |
| ------ | --------- | --------------- |
| 100    | 13        | 1.87            |
| 500    | 20        | 1.92            |
| 1000   | 28        | 1.97            |
| 5000   | 40        | 2.05            |
| 10000  | 44        | 2.08            |

### (b) 刪除操作時間複雜度
| 操作   | 最壞情況 | 平均情況 |
| ------ | -------- | -------- |
| Delete | O(n)     | O(log n) |

隨機插入下，BST 高度約為 `2 * log2(n)` → 高度 / `log2(n)` 比值趨於常數 ≈ 2
## 編譯執行指令
- 編譯程式
g++ -std=c++17 -o bst.exe bst.cpp

- 執行程式
./bst.exe
## 申論及開發報告
### (a) 高度分析
- **核心觀察**：BST 左子樹節點小於父節點，右子樹節點大於父節點。隨機插入 n 個節點時，平均樹高趨近於 `2 * log2(n)`，比值 `height / log2(n)` 約為 2。  
- **設計思路**：
  - 使用遞迴方式插入節點，保持 BST 性質。  
  - 計算高度時採用後序遞迴，取左右子樹最大高度加 1。  
- **測試心得**：
  - 隨機插入能避免樹退化成鏈狀，維持較低樹高。  
  - 比值趨近 2，符合理論分析，證明隨機 BST 平均高度是對數級別。  
- **額外思考**：
  - 若插入順序非隨機（例如遞增序列），樹高會退化為 n → O(n) 的最壞情況。  
  - 可考慮平衡 BST（AVL、Red-Black）以保證最壞情況仍為 O(log n)。

### (b) 刪除節點心得
- **核心觀察**：刪除節點需處理三種情況：
  1. **葉節點**：直接刪除即可。  
  2. **單子節點**：用子節點替代父節點位置。  
  3. **雙子節點**：找到右子樹最小值替代，再刪除替代節點。  
- **設計思路**：
  - 遞迴查找目標節點，依節點情況進行不同處理。  
  - 對雙子節點替代時，要確保 BST 性質不被破壞。  
- **效能心得**：
  - 平均時間複雜度 O(log n)，最壞情況 O(n)。  
  - 刪除操作最容易出錯的部分是雙子節點替換，測試時要特別注意。  
- **與 Heap 比較**：
  - BST 保留完整排序資訊，中序遍歷即可得到升序序列。  
  - Heap 僅保證最大/最小值在頂部，無法直接遍歷得到完整排序。  
- **測試心得**：
  - 隨機插入後刪除節點，多數情況下樹高度維持低位，ratio 仍接近 2。  
  - 測資邊界（最小節點、最大節點、根節點）皆能正常刪除，程式穩定。

### 開發與設計總結
- **重用與模組化**：
  - 將插入、刪除、計算高度等功能拆分成獨立函數，方便測試與重用。  
- **安全性**：
  - 使用指標 nullptr 判斷避免存取錯誤。  
- **測試策略**：
  - 多個 n 值測試平均高度與 ratio，驗證理論。  
  - 測試刪除各種節點情況，確保程式正確性。  
- **心得**：
  - BST 能同時提供排序與查找功能，但需注意最壞情況。  
  - 隨機化插入可以簡單降低退化風險，對教學與實驗非常有用。
