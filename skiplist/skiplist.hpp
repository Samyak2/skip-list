/* 
Skip list implementation
Only the skiplist container should be visible
*/
#include <vector>
using std::vector;

struct SLNode {
    // Poorly named - left, right, up, down pointers
    SLNode *l, *r, *u, *d;
    // Value, should be templated
    int val;
    // Count is integer only
    int count;
    SLNode(int val_=0)
    : l(nullptr), r(nullptr), u(nullptr), d(nullptr), val(val_) { this->count = 0; }
};

template<typename value_type>
class skiplist {
    private:
    vector<SLNode> key;
    int size_;

    public:
    class iterator {};
    
    skiplist();
    int size() {return size_;};

    // specialized functions
    void insert(value_type value);
    void erase(value_type value);
    iterator find(value_type value);
};