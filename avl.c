
#include <stdio.h>
#include <stdlib.h>

/* ---------- Node structure ---------- */
typedef struct Node {
    int key;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

/* ---------- Helper functions ---------- */
int height(Node *node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

Node *createNode(int key)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->key = key;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

int getBalance(Node *node)
{
    if (node == NULL)
        return 0;

    return height(node->left) - height(node->right);
}

/* ---------- Rotations ---------- */
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

/* ---------- Insert ---------- */
Node *insert(Node *root, int key)
{
    if (root == NULL)
        return createNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else {
        printf("Duplicate key not allowed.\n");
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    /* LL case */
    if (balance > 1 && key < root->left->key)
        return rightRotate(root);

    /* RR case */
    if (balance < -1 && key > root->right->key)
        return leftRotate(root);

    /* LR case */
    if (balance > 1 && key > root->left->key) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    /* RL case */
    if (balance < -1 && key < root->right->key) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* ---------- Delete ---------- */
Node *minValueNode(Node *node)
{
    Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

Node *deleteNode(Node *root, int key)
{
    if (root == NULL)
        return root;

    /* Normal BST deletion */
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        /* Node has zero or one child */
        if (root->left == NULL || root->right == NULL) {
            Node *temp;

            if (root->left != NULL)
                temp = root->left;
            else
                temp = root->right;

            if (temp == NULL) {
                free(root);
                return NULL;
            } else {
                Node *oldRoot = root;
                root = temp;
                free(oldRoot);
            }
        }
        /* Node has two children */
        else {
            Node *temp = minValueNode(root->right);

            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    /* Update height and check balance */
    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    /* LL case */
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    /* LR case */
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    /* RR case */
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    /* RL case */
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

/* ---------- Search ---------- */
Node *search(Node *root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key)
        return search(root->left, key);

    return search(root->right, key);
}

/* ---------- Traversals ---------- */
void inorder(Node *root)
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

void preorder(Node *root)
{
    if (root != NULL) {
        printf("%d ", root->key);
        preorder(root->left);
        preorder(root->right);
    }
}

/* ---------- Free allocated memory ---------- */
void freeTree(Node *root)
{
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/* ---------- Clear invalid input ---------- */
void clearInput(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

/* ---------- Main menu ---------- */
int main(void)
{
    Node *root = NULL;
    int choice, key;

    while (1) {
        printf("\n\n========================\n");
        printf("        AVL TREE\n");
        printf("========================\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Search\n");
        printf("4. Inorder Traversal\n");
        printf("5. Preorder Traversal\n");
        printf("6. Exit\n");
        printf("========================\n");
        printf("Enter your choice: ");
        fflush(stdout);

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInput();
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter key to insert: ");
                fflush(stdout);

                if (scanf("%d", &key) != 1) {
                    printf("Invalid key.\n");
                    clearInput();
                    break;
                }

                root = insert(root, key);
                printf("Key inserted successfully.\n");
                break;

            case 2:
                printf("Enter key to delete: ");
                fflush(stdout);

                if (scanf("%d", &key) != 1) {
                    printf("Invalid key.\n");
                    clearInput();
                    break;
                }

                if (search(root, key) == NULL) {
                    printf("Key not found.\n");
                } else {
                    root = deleteNode(root, key);
                    printf("Key deleted successfully.\n");
                }
                break;

            case 3:
                printf("Enter key to search: ");
                fflush(stdout);

                if (scanf("%d", &key) != 1) {
                    printf("Invalid key.\n");
                    clearInput();
                    break;
                }

                if (search(root, key) != NULL)
                    printf("Key found.\n");
                else
                    printf("Key not found.\n");

                break;

            case 4:
                printf("Inorder traversal: ");

                if (root == NULL)
                    printf("Tree is empty.");
                else
                    inorder(root);

                printf("\n");
                break;

            case 5:
                printf("Preorder traversal: ");

                if (root == NULL)
                    printf("Tree is empty.");
                else
                    preorder(root);

                printf("\n");
                break;

            case 6:
                freeTree(root);
                printf("Program terminated.\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}

