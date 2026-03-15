#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include <cstddef>

#include "exceptions.hpp"

namespace sjtu {

template <class T>
class deque {
   public:
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
            end = stat + chunk_size;
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
                node_move(node + 1);
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
                node_move(node - 1);
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
        T** node;
        const void* identity;
        void node_move(T** new_node) {
            node = new_node;
            stat = *new_node;
            end = stat + chunk_size;
        }

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
                node_move(node + 1);
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
                node_move(node - 1);
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

   private:
    T** map;
    size_t map_size;
    size_t first_block;
    size_t back_block;
    size_t first_offset;
    size_t back_offset;

   public:
    static const size_t chunk_size = 16;
    /**
     * TODO Constructors
     */
    deque() {
        map_size = 8;
        map = (T**)malloc(sizeof(T*) * map_size);
        for (size_t i = 0; i < map_size; i++) map[i] = nullptr;
        first_block = map_size / 2;
        back_block = first_block;
        first_offset = chunk_size / 2;
        back_offset = first_offset;
    }
    deque(const deque& other) {
        map_size = other.map_size;
        map = (T**)malloc(sizeof(T*) * map_size);
        if (other.map == nullptr) {
            map = nullptr;
            return;
        }
        for (size_t i = 0; i < map_size; i++) {
            T* new_block = (T*)malloc(sizeof(T) * chunk_size);
            size_t start = 0;
            size_t end = chunk_size - 1;
            if (i == other.first_block) start = other.first_offset;
            if (i == other.back_block) end = other.back_offset - 1;
            for (size_t j = start; j <= end; ++j) {
                new (new_block + j) T(other.map[i][j]);
            }
            map[i] = new_block;
        }
        first_block = other.first_block;
        back_block = other.back_block;
        first_offset = other.first_offset;
        back_offset = other.back_offset;
    }
    /**
     * TODO Deconstructor
     */
    ~deque() {
        for (size_t block = first_block; block <= back_block; ++block) {
            T* block_ptr = map[block];
            size_t start = 0;
            size_t end = chunk_size - 1;
            if (block == first_block) start = first_offset;
            if (block == back_block) end = back_offset - 1;
            for (size_t i = start; i <= end; ++i) {
                block_ptr[i].~T();
            }
            free(block_ptr);
        }
        free(map);
    }
    /**
     * TODO assignment operator
     */
    deque& operator=(const deque& other) {
        if (this == &other) {
            return *this;
        }
        for (size_t block = first_block; block <= back_block; ++block) {
            T* block_ptr = map[block];
            size_t start = 0;
            size_t end = chunk_size - 1;
            if (block == first_block) start = first_offset;
            if (block == back_block) end = back_offset - 1;
            for (size_t i = start; i <= end; ++i) {
                block_ptr[i].~T();
            }
            free(block_ptr);
        }
        free(map);
        map_size = other.map_size;
        map = (T**)malloc(sizeof(T*) * map_size);
        if (other.map == nullptr) {
            map = nullptr;
            return;
        }
        for (size_t i = 0; i < map_size; i++) {
            T* new_block = (T*)malloc(sizeof(T) * chunk_size);
            size_t start = 0;
            size_t end = chunk_size - 1;
            if (i == other.first_block) start = other.first_offset;
            if (i == other.back_block) end = other.back_offset - 1;
            for (size_t j = start; j <= end; ++j) {
                new (new_block + j) T(other.map[i][j]);
            }
            map[i] = new_block;
        }
        first_block = other.first_block;
        back_block = other.back_block;
        first_offset = other.first_offset;
        back_offset = other.back_offset;
    }
    /**
     * access specified element with bounds checking
     * throw index_out_of_bound if out of bound.
     */
    T& at(const size_t& pos) {
        if (pos < 0 || pos >= this->size()) {
            throw index_out_of_bound();
        }
        // first block
        size_t first_part = chunk_size - first_offset;
        if (pos < first_part) {
            return map[first_block][first_offset + pos];
        }
        // middle block
        size_t pos2 = pos - first_part;
        size_t middle_blocks = back_block - first_block - 1;
        if (pos2 < middle_blocks * chunk_size) {
            size_t block_idx = first_block + 1 + pos2 / chunk_size;
            size_t offset = pos2 % chunk_size;
            return map[block_idx][offset];
        }
        // back block
        pos2 -= middle_blocks * chunk_size;
        return map[back_block][pos2];
    }
    const T& at(const size_t& pos) const {
        if (pos < 0 || pos >= this->size()) {
            throw index_out_of_bound();
        }
        // first block
        size_t first_part = chunk_size - first_offset;
        if (pos < first_part) {
            return map[first_block][first_offset + pos];
        }
        // middle block
        size_t pos2 = pos - first_part;
        size_t middle_blocks = back_block - first_block - 1;
        if (pos2 < middle_blocks * chunk_size) {
            size_t block_idx = first_block + 1 + pos2 / chunk_size;
            size_t offset = pos2 % chunk_size;
            return map[block_idx][offset];
        }
        // back block
        pos2 -= middle_blocks * chunk_size;
        return map[back_block][pos2];
    }
    T& operator[](const size_t& pos) {
        if (pos < 0 || pos >= this->size()) {
            throw index_out_of_bound();
        }
        // first block
        size_t first_part = chunk_size - first_offset;
        if (pos < first_part) {
            return map[first_block][first_offset + pos];
        }
        // middle block
        size_t pos2 = pos - first_part;
        size_t middle_blocks = back_block - first_block - 1;
        if (pos2 < middle_blocks * chunk_size) {
            size_t block_idx = first_block + 1 + pos2 / chunk_size;
            size_t offset = pos2 % chunk_size;
            return map[block_idx][offset];
        }
        // back block
        pos2 -= middle_blocks * chunk_size;
        return map[back_block][pos2];
    }
    const T& operator[](const size_t& pos) const {
        if (pos < 0 || pos >= this->size()) {
            throw index_out_of_bound();
        }
        // first block
        size_t first_part = chunk_size - first_offset;
        if (pos < first_part) {
            return map[first_block][first_offset + pos];
        }
        // middle block
        size_t pos2 = pos - first_part;
        size_t middle_blocks = back_block - first_block - 1;
        if (pos2 < middle_blocks * chunk_size) {
            size_t block_idx = first_block + 1 + pos2 / chunk_size;
            size_t offset = pos2 % chunk_size;
            return map[block_idx][offset];
        }
        // back block
        pos2 -= middle_blocks * chunk_size;
        return map[back_block][pos2];
    }
    /**
     * access the first element
     * throw container_is_empty when the container is empty.
     */
    const T& front() const {
        if (empty()) {
            throw container_is_empty();
        }
        return map[first_block][first_offset];
    }
    /**
     * access the last element
     * throw container_is_empty when the container is empty.
     */
    const T& back() const {
        if (empty()) {
            throw container_is_empty();
        }
        return map[back_block][back_offset];
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(&map[first_block][first_offset],
                        &map[first_block][first_offset],
                        &map[back_block][back_offset], map[first_block], map);
    }
    const_iterator cbegin() const {
        return const_iterator(
            &map[first_block][first_offset], &map[first_block][first_offset],
            &map[back_block][back_offset], map[first_block], map);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(&map[back_block][back_offset],
                        &map[first_block][first_offset],
                        &map[back_block][back_offset], map[back_block], map);
    }
    const_iterator cend() const {
        return const_iterator(
            &map[back_block][back_offset], &map[first_block][first_offset],
            &map[back_block][back_offset], map[back_block], map);
    }
    /**
     * checks whether the container is empty.
     */
    bool empty() const {
        return (first_offset == back_offset && first_block == back_block);
    }
    /**
     * returns the number of elements
     */
    size_t size() const {
        if (first_block == back_block) {
            return back_offset - first_offset;
        } else {
            return (chunk_size - first_offset) + (back_offset) +
                   (back_block - first_block - 1) * chunk_size;
        }
    }
    /**
     * clears the contents
     */
    void clear() {
        for (size_t block = first_block; block <= back_block; ++block) {
            T* block_ptr = map[block];
            size_t start = 0;
            size_t end = chunk_size - 1;
            if (block == first_block) start = first_offset;
            if (block == back_block) end = back_offset - 1;
            for (size_t i = start; i <= end; ++i) {
                block_ptr[i].~T();
            }
            free(block_ptr);
        }
        free(map);
        map_size = 8;
        map = (T**)malloc(sizeof(T*) * map_size);
        for (size_t i = 0; i < map_size; i++) map[i] = nullptr;
        first_block = map_size / 2;
        back_block = first_block;
        first_offset = chunk_size / 2;
        back_offset = first_offset;
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
