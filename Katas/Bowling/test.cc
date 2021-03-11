#include <cassert>
#include <iostream>
using namespace std;

enum class bonus_t {
    zero,
    one,
    two,
};

template <class E, E last, E current>
void backwardEnum(E& v) {
    if (v == current) {
        v = last;
    }
}

template <class E, E last, E pre, E current, E... tail>
void backwardEnum(E& v) {
    if (v == current) {
        v = pre;
    } else {
        backwardEnum<E, last, current, tail...>(v);
    }
}

template <typename E, E last, E head, E... values>
struct EnumValues {
    static void backward(E& v) {
        if (v == head) {
            v = last;
        } else {
            backwardEnum<E, last, head, values...>(v);
        }
    }
};

using bonus_value = EnumValues<bonus_t, bonus_t::zero, bonus_t::zero>;

bonus_t& operator--(bonus_t& f) {
    bonus_value::backward(f);
    return f;
}

int main() {
    bonus_t test = bonus_t::zero;
    --test;
    assert(test == bonus_t::zero);
    --test;
    assert(test == bonus_t::zero);
    --test;
    assert(test == bonus_t::zero);
    --test;
    assert(test == bonus_t::zero);
    return 0;
}