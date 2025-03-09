#ifndef SJTU_LRU_HPP
#define SJTU_LRU_HPP

#include "class-integer.hpp"
#include "class-matrix.hpp"
#include "exceptions.hpp"
#include "utility.hpp"

class Hash {
   public:
    unsigned int operator()(Integer lhs) const {
        int val = lhs.val;
        return std::hash<int>()(val);
    }
};
class Equal {
   public:
    bool operator()(const Integer &lhs, const Integer &rhs) const {
        return lhs.val == rhs.val;
    }
};

namespace sjtu {

template <class T>
class Node {
   public:
    T data;
    Node *prev;
    Node *next;
    Node(const T &val) : data(val), prev(nullptr), next(nullptr) {}
};

template <class T>
class double_list {
   private:
    Node<T> *head;
    Node<T> *tail;

   public:
    double_list() {
        head = nullptr;
        tail = nullptr;
    }
    double_list(const double_list<T> &other) : head(nullptr), tail(nullptr) {
        Node<T> *tmp = other.head;
        while (tmp) {
            insert_tail(tmp->data);
            tmp = tmp->next;
        }
    }

    ~double_list() { clear(); }

    class iterator {
       public:
        Node<T> *cur;
        iterator() { cur = nullptr; }
        iterator(const iterator &other) { cur = other.cur; }
        iterator(Node<T> *ptr) : cur(ptr){};
        ~iterator() {}
        // iter++
        iterator operator++(int) {
            iterator tmp = *this;
            if (cur == nullptr) {
                throw "invalid";
            }
            cur = cur->next;
            return tmp;
        }

        // ++iter
        iterator &operator++() {
            if (cur == nullptr) {
                throw "invalid";
            }
            cur = cur->next;
            return *this;
        }

        // iter--
        iterator operator--(int) {
            iterator tmp = *this;
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return tmp;
        }

        //--iter
        iterator &operator--() {
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return *this;
        }
        /**
         * if the iter didn't point to a value
         * throw " invalid"
         */
        T &operator*() const {
            if (cur) {
                return cur->data;
            } else {
                throw "invalid";
            }
        }

        // other operation
        T *operator->() const noexcept { return &cur->data; }
        bool operator==(const iterator &rhs) const {
            if (cur == rhs.cur) {
                return true;
            } else {
                return false;
            }
        }
        bool operator!=(const iterator &rhs) const {
            if (cur != rhs.cur) {
                return true;
            } else {
                return false;
            }
        }
    };
    class const_iterator {
       public:
        const Node<T> *cur;
        const_iterator() : cur(nullptr) {}
        const_iterator(const Node<T> *ptr) : cur(ptr) {}
        const_iterator(const const_iterator &other) : cur(other.cur) {}
        // iter++
        // iter--
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return tmp;
        }

        //--iter
        const_iterator &operator--() {
            if (cur->prev == nullptr) {
                throw "invalid";
            }
            cur = cur->prev;
            return *this;
        }
        const_iterator operator++(int) {
            if (cur == nullptr) {
                throw "invalid";
            }
            const_iterator tmp(*this);
            cur = cur->next;
            return tmp;
        }
        const_iterator &operator++() {
            if (cur == nullptr) {
                throw "invalid";
            }
            cur = cur->next;
            return *this;
        }
        const T &operator*() const {
            if (!cur) throw "invalid";
            return cur->data;
        }
        const T *operator->() const noexcept { return &cur->data; }

        bool operator==(const const_iterator &rhs) const {
            return cur == rhs.cur;
        }
        bool operator!=(const const_iterator &rhs) const {
            return cur != rhs.cur;
        }
    };

    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }

    // return an iterator to the beginning
    iterator begin() { return iterator(head); }
    /**
     * return an iterator to the ending
     * in fact, it returns the iterator point to nothing,
     * just after the last element.
     */
    iterator end() { return iterator(nullptr); }
    /**
     * if the iter didn't point to anything, do nothing,
     * otherwise, delete the element pointed by the iter
     * and return the iterator point sat the same "index"
     * e.g.
     * 	if the origin iterator point at the 2nd element
     * 	the returned iterator also point at the
     *  2nd element of the list after the operation
     *  or nothing if the list after the operation
     *  don't contain 2nd elememt.
     */
    iterator erase(iterator pos) {
        if (pos.cur == nullptr) {
            return pos;
        }
        Node<T> *tmp = pos.cur;
        if (tmp->prev) {
            tmp->prev->next = tmp->next;
        }
        if (tmp->next) {
            tmp->next->prev = tmp->prev;
        }
        if (tmp == head) {
            head = tmp->next;
        }
        if (tmp == tail) {
            tail = tmp->prev;
        }
        iterator newIter(tmp->next);
        delete tmp;
        return newIter;
    }
    iterator get_tail() const{
		return iterator(tail);
	}
    /**
     * the following are operations of double list
     */
    void insert_head(const T &val) {
        Node<T> *newNode = new Node<T>(val);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    void insert_tail(const T &val) {
        Node<T> *newNode = new Node<T>(val);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    void delete_head() {
        if (!head) return;
        Node<T> *tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        delete tmp;
    }
    void delete_tail() {
        if (!tail) return;
        Node<T> *tmp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        delete tmp;
    }
    /**
     * if didn't contain anything, return true,
     * otherwise false.
     */
    bool empty() {
        if (head == nullptr) {
            return true;
        } else {
            return false;
        }
    }
    bool empty() const{
        if (head == nullptr) {
            return true;
        } else {
            return false;
        }
    }
    void clear() {
        Node<T> *tmp = head;
        while (tmp) {
            Node<T> *next = tmp->next;
            delete tmp;
            tmp = next;
        }
        head = tail = nullptr;
    }
};

template <class Key, class T, class Hash = std::hash<Key>,
          class Equal = std::equal_to<Key> >
class hashmap {
   public:
    using value_type = pair<const Key, T>;

    struct Node {
        value_type data;
        Node *next;
        typename double_list<value_type>::iterator list_iter;
        Node(const value_type &other) : data(other), next(nullptr) {}
    };

    std::vector<Node *> buckets;
    Hash hash;
    Equal equal;
    size_t num_elem;
    static const size_t init_cnt = 16;
    /**
     * elements
     * add whatever you want
     */

    /**
     * the follows are constructors and destructors
     * you can also add some if needed.
     */
    hashmap() : buckets(init_cnt, nullptr), num_elem(0) {}
    hashmap(const hashmap &other)
        : buckets(other.buckets.size(), nullptr),
          num_elem(other.num_elem),
          hash(other.hash),
          equal(other.equal) {
        for (int i = 0; i < other.buckets.size(); ++i) {
            if (other.buckets[i]) {
                Node *src = other.buckets[i];
                Node *new_head = new Node(src->data);
                buckets[i] = new_head;
                src = src->next;
                Node *tail = new_head;
                while (src) {
                    tail->next = new Node(src->data);
                    tail = tail->next;
                    src = src->next;
                }
            }
        }
    }
    ~hashmap() {
        int s = buckets.size();
        for (int i = 0; i < s; ++i) {
            if (buckets[i]) {
                Node *src = buckets[i];
                while (src) {
                    Node *tmp = src->next;
                    delete src;
                    src = tmp;
                }
            }
        }
    }
    hashmap &operator=(const hashmap &other) {
        if (this != &other) {
            clear();
        } else {
            return *this;
        }
        buckets.resize(other.buckets.size(), nullptr);
        num_elem = other.num_elem;
        hash = other.hash;
        equal = other.equal;
        for (int i = 0; i < other.buckets.size(); ++i) {
            if (other.buckets[i]) {
                Node *src = other.buckets[i];
                Node *new_head = new Node(src->data);
                buckets[i] = new_head;
                src = src->next;
                Node *tail = new_head;
                while (src) {
                    tail->next = new Node(src->data);
                    tail = tail->next;
                    src = src->next;
                }
            }
        }
        return *this;
    }

    class iterator {
       private:
        const hashmap *map;
        int bucket_index;
        Node *cur;

       public:
        /**
         * elements
         * add whatever you want
         */

        // --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
         */
        iterator() : map(nullptr), bucket_index(0), cur(nullptr) {}
        iterator(const iterator &t)
            : map(t.map), bucket_index(t.bucket_index), cur(t.cur) {}
        iterator(const hashmap *m, int idx, Node *node)
            : map(m), bucket_index(idx), cur(node) {
            if (!cur && map) {
                int s = map->buckets.size();
                while (bucket_index < s &&
                       map->buckets[bucket_index] == nullptr) {
                    ++bucket_index;
                }
                if (bucket_index < s)
                    cur = map->buckets[bucket_index];
                else
                    cur = nullptr;
            }
        }
        ~iterator() {}

        /**
         * if point to nothing
         * throw
         */
        value_type &operator*() const {
            if (!cur) {
                throw "invalid";
            } else
                return cur->data;
        }
        /**
         * other operation
         */
        value_type *operator->() const noexcept {
            if (!cur) {
                throw "invalid";
            } else
                return &cur->data;
        }
        bool operator==(const iterator &rhs) const { return cur == rhs.cur; }
        bool operator!=(const iterator &rhs) const { return cur != rhs.cur; }
    };

    void clear() {
        int s = buckets.size();
        for (int i = 0; i < s; ++i) {
            if (buckets[i]) {
                Node *src = buckets[i];
                while (src) {
                    Node *tmp = src->next;
                    delete src;
                    src = tmp;
                }
                buckets[i] = nullptr;
            }
        }
        num_elem = 0;
    }
    /**
     * you need to expand the hashmap dynamically
     */
    void expand() {
        std::vector<Node *> new_buckets(buckets.size() * 2, nullptr);
        int s = buckets.size();
        for (int i = 0; i < s; ++i) {
            Node *cur = buckets[i];
            while (cur) {
                Node *tmp = cur->next;
                int new_idx = hash(cur->data.first) % (2 * s);
                cur->next = new_buckets[new_idx];
                new_buckets[new_idx] = cur;
                cur = tmp;
            }
        }
        buckets = new_buckets;
    }

    /**
     * the iterator point at nothing
     */
    iterator end() const { return iterator(this, buckets.size(), nullptr); }
    /**
     * find, return a pointer point to the value
     * not find, return the end (point to nothing)
     */
    iterator find(const Key &key) const {
        int idx = hash(key) % buckets.size();
        Node *src = buckets[idx];
        while (src) {
            if (equal(src->data.first, key)) return iterator(this, idx, src);
            src = src->next;
        }
        return iterator(this, buckets.size(), nullptr);
    }
    /**
     * already have a value_pair with the same key
     * -> just update the value, return false
     * not find a value_pair with the same key
     * -> insert the value_pair, return true
     */
    sjtu::pair<iterator, bool> insert(const value_type &value_pair) {
        Key key = value_pair.first;
        int idx = hash(key) % buckets.size();
        Node *cur = buckets[idx];
        bool flag = true;
        while (cur) {
            if (equal(key, cur->data.first)) {
                flag = false;
                cur->data.second = value_pair.second;
                auto it = iterator(this, idx, cur);
                return sjtu::pair<iterator, bool>(it, flag);
            } else {
                cur = cur->next;
            }
        }
        Node *newnode = new Node(value_pair);
        newnode->next = buckets[idx];
        buckets[idx] = newnode;
        num_elem++;
        if (num_elem > buckets.size() * 0.75) {
            expand();
        }
        auto it = iterator(this, idx, newnode);
        return sjtu::pair<iterator, bool>(it, flag);
    }
    /**
     * the value_pair exists, remove and return true
     * otherwise, return false
     */
    bool remove(const Key &key) {
        int idx = hash(key) % buckets.size();
        Node *src = buckets[idx];
        if (!src) return false;
        if (equal(src->data.first, key)) {
            buckets[idx] = buckets[idx]->next;
            delete src;
            num_elem--;
            return true;
        }

        Node *cur = src->next;
        while (cur) {
            if (equal(cur->data.first, key)) {
                src->next = cur->next;
                delete cur;
                num_elem--;
                return true;
            }
            src = cur;
            cur = cur->next;
        }
        return false;
    }
};

template <class Key, class T, class Hash = std::hash<Key>,
          class Equal = std::equal_to<Key> >
class linked_hashmap : public hashmap<Key, T, Hash, Equal> {
   public:
    typedef pair<const Key, T> value_type;
    using Node = typename hashmap<Key, T, Hash, Equal>::Node;
    double_list<value_type> db;

    class const_iterator;

    class iterator {
       public:
        typename double_list<value_type>::iterator list_iter;
        /**
         * elements
         * add whatever you want
         */
        // --------------------------
        iterator() {}
        iterator(const typename double_list<value_type>::iterator &it)
            : list_iter(it) {}
        iterator(const iterator &other) : list_iter(other.list_iter) {}
        ~iterator() {}

        /**
         * iter++
         */
        iterator operator++(int) { 
            iterator tmp = *this;
            ++list_iter; 
            return tmp;
        }
        /**
         * ++iter
         */
        iterator &operator++() {
            ++list_iter;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) { 
            iterator tmp = *this;
            --list_iter; 
            return tmp;
        }
        /**
         * --iter
         */
        iterator &operator--() {
            --list_iter;
            return *this;
        }

        /**
         * if the iter didn't point to a value
         * throw "star invalid"
         */
        value_type &operator*() const {
            if (list_iter.cur == nullptr) {
                throw "star invalid";
            }
            return *list_iter;
        }
        value_type *operator->() const noexcept {
            if (list_iter.cur == nullptr) {
                throw "star invalid";
            }
            return &(*list_iter);
        }

        /**
         * operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        bool operator==(const iterator &rhs) const {
            return list_iter == rhs.list_iter;
        }
        bool operator!=(const iterator &rhs) const {
            return list_iter != rhs.list_iter;
        }
        bool operator==(const const_iterator &rhs) const {
            return list_iter == rhs.list_iter;
        }
        bool operator!=(const const_iterator &rhs) const {
            return list_iter != rhs.list_iter;
        }
    };

    class const_iterator {
       public:
        typename double_list<value_type>::const_iterator list_iter;
        /**
         * elements
         * add whatever you want
         */
        // --------------------------
        const_iterator() {}
        const_iterator(
            const typename double_list<value_type>::const_iterator &it)
            : list_iter(it) {}
        const_iterator(const iterator &other) : list_iter(other.list_iter) {}
        const_iterator(const const_iterator &other)
            : list_iter(other.list_iter) {}

        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator tmp(list_iter);
            ++list_iter;
            return tmp;
        }
        /**
         * ++iter
         */
        const_iterator &operator++() {
            ++list_iter;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator tmp(list_iter);
            --list_iter;
            return tmp;
        }
        /**
         * --iter
         */
        const_iterator &operator--() {
            --list_iter;
            return *this;
        }

        /**
         * if the iter didn't point to a value
         * throw
         */
        const value_type &operator*() const {
            if (list_iter.cur == nullptr) {
                throw "star invalid";
            }
            return *list_iter;
        }
        const value_type *operator->() const noexcept {
            if (list_iter.cur == nullptr) {
                throw "star invalid";
            }
            return &(*list_iter);
        }

        /**
         * operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        bool operator==(const iterator &rhs) const {
            return list_iter == rhs.list_iter;
        }
        bool operator!=(const iterator &rhs) const {
            return list_iter != rhs.list_iter;
        }
        bool operator==(const const_iterator &rhs) const {
            return list_iter == rhs.list_iter;
        }
        bool operator!=(const const_iterator &rhs) const {
            return list_iter != rhs.list_iter;
        }
    };

    linked_hashmap() : hashmap<Key, T, Hash, Equal>(), db() {}
    linked_hashmap(const linked_hashmap &other)
        : hashmap<Key, T, Hash, Equal>(), db() {
        for (auto it = other.db.begin(); it != other.db.end(); ++it) {
            this->insert(*it);
        }
    }
    ~linked_hashmap() {}
    linked_hashmap &operator=(const linked_hashmap &other) {
        if (this == &other) return *this;
        this->clear();
        for (auto it = other.db.begin(); it != other.db.end(); ++it) {
            this->insert(*it);
        }
        return *this;
    }

    /**
     * return the value connected with the Key(O(1))
     * if the key not found, throw
     */
    T &at(const Key &key) {
        int idx = this->hash(key) % this->buckets.size();
        Node *src = this->buckets[idx];
        while (src) {
            if (this->equal(src->data.first, key)) {
                return src->data.second;
            }
            src = src->next;
        }
        throw "invalid";
    }
    const T &at(const Key &key) const {
        int idx = this->hash(key) % this->buckets.size();
        Node *src = this->buckets[idx];
        while (src) {
            if (this->equal(src->data.first, key)) {
                return src->data.second;
            }
            src = src->next;
        }
        throw "invalid";
    }
    T &operator[](const Key &key) {
        int idx = this->hash(key) % this->buckets.size();
        Node *src = this->buckets[idx];
        while (src) {
            if (this->equal(src->data.first, key)) {
                return src->data.second;
            }
            src = src->next;
        }
        throw "invalid";
    }
    const T &operator[](const Key &key) const {
        int idx = this->hash(key) % this->buckets.size();
        Node *src = this->buckets[idx];
        while (src) {
            if (this->equal(src->data.first, key)) {
                return src->data.second;
            }
            src = src->next;
        }
        throw "invalid";
    }

    /**
     * return an iterator point to the first
     * inserted and existed element
     */
    iterator begin() { return iterator(db.begin()); }
    const_iterator cbegin() const { return const_iterator(db.begin()); }
    /**
     * return an iterator after the last inserted element
     */
    iterator end() { return iterator(db.end()); }
    const_iterator cend() const { return const_iterator(db.end()); }
    /**
     * if didn't contain anything, return true,
     * otherwise false.
     */
    bool empty() const {
        if (db.empty()) return true;
        return false;
    }

    void clear() {
        hashmap<Key, T, Hash, Equal>::clear();
        db.clear();
    }

    size_t size() const { return this->num_elem; }
    /**
     * insert the value_piar
     * if the key of the value_pair exists in the map
     * update the value instead of adding a new element，
     * then the order of the element moved from inner of the
     * list to the head of the list
     * and return false
     * if the key of the value_pair doesn't exist in the map
     * add a new element and return true
     */
    pair<iterator, bool> insert(const value_type &value) {
        Key key = value.first;
        int idx = this->hash(key) % this->buckets.size();
        Node *cur = this->buckets[idx];
        bool flag = true;
        while (cur) {
            if (this->equal(key, cur->data.first)) {
                flag = false;
                cur->data.second = value.second;
                db.erase(cur->list_iter);
                db.insert_tail(value);
                cur->list_iter = db.get_tail();
                auto it = iterator(cur->list_iter);
                return sjtu::pair<iterator, bool>(it, flag);
            } else {
                cur = cur->next;
            }
        }
        Node *newnode = new Node(value);
        newnode->next = this->buckets[idx];
        this->buckets[idx] = newnode;
        db.insert_tail(value);
        newnode->list_iter = db.get_tail();
        this->num_elem++;
        if (this->num_elem > this->buckets.size() * 0.75) {
            this->expand();
        }
        auto it = iterator(newnode->list_iter);
        return sjtu::pair<iterator, bool>(it, flag);
    }
    /**
     * erase the value_pair pointed by the iterator
     * if the iterator points to nothing
     * throw
     */
    void remove(iterator pos) {
        if (pos == end()) throw "iterator invalid";
        Key key = pos.list_iter.cur->data.first;
        hashmap<Key, T, Hash, Equal>::remove(key);
        db.erase(pos.list_iter);
    }
    /**
     * return how many value_pairs consist of key
     * this should only return 0 or 1
     */
    size_t count(const Key &key) const {
        int idx = this->hash(key) % this->buckets.size();
        Node *src = this->buckets[idx];
        while (src) {
            if (this->equal(src->data.first, key)) return 1;
            src = src->next;
        }
        return 0;
    }
    /**
     * find the iterator points at the value_pair
     * which consist of key
     * if not find, return the iterator
     * point at nothing
     */
    iterator find(const Key &key) {
        int idx = this->hash(key) % this->buckets.size();
        Node *src = this->buckets[idx];
        while (src) {
            if (this->equal(src->data.first, key))
                return iterator(src->list_iter);
            src = src->next;
        }
        return db.end();
    }
};

class lru {
    using lmap = sjtu::linked_hashmap<Integer, Matrix<int>, Hash, Equal>;
    using value_type = sjtu::pair<const Integer, Matrix<int> >;

   public:
   lmap mp;
   int size;

    lru(int size) :size(size) {
    }
    ~lru() {}
    /**
     * save the value_pair in the memory
     * delete something in the memory if necessary
     */
    void save(const value_type &v)  {
        auto res = mp.insert(v);
        if(!res.second){
        }else{
            if(mp.size()>size){
                auto tail_it = mp.begin();
                mp.remove(tail_it);
            }
        }
    }
    /**
     * return a pointer contain the value
     */
    Matrix<int> *get(const Integer &v) {
        auto it = mp.find(v);
        if(it==mp.end()) return nullptr;
            else{
                auto val = it->second;
                mp.remove(it);
                mp.insert({v,val});
            }
        return &(mp.at(v));
    }
    /**
     * just print everything in the memory
     * to debug or test.
     * this operation follows the order, but don't
     * change the order.
     */
    void print() {
        lmap::iterator it;
        for(it = mp.begin(); it!=mp.end();++it){
            std::cout<<(*it).first.val<<" "<<(*it).second<<std::endl;
        }
    }
};
}  // namespace sjtu

#endif