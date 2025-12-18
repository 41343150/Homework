#include <iostream>
using namespace std;

struct Term {
    double coef;
    int exp;
    Term* next;

    Term(double c = 0, int e = 0, Term* n = nullptr)
        : coef(c), exp(e), next(n) {}
};

class Polynomial {
private:
    Term* head; // 排序：由大到小

public:
    Polynomial() : head(nullptr) {}

    ~Polynomial() {
        clear();
    }

    void clear() {
        while (head) {
            Term* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    // 插入並保持降冪、同次合併
    void newTerm(double coef, int exp) {
        if (coef == 0) return;

        // 1. 空串或插前面
        if (!head || head->exp < exp) {
            head = new Term(coef, exp, head);
            return;
        }

        Term* cur = head;
        Term* prev = nullptr;

        // 找插入位置
        while (cur && cur->exp > exp) {
            prev = cur;
            cur = cur->next;
        }

        // 2. 同次方 -> 合併
        if (cur && cur->exp == exp) {
            cur->coef += coef;
            if (cur->coef == 0) {
                // 合併後係數變0 -> 刪掉
                if (prev) prev->next = cur->next;
                else head = cur->next;
                delete cur;
            }
            return;
        }

        // 3. 插中間或後面
        Term* newNode = new Term(coef, exp, cur);
        if (prev) prev->next = newNode;
        else head = newNode;
    }

    // 加法
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Term* p = head;
        Term* q = b.head;

        while (p && q) {
            if (p->exp > q->exp) {
                result.newTerm(p->coef, p->exp);
                p = p->next;
            }
            else if (p->exp < q->exp) {
                result.newTerm(q->coef, q->exp);
                q = q->next;
            }
            else {
                double c = p->coef + q->coef;
                if (c != 0) result.newTerm(c, p->exp);
                p = p->next;
                q = q->next;
            }
        }
        while (p) { result.newTerm(p->coef, p->exp); p = p->next; }
        while (q) { result.newTerm(q->coef, q->exp); q = q->next; }

        return result;
    }

    // 減法
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Term* q = b.head;
        while (q) {
            result.newTerm(-q->coef, q->exp);
            q = q->next;
        }
        return *this + result;
    }

    // 乘法
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        for (Term* p = head; p; p = p->next) {
            for (Term* q = b.head; q; q = q->next) {
                result.newTerm(p->coef * q->coef, p->exp + q->exp);
            }
        }
        return result;
    }

    // Evaluate
    double Evaluate(double x) const {
        double sum = 0;
        for (Term* p = head; p; p = p->next) {
            double powX = 1;
            for (int i = 0; i < p->exp; i++) powX *= x;
            sum += p->coef * powX;
        }
        return sum;
    }

    // 輸入：先輸入幾項，再輸入（coef exp）
    friend istream& operator>>(istream& is, Polynomial& poly) {
        int n;
        is >> n;
        for (int i = 0; i < n; i++) {
            double c; int e;
            cout << "輸入第 " << i + 1 << " 項的係數與指數：";
            is >> c >> e;
            poly.newTerm(c, e);
        }
        return is;
    }

    // 輸出：格式 x^exp
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        if (!poly.head) return os << "0";

        bool first = true;
        for (Term* p = poly.head; p; p = p->next) {
            if (!first) os << " + ";
            first = false;
            os << p->coef << "x^" << p->exp;
        }
        return os;
    }
};

int main() {
    Polynomial A, B;
    double x;

    cout << "輸入 A 的項數：";
    cin >> A;

    cout << "輸入 B 的項數：";
    cin >> B;

    cout << "輸入 x：";
    cin >> x;

    cout << "A = " << A << endl;
    cout << "B = " << B << endl;

    cout << "A + B = " << (A + B) << endl;
    cout << "A - B = " << (A - B) << endl;
    cout << "A * B = " << (A * B) << endl;

    cout << "A(" << x << ") = " << A.Evaluate(x) << endl;

    return 0;
}
