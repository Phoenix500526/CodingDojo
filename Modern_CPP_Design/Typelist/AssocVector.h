#pragma once

#include <functional>
#include <vector>
#include <utility>


template <typename Value, typename C>
class AssocVectorCompare : public C
{
    using KeyType = typename C::first_argument_type;
    using Data = std::pair<KeyType, Value>;
public:
    AssocVectorCompare() {}
    AssocVectorCompare(const C& obj) : C(obj) {}
    bool operator()(const KeyType& lhs, const KeyType& rhs) {
        return C::operator()(lhs, rhs);
    }
    bool operator()(const Data& lhs, const Data& rhs) {
        return operator()(lhs.first, rhs.first);
    }
    bool operator()(const KeyType& lhs, const Data& rhs) {
        return operator()(lhs, rhs.first);
    }
    bool operator()(const Data& lhs, const KeyType& rhs) {
        return operator()(lhs.first, rhs);
    }
};

template <typename K, typename V, typename C = std::less<K>, typename A = std::allocator<std::pair<K, V>>>
class AssocVector 
    : private std::vector<std::pair<K, V>, A>
    , private AssocVectorCompare<V, C>
{
    using Base = std::vector<std::pair<K, V>, A>;
    using MyCompare = AssocVectorCompare<V, C>;
public:
    using key_type = K;
    using mapped_type = V;
    using value_type = typename Base::value_type;
    using key_compare = C;
    using allocator_type = A;
    using reference = typename A::reference;
    using const_reference = typename A::const_reference;
    using iterator = typename Base::iterator;
    using const_iterator = typename Base::const_iterator;
    using size_type = typename Base::size_type;
    using difference_type = typename Base::difference_type;
    using pointer = typename A::pointer;
    using const_pointer = typename A::const_pointer;
    using reverse_iterator = typename Base::reverse_iterator;
    using const_reverse_iterator = typename Base::const_reverse_iterator;
    
    class value_compare : public std::binary_function<value_type, value_type, bool>, private key_compare 
    {
        friend class AssocVector;
    protected:
        explicit value_compare(const key_compare& pred) : key_compare(pred) {}
    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return key_compare::operator()(lhs.first, rhs.first);
        }
    };

    explicit AssocVector(const key_compare& comp = key_compare(), 
            const A& alloc = A()) : Base(alloc), MyCompare(comp) {}

    template <class InputIterator>
    AssocVector(const InputIterator& first, const InputIterator& last,
            const key_compare& comp = key_compare(), const A& alloc = A())
            : Base(first, last, alloc), MyCompare(comp) {
                MyCompare& self = *this;
                std::sort(begin(), end(), self);
            }

    AssocVector& operator=(const AssocVector& rhs){
        AssocVector(rhs).swap(*this);
        return *this;
    }

    iterator begin() { return Base::begin(); }
    const_iterator begin() const{ return Base::begin(); }
    iterator end() { return Base::end(); }
    const_iterator end() const { return Base::end(); }
    reverse_iterator rbegin() {return Base::rbegin(); }
    const_reverse_iterator rbegin() const{ return Base::rbegin(); }
    reverse_iterator rend() {return Base::rend(); }
    const_reverse_iterator rend() const{ return Base::rend(); }

    bool empty() const {return Base::empty(); }
    size_type size() const { return Base::size(); }
    size_type max_size() const { return Base::max_size(); }

    std::pair<iterator, bool> insert(const value_type& value){
        bool found(true);
        iterator it(lower_bound(value.first));
        if(it == end() || this->operator()(value.first, it->first)){
            it = Base::insert(it, value);
            found = false;
        }
        return std::make_pair(it, found);
    }

    iterator insert(iterator pos, const value_type& val) {
        if( (pos == begin() || this->operator()(*(pos-1),val)) && 
            (pos == end()   || this->operator()(val, *pos)) ) {
                return Base::insert(pos, val);
        }
        return insert(val).first;
    }

    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    { for (; first != last; ++first) insert(*first); }

    void erase(iterator pos) { Base::erase(pos); }
    void erase(iterator first, iterator last) { Base::erase(first, last); }
    bool erase(const key_type& k) {
        iterator i(find(k));
        if (i == end()) return false;
        erase(i);
        return true;
    }

    void clear() { Base::clear(); }

    void swap(AssocVector& other) {
        Base::swap(other);
        MyCompare& me = *this;
        MyCompare& rhs = other;
        std::swap(me, rhs);
    }
        
    mapped_type& operator[](const key_type& key) {
        return insert(value_type(key, mapped_type())).first->second;
    }

    iterator lower_bound(const key_type& key) {
        MyCompare& compare = *this;
        return std::lower_bound(begin(), end(), key, compare);
    }

    const_iterator lower_bound(const key_type& key) const {
        MyCompare& compare = *this;
        return std::lower_bound(begin(), end(), key, compare); 
    }

    iterator find(const key_type& key) {
        iterator iter = lower_bound(key);
        if(iter != end() && iter->first != key)
            iter = end();
        return iter;
    }

    const_iterator find(const key_type& key) const {
        const_iterator iter = lower_bound(key);
        if(iter != end() && iter->first != key)
            iter = end();
        return iter;
    }

    template <typename K1, typename V1, typename C1, typename A1>
    friend bool operator==(const AssocVector<K1, V1, C1, A1>&, const AssocVector<K1, V1, C1, A1>&);

    template <typename K1, typename V1, typename C1, typename A1>
    friend bool operator!=(const AssocVector<K1, V1, C1, A1>&, const AssocVector<K1, V1, C1, A1>&);
};


template <typename K1, typename V1, typename C1, typename A1>
inline bool operator==(const AssocVector<K1, V1, C1, A1>& lhs, const AssocVector<K1, V1, C1, A1>& rhs){
    const std::vector<std::pair<K1, V1>, A1>& me = lhs;
    return me == rhs;
}

template <typename K1, typename V1, typename C1, typename A1>
inline bool operator!=(const AssocVector<K1, V1, C1, A1>& lhs, const AssocVector<K1, V1, C1, A1>& rhs){
    return !(lhs == rhs);
}