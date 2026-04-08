/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <cstddef>
#include <functional>

#include "exceptions.hpp"
#include "utility.hpp"

namespace sjtu {

template <class Key, class T, class Compare = std::less<Key> >
class map {
   private:
    typedef pair<const Key, T> value_type;
    struct Node {
        value_type data;
        Node* lc;
        Node* rc;
        Node* fa;
        int height;
        Node(value_type& element, Node* lc, Node* rc, int h = 0)
            : data(element), lc(lc), rc(rc), height(h) {
        }
    };
    Node* root;
    Compare comp;
    size_t capacity;

    void destroy(Node* cur) {
        if (cur == nullptr) return;
        destroy(cur->lc);
        destroy(cur->rc);
        delete cur;
    }

    Node* clone(Node* other) {
        if (other == nullptr) return nullptr;
        Node* p = new Node(other->val);
        p->height = other->height;
        try {
            p->lc = clone(other->lc);
            p->rc = clone(other->rc);
        } catch (...) {
            destroy(p);
            throw;
        }
        return p;
    }

   public:
    /**
     * the internal type of data.
     * it should have a default constructor, a copy constructor.
     * You can use sjtu::map as value_type by typedef.
     */
    // typedef pair<const Key, T> value_type;

    /**
     * see BidirectionalIterator at CppReference for help.
     *
     * if there is anything wrong throw invalid_iterator.
     *     like it = map.begin(); --it;
     *       or it = map.end(); ++end();
     */
    class const_iterator;
    class iterator {
       private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        Node* cur;
        Node* root_;

       public:
        iterator() : cur(nullptr), root_(nullptr) {
        }
        iterator(Node* cur, Node* root_) : cur(cur), root_(root_) {
        }
        iterator(const iterator& other) : cur(other.cur), root_(other.root_) {
        }

        /**
         * TODO iter++
         */
        iterator operator++(int) {
            if (!cur) throw invalid_iterator();

            if (cur->rc) {
                cur = cur->rc;
                while (cur->lc) cur = cur->lc;
            } else {
                Node* p = cur;
                Node* parent = p->fa;
                while (parent && p == parent->rc) {
                    p = parent;
                    parent = parent->fa;
                }
                cur = parent;
            }
            return *this;
        }

        /**
         * TODO ++iter
         */
        iterator& operator++() {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * TODO iter--
         */
        iterator operator--(int) {
            if (!cur) {                                // end的情况
                if (!root_) throw invalid_iterator();  // empty tree
                cur = root_;
                while (cur->rc) cur = cur->rc;  // find the last element
                return *this;
            }
            if (cur->lc) {
                cur = cur->lc;
                while (cur->rc) cur = cur->rc;
            } else {
                Node* p = cur;
                Node* parent = p->fa;
                while (parent && p == parent->lc) {
                    p = parent;
                    parent = parent->fa;
                }
                if (!parent) throw invalid_iterator();  // cur = begin
                cur = parent;
            }
            return *this;
        }

        /**
         * TODO --iter
         */
        iterator& operator--() {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        value_type& operator*() const {
            if (!cur) throw invalid_iterator();
            return cur->data;
        }

        bool operator==(const iterator& rhs) const {
            return (cur == rhs.cur);
        }

        bool operator==(const const_iterator& rhs) const {
            return (cur == rhs.cur);
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator& rhs) const {
            return (cur != rhs.cur);
        }

        bool operator!=(const const_iterator& rhs) const {
            return (cur != rhs.cur);
        }

        /**
         * for the support of it->first.
         * See
         * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
         * for help.
         */
        value_type* operator->() const noexcept {
            if (!cur) throw invalid_iterator();
            return &(cur->data);
        }
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
       private:
        // data members.
        const Node* cur;
        const Node* root_;

       public:
        const_iterator() : cur(nullptr), root_(nullptr) {
        }
        const_iterator(Node* cur, Node* root_) : cur(cur), root_(root_) {
        }
        const_iterator(const const_iterator& other)
            : cur(other.cur), root_(other.root_) {
        }
        const_iterator(const iterator& other)
            : cur(other.cur), root_(other.root_) {
        }

        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            if (!cur) throw invalid_iterator();

            if (cur->rc) {
                cur = cur->rc;
                while (cur->lc) cur = cur->lc;
            } else {
                Node* p = cur;
                Node* parent = p->fa;
                while (parent && p == parent->rc) {
                    p = parent;
                    parent = parent->fa;
                }
                cur = parent;
            }
            return *this;
        }

        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            if (!cur) {                                // end的情况
                if (!root_) throw invalid_iterator();  // empty tree
                cur = root_;
                while (cur->rc) cur = cur->rc;  // find the last element
                return *this;
            }
            if (cur->lc) {
                cur = cur->lc;
                while (cur->rc) cur = cur->rc;
            } else {
                Node* p = cur;
                Node* parent = p->fa;
                while (parent && p == parent->lc) {
                    p = parent;
                    parent = parent->fa;
                }
                if (!parent) throw invalid_iterator();  // cur = begin
                cur = parent;
            }
            return *this;
        }

        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        value_type& operator*() const {
            if (!cur) throw invalid_iterator();
            return cur->data;
        }

        bool operator==(const iterator& rhs) const {
            return (cur == rhs.cur);
        }

        bool operator==(const const_iterator& rhs) const {
            return (cur == rhs.cur);
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator& rhs) const {
            return (cur != rhs.cur);
        }

        bool operator!=(const const_iterator& rhs) const {
            return (cur != rhs.cur);
        }

        /**
         * for the support of it->first.
         * See
         * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
         * for help.
         */
        value_type* operator->() const noexcept {
            if (!cur) throw invalid_iterator();
            return &(cur->data);
        }
    };

   public:
    /**
     * TODO two constructors
     */
    map() {
        root = nullptr;
    }

    map(const map& other) {
        root(clone(other.root));
        comp = other.comp;
        capacity = other.capacity;
    }

    /**
     * TODO assignment operator
     */
    map& operator=(const map& other) {
        if (this == &other) return *this;
        Node* tmp = clone(other.root);
        destroy(root);
        root = tmp;
        comp = other.comp;
        capacity = other.capacity;
        return *this;
    }

    /**
     * TODO Destructors
     */
    ~map() {
        destroy(root);
        root = nullptr;
        capacity = 0;
    }

    /**
     * TODO
     * access specified element with bounds checking
     * Returns a reference to the mapped value of the element with key
     * equivalent to key. If no such element exists, an exception of type
     * `index_out_of_bound'
     */
    T& at(const Key& key) {
        iterator it = find(key);
        try {
            T result = *it;
            return result;
        } catch (...) {
            throw index_out_of_bound();
        }
    }

    const T& at(const Key& key) const {
        iterator it = find(key);
        try {
            const T result = *it;
            return result;
        } catch (...) {
            throw index_out_of_bound();
        }
    }

    /**
     * TODO
     * access specified element
     * Returns a reference to the value that is mapped to a key equivalent to
     * key, performing an insertion if such key does not already exist.
     */
    T& operator[](const Key& key) {
        iterator it = find(key);
        try {
            T result = *it;
            return result;
        } catch (...) {
            T tmp;
            auto cur_result = insert(value_type(key, tmp));
            return tmp;
        }
    }

    /**
     * behave like at() throw index_out_of_bound if such key does not exist.
     */
    const T& operator[](const Key& key) const {
        iterator it = find(key);
        try {
            const T result = *it;
            return result;
        } catch (...) {
            throw index_out_of_bound();
        }
    }

    /**
     * return a iterator to the beginning
     */
    iterator begin() {
        Node* p = root;
        while (p->lc != nullptr) {
            p = p->lc;
        }
        return iterator(p, root);
    }

    const_iterator cbegin() const {
        Node* p = root;
        while (p->lc != nullptr) {
            p = p->lc;
        }
        return const_iterator(p, root);
    }

    /**
     * return a iterator to the end
     * in fact, it returns past-the-end.
     */
    iterator end() {
        Node* p = root;
        while (p->rc != nullptr) {
            p = p->rc;
        }
        return iterator(p, root);
    }

    const_iterator cend() const {
        Node* p = root;
        while (p->rc != nullptr) {
            p = p->rc;
        }
        return const_iterator(p, root);
    }

    /**
     * checks whether the container is empty
     * return true if empty, otherwise false.
     */
    bool empty() const {
        return (capacity == 0);
    }

    /**
     * returns the number of elements.
     */
    size_t size() const {
        return capacity;
    }

    /**
     * clears the contents
     */
    void clear() {
        destroy(root);
        capacity = 0;
    }

    /**
     * insert an element.
     * return a pair, the first of the pair is
     *   the iterator to the new element (or the element that prevented the
     * insertion), the second one is true if insert successfully, or false.
     */
    pair<iterator, bool> insert(const value_type& value) {
    }

    /**
     * erase the element at pos.
     *
     * throw if pos pointed to a bad element (pos == this->end() || pos points
     * an element out of this)
     */
    void erase(iterator pos) {
    }

    /**
     * Returns the number of elements with key
     *   that compares equivalent to the specified argument,
     *   which is either 1 or 0
     *     since this container does not allow duplicates.
     * The default method of check the equivalence is !(a < b || b > a)
     */
    size_t count(const Key& key) const {
        iterator it = find(key);
        try {
            T result = *it;
            return 1;
        } catch (...) {
            return 0;
        }
    }

    /**
     * Finds an element with key equivalent to key.
     * key value of the element to search for.
     * Iterator to an element with key equivalent to key.
     *   If no such element is found, past-the-end (see end()) iterator is
     * returned.
     */
    iterator find(const Key& key) {
        Node* cur = root;
        while (cur != nullptr && cur->data.first != key) {
            if (cur->data.first > key) {
                cur = cur->lc;
            } else
                cur = cur->rc;
        }
        if (cur == nullptr)
            return iterator(nullptr, root);
        else
            return iterator(cur, root);
    }

    const_iterator find(const Key& key) const {
        Node* cur = root;
        while (cur != nullptr && cur->data.first != key) {
            if (cur->data.first > key) {
                cur = cur->lc;
            } else
                cur = cur->rc;
        }
        if (cur == nullptr)
            return const_iterator(nullptr, root);
        else
            return const_iterator(cur, root);
    }
};

}  // namespace sjtu

#endif
