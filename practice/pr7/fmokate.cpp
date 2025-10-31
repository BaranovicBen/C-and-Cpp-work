#ifndef __TRAINER__
#include <cassert>

template<typename T_>
class SetTester;
#endif

template<typename T_>
struct Set {

    struct Node {
        Node(const T_ & v)
            : m_Value(v) {}
        Node * m_Left = nullptr;
        Node * m_Right = nullptr;
        Node * m_Prev = nullptr;
        Node * m_Next = nullptr;
        T_ m_Value;
    };

    Node * m_Root = nullptr; 
    Node * m_First = nullptr; 
    Node * m_Last = nullptr;

    friend SetTester<T_>;

    Set() = default;
    Set(const Set &) = delete;
    Set & operator=(const Set &) = delete;
    ~Set() {
        Node* cur = m_First;
        while(cur){
            Node* nxt = cur->m_Next;
            delete cur;
            cur =nxt;
        }
        m_Root = nullptr;
        m_First = nullptr;
        m_Last = nullptr;
    }

    bool contains(const T_& val) {
        auto ptr = m_Root;
        while(ptr) {
            if(val < ptr->m_Value) {
                ptr = ptr->m_Left;
            }
            else if(ptr->m_Value < val) {
                ptr = ptr->m_Right;
            }
            else {
                return true;
            }
        }
        return false;
    }

bool insert(const T_ & value) {
        if (contains(value))
            return false;
        Node* parent = nullptr;
        Node* cur    = m_Root;
        while (cur) {
            parent = cur;
            if (value < cur->m_Value)
                cur = cur->m_Left;
            else
                cur = cur->m_Right;
        }
        Node* node = new Node(value);
        if (!parent) {
            m_Root = node;
        }
        else if (value < parent->m_Value) {
            parent->m_Left = node;
        }
        else {
            parent->m_Right = node;
        }
        node->m_Prev = m_Last;
        node->m_Next = nullptr;
        if (m_Last) {
            m_Last->m_Next = node;
        }
        else {
            m_First = node;
        }
        m_Last = node;

        return true;
    }

    bool erase(const T_ & value) {
        Node* parent = nullptr;
        Node* z      = m_Root;
        while (z) {
            if (value < z->m_Value) {
                parent = z;
                z = z->m_Left;
            }
            else if (z->m_Value < value) {
                parent = z;
                z = z->m_Right;
            }
            else {
                break;
            }
        }
        if (!z) return false;

        if (z->m_Prev) z->m_Prev->m_Next = z->m_Next;
        else            m_First            = z->m_Next;
        if (z->m_Next) z->m_Next->m_Prev = z->m_Prev;
        else            m_Last             = z->m_Prev;

        if (!z->m_Left || !z->m_Right) {
            Node* child = z->m_Left ? z->m_Left : z->m_Right;
            if (!parent)           m_Root = child;
            else if (z == parent->m_Left)  parent->m_Left  = child;
            else                            parent->m_Right = child;
            delete z;
        }
        else {
            Node* succParent = z;
            Node* succ       = z->m_Right;
            while (succ->m_Left) {
                succParent = succ;
                succ       = succ->m_Left;
            }
            Node* succRight = succ->m_Right;
            if (succParent->m_Left == succ) succParent->m_Left  = succRight;
            else                            succParent->m_Right = succRight;
            succ->m_Left  = z->m_Left;
            if (succParent != z)
                succ->m_Right = z->m_Right;
            if (!parent)           m_Root = succ;
            else if (z == parent->m_Left)  parent->m_Left  = succ;
            else                            parent->m_Right = succ;
            delete z;
        }

        return true;
    }
};

#ifndef __TRAINER__
struct UselessInt {
    int val;
    UselessInt(int v) : val(v) {}
    bool operator<(const UselessInt & other) const {
        return val < other.val;
    }
};

int main () {
    Set<UselessInt> t;
    assert(!t.erase(5));
    assert(t.insert(5));
    assert(!t.insert(5));
    assert(t.contains(5));
    assert(t.erase(5));
    assert(t.insert(5));
    assert(t.insert(7));
    assert(t.insert(2));
    assert(t.insert(3));
    assert(!t.contains(1));
    assert(t.contains(2));
    assert(t.contains(3));
    assert(!t.contains(4));
    assert(t.contains(5));
    assert(!t.contains(6));
    assert(t.contains(7));
    assert(!t.contains(8));
    assert(t.insert(6));
    assert(t.erase(2));
    assert(!t.contains(1));
    assert(!t.contains(2));
    assert(t.contains(3));
    assert(!t.contains(4));
    assert(t.contains(5));
    assert(t.contains(6));
    assert(t.contains(7));
    assert(!t.contains(8));
    assert(t.erase(7));
    assert(!t.contains(1));
    assert(!t.contains(2));
    assert(t.contains(3));
    assert(!t.contains(4));
    assert(t.contains(5));
    assert(t.contains(6));
    assert(!t.contains(7));
    assert(!t.contains(8));
    assert(t.erase(5));
    assert(!t.contains(1));
    assert(!t.contains(2));
    assert(t.contains(3));
    assert(!t.contains(4));
    assert(!t.contains(5));
    assert(t.contains(6));
    assert(!t.contains(7));
    assert(!t.contains(8));
    return 0;
}
#endif