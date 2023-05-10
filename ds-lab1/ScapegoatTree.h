#include <vector>
#include <iostream>
using namespace std;

class ScapegoatTree
{
private:
    struct node {
        int key;
        node *parent;
        node *lc;
        node *rc;

        explicit node(int k, node *p=nullptr, node *l=nullptr, node *r=nullptr):
        key(k), parent(p), lc(l), rc(r) {}
        node() = default;
        ~node() = default;
    };
    double alpha;
    node *root;
    int max_size;
    int size;
    int rebalanceTimes;

    void setMaxSize() {max_size = size > max_size ? size : max_size;}
    node *rebuild(node *arr[], int start, int end, node *p);
    void Rebuild(node *p);
    node *findScapeGoat(node *p);
    int calSize(node *p);
    void inOrder(node *p, node *arr[], int &pos);
    static int removeMin(node *p);
    void Reset(node *p);

public:
    explicit ScapegoatTree(double a): alpha(a),
    root(nullptr), max_size(0), size(0), rebalanceTimes(0) {}
    ~ScapegoatTree() = default;

    void Reset() {Reset(root); size = max_size = 0; root = nullptr;}
    void traverse() const;

    /**
     * @brief Get the Rebalance Times
     *
     * @return int
     */
	int GetRebalanceTimes() const;

    /**
     * @brief Look up a key in scapegoat tree. Same as trivial binary search tree
     *
     * @param key
     */
    int Search(int key);

    /**
     * @brief Insert a new node into the tree. If the key is already in the tree, then do nothing
     *
     * @param key
     */
    void Insert(int key);

    /**
     * @brief Delete a node of the tree. If the key is not in the tree, do nothing.
     *
     * @param key
     */
    void Delete(int key);
};
