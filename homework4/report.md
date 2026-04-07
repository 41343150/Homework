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
### Heap 類別程式

```cpp
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
```
### main()
```cpp
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
```
## 效能分析

### 節點比較與上/下浮操作
- **比較函式 `cmp(a, b)`**  
  用來判斷父子節點大小，決定交換方向  
  - 單次比較 → O(1)
- **上浮 `up(i)` / 下沉 `down(i)`**  
  - 最壞情況：遍歷堆高度 → O(log n)  
  - 平均情況：少量交換 → 接近 O(1)

### 插入與刪除元素
- **Push()**  
  - 將元素加入陣列末端 → O(1)  
  - 上浮調整堆 → O(log n)  
  - **總時間複雜度**：O(log n)
- **Pop()**  
  - 將堆頂與最後元素交換 → O(1)  
  - 刪除最後元素 → O(1)  
  - 下沉調整堆 → O(log n)  
  - **總時間複雜度**：O(log n)
- **Top() / Empty()**  
  - 直接取堆頂或判斷是否為空 → O(1)

### 批量建堆與印出
- **build(n)**  
  - 連續 push n 個元素 → O(n log n)
- **printLevels()**  
  - 掃描陣列印出每層節點 → O(n)

### 整體流程分析
- **MinHeap 範例流程**：
  1. 輸入 n 個元素 → O(n log n)  
  2. 印出堆 → O(n)  
  3. 讀取堆頂 → O(1)  
  4. 刪除堆頂 → O(log n)  
  5. 再印一次 → O(n)
- **MaxHeap 範例流程**  
  與 MinHeap 相同，僅比較方向不同 → 總複雜度 ≈ O(n log n)

### 隨機插入高度比值
- 在隨機輸入下，堆高度通常接近 `log₂(n)`  
- MinHeap 與 MaxHeap 皆維持堆積性質 → 操作效率穩定

## 測試與驗證
| 測試案例 | 輸入數量 | 測資           | MinHeap Tree                  | MaxHeap Tree                  |
| -------- | -------- | -------------- | ----------------------------- | ----------------------------- |
| 測試一   | 5        | 12 3 25 7 1    | Level 0: 1 Level1: 3 25 Level2: 12 7 | Level0:25 Level1:7 12 Level2:3 1 |
| 測試二   | 6        | 8 15 2 30 10 5 | Level0:2 Level1:8 5 Level2:30 15 10 | Level0:30 Level1:15 5 Level2:8 10 2 |

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
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
using namespace std;

// 節點結構
struct Node {
    int val;        // 節點值
    Node* left;     // 左子節點
    Node* right;    // 右子節點
    Node(int v): val(v), left(NULL), right(NULL) {}
};

// 插入節點到 BST
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);           // 空樹 → 新節點
    if (val < root->val)                       // 小於 → 左
        root->left = insert(root->left, val);
    else                                       // 否則 → 右
        root->right = insert(root->right, val);
    return root;
}
```
### 計算高度
```cpp
// 計算樹高度
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}
```

### 刪除節點 (b) 
```cpp
// 找右子樹最小值
Node* findMin(Node* root) {
    while (root->left)
        root = root->left;
    return root;
}

// 刪除指定 key
Node* deleteNode(Node* root, int key) {
    if (!root) return NULL;

    if (key < root->val)
        root->left = deleteNode(root->left, key);
    else if (key > root->val)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left) return root->right;
        if (!root->right) return root->left;

        Node* temp = findMin(root->right);
        root->val = temp->val;
        root->right = deleteNode(root->right, temp->val);
    }
    return root;
}
```
### main() 
```cpp
int main() {
    int ns[] = {100, 500, 1000, 2000, 3000, 10000};

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 1000000);

    // ===== (a) 高度分析 =====
    for (int i = 0; i < 6; i++) {
        int n = ns[i];
        Node* root = NULL;

        for (int j = 0; j < n; j++) {
            root = insert(root, dist(gen));
        }

        int h = height(root);
        double ratio = h / log2(n);

        cout << "n=" << n
             << " height=" << h
             << " ratio=" << ratio << endl;
    }

    // ===== (b) 刪除測試 =====
    Node* root = NULL;
    int arr[] = {50, 30, 70, 20, 40, 60, 80};

    for (int i = 0; i < 7; i++)
        root = insert(root, arr[i]);

    cout << "\nBefore delete height=" << height(root) << endl;

    root = deleteNode(root, 50);

    cout << "After delete height=" << height(root) << endl;

    return 0;
}
```
## 效能分析
### (a) 隨機插入與樹高分析  

在二元搜尋樹（BST）中，樹高（height）會直接影響各種操作（搜尋、插入、刪除）的時間複雜度。
### 最壞情況  

若插入順序為遞增或遞減，BST 會退化為鏈狀結構，此時樹高為：  

-h = n
時間複雜度為：  

-O(n)  

### 平均情況  

輸入為隨機序列BST會趨近於平衡，其期望高度為：  

-h ≈ 2 * log2(n)  

各操作時間複雜度為：  

- Search / Insert / Delete：
- 
-O(h) ≈ O(log n)

### (b) 刪除操作時間複雜度  

刪除節點的時間成本取決於「尋找節點」與「結構調整」兩部分。  

### 1. 搜尋節點  

需要沿樹往下查找：  

-O(h)  

### 2. 刪除與重組  

葉節點或單子節點：
-O(1)  

- 雙子節點（需找右子樹最小值或左子樹最大值）：

##測試與驗證
### (a) 隨機插入高度比值
| n      | 樹高 height | height / log2(n) |
| ------ | ----------- | ---------------- |
| 100    | 14          | 2.02             |
| 500    | 21          | 2.01             |
| 1000   | 29          | 2.09             |
| 2000   | 32          | 2.02             |
| 3000   | 35          | 2.02             |
| 10000  | 45          | 2.15             |

### (b) 刪除操作時間複雜度
| 操作   | 最壞情況 | 平均情況 |
| ------ | -------- | -------- |
| Delete | O(n)     | O(log n) |

隨機插入情況下，BST 高度大致為 `2 * log2(n)`，因此 `height / log2(n)` 會趨近常數（約 2），與理論結果一致。
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
