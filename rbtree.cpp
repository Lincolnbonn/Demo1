#define BLACK 1
#define RED 0
#include <iostream>
#include <ctime>
using namespace std;

class bst {
private:

    struct Node {
        int value;//存data
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
        if (n->sibling()->color == RED) { //S是红色    //case 1: x的兄弟w是红色的
            n->parent->color = RED;//n父亲红色
            n->sibling()->color = BLACK;//n的兄弟变黑
            if (n == n->parent->leftTree) //如果n是n父亲的左子
                rotate_left(n->sibling());//n的兄弟左旋
            else
                rotate_right(n->sibling());//右子，兄弟
        }
        if (n->parent->color == BLACK && n->sibling()->color == BLACK//N的父亲、S和S的儿子都是黑色
            && n->sibling()->leftTree->color == BLACK && n->sibling()->rightTree->color == BLACK) 
           { //  case 2: x的兄弟是黑色， 且w的两个孩子也都是黑色
             n->sibling()->color = RED;//重绘S为红色
             delete_case(n->parent);//if (n->parent == NULL), n->color = BLACK;
            }
        // S和S的儿子都有黑色，但是N的父亲是红色, 这不影响不通过N的路径
        else if (n->parent->color == RED && n->sibling()->color == BLACK 
            && n->sibling()->leftTree->color == BLACK && n->sibling()->rightTree->color == BLACK) {
           //case 4: x的兄弟w是黑色的： 并且w的右孩子是红色的， 左孩子任意颜色
           
            n->sibling()->color = RED;//简单交换N的兄弟和父亲的颜色
            n->parent->color = BLACK;
        }
        else {
            //S是黑色， S的左儿子是红色， S的右儿子是黑色， 而N是它父亲的左儿子
            //在这种情形下，在S上做右旋转，交换S，P颜色，SR->黑色，重新获得平衡
           
            //x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
            if (n->sibling()->color == BLACK) { 
                if (n == n->parent->leftTree && n->sibling()->leftTree->color == RED
                    && n->sibling()->rightTree->color == BLACK) {
                    n->sibling()->color = RED;
                    n->sibling()->leftTree->color = BLACK;
                    rotate_right(n->sibling()->leftTree);
                }
                else if (n == n->parent->rightTree && n->sibling()->leftTree->color == BLACK
                    && n->sibling()->rightTree->color == RED) //w的左孩子是黑色，右孩子红色
                { 
                    n->sibling()->color = RED;
                    n->sibling()->rightTree->color = BLACK;
                    rotate_left(n->sibling()->rightTree);
                }
            }
            n->sibling()->color = n->parent->color; //交换S，P颜色
            n->parent->color = BLACK; 
            if (n == n->parent->leftTree) {//情形6： S是黑色，S的右儿子是红色，而N是它父亲的左儿子
                n->sibling()->rightTree->color = BLACK;
                rotate_left(n->sibling());
            }
            else {
                n->sibling()->leftTree->color = BLACK;//w的左孩子是黑色的，右孩子任意颜色。
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
        array[i - 1] = data; //存4 
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
/*打印结果： 
5 
15815 17872 19892 24876
15815 17872 24876
17872 24876
17872
2 3
*/