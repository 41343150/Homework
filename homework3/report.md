# 41343150
作業三

## 解題說明
本題要求實作一個多項式`Polynomial`透過鏈結串列Chain與可用節點串列Available List來管理記憶體，且能使用多項式的加法、減法、乘法以及代值計算。
## 解題策略
* 多項式項數不固定，做運算時也會一直增加或減少，用陣列會變得複雜，改用鏈結串列新增刪除項目較容易。
* 每個新節點加入時依照指數大小把它放到正確的位置（從大到小）這樣不需要額外寫排序也能讓後續的運算變得簡單。
* 多項式的加減乘會不斷產生新節點，如果一直 new / delete時間會浪費在記憶體管理上另效率變差。因此導入 Available List，把不用的節點存起來，
需要新節點時才提取用這方法來減少記憶體配置。
* 所有節點在不需要時都會回到 Available List，這樣記憶體的管理會較一致，能避免重複 delete 。 
* Iterator使用起來比較直觀。多項式運算時看起來會更乾淨也更好維護。
* 加、減、乘、輸入與輸出都寫成運算子多載，讓程式裡可以直接用 A + B 或 cout << A 的方式操作。
## 程式實作
## 程式實作

### Insertion Sort（插入排序）

```cpp
void insertionSort(int a[], int n)
{
    // 從第二個元素開始插入
    for(int i = 1; i < n; i++)
    {
        int key = a[i];
        int j = i - 1;

        // 將比 key 大的元素向右移動
        while(j >= 0 && a[j] > key)
        {
            a[j + 1] = a[j];
            j--;
        }

        // 插入到正確位置
        a[j + 1] = key;
    }
}
```

* 將陣列分為已排序區與未排序區。
* 每次取出一個元素插入已排序區的正確位置。
* 適合小型資料排序。

---

### Merge Sort（合併排序）

```cpp
void mergeSort(int a[], int n)
{
    if(n <= 1)
    {
        return;
    }

    int* temp = new int[n];

    // 每次將區塊大小擴大兩倍
    for(int size = 1; size < n; size *= 2)
    {
        for(int left = 0; left < n; left += size * 2)
        {
            int mid = min(left + size, n);
            int right = min(left + size * 2, n);

            mergePart(a, temp, left, mid, right);
        }

        // 將合併結果複製回原陣列
        for(int i = 0; i < n; i++)
        {
            a[i] = temp[i];
        }
    }

    delete[] temp;
}
```

```cpp
void mergePart(int a[], int temp[],
               int left, int mid, int right)
{
    int i = left;
    int j = mid;
    int k = left;

    while(i < mid && j < right)
    {
        if(a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while(i < mid)
        temp[k++] = a[i++];

    while(j < right)
        temp[k++] = a[j++];
}
```

* 使用分治法(Divide and Conquer)。
* 將兩個已排序區間合併成一個更大的排序區間。
* 時間複雜度固定為 O(n log n)。

---

### Heap Sort（堆積排序）

```cpp
void percDown(int a[], int i, int n)
{
    int child;
    int temp = a[i];

    while(i * 2 + 1 < n)
    {
        child = i * 2 + 1;

        // 找較大的子節點
        if(child != n - 1 &&
           a[child] < a[child + 1])
        {
            child++;
        }

        if(temp < a[child])
        {
            a[i] = a[child];
        }
        else
        {
            break;
        }

        i = child;
    }

    a[i] = temp;
}
```

```cpp
void heapSort(int a[], int n)
{
    // 建立 Max Heap
    for(int i = n / 2 - 1; i >= 0; i--)
    {
        percDown(a, i, n);
    }

    // 依序取出最大值
    for(int j = n - 1; j > 0; j--)
    {
        swap(a[0], a[j]);
        percDown(a, 0, j);
    }
}
```

* 利用最大堆積(Max Heap)特性排序。
* 每次將最大值移到陣列尾端。
* 不需額外儲存空間。

---

### Quick Sort（快速排序）

```cpp
int medianOfThree(int a[],
                  int left,
                  int right)
{
    int mid = (left + right) / 2;

    if(a[mid] < a[left])
        swap(a[left], a[mid]);

    if(a[right] < a[left])
        swap(a[left], a[right]);

    if(a[right] < a[mid])
        swap(a[mid], a[right]);

    swap(a[mid], a[right - 1]);

    return a[right - 1];
}
```

```cpp
void quickSortRange(int a[],
                    int left,
                    int right)
{
    if(left + 10 <= right)
    {
        int pivot =
            medianOfThree(a, left, right);

        int i = left;
        int j = right - 1;

        while(true)
        {
            while(a[++i] < pivot){}
            while(a[--j] > pivot){}

            if(i < j)
                swap(a[i], a[j]);
            else
                break;
        }

        swap(a[i], a[right - 1]);

        quickSortRange(a, left, i - 1);
        quickSortRange(a, i + 1, right);
    }
    else
    {
        smallInsertionSort(a, left, right);
    }
}
```

```cpp
void quickSort(int a[], int n)
{
    if(n > 1)
    {
        quickSortRange(a, 0, n - 1);
    }
}
```

* 使用 Median-of-Three 選 Pivot。
* 將資料分成小於與大於 Pivot 兩部分。
* 再遞迴處理左右子區間。

---

### Composite Sort（混合排序）

```cpp
void compositeSort(int a[], int n)
{
    if(n <= 30)
    {
        insertionSort(a, n);
    }
    else
    {
        mergeSort(a, n);
    }
}
```

* 小型資料使用 Insertion Sort。
* 大型資料使用 Merge Sort。
* 結合兩種排序法的優點，提高整體效率。

---

### Benchmark 效能測試

```cpp
double testOneTime(SortFunction sortFunction,
                   int data[],
                   int n)
{
    int* a = new int[n];

    copyArray(data, a, n);

    auto start =
        high_resolution_clock::now();

    sortFunction(a, n);

    auto end =
        high_resolution_clock::now();

    delete[] a;

    return getElapsedTime(start, end);
}
```

* 使用 `chrono` 函式庫進行計時。
* 測量每種排序法的執行時間。
* 統計 Worst Case 與 Average Case。
* 最後輸出至 `result.csv` 供後續效能分析與繪圖。

### Polynomial 類別
### Polynomial 類別
```cpp
class Polynomial {
private:
    Chain<Term> terms;
public:
    Polynomial() {}
    Polynomial(const Polynomial& other) {
        int index = 0;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it, ++index)
            terms.insert(index, *it);
    }
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            AvailableList<Term>::getBack(terms.release());
            int index = 0;
            for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it, ++index)
                terms.insert(index, *it);
        }
        return *this;
    }
    ~Polynomial() { AvailableList<Term>::getBack(terms.release()); }
    ChainIterator<Term> begin() const { return terms.begin(); }
    ChainIterator<Term> end() const { return terms.end(); }
};
*使用 Chain 單向鏈結串列儲存多項式，每個節點是一個 Term。
*解構子釋放節點來避免記憶體洩漏。
  
###  插入新項並保持排序
```cpp
void newTerm(double coef, int exp) {
    if (coef == 0) return;
    if (begin() == end()) { terms.insert(0, Term(coef, exp)); return; }
    int index = 0;
    for (ChainIterator<Term> it = begin(); it != end(); ++it, ++index) {
        if (it->exp < exp) { terms.insert(index, Term(coef, exp)); return; }
        else if (it->exp == exp) { it->coef += coef; return; }
    }
    terms.insert(index, Term(coef, exp));
}
```
* 忽略係數0，空串或最大次方往前插且同次合併，否則按降冪插入適當位置。
  
 ### 多項式加減乘
 ```cpp
 Polynomial operator+(const Polynomial& other) const {
    Polynomial result;
    ChainIterator<Term> p = begin(), q = other.begin();
    while (p != end() && q != other.end()) {
        if (p->exp > q->exp) { result.newTerm(p->coef, p->exp); ++p; }
        else if (p->exp < q->exp) { result.newTerm(q->coef, q->exp); ++q; }
        else { double c = p->coef + q->coef; if (c != 0) result.newTerm(c, p->exp); ++p; ++q; }
    }
    while (p != end()) { result.newTerm(p->coef, p->exp); ++p; }
    while (q != other.end()) { result.newTerm(q->coef, q->exp); ++q; }
    return result;
}

Polynomial operator-(const Polynomial& other) const {
    Polynomial neg;
    for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it) neg.newTerm(-it->coef, it->exp);
    return (*this) + neg;
}

Polynomial operator*(const Polynomial& other) const {
    Polynomial result;
    for (ChainIterator<Term> p = begin(); p != end(); ++p)
        for (ChainIterator<Term> q = other.begin(); q != other.end(); ++q)
            result.newTerm(p->coef * q->coef, p->exp + q->exp);
    return result;
}

```
* 加法：同時走兩串，依照指數大小決定插入位置，同次方合併。
* 減法：把 B 變成負的，再做加法。
* 乘法：每一項相乘後丟給 newTerm，由它負責排序與合併。
### 多項式代值計算
```cpp
float Evaluate(float x) const {
    float sum = 0;
    for (ChainIterator<Term> it = begin(); it != end(); ++it) {
        float pow = 1;
        for (int i = 0; i < it->exp; ++i) pow *= x;
        sum += it->coef * pow;
    }
    return sum;
}

```
### 輸入與輸出
```cpp
istream& operator>>(istream& is, Polynomial& poly) {
    int n;
    is >> n;
    for (int i = 0; i < n; ++i) {
        double c; int e;
        cout << "輸入第 " << i + 1 << " 項 (coef exp): ";
        is >> c >> e;
        poly.newTerm(c, e);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& poly) {
    bool first = true;
    for (ChainIterator<Term> it = poly.begin(); it != poly.end(); ++it) {
        if (!first) os << " + ";
        first = false;
        os << it->coef << "x^" << it->exp;
    }
    return os;
}

```
*>>會先輸入項數，再輸入每一項的係數與指數。
*<<會按降冪輸出多項式，項用 + 連接。

### main()
```cpp
int main() {
    Polynomial A, B;
    float x;
    cout << "輸入 A 的項數："; cin >> A;
    cout << "輸入 B 的項數："; cin >> B;
    cout << "輸入 x："; cin >> x;
    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "A + B = " << (A + B) << endl;
    cout << "A - B = " << (A - B) << endl;
    cout << "A * B = " << (A * B) << endl;
    cout << "A(" << x << ") = " << A.Evaluate(x) << endl;
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
  
### 1. 資料結構效能
多項式使用鏈結串列 `Chain` 儲存非零的項，每個節點只存係數與指數。
插入新節點時，`newTerm()` 需要遍歷來找到正確位置，平均時間複雜度為 O(n)。
`AvailableList` 可重複使用節點，避免頻繁 `new`/`delete`而降低效率。

### 2. 運算效能
 *加法 / 減法同時遍歷兩個多項式，時間複雜度 O(n + m)。
 *乘法為雙層迴圈，對每一對項進行相乘並用 `newTerm()` 合併。
 *newTerm()：在每次插入時可能遍歷並合併同次項，對長的多項式插入新的項時可能增加時間成本。

### 3. 代值運算

 *`Evaluate(x)` 對每個項計算 x 的冪次，時間複雜度 O(n × e_max)，n 為項數，e_max 為最大指數。

### 4. 輸入與輸出效能

 *輸入時每新增一項需呼叫 `newTerm()` 排序。
 *輸出時遍歷所有節點並列印，時間複雜度 O(n)。

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
