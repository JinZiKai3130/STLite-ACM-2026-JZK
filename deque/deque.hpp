#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include <cstddef>

#include "exceptions.hpp"

namespace sjtu {

template <class T>
class deque {
   public:
    static const int chunk_size = 16;
    class const_iterator;
    class iterator {
       private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        T* ptr;
        T* stat;  // there is an element on the address
        T* end;   // no element on the address
        T** node;
        const void* identity;
        void node_move(T** new_node) {
            node = new_node;
            stat = *new_node;
            end = new_node + chunk_size;
        }

       public:
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is
         * **undefined**. as well as operator-
         */
        iterator() = default;
        iterator(T* ptr, T* stat, T* end, T** node, const void* indentity)
            : ptr(ptr), stat(stat), end(end), node(node), identity(identity) {
        }
        iterator(const iterator& other)
            : ptr(other.ptr),
              stat(other.stat),
              end(other.end),
              node(other.node),
              identity(other.identity) {
        }
        iterator operator+(const int& n) const {
            // TODO
            auto result = *this;
            result += n;
            return result;
        }
        iterator operator-(const int& n) const {
            // TODO
            auto result = *this;
            result -= n;
            return result;
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw
        // invaild_iterator.
        int operator-(const iterator& rhs) const {
            // TODO
            if (identity != rhs.identity) {
                throw invalid_iterator();
            }
            if (node == rhs.node) {
                return ptr - rhs.ptr;
            } else {
                return (node - rhs.node) * chunk_size + (ptr - stat) -
                       (rhs.ptr - rhs.stat);
            }
        }
        iterator operator+=(const int& n) {
            // TODO
            int d1 = end - ptr - 1;
            if (n <= d1) {
                ptr += n;
                return *this;
            } else {
                int d2 = (n - d1) % chunk_size;
                node_move(node + (n - d1) / chunk_size);
                ptr = stat + d2;
                return *this;
            }
        }
        iterator operator-=(const int& n) {
            // TODO
            int d1 = ptr - stat;
            if (n <= d1) {
                ptr -= n;
                return *this;
            } else {
                int d2 = (n - d1) % chunk_size;
                node_move(node - (n - d1) / chunk_size);
                ptr = end - d2 - 1;
                return *this;
            }
        }
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            auto result = *this;
            ++*this;
            return result;
        }
        /**
         * TODO ++iter
         */
        iterator& operator++() {
            ++ptr;
            if (ptr == end) {
                set_node(node + 1);
                ptr = stat;
            }
            return *this;
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            auto result = *this;
            --*this;
            return result;
        }
        /**
         * TODO --iter
         */
        iterator& operator--() {
            if (ptr == stat) {
                set_node(node - 1);
                ptr = end;
            }
            --ptr;
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const {
            return *ptr;
        }
        /**
         * TODO it->field
         */
        T* operator->() const noexcept {
            return ptr;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        bool operator==(const iterator& rhs) const {
            return (ptr == rhs.ptr);
        }
        bool operator==(const const_iterator& rhs) const {
            return (ptr == rhs.ptr);
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator& rhs) const {
            return (ptr != rhs.ptr);
        }
        bool operator!=(const const_iterator& rhs) const {
            return (ptr != rhs.ptr);
        }
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
       private:
        // data members.
        const T* ptr;
        const T* stat;  // there is an element on the address
        const T* end;   // no element on the address
        const T** node;
        const void* identity;

       public:
        const_iterator() = default;
        const_iterator(const iterator& other)
            : ptr(other.ptr),
              stat(other.stat),
              end(other.end),
              node(other.node),
              identity(other.identity) {
            // TODO
        }
        const_iterator operator+(const int& n) const {
            // TODO
            auto result = *this;
            result += n;
            return result;
        }
        const_iterator operator-(const int& n) const {
            // TODO
            auto result = *this;
            result -= n;
            return result;
        }
        int operator-(const const_iterator& rhs) const {
            // TODO
            if (identity != rhs.identity) {
                throw invalid_iterator();
            }
            if (node == rhs.node) {
                return ptr - rhs.ptr;
            } else {
                return (node - rhs.node) * chunk_size + (ptr - stat) -
                       (rhs.ptr - rhs.stat);
            }
        }
        const_iterator operator+=(const int& n) {
            // TODO
            int d1 = end - ptr - 1;
            if (n <= d1) {
                ptr += n;
                return *this;
            } else {
                int d2 = (n - d1) % chunk_size;
                node_move(node + (n - d1) / chunk_size);
                ptr = stat + d2;
                return *this;
            }
        }
        const_iterator operator-=(const int& n) {
            // TODO
            int d1 = ptr - stat;
            if (n <= d1) {
                ptr -= n;
                return *this;
            } else {
                int d2 = (n - d1) % chunk_size;
                node_move(node - (n - d1) / chunk_size);
                ptr = end - d2 - 1;
                return *this;
            }
        }
        const_iterator operator++(int) {
            auto result = *this;
            ++*this;
            return result;
        }
        const_iterator& operator++() {
            ++ptr;
            if (ptr == end) {
                set_node(node + 1);
                ptr = stat;
            }
            return *this;
        }
        const_iterator operator--(int) {
            auto result = *this;
            --*this;
            return result;
        }
        const_iterator& operator--() {
            if (ptr == stat) {
                set_node(node - 1);
                ptr = end;
            }
            --ptr;
            return *this;
        }
        const T& operator*() const {
            return *ptr;
        }
        const T* operator->() const noexcept {
            return ptr;
        }
        bool operator==(const iterator& rhs) const {
            return (ptr == rhs.ptr);
        }
        bool operator==(const const_iterator& rhs) const {
            return (ptr == rhs.ptr);
        }
        bool operator!=(const iterator& rhs) const {
            return (ptr != rhs.ptr);
        }
        bool operator!=(const const_iterator& rhs) const {
            return (ptr != rhs.ptr);
        }
    };
    /**
     * TODO Constructors
     */
    deque() {
    }
    deque(const deque& other) {
    }
    /**
     * TODO Deconstructor
     */
    ~deque() {
    }
    /**
     * TODO assignment operator
     */
    deque& operator=(const deque& other) {
    }
    /**
     * access specified element with bounds checking
     * throw index_out_of_bound if out of bound.
     */
    T& at(const size_t& pos) {
    }
    const T& at(const size_t& pos) const {
    }
    T& operator[](const size_t& pos) {
    }
    const T& operator[](const size_t& pos) const {
    }
    /**
     * access the first element
     * throw container_is_empty when the container is empty.
     */
    const T& front() const {
    }
    /**
     * access the last element
     * throw container_is_empty when the container is empty.
     */
    const T& back() const {
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
    }
    const_iterator cbegin() const {
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
    }
    const_iterator cend() const {
    }
    /**
     * checks whether the container is empty.
     */
    bool empty() const {
    }
    /**
     * returns the number of elements
     */
    size_t size() const {
    }
    /**
     * clears the contents
     */
    void clear() {
    }
    /**
     * inserts elements at the specified locat on in the container.
     * inserts value before pos
     * returns an iterator pointing to the inserted value
     *     throw if the iterator is invalid or it point to a wrong place.
     */
    iterator insert(iterator pos, const T& value) {
    }
    /**
     * removes specified element at pos.
     * removes the element at pos.
     * returns an iterator pointing to the following element, if pos pointing to
     * the last element, end() will be returned. throw if the container is
     * empty, the iterator is invalid or it points to a wrong place.
     */
    iterator erase(iterator pos) {
    }
    /**
     * adds an element to the end
     */
    void push_back(const T& value) {
    }
    /**
     * removes the last element
     *     throw when the container is empty.
     */
    void pop_back() {
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T& value) {
    }
    /**
     * removes the first element.
     *     throw when the container is empty.
     */
    void pop_front() {
    }
};

}  // namespace sjtu

#endif
