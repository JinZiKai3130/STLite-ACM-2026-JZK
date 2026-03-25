#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cmath>    // maybe you need it
#include <cstddef>  // for size_t
#include <cstdlib>
#include <functional>  // for std::less

#include "exceptions.hpp"

namespace sjtu {

template <class T, class Compare = std::less<T>>
class priority_queue {
   private:
    T* v;
    int* lc;
    int* rc;
    int* dis;
    int* popped;
    size_t len;
    size_t max_len;
    size_t popped_ptr;
    int root;
    const int init_chunk = 8;
    Compare comp;

    void double_space() {
        const auto tmp = *this;
        for (int i = 0; i < len; i++) {
            v[i].~T();
        }
        free(v);
        free(popped);
        free(lc);
        free(rc);
        free(dis);
        max_len *= 2;
        v = (T*)malloc(sizeof(T) * max_len);
        lc = (int*)malloc(sizeof(int) * max_len);
        rc = (int*)malloc(sizeof(int) * max_len);
        dis = (int*)malloc(sizeof(int) * max_len);
        popped = (int*)malloc(sizeof(int) * max_len);
        for (size_t i = 0; i < len; i++) {
            new (&v[i]) T(tmp.v[i]);
            new (&lc[i]) int(tmp.lc[i]);
            new (&rc[i]) int(tmp.rc[i]);
            new (&dis[i]) int(tmp.dis[i]);
            new (&popped[i]) int(tmp.popped[i]);
        }
    }

    int merge_node(int x, int y) {
        if (x == -1) return y;
        if (y == -1) return x;
        if (comp(v[x], v[y])) std::swap(x, y);  // v[x]小于v[y]
        rc[x] = merge_node(rc[x], y);
        int dis_l = (lc[x] == -1 ? -1 : dis[lc[x]]);
        int dis_r = (rc[x] == -1 ? -1 : dis[rc[x]]);
        if (dis_l < dis_r) {
            std::swap(lc[x], rc[x]);
        }
        dis[x] = (rc[x] == -1 ? 0 : dis[rc[x]] + 1);
        return x;
    }

   public:
    priority_queue() {
        v = (T*)malloc(sizeof(T) * init_chunk);
        lc = (int*)malloc(sizeof(int) * init_chunk);
        rc = (int*)malloc(sizeof(int) * init_chunk);
        dis = (int*)malloc(sizeof(int) * init_chunk);
        popped = (int*)malloc(sizeof(int) * init_chunk);
        len = 0;
        root = -1;
        popped_ptr = 0;
        max_len = init_chunk;
    }
    priority_queue(const priority_queue& other) {
        len = other.len;
        max_len = other.max_len;
        root = other.root;
        v = (T*)malloc(sizeof(T) * max_len);
        lc = (int*)malloc(sizeof(int) * max_len);
        rc = (int*)malloc(sizeof(int) * max_len);
        dis = (int*)malloc(sizeof(int) * max_len);
        popped = (int*)malloc(sizeof(int) * max_len);
        popped_ptr = other.popped_ptr;
        for (size_t i = 0; i < len; i++) {
            new (&v[i]) T(other.v[i]);
            new (&lc[i]) int(other.lc[i]);
            new (&rc[i]) int(other.rc[i]);
            new (&dis[i]) int(other.dis[i]);
            new (&popped[i]) int(other.popped[i]);
        }
    }
    ~priority_queue() {
        for (size_t i = 0; i < len; i++) {
            v[i].~T();
        }
        free(v);
        free(lc);
        free(rc);
        free(dis);
        free(popped);
    }

    priority_queue& operator=(const priority_queue& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        free(v);
        free(lc);
        free(rc);
        free(dis);
        free(popped);
        len = other.len;
        max_len = other.max_len;
        root = other.root;
        v = (T*)malloc(sizeof(T) * max_len);
        lc = (int*)malloc(sizeof(int) * max_len);
        rc = (int*)malloc(sizeof(int) * max_len);
        dis = (int*)malloc(sizeof(int) * max_len);
        popped = (int*)malloc(sizeof(int) * max_len);
        popped_ptr = other.popped_ptr;
        for (size_t i = 0; i < len; i++) {
            new (&v[i]) T(other.v[i]);
            new (&lc[i]) int(other.lc[i]);
            new (&rc[i]) int(other.rc[i]);
            new (&dis[i]) int(other.dis[i]);
            new (&popped[i]) int(other.popped[i]);
        }
        return *this;
    }

    /**
     * @brief Push one element into the queue.
     * @note Its time complexity shall be O(log n).
     */
    void push(const T& cur_element) {
        int idx;
        if (popped_ptr != 0) {
            idx = popped[popped_ptr];
            // popped[popped_ptr].~T();
            popped_ptr--;
        } else {
            idx = len;
            if (len == max_len) double_space();
        }
        new (&v[idx]) T(cur_element);
        lc[idx] = -1;
        rc[idx] = -1;
        dis[idx] = 0;
        root = merge_node(root, idx);
        ++len;
    }

    /**
     * @return A const reference of the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(1).
     */
    const T& top() const {
        if (empty()) {
            throw container_is_empty();
        }
        return v[root];
    }

    /**
     * @brief remove the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(n).
     */
    void pop() {
        if (empty()) {
            throw container_is_empty();
        }
        popped_ptr++;
        new (&popped[popped_ptr]) int(root);
        int old_root = root;
        root = merge_node(lc[old_root], rc[old_root]);
        v[old_root].~T();
        --len;
        if (len == 0) root = -1;
    }

    /**
     * @return number of elements in the queue.
     */
    size_t size() const {
        return len;
    }

    /**
     * @return whether there is any element in the queue.
     */
    bool empty() const {
        return (len == 0);
    }

    /**
     * @brief Clear all elements in the queue.
     * @note Its time complexity shall be O(n).
     */
    void clear() {
        for (size_t i = 0; i < len; i++) {
            v[i].~T();
        }
        free(v);
        free(lc);
        free(rc);
        free(dis);
        free(popped);
        v = (T*)malloc(sizeof(T) * init_chunk);
        lc = (int*)malloc(sizeof(int) * init_chunk);
        rc = (int*)malloc(sizeof(int) * init_chunk);
        dis = (int*)malloc(sizeof(int) * init_chunk);
        popped = (int*)malloc(sizeof(int) * init_chunk);
        len = 0;
        popped_ptr = 0;
        root = -1;
        max_len = init_chunk;
    }

    /**
     * @brief Merge element sets of two queues.
     * @note Its time complexity shall be O(log n).
     */
    void merge(priority_queue& other) {
        if (&other == this || other.empty()) return;

        len += other.len;
        other.root = -1;
        other.len = 0;
    }
};

}  // namespace sjtu

#endif