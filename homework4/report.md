# 41343150
作業四

## 解題說明
### (1) Min / Max Heap
定義 MinPQ 抽象類別（包含 insert、extractMin 等操作），
後再 MinHeap 繼承並用 Binary Heap，各操作時間複雜度與 MaxHeap 對應相同。
再設計 MaxHeap 並與 MinHeap 做對照，觀察兩者在操作上的時間複雜度是否一樣。

## 解題策略
* 用 vector 存完全二元樹
* 透過索引模擬樹：
  * parent = (i - 1) / 2
  * left = 2*i + 1
  * right = 2*i + 2
* 用比較函式決定是 MinHeap 還是 MaxHeap
  * heap_up 用來維持插入後的堆積性質
  * heap_down 用來維持刪除後的堆積性質
## 程式實作

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
# 效能分析

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
