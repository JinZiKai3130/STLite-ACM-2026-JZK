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
        T* stat;
        T* end;
        T** node;
        const deque* identity;
        void node_move(T** new_node) {
            node = new_node;
            stat = *new_node;
            const auto* deq = identity;
            if (new_node == deq->map + deq->back_block) {
                end = stat + deq->back_offset;
            } else {
                end = stat + chunk_size;
            }
        }

       public:
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is
         * **undefined**. as well as operator-
         */
        iterator() = default;
        iterator(T* ptr, T* stat, T* end, T** node, const deque* identity)
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
        // return the distance between two iterator,
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
            if (n == 0) return *this;
            if (n < 0) return (*this -= -n);

            const auto* deq = identity;
            long long global =
                (node - deq->map) * 1ll * chunk_size + (ptr - stat) + n;

            node_move(deq->map + (global / (long long)chunk_size));
            ptr = stat + (global % (long long)chunk_size);
            return *this;
        }

        iterator operator-=(const int& n) {
            if (n == 0) return *this;
            if (n < 0) return (*this += -n);

            const auto* deq = identity;
            long long global =
                (node - deq->map) * 1ll * chunk_size + (ptr - stat) - n;

            node_move(deq->map + (global / (long long)chunk_size));
            ptr = stat + (global % (long long)chunk_size);
            return *this;
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
            const auto* deq = identity;
            if (node == deq->map + deq->back_block && ptr == end) {
                throw invalid_iterator();
            }
            ++ptr;
            if (ptr == end) {
                if (node != deq->map + deq->back_block) {
                    node_move(node + 1);
                    ptr = stat;
                }
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
            const auto* deq = identity;
            if (node == deq->map + deq->first_block &&
                ptr == stat + deq->first_offset) {
                throw invalid_iterator();
            }

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
            const auto* deq = identity;
            if (node == deq->map + deq->back_block && ptr == end) {
                throw invalid_iterator();
            }
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
        const T* stat;
        const T* end;
        T** node;
        const deque* identity;
        void node_move(T** new_node) {
            node = new_node;
            stat = *new_node;
            const auto* deq = identity;
            if (new_node == deq->map + deq->back_block)
                end = stat + deq->back_offset;
            else
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
        const_iterator(T* ptr, T* stat, T* end, T** node, const deque* identity)
            : ptr(ptr), stat(stat), end(end), node(node), identity(identity) {
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
            if (n == 0) return *this;
            if (n < 0) return (*this -= -n);

            const auto* deq = identity;
            long long global =
                (node - deq->map) * 1ll * chunk_size + (ptr - stat) + n;

            node_move(deq->map + (global / (long long)chunk_size));
            ptr = stat + (global % (long long)chunk_size);
            return *this;
        }

        const_iterator operator-=(const int& n) {
            if (n == 0) return *this;
            if (n < 0) return (*this += -n);

            const auto* deq = identity;
            long long global =
                (node - deq->map) * 1ll * chunk_size + (ptr - stat) - n;

            node_move(deq->map + (global / (long long)chunk_size));
            ptr = stat + (global % (long long)chunk_size);
            return *this;
        }
        const_iterator operator++(int) {
            auto result = *this;
            ++*this;
            return result;
        }
        const_iterator& operator++() {
            const auto* deq = identity;
            if (node == deq->map + deq->back_block && ptr == end) {
                throw invalid_iterator();
            }
            ++ptr;
            if (ptr == end) {
                if (node != deq->map + deq->back_block) {
                    node_move(node + 1);
                    ptr = stat;
                }
            }
            return *this;
        }
        const_iterator operator--(int) {
            auto result = *this;
            --*this;
            return result;
        }
        const_iterator& operator--() {
            const auto* deq = identity;
            if (node == deq->map + deq->first_block &&
                ptr == stat + deq->first_offset) {
                throw invalid_iterator();
            }

            if (ptr == stat) {
                node_move(node - 1);
                ptr = end;
            }
            --ptr;
            return *this;
        }
        const T& operator*() const {
            const auto* deq = identity;
            if (node == deq->map + deq->back_block && ptr == end) {
                throw invalid_iterator();
            }
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

    void expand() {
        size_t new_map_size = map_size * 2;
        T** new_map = (T**)malloc(sizeof(T*) * new_map_size);
        for (size_t i = 0; i < new_map_size; ++i) {
            new_map[i] = nullptr;
        }
        size_t used_blocks = back_block - first_block + 1;
        size_t new_first_block = (new_map_size - used_blocks) / 2;

        for (size_t i = 0; i < used_blocks; ++i) {
            new_map[new_first_block + i] = map[first_block + i];
        }
        back_block = new_first_block + (back_block - first_block);
        first_block = new_first_block;
        free(map);
        map = new_map;
        map_size = new_map_size;
    }

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
        map[first_block] = (T*)malloc(sizeof(T) * chunk_size);
    }
    deque(const deque& other) {
        map_size = other.map_size;
        map = (T**)malloc(sizeof(T*) * map_size);
        for (size_t i = 0; i < map_size; ++i) map[i] = nullptr;
        first_block = other.first_block;
        back_block = other.back_block;
        first_offset = other.first_offset;
        back_offset = other.back_offset;
        for (size_t b = other.first_block; b <= other.back_block; ++b) {
            T* new_block = (T*)malloc(sizeof(T) * chunk_size);
            map[b] = new_block;
            size_t start = 0;
            size_t end_exclusive = chunk_size;
            if (b == other.first_block) start = other.first_offset;
            if (b == other.back_block) end_exclusive = other.back_offset;
            for (size_t j = start; j < end_exclusive; ++j) {
                new (new_block + j) T(other.map[b][j]);
            }
        }
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
        if (map != nullptr) {
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
        map_size = other.map_size;
        map = (T**)malloc(sizeof(T*) * map_size);
        for (size_t i = 0; i < map_size; ++i) map[i] = nullptr;
        first_block = other.first_block;
        back_block = other.back_block;
        first_offset = other.first_offset;
        back_offset = other.back_offset;
        for (size_t b = other.first_block; b <= other.back_block; ++b) {
            T* new_block = (T*)malloc(sizeof(T) * chunk_size);
            map[b] = new_block;
            size_t start = 0;
            size_t end_exclusive = chunk_size;
            if (b == other.first_block) start = other.first_offset;
            if (b == other.back_block) end_exclusive = other.back_offset;
            for (size_t j = start; j < end_exclusive; ++j) {
                new (new_block + j) T(other.map[b][j]);
            }
        }
        return *this;
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
        return map[back_block][back_offset - 1];
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        T* stat = map[first_block];
        T* end = (first_block == back_block) ? (stat + back_offset)
                                             : (stat + chunk_size);
        return iterator(stat + first_offset, stat, end, &map[first_block],
                        this);
    }
    const_iterator cbegin() const {
        T* stat = map[first_block];
        T* end = (first_block == back_block) ? (stat + back_offset)
                                             : (stat + chunk_size);
        return const_iterator(stat + first_offset, stat, end, &map[first_block],
                              this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        T* s = map[back_block];
        T* e = s + back_offset;
        return iterator(e, s, e, &map[back_block], this);
    }
    const_iterator cend() const {
        T* s = map[back_block];
        T* e = s + back_offset;
        return const_iterator(e, s, e, &map[back_block], this);
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
        map[first_block] = (T*)malloc(sizeof(T) * chunk_size);
    }
    /**
     * inserts elements at the specified locat on in the container.
     * inserts value before pos
     * returns an iterator pointing to the inserted value
     *     throw if the iterator is invalid or it point to a wrong place.
     */
    iterator insert(iterator pos, const T& value) {
        int idx;
        try {
            idx = pos - begin();
        } catch (...) {
            throw invalid_iterator();
        }
        size_t n = size();
        if (idx < 0 || idx > n) {
            throw index_out_of_bound();
        }
        if (idx == 0) {
            push_front(value);
            return begin();
        }
        if (idx == n) {
            push_back(value);
            return end() - 1;
        }
        size_t p = idx;
        if (p < n - p) {
            push_front(front());
            for (size_t i = 0; i < p; ++i) {
                (*this)[i] = (*this)[i + 1];
            }
            (*this)[p] = value;
        } else {
            push_back(back());
            for (size_t i = n; i > p; --i) {
                (*this)[i] = (*this)[i - 1];
            }
            (*this)[p] = value;
        }
        return begin() + idx;
    }
    /**
     * removes specified element at pos.
     * removes the element at pos.
     * returns an iterator pointing to the following element, if pos pointing to
     * the last element, end() will be returned. throw if the container is
     * empty, the iterator is invalid or it points to a wrong place.
     */
    iterator erase(iterator pos) {
        int idx;
        try {
            idx = pos - begin();
        } catch (...) {
            throw invalid_iterator();
        }
        size_t n = size();
        if (idx < 0 || idx >= n) {
            throw index_out_of_bound();
        }
        size_t p = idx;
        if (p < n - 1 - p) {  // 从头开始
            for (size_t i = p; i > 0; --i) {
                (*this)[i] = (*this)[i - 1];
            }
            pop_front();
        } else {  // 从尾开始
            for (size_t i = p; i + 1 < n; ++i) {
                (*this)[i] = (*this)[i + 1];
            }
            pop_back();
        }
        return begin() + idx;
    }
    /**
     * adds an element to the end
     */
    void push_back(const T& value) {
        if (back_offset == chunk_size) {
            if (back_block + 1 >= map_size) {
                expand();
            }
            T* new_block = (T*)malloc(sizeof(T) * chunk_size);
            map[back_block + 1] = new_block;
            new (&new_block[0]) T(value);
            back_block++;
            back_offset = 1;
        } else {
            new (&map[back_block][back_offset]) T(value);
            back_offset++;
        }
    }
    /**
     * removes the last element
     *     throw when the container is empty.
     */
    void pop_back() {
        if (first_block == back_block && first_offset == back_offset) {
            throw container_is_empty();
        }
        map[back_block][back_offset - 1].~T();
        back_offset--;
        if (back_offset == 0) {
            if (first_block == back_block) {  // 完全是空的
                first_offset = back_offset = chunk_size / 2;
            } else {
                free(map[back_block]);
                map[back_block] = nullptr;
                back_block--;
                back_offset = chunk_size;
            }
        }
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T& value) {
        if (first_offset == 0) {
            if (first_block == 0) {
                expand();
            }
            T* new_block = (T*)malloc(sizeof(T) * chunk_size);
            map[first_block - 1] = new_block;
            new (&new_block[chunk_size - 1]) T(value);
            first_block--;
            first_offset = chunk_size - 1;
        } else {
            new (&map[first_block][first_offset - 1]) T(value);
            first_offset--;
        }
    }
    /**
     * removes the first element.
     *     throw when the container is empty.
     */
    void pop_front() {
        if (first_block == back_block && first_offset == back_offset) {
            throw container_is_empty();
        }
        map[first_block][first_offset].~T();
        first_offset++;
        if (first_offset == chunk_size) {
            if (first_block == back_block) {  // 完全是空的
                first_offset = back_offset = chunk_size / 2;
            } else {
                free(map[first_block]);
                map[first_block] = nullptr;
                first_block++;
                first_offset = 0;
            }
        }
    }
};

}  // namespace sjtu

#endif
