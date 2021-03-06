#include "catch.hpp"
#include <memory>
#include <vector>
#include <cstdlib>
#include <cstdarg>

namespace ch13 {

void make_exception(const char* fmt, ...)
{
    static char buf[16];
    char* p;

    va_list ap;
	va_start(ap, fmt);
    int size = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (size < 0) {
        throw std::runtime_error{"vsnprintf failed"};
    }
    if (size < sizeof(buf)) { // NOTE: need 1 byte for \0 terminator
        throw std::runtime_error{buf};
    }
    std::cout << "allocating for make_exception...\n";
    ++size; // for null terminator
    p = new char[size];
    if (!p) {
        throw std::bad_alloc{};
    }
    va_start(ap, fmt);
    size = vsnprintf(p, size, fmt, ap);
    va_end(ap);
    if (size < 0) {
        throw std::runtime_error{"vsnprintf failed"};
    }
    throw std::runtime_error{p};
}

struct IntSetArray {
    IntSetArray(int maxelements, int maxval)
        : _buf{new int[1 + maxelements]}
        , _capacity{maxelements}
        , _count{0}
    {
        _buf[0] = maxval;
    }

    ~IntSetArray() noexcept { delete[] _buf; }

    void insert(int t) {
        if (t > _buf[_count]) {
            make_exception("value=%d greater than maxval=%d, count=%d", t, _buf[_count], _count);
        }
        if (_count == _capacity) {
            throw std::runtime_error{"Set is full"};
        }
        int i;
        for (i = 0; _buf[i] < t; ++i)
            ;
        if (_buf[i] == t)
            return;
        memmove(&_buf[i+1], &_buf[i], (_count + 1 - i)*sizeof(int));
        _buf[i] = t;
        ++_count;
    }

    const int* begin() const noexcept { return &_buf[0]; }
    const int* end() const noexcept { return &_buf[_count]; }

    int* _buf;
    int _capacity;
    int _count;
};

struct IntSetList {
    struct Node {
        Node(int v, Node* n = nullptr) : next(n), val(v) {}
        ~Node() {
            if (next != nullptr) {
                delete next;
            }
        }
        Node* next;
        int val;
    };

    IntSetList(int maxelements, int maxval)
        : _head(new Node(maxval))
    {
    }

    const Node* head() const {
        return _head;
    }

    void insert(int t) {
        Node* cur = _head;
        while (cur->val < t) {
            cur = cur->next;
        }
        if (cur->val == t) {
            return;
        }
        Node* n = new Node(cur->val, cur->next);
        cur->val = t;
        cur->next = n;
    }

    template <class F>
    void visit(F f) {
        Node* c = _head;
        while (c->next != nullptr) {
            f(c->val);
            c = c->next;
        }
    }

    Node* _head;
};

template <class C>
void printer(const C& c) {
    std::cout << ">> ";
    for (auto it : c)
        std::cout << it << " ";
    std::cout << "\n";
}

} /*ch13*/

TEST_CASE("IntSetArray", "isa")
{
    using namespace ch13;

    IntSetArray ia(10, 400);
    printer(ia);
    ia.insert(1);
    ia.insert(1);
    printer(ia);
    ia.insert(2);
    printer(ia);
    ia.insert(42);
    ia.insert(3);
    ia.insert(100);
    ia.insert(0);
    printer(ia);
}

TEST_CASE("IntSetList", "isl")
{
    using namespace ch13;

    IntSetList ia(10, 400);
    ia.insert(1);
    printf(">> "); ia.visit([](int x) { printf("%d ", x); }); printf("\n");
    ia.insert(2);
    ia.insert(2);
    ia.insert(3);
    ia.insert(6);
    printf(">> "); ia.visit([](int x) { printf("%d ", x); }); printf("\n");
    ia.insert(4);
    printf(">> "); ia.visit([](int x) { printf("%d ", x); }); printf("\n");

}
