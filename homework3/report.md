# 41343150
# Homework-2-Polynomial

## 解題說明
本題要求實作一個多項式`Polynomial`，能夠新增項、加法、乘法、代入 x 計算，並且照次方大小格式化顯示的多項式類別。
   
## 解題策略
* 使用 `Term` 結構來存放每一項的資料（係數、指數），讓項目比較清楚。
* 所有項目都存進動態陣列 `termArray`，如果容量滿了就自動擴充，避免項數增加時空間不足。
* 新增項目是透過 `NewTerm()` 完成的。加入新項前會先檢查是否有相同指數的項，如果有就把係數加在一起，若相加變成 0 也會把該項移除。
* 在 `NewTerm()` 的最後用簡單的氣泡排序，將所有項依照指數由大排到小，方便後續做加法、乘法和輸出格式。
* 做加法時，依序比較兩個多項式的指數，指數相同就相加，不同就把較大的先放進結果。
* 做乘法時，用雙層迴圈把每個項都相乘，再用 `NewTerm()` 自動處理合併同指數項。
* 輸出多項式時會處理正負號、x 的次方顯示等格式，讓結果看起來符合一般的多項式寫法。

## 程式實作
```cpp
#include <iostream>
#include <cmath>
using namespace std;

// ------------------------------
// PolyNode (for Polynomial only)
// ------------------------------
class PolyNode {
public:
    int coef, exp;
    PolyNode* link;
};

// ------------------------------
// Available List (Global Manager)
// ------------------------------
class Avail {
public:
    static PolyNode* head;

    static bool isEmpty() { return head == nullptr; }

    static PolyNode* getNode() {
        if (isEmpty()) return new PolyNode;
        PolyNode* p = head;
        head = head->link;
        return p;
    }

    static void returnNode(PolyNode* p) {
        p->link = head;
        head = p;
    }

    // 回收整串（除了 header 本身）
    static void getBack(PolyNode* header) {
        PolyNode* cur = header->link;
        while (cur != header) {
            PolyNode* next = cur->link;
            returnNode(cur);
            cur = next;
        }
        header->link = header; // 重設為空串
    }
};

PolyNode* Avail::head = nullptr;

// ------------------------------
// Polynomial
// ------------------------------
class Polynomial {
private:
    PolyNode* header;   // 環狀串列 header

public:
    // Constructor
    Polynomial() {
        header = Avail::getNode();
        header->coef = 0;
        header->exp = -1;
        header->link = header;
    }

    // Destructor
    ~Polynomial() {
        Avail::getBack(header);
    }

    // Copy Constructor
    Polynomial(const Polynomial& a) {
        header = Avail::getNode();
        header->coef = 0;
        header->exp = -1;
        header->link = header;

        PolyNode* cur = a.header->link;
        PolyNode* last = header;

        while (cur != a.header) {
            PolyNode* p = Avail::getNode();
            p->coef = cur->coef;
            p->exp = cur->exp;

            // append
            p->link = header;
            last->link = p;
            last = p;

            cur = cur->link;
        }
    }

    // Assignment Operator
    const Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;

        Avail::getBack(header);

        PolyNode* cur = a.header->link;
        PolyNode* last = header;

        while (cur != a.header) {
            PolyNode* p = Avail::getNode();
            p->coef = cur->coef;
            p->exp = cur->exp;

            p->link = header;
            last->link = p;
            last = p;

            cur = cur->link;
        }
        return *this;
    }

    // Insert (keep descending order)
    void insertTerm(int c, int e) {
        if (c == 0) return;

        PolyNode* cur = header;

        // find insertion point
        while (cur->link != header && cur->link->exp > e)
            cur = cur->link;

        // same exponent → combine
        if (cur->link != header && cur->link->exp == e) {
            cur->link->coef += c;
            if (cur->link->coef == 0) {
                // remove node
                PolyNode* rm = cur->link;
                cur->link = rm->link;
                Avail::returnNode(rm);
            }
            return;
        }

        PolyNode* p = Avail::getNode();
        p->coef = c;
        p->exp = e;
        p->link = cur->link;
        cur->link = p;
    }

    // Addition
    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        PolyNode* A = header->link;
        PolyNode* B = b.header->link;

        while (A != header && B != b.header) {
            if (A->exp == B->exp) {
                r.insertTerm(A->coef + B->coef, A->exp);
                A = A->link;
                B = B->link;
            } else if (A->exp > B->exp) {
                r.insertTerm(A->coef, A->exp);
                A = A->link;
            } else {
                r.insertTerm(B->coef, B->exp);
                B = B->link;
            }
        }
        while (A != header) { r.insertTerm(A->coef, A->exp); A = A->link; }
        while (B != b.header) { r.insertTerm(B->coef, B->exp); B = B->link; }

        return r;
    }

    // Subtraction (this - b)
    Polynomial operator-(const Polynomial& b) const {
        Polynomial r;
        PolyNode* A = header->link;
        PolyNode* B = b.header->link;

        while (A != header && B != b.header) {
            if (A->exp == B->exp) {
                r.insertTerm(A->coef - B->coef, A->exp);
                A = A->link;
                B = B->link;
            } else if (A->exp > B->exp) {
                r.insertTerm(A->coef, A->exp);
                A = A->link;
            } else {
                r.insertTerm(-B->coef, B->exp);
                B = B->link;
            }
        }
        while (A != header) { r.insertTerm(A->coef, A->exp); A = A->link; }
        while (B != b.header) { r.insertTerm(-B->coef, B->exp); B = B->link; }

        return r;
    }

    // Multiplication
    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        PolyNode* A = header->link;

        while (A != header) {
            PolyNode* B = b.header->link;
            while (B != b.header) {
                r.insertTerm(A->coef * B->coef, A->exp + B->exp);
                B = B->link;
            }
            A = A->link;
        }
        return r;
    }

    // Evaluate
    float Evaluate(float x) const {
        float sum = 0;
        PolyNode* cur = header->link;
        while (cur != header) {
            sum += cur->coef * pow(x, cur->exp);
            cur = cur->link;
        }
        return sum;
    }

    // Friend I/O operators
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, Polynomial&);
};

// ------------------------------
// Input >>
// ------------------------------
istream& operator>>(istream& is, Polynomial& x) {
    int n; is >> n;

    Avail::getBack(x.header);

    for (int i = 0; i < n; i++) {
        int c, e;
        is >> c >> e;
        x.insertTerm(c, e);
    }
    return is;
}

// ------------------------------
// Output <<
// ------------------------------
ostream& operator<<(ostream& os, Polynomial& x) {
    PolyNode* cur = x.header->link;
    int cnt = 0;

    while (cur != x.header) {
        cnt++;
        cur = cur->link;
    }

    os << cnt << " ";
    cur = x.header->link;
    while (cur != x.header) {
        os << cur->coef << " " << cur->exp << " ";
        cur = cur->link;
    }
    return os;
}

// ------------------------------
// Main (for testing)
// ------------------------------
int main() {
    Polynomial A, B;

    cout << "Input A: ";  cin >> A;
    cout << "Input B: ";  cin >> B;

    cout << "A = " << A << endl;
    cout << "B = " << B << endl;

    Polynomial C = A + B;
    Polynomial D = A - B;
    Polynomial E = A * B;

    cout << "A + B = " << C << endl;
    cout << "A - B = " << D << endl;
    cout << "A * B = " << E << endl;

    cout << "Evaluate A at x=2: " << A.Evaluate(2) << endl;

    return 0;
}

```
## 效能分析
**資料結構與排序影響**:
多項式以動態陣列儲存非零項，每項包含係數與指數。當容量不足時會自動倍增，平均新增效率為 O(1)如需重新配置記憶體，時間為 O(n)。
每次插入新項後都會依照指數大小排序，項數增加時排序成本上升 O(n²)。  

**運算效能分析**:
Add 函式依序遍歷兩個多項式的項進行合併，運作方式類似合併排序，效率不錯，效能主要取決於兩多項式的項數。  
Mult 函式採用雙層迴圈，每次相乘後呼叫 Add 函式來合併結果，整體效率較低。
Eval 函式利用 pow 計算每個項的冪次後相加，時間隨項數線性增加。
NewTerm 函式在插入新項時會使用氣泡排序並合併同次項，當項數多時效能下降。

**輸入、輸出與代入**:
代入運算只需線性遍歷所有項目，時間複雜度為 O(n)，空間需求非常。輸入階段每新增一項都需排序與合併，最差的狀況為 O(n²)。
輸出則為線性列印所有項，時間為 O(n)。  

## 測試與驗證
**測試1：多項式 p₁(x)**
| 項次 | 輸入參數 (係數, 指數) | 預期輸出 P₁(x)        | 實際輸出 P₁(x)        |
| -- | ------------- | ----------------- | ----------------- |
| 1  | (4, 3)        | 4x³ + 2x² + x + 5 | 4x³ + 2x² + x + 5 |
| 2  | (2, 2)        |                   |                   |
| 3  | (1, 1)        |                   |                   |
| 4  | (5, 0)        |                   |                   |
**測試2：多項式 p₂(x)**
| 項次 | 輸入參數 (係數, 指數) | 預期輸出 P₂(x)   | 實際輸出 P₂(x)   |
| -- | ------------- | ------------ | ------------ |
| 1  | (3, 2)        | 3x² + 2x + 4 | 3x² + 2x + 4 |
| 2  | (2, 1)        |              |              |
| 3  | (4, 0)        |              |              |
**測試3：多項式加法 p₁(x) + p₂(x)**
| 測試項目          | 預期輸出               | 實際輸出               |
| ------------- | ------------------ | ------------------ |
| p₁(x) + p₂(x) | 4x³ + 5x² + 3x + 9 | 4x³ + 5x² + 3x + 9 |
**測試4：多項式乘法 p₁(x) × p₂(x)**
| 測試項目          | 預期輸出                                 | 實際輸出                                 |
| ------------- | ------------------------------------ | ------------------------------------ |
| p₁(x) × p₂(x) | 12x⁵ + 14x⁴ + 15x³ + 18x² + 13x + 20 | 12x⁵ + 14x⁴ + 15x³ + 18x² + 13x + 20 |
**測試5：多項式代入運算**
| 測試項目  | 預期輸出 | 實際輸出 |
| ----- | ---- | ---- |
| p₁(2) | 47   | 47   |

## 編譯執行指令
**編譯**
g++ Polynomial.cpp -o Polynomial

**執行**
./Polynomial

## 申論及開發報告

程式以物件導向設計 (OOP) 為基礎，使用兩類別 Term 與 Polynomial 封裝多項式結構與運算邏輯。
Term 負責儲存單一項的係數與指數，Polynomial 則負責整體多項式的管理與運算，包含加法、乘法與代入計算等。
程式中使用動態陣列儲存多項式項次，當項數超出容量時自動擴充，確保可處理任意長度的多項式，此設計能在維持高效運算的同時避免記憶體浪費。

**效能討論**  
程式同時也有一些效能上的限制，例如:
NewTerm() 使用雙層迴圈排序，當項數增加時效率下降。
Mult() 採用雙層迴圈逐項相乘，對大型多項式效能不佳。

**心得討論**  
這份程式提供了一個完整的多項式類別實作，展示了物件導向程式設計的核心概念，包括類別封裝、成員函式、多載運算子與動態記憶體管理。
透過撰寫 Add()、Mult() 與 Eval() 等運算方法，可以學習如何將數學概念轉換為程式邏輯，並練習資料結構操作與演算法設計。


