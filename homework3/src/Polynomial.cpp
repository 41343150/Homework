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
    Term* head;

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

    void newTerm(double coef, int exp) {
        if (coef == 0) return;
        if (!head || head->exp < exp) {
            head = new Term(coef, exp, head);
            return;
        }

        Term* cur = head;
        Term* prev = nullptr;

        while (cur && cur->exp > exp) {
            prev = cur;
            cur = cur->next;
        }

        if (cur && cur->exp == exp) {
            cur->coef += coef;
            if (cur->coef == 0) {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                delete cur;
            }
            return;
        }

        Term* newNode = new Term(coef, exp, cur);
        if (prev) prev->next = newNode;
        else head = newNode;
    }

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

    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Term* q = b.head;
        while (q) {
            result.newTerm(-q->coef, q->exp);
            q = q->next;
        }
        return *this + result;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        for (Term* p = head; p; p = p->next) {
            for (Term* q = b.head; q; q = q->next) {
                result.newTerm(p->coef * q->coef, p->exp + q->exp);
            }
        }
        return result;
    }

    double Evaluate(double x) const {
        double sum = 0;
        for (Term* p = head; p; p = p->next) {
            double powX = 1;
            for (int i = 0; i < p->exp; i++) powX *= x;
            sum += p->coef * powX;
        }
        return sum;
    }

    friend istream& operator>>(istream& is, Polynomial& poly) {
        int n;
        is >> n;
        for (int i = 0; i < n; i++) {
            double c; 
            int e;
            cout << "輸入第 " << i + 1 << " 項的係數與指數：";
            is >> c >> e;
            poly.newTerm(c, e);
        }
        return is;
    }

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
