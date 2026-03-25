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
    size_t len;
    size_t max_len;
    int root;
    const int init_chunk = 8;
    Compare comp;

    void double_space() {
        const auto tmp = *this;
        for (int i = 0; i < len; i++) {
            v[i].~T();
        }
        free(v);
        free(lc);
        free(rc);
        free(dis);
        max_len *= 2;
        v = (T*)malloc(sizeof(T) * max_len);
        lc = (int*)malloc(sizeof(int) * max_len);
        rc = (int*)malloc(sizeof(int) * max_len);
        dis = (int*)malloc(sizeof(int) * max_len);
        for (size_t i = 0; i < len; i++) {
            new (&v[i]) T(tmp.v[i]);
            new (&lc[i]) int(tmp.lc[i]);
            new (&rc[i]) int(tmp.rc[i]);
            new (&dis[i]) int(tmp.dis[i]);
        }
    }

   public:
    priority_queue() {
        v = (T*)malloc(sizeof(T) * init_chunk);
        lc = (int*)malloc(sizeof(int) * init_chunk);
        rc = (int*)malloc(sizeof(int) * init_chunk);
        dis = (int*)malloc(sizeof(int) * init_chunk);
        len = 0;
        root = -1;
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

        for (size_t i = 0; i < len; i++) {
            new (&v[i]) T(other.v[i]);
            new (&lc[i]) int(other.lc[i]);
            new (&rc[i]) int(other.rc[i]);
            new (&dis[i]) int(other.dis[i]);
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
    }

    priority_queue& operator=(const priority_queue& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        len = other.len;
        max_len = other.max_len;
        root = other.root;
        v = (T*)malloc(sizeof(T) * max_len);
        lc = (int*)malloc(sizeof(int) * max_len);
        rc = (int*)malloc(sizeof(int) * max_len);
        dis = (int*)malloc(sizeof(int) * max_len);

        for (size_t i = 0; i < len; i++) {
            new (&v[i]) T(other.v[i]);
            new (&lc[i]) int(other.lc[i]);
            new (&rc[i]) int(other.rc[i]);
            new (&dis[i]) int(other.dis[i]);
        }
        return *this;
    }

    /**
     * @brief Push one element into the queue.
     * @note Its time complexity shall be O(log n).
     */
    void push(const T& cur_element) {
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
        len = 0;
        root = -1;
        max_len = init_chunk;
    }

    /**
     * @brief Merge element sets of two queues.
     * @note Its time complexity shall be O(log n).
     */
    void merge(priority_queue& other) {
    }
};

}  // namespace sjtu

#endif