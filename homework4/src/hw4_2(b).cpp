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

// 插入（和 A 一樣）
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);
    if (val < root->val)
        root->left = insert(root->left, val);
    else
        root->right = insert(root->right, val);
    return root;
}

// 高度（和 A 一樣）
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

// 找最小值
Node* findMin(Node* root) {
    while (root->left)
        root = root->left;
    return root;
}

// 刪除
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
