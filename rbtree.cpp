#define BLACK 1
#define RED 0
#include <iostream>
#include <ctime>
using namespace std;

class bst {
private:

    struct Node {
        int value;//��data
        bool color; //BLACK 1 RED 0
        Node *leftTree, *rightTree, *parent;

        Node() {
            color = RED;
            leftTree = NULL;
            rightTree = NULL;
            parent = NULL;
            value = 0;
        }

        Node* grandparent() {
            if (parent == NULL) {
                return NULL;
            }
            return parent->parent;
        }

        Node* uncle() {
            if (grandparent() == NULL) {
                return NULL;
            }
            if (parent == grandparent()->rightTree)
                return grandparent()->leftTree;
            else
                return grandparent()->rightTree;
        }

        Node* sibling() {
            if (parent->leftTree == this)
                return parent->rightTree;
            else
                return parent->leftTree;
        }
    };

    void rotate_right(Node *n) {
        Node *gp = n->grandparent();
        Node *fa = n->parent;
        Node *y = n->rightTree;

        fa->leftTree = y;

        if (y != NIL)
            y->parent = fa;
        n->rightTree = fa;
        fa->parent = n;

        if (root == fa)
            root = n;
        n->parent = gp;

        if (gp != NULL) {
            if (gp->leftTree == fa)
                gp->leftTree = n;
            else
                gp->rightTree = n;
        }

    }

    void rotate_left(Node *n) {
        if (n->parent == NULL) {
            root = n;
            return;
        }
        Node *gp = n->grandparent();
        Node *fa = n->parent;
        Node *y = n->leftTree;

        fa->rightTree = y;

        if (y != NIL)
            y->parent = fa;
        n->leftTree = fa;
        fa->parent = n;

        if (root == fa)
            root = n;
        n->parent = gp;

        if (gp != NULL) {
            if (gp->leftTree == fa)
                gp->leftTree = n;
            else
                gp->rightTree = n;
        }
    }

    void inorder(Node *n) {
        if (n == NIL)
            return;

        if (n->leftTree)
            inorder(n->leftTree);

        cout << n->value << " ";

        if (n->rightTree)
            inorder(n->rightTree);
    }

    string outputColor(bool color) {
        return color ? "BLACK" : "RED";
    }

    Node* getSmallestChild(Node *n) {
        if (n->leftTree == NIL)
            return n;
        return getSmallestChild(n->leftTree);
    }

    bool delete_child(Node *n, int data) {
        if (n->value > data) {
            if (n->leftTree == NIL) {
                return false;
            }
            return delete_child(n->leftTree, data);
        }
        else if (n->value < data) {
            if (n->rightTree == NIL) {
                return false;
            }
            return delete_child(n->rightTree, data);
        }
        else if (n->value == data) {
            if (n->rightTree == NIL) {
                delete_one_child(n);
                return true;
            }
            Node *smallest = getSmallestChild(n->rightTree);
            swap(n->value, smallest->value);
            delete_one_child(smallest);

            return true;
        }
        else {
            return false;
        }
    }

    void delete_one_child(Node *n) {
        Node *child = n->leftTree == NIL ? n->rightTree : n->leftTree;
        if (n->parent == NULL && n->leftTree == NIL && n->rightTree == NIL) {
            n = NULL;
            root = n;
            return;
        }

        if (n->parent == NULL) {
            delete  n;
            child->parent = NULL;
            root = child;
            root->color = BLACK;
            return;
        }

        if (n->parent->leftTree == n) {
            n->parent->leftTree = child;
        }
        else {
            n->parent->rightTree = child;
        }
        child->parent = n->parent;

        if (n->color == BLACK) {
            if (child->color == RED) {
                child->color = BLACK;
            }
            else
                delete_case(child);
        }

        delete n;
    }

    void delete_case(Node *n) {
        if (n->parent == NULL) {
            n->color = BLACK;
            return;
        }
        if (n->sibling()->color == RED) { //S�Ǻ�ɫ    //case 1: x���ֵ�w�Ǻ�ɫ��
            n->parent->color = RED;//n���׺�ɫ
            n->sibling()->color = BLACK;//n���ֵܱ��
            if (n == n->parent->leftTree) //���n��n���׵�����
                rotate_left(n->sibling());//n���ֵ�����
            else
                rotate_right(n->sibling());//���ӣ��ֵ�
        }
        if (n->parent->color == BLACK && n->sibling()->color == BLACK//N�ĸ��ס�S��S�Ķ��Ӷ��Ǻ�ɫ
            && n->sibling()->leftTree->color == BLACK && n->sibling()->rightTree->color == BLACK) 
           { //  case 2: x���ֵ��Ǻ�ɫ�� ��w����������Ҳ���Ǻ�ɫ
             n->sibling()->color = RED;//�ػ�SΪ��ɫ
             delete_case(n->parent);//if (n->parent == NULL), n->color = BLACK;
            }
        // S��S�Ķ��Ӷ��к�ɫ������N�ĸ����Ǻ�ɫ, �ⲻӰ�첻ͨ��N��·��
        else if (n->parent->color == RED && n->sibling()->color == BLACK 
            && n->sibling()->leftTree->color == BLACK && n->sibling()->rightTree->color == BLACK) {
           //case 4: x���ֵ�w�Ǻ�ɫ�ģ� ����w���Һ����Ǻ�ɫ�ģ� ����������ɫ
           
            n->sibling()->color = RED;//�򵥽���N���ֵܺ͸��׵���ɫ
            n->parent->color = BLACK;
        }
        else {
            //S�Ǻ�ɫ�� S��������Ǻ�ɫ�� S���Ҷ����Ǻ�ɫ�� ��N�������׵������
            //�����������£���S��������ת������S��P��ɫ��SR->��ɫ�����»��ƽ��
           
            //x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��  
            if (n->sibling()->color == BLACK) { 
                if (n == n->parent->leftTree && n->sibling()->leftTree->color == RED
                    && n->sibling()->rightTree->color == BLACK) {
                    n->sibling()->color = RED;
                    n->sibling()->leftTree->color = BLACK;
                    rotate_right(n->sibling()->leftTree);
                }
                else if (n == n->parent->rightTree && n->sibling()->leftTree->color == BLACK
                    && n->sibling()->rightTree->color == RED) //w�������Ǻ�ɫ���Һ��Ӻ�ɫ
                { 
                    n->sibling()->color = RED;
                    n->sibling()->rightTree->color = BLACK;
                    rotate_left(n->sibling()->rightTree);
                }
            }
            n->sibling()->color = n->parent->color; //����S��P��ɫ
            n->parent->color = BLACK; 
            if (n == n->parent->leftTree) {//����6�� S�Ǻ�ɫ��S���Ҷ����Ǻ�ɫ����N�������׵������
                n->sibling()->rightTree->color = BLACK;
                rotate_left(n->sibling());
            }
            else {
                n->sibling()->leftTree->color = BLACK;//w�������Ǻ�ɫ�ģ��Һ���������ɫ��
                rotate_right(n->sibling());
            }
        }
    }

    void insert(Node *n, int data) {
        if (n->value >= data) {
            if (n->leftTree != NIL)
                insert(n->leftTree, data);
            else {
                Node *tmp = new Node();
                tmp->value = data;
                tmp->leftTree = tmp->rightTree = NIL;
                tmp->parent = n;
                n->leftTree = tmp;
                insert_case(tmp);
            }
        }
        else {
            if (n->rightTree != NIL)
                insert(n->rightTree, data);
            else {
                Node *tmp = new Node();
                tmp->value = data;
                tmp->leftTree = tmp->rightTree = NIL;
                tmp->parent = n;
                n->rightTree = tmp;
                insert_case(tmp);
            }
        }
    }

    void insert_case(Node *n) {
        if (n->parent == NULL) {
            root = n;
            n->color = BLACK;
            return;
        }
        if (n->parent->color == RED) {
            if (n->uncle()->color == RED) {
                n->parent->color = n->uncle()->color = BLACK;
                n->grandparent()->color = RED;
                insert_case(n->grandparent());
            }
            else {
                if (n->parent->rightTree == n && n->grandparent()->leftTree == n->parent) {
                    rotate_left(n);
                    rotate_right(n);
                    n->color = BLACK;
                    n->leftTree->color = n->rightTree->color = RED;
                }
                else if (n->parent->leftTree == n && n->grandparent()->rightTree == n->parent) {
                    rotate_right(n);
                    rotate_left(n);
                    n->color = BLACK;
                    n->leftTree->color = n->rightTree->color = RED;
                }
                else if (n->parent->leftTree == n && n->grandparent()->leftTree == n->parent) {
                    n->parent->color = BLACK;
                    n->grandparent()->color = RED;
                    rotate_right(n->parent);
                }
                else if (n->parent->rightTree == n && n->grandparent()->rightTree == n->parent) {
                    n->parent->color = BLACK;
                    n->grandparent()->color = RED;
                    rotate_left(n->parent);
                }
            }
        }
    }

    void DeleteTree(Node *n) {
        if (!n || n == NIL) {
            return;
        }
        DeleteTree(n->leftTree);
        DeleteTree(n->rightTree);
        delete n;
    }
public:

    bst() {
        NIL = new Node();
        NIL->color = BLACK;
        root = NULL;
    }

    ~bst() {
        if (root)
            DeleteTree(root);
        delete NIL;
    }

    void inorder() {
        if (root == NULL)
            return;
        inorder(root);
        cout << endl;
    }

    void insert(int x) {
        if (root == NULL) {
            root = new Node();
            root->color = BLACK;
            root->leftTree = root->rightTree = NIL;
            root->value = x;
        }
        else {
            insert(root, x);
        }
    }

    bool delete_value(int data) {
        return delete_child(root, data);
    }
private:
    Node *root, *NIL;
};

int main(int argc, char **argv)
{
    int n, data;
    bst *tree = new bst();
    cin >> n;
    int* array = (int*)malloc(n * sizeof(int));
    

    srand(time(NULL));
    int i;
    for (i = 1; i <= n; i++)
    {   
        data = rand(); //cin >> data
        array[i - 1] = data; //��4 
        tree->insert(data);
    }

    i = 0;
    while (i<n) {
        if (tree->delete_value(array[i]) == false) {
            return -1;
        }
        i++;
        tree->inorder();
    }
    tree->insert(3); 
    tree->insert(2); 
    tree->inorder(); 
    delete tree; 
    system("pause"); 
    return 0; 
   
}
/*��ӡ����� 
5 
15815 17872 19892 24876
15815 17872 24876
17872 24876
17872
2 3
*/