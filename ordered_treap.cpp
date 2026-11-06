template <class T, bool AllowDuplicates, class Compare = std::less<T>>
class OrderedTreap {
    struct Node {
        T key;
        uint64_t priority;
        int cnt = 1;
        int sz = 1;
        Node *left = nullptr, *right = nullptr;
        Node(const T& key, uint64_t priority) : key(key), priority(priority) {}
    };

    Node* root_ = nullptr;
    Compare comp_;

    static uint64_t random_priority() {
        static uint64_t state =
            std::chrono::steady_clock::now().time_since_epoch().count();
        state += 0x9e3779b97f4a7c15ULL;
        uint64_t z = state;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }

    static int size_of(const Node* t) { return t ? t->sz : 0; }
    static void update(Node* t) {
        if (t) t->sz = size_of(t->left) + t->cnt + size_of(t->right);
    }
    static Node* rotate_right(Node* t) {
        Node* x = t->left;
        t->left = x->right;
        x->right = t;
        update(t);
        update(x);
        return x;
    }
    static Node* rotate_left(Node* t) {
        Node* x = t->right;
        t->right = x->left;
        x->left = t;
        update(t);
        update(x);
        return x;
    }
    Node* insert_impl(Node* t, const T& x, bool& inserted) {
        if (!t) {
            inserted = true;
            return new Node(x, random_priority());
        }
        if (comp_(x, t->key)) {
            t->left = insert_impl(t->left, x, inserted);
            if (t->left->priority > t->priority) t = rotate_right(t);
        } else if (comp_(t->key, x)) {
            t->right = insert_impl(t->right, x, inserted);
            if (t->right->priority > t->priority) t = rotate_left(t);
        } else {
            if constexpr (AllowDuplicates) {
                ++t->cnt;
                inserted = true;
            }
        }
        update(t);
        return t;
    }
    static Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->priority > b->priority) {
            a->right = merge(a->right, b);
            update(a);
            return a;
        }
        b->left = merge(a, b->left);
        update(b);
        return b;
    }
    Node* erase_impl(Node* t, const T& x, bool& erased) {
        if (!t) return nullptr;
        if (comp_(x, t->key)) {
            t->left = erase_impl(t->left, x, erased);
        } else if (comp_(t->key, x)) {
            t->right = erase_impl(t->right, x, erased);
        } else {
            erased = true;
            if (t->cnt > 1) {
                --t->cnt;
            } else {
                Node* next = merge(t->left, t->right);
                delete t;
                return next;
            }
        }
        update(t);
        return t;
    }
    static Node* erase_at_impl(Node* t, int k) {
        int left_size = size_of(t->left);
        if (k < left_size) {
            t->left = erase_at_impl(t->left, k);
        } else if (k >= left_size + t->cnt) {
            t->right = erase_at_impl(t->right, k - left_size - t->cnt);
        } else {
            if (t->cnt > 1) {
                --t->cnt;
            } else {
                Node* next = merge(t->left, t->right);
                delete t;
                return next;
            }
        }
        update(t);
        return t;
    }
    static void destroy(Node* t) {
        if (!t) return;
        destroy(t->left);
        destroy(t->right);
        delete t;
    }
    static void collect(const Node* t, std::vector<T>& v) {
        if (!t) return;
        collect(t->left, v);
        for (int i = 0; i < t->cnt; ++i) v.push_back(t->key);
        collect(t->right, v);
    }

public:
    OrderedTreap() = default;
    explicit OrderedTreap(const Compare& comp) : comp_(comp) {}
    OrderedTreap(std::initializer_list<T> init, const Compare& comp = Compare())
        : comp_(comp) {
        for (const auto& x : init) insert(x);
    }
    ~OrderedTreap() { clear(); }
    OrderedTreap(const OrderedTreap&) = delete;
    OrderedTreap& operator=(const OrderedTreap&) = delete;
    OrderedTreap(OrderedTreap&& other) noexcept(std::is_nothrow_move_constructible_v<Compare>)
        : root_(std::exchange(other.root_, nullptr)), comp_(std::move(other.comp_)) {}
    OrderedTreap& operator=(OrderedTreap&& other) noexcept(std::is_nothrow_move_assignable_v<Compare>) {
        if (this != &other) {
            clear();
            comp_ = std::move(other.comp_);
            root_ = std::exchange(other.root_, nullptr);
        }
        return *this;
    }

    int size() const { return size_of(root_); }
    bool empty() const { return root_ == nullptr; }
    void clear() { destroy(root_); root_ = nullptr; }

    // Returns true iff an element was actually added.
    bool insert(const T& x) {
        bool inserted = false;
        root_ = insert_impl(root_, x, inserted);
        return inserted;
    }
    // Removes exactly one occurrence. Returns false if absent.
    bool erase(const T& x) {
        bool erased = false;
        root_ = erase_impl(root_, x, erased);
        return erased;
    }
    // Returns false if k is out of range.
    bool erase_at(int k) {
        if (k < 0 || k >= size()) return false;
        root_ = erase_at_impl(root_, k);
        return true;
    }
    const T& kth(int k) const {
        assert(0 <= k && k < size());
        Node* t = root_;
        while (true) {
            int left_size = size_of(t->left);
            if (k < left_size) t = t->left;
            else if (k < left_size + t->cnt) return t->key;
            else {
                k -= left_size + t->cnt;
                t = t->right;
            }
        }
    }
    const T& operator[](int k) const { return kth(k); }
    // Returns the first index whose value is >= x.
    int lower_bound(const T& x) const {
        int ans = 0;
        const Node* t = root_;
        while (t) {
            if (comp_(t->key, x)) {
                ans += size_of(t->left) + t->cnt;
                t = t->right;
            } else t = t->left;
        }
        return ans;
    }
    // Returns the first index whose value is > x.
    int upper_bound(const T& x) const {
        int ans = 0;
        const Node* t = root_;
        while (t) {
            if (!comp_(x, t->key)) {
                ans += size_of(t->left) + t->cnt;
                t = t->right;
            } else t = t->left;
        }
        return ans;
    }
    int count(const T& x) const {
        const Node* t = root_;
        while (t) {
            if (comp_(x, t->key)) t = t->left;
            else if (comp_(t->key, x)) t = t->right;
            else return t->cnt;
        }
        return 0;
    }
    bool contains(const T& x) const { return count(x) != 0; }
    const T& front() const { return kth(0); }
    const T& back() const { return kth(size() - 1); }
    bool pop_front() { return erase_at(0); }
    bool pop_back() { return erase_at(size() - 1); }
    std::vector<T> to_vector() const {
        std::vector<T> v;
        v.reserve(size());
        collect(root_, v);
        return v;
    }
};

template <class T, class Compare = std::less<T>>
using SortedList = OrderedTreap<T, true, Compare>;

template <class T, class Compare = std::less<T>>
using SortedSet = OrderedTreap<T, false, Compare>;