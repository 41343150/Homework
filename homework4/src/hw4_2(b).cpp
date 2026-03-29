#include <iostream>
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

// inorder 印出
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

int main() {
    Node* root = NULL;
    int arr[] = {50, 30, 70, 20, 40, 60, 80};

    for (int i = 0; i < 7; i++)
        root = insert(root, arr[i]);

    inorder(root);
    cout << endl;

    root = deleteNode(root, 50);

    inorder(root);
    cout << endl;

    return 0;
}
