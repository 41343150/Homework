#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
using namespace std;

// 節點結構
struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int v): val(v), left(NULL), right(NULL) {}
};

// 插入
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);
    if (val < root->val)
        root->left = insert(root->left, val);
    else
        root->right = insert(root->right, val);
    return root;
}

// 高度
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

int main() {
    int ns[] = {100, 500, 1000, 2000, 3000, 10000};

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 1000000);

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

    return 0;
}
