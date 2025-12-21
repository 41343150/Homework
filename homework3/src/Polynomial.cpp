#include <iostream>
#include <cstdlib>
using namespace std;

template <class T> class ChainNode;
template <class T> class ChainIterator;
template <class T> class Chain;
template <class T> class AvailableList;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode)
        : element(elem), next(nextNode) {}
    void setElement(const T& elem) { element = elem; }
    void setNext(ChainNode<T>* nextNode) { next = nextNode; }
};

template <class T>
class ChainIterator {
private:
    ChainNode<T>* currentNode;
public:
    ChainIterator(ChainNode<T>* startNode = nullptr) : currentNode(startNode) {}
    T& operator*() const { return currentNode->element; }
    T* operator->() const { return &currentNode->element; }
    ChainIterator& operator++() { if (currentNode) currentNode = currentNode->next; return *this; }
    ChainIterator operator++(int) { ChainIterator old = *this; if (currentNode) currentNode = currentNode->next; return old; }
    bool operator!=(const ChainIterator& right) const { return currentNode != right.currentNode; }
    bool operator==(const ChainIterator& right) const { return currentNode == right.currentNode; }
};

template <class T>
class AvailableList {
private:
    static ChainNode<T>* availableHead;
public:
    static ChainNode<T>* getNode() {
        if (!availableHead) return nullptr;
        ChainNode<T>* node = availableHead;
        availableHead = availableHead->next;
        node->next = nullptr;
        return node;
    }
    static void getBack(ChainNode<T>* firstNode) {
        if (!firstNode) return;
        ChainNode<T>* current = firstNode;
        while (current->next) current = current->next;
        current->next = availableHead;
        availableHead = firstNode;
    }
    static ChainNode<T>* getOneNodeOrNew(const T& elem) {
        ChainNode<T>* node = getNode();
        if (!node) return new ChainNode<T>(elem);
        node->setElement(elem);
        return node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::availableHead = nullptr;

template <class T>
class Chain {
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() {
        ChainNode<T>* cur = head;
        while (cur) { ChainNode<T>* next = cur->next; delete cur; cur = next; }
    }
    ChainNode<T>* release() { ChainNode<T>* old = head; head = nullptr; return old; }
    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }
    void insert(int idx, const T& element) {
        ChainNode<T>* newNode = AvailableList<T>::getOneNodeOrNew(element);
        if (idx == 0) { newNode->next = head; head = newNode; return; }
        ChainNode<T>* prev = head;
        for (int i = 0; i < idx - 1 && prev; ++i) prev = prev->next;
        if (!prev) { AvailableList<T>::getBack(newNode); return; }
        newNode->next = prev->next; prev->next = newNode;
    }
};

struct Term {
    double coef;
    int exp;
    Term() : coef(0.0), exp(0) {}
    Term(double c, int e) : coef(c), exp(e) {}
};

AvailableList<Term> globalASL;

class Polynomial {
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);
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
    float Evaluate(float x) const {
        float sum = 0;
        for (ChainIterator<Term> it = begin(); it != end(); ++it) {
            float pow = 1;
            for (int i = 0; i < it->exp; ++i) pow *= x;
            sum += it->coef * pow;
        }
        return sum;
    }
};

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
