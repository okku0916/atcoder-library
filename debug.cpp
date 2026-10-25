// Debug print (C++17)
// 先に全overloadを宣言し、入れ子のコンテナでも正しく解決されるようにする
namespace dbg {
    template<class T> void one(const T& x);
    template<class T, class U> void one(const pair<T, U>& x);
    template<class... Ts> void one(const tuple<Ts...>& x);
    template<class T, class A> void one(const vector<T, A>& x);
    template<class T, size_t N> void one(const array<T, N>& x);
    template<class T, class A> void one(const deque<T, A>& x);
    template<class T, class A> void one(const list<T, A>& x);
    template<class T, class C, class A> void one(const set<T, C, A>& x);
    template<class T, class C, class A> void one(const multiset<T, C, A>& x);
    template<class T, class H, class E, class A> void one(const unordered_set<T, H, E, A>& x);
    template<class T, class H, class E, class A> void one(const unordered_multiset<T, H, E, A>& x);
    template<class K, class V, class C, class A> void one(const map<K, V, C, A>& x);
    template<class K, class V, class C, class A> void one(const multimap<K, V, C, A>& x);
    template<class K, class V, class H, class E, class A> void one(const unordered_map<K, V, H, E, A>& x);
    template<class K, class V, class H, class E, class A> void one(const unordered_multimap<K, V, H, E, A>& x);
    template<class T, class C> void one(stack<T, C> x);
    template<class T, class C> void one(queue<T, C> x);
    template<class T, class C, class Comp> void one(priority_queue<T, C, Comp> x);
    template<size_t N> void one(const bitset<N>& x);

    template<class It>
    void sequence(It first, It last, const char* left, const char* right) {
        cerr << left;
        bool initial = true;
        for (; first != last; ++first) {
            if (!initial) cerr << ", ";
            initial = false;
            one(*first);
        }
        cerr << right;
    }

    template<class T> void one(const T& x) { cerr << x; }
    template<class T, class U> void one(const pair<T, U>& x) {
        cerr << '('; one(x.first); cerr << ", "; one(x.second); cerr << ')';
    }
    template<class... Ts> void one(const tuple<Ts...>& x) {
        cerr << '(';
        bool first = true;
        apply([&](const auto&... items) {
            (([&] { if (!first) cerr << ", ";
            first = false; one(items); }()), ...);
        }, x);
        cerr << ')';
    }
    template<class T, class A> void one(const vector<T, A>& x) { sequence(x.begin(), x.end(), "[", "]"); }
    template<class T, size_t N> void one(const array<T, N>& x) { sequence(x.begin(), x.end(), "[", "]"); }
    template<class T, class A> void one(const deque<T, A>& x) { sequence(x.begin(), x.end(), "[", "]"); }
    template<class T, class A> void one(const list<T, A>& x) { sequence(x.begin(), x.end(), "[", "]"); }
    template<class T, class C, class A> void one(const set<T, C, A>& x) { sequence(x.begin(), x.end(), "{", "}"); }
    template<class T, class C, class A> void one(const multiset<T, C, A>& x) { sequence(x.begin(), x.end(), "{", "}"); }
    template<class T, class H, class E, class A> void one(const unordered_set<T, H, E, A>& x) { sequence(x.begin(), x.end(), "{", "}"); }
    template<class T, class H, class E, class A> void one(const unordered_multiset<T, H, E, A>& x) { sequence(x.begin(), x.end(), "{", "}"); }
    template<class K, class V, class C, class A> void one(const map<K, V, C, A>& x) {
        cerr << '{'; bool first = true;
        for (const auto& [k, v] : x) {
            if (!first) cerr << ", ";
            first = false;
            one(k); cerr << ": "; one(v);
        }
        cerr << '}';
    }
    template<class K, class V, class C, class A> void one(const multimap<K, V, C, A>& x) {
        cerr << '{'; bool first = true;
        for (const auto& [k, v] : x) {
            if (!first) cerr << ", ";
            first = false;
            one(k); cerr << ": "; one(v);
        }
        cerr << '}';
    }
    template<class K, class V, class H, class E, class A> void one(const unordered_map<K, V, H, E, A>& x) {
        cerr << '{'; bool first = true;
        for (const auto& [k, v] : x) {
            if (!first) cerr << ", ";
            first = false;
            one(k); cerr << ": "; one(v);
        }
        cerr << '}';
    }
    template<class K, class V, class H, class E, class A> void one(const unordered_multimap<K, V, H, E, A>& x) {
        cerr << '{'; bool first = true;
        for (const auto& [k, v] : x) {
            if (!first) cerr << ", ";
            first = false;
            one(k); cerr << ": "; one(v);
        }
        cerr << '}';
    }
    template<class T, class C> void one(stack<T, C> x) {
        cerr << "[top: "; bool first = true;
        while (!x.empty()) {
            if (!first) cerr << ", ";
            first = false;
            one(x.top()); x.pop();
        }
        cerr << ']';
    }
    template<class T, class C> void one(queue<T, C> x) {
        cerr << "[front: "; bool first = true;
        while (!x.empty()) {
            if (!first) cerr << ", ";
            first = false;
            one(x.front()); x.pop();
        }
        cerr << ']';
    }
    template<class T, class C, class Comp> void one(priority_queue<T, C, Comp> x) {
        cerr << "[top: "; bool first = true;
        while (!x.empty()) {
            if (!first) cerr << ", ";
            first = false;
            one(x.top()); x.pop();
        }
        cerr << ']';
    }
    template<size_t N> void one(const bitset<N>& x) { cerr << x; }

    template<class T> struct vector_depth : integral_constant<int, 0> {};
    template<class T, class A> struct vector_depth<vector<T, A>> : integral_constant<int, 1 + vector_depth<T>::value> {};

    template<class T> void value(const T& x) { one(x); cerr << '\n'; }
    template<class T, class A1, class A2> void value(const vector<vector<T, A1>, A2>& x) {
        for (const auto& row : x) { one(row); cerr << '\n'; }
    }
    template<class T, class A1, class A2, class A3>
    void value(const vector<vector<vector<T, A1>, A2>, A3>& x) {
        for (size_t i = 0; i < x.size(); ++i) {
            cerr << '[' << i << "]\n";
            for (const auto& row : x[i]) { one(row); cerr << '\n'; }
            if (i + 1 < x.size()) cerr << '\n';
        }
    }

    // 式名のカンマ区切りを解析。関数引数や添字内のカンマは分割しない
    inline vector<string> names(const string& s) {
        vector<string> result;
        size_t start = 0;
        int paren = 0, bracket = 0, brace = 0;
        char quote = 0;
        bool escaped = false;
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i];
            if (quote) {
                if (escaped) escaped = false;
                else if (c == 92) escaped = true;
                else if (c == quote) quote = 0;
                continue;
            }
            if (c == 39 || c == 34) { quote = c; continue; }
            if (c == '(') ++paren;
            else if (c == ')') --paren;
            else if (c == '[') ++bracket;
            else if (c == ']') --bracket;
            else if (c == '{') ++brace;
            else if (c == '}') --brace;
            else if (c == ',' && !paren && !bracket && !brace) {
                auto part = s.substr(start, i - start);
                auto begin = part.find_first_not_of(' ');
                result.push_back(begin == string::npos ? "" : part.substr(begin, part.find_last_not_of(' ') - begin + 1));
                start = i + 1;
            }
        }
        auto part = s.substr(start);
        auto begin = part.find_first_not_of(' ');
        result.push_back(begin == string::npos ? "" : part.substr(begin, part.find_last_not_of(' ') - begin + 1));
        return result;
    }

    template<class... Ts> void print_named(const char* name_str, const Ts&... xs) {
        auto ns = names(name_str);
        size_t i = 0;
        bool on_line = false;
        auto emit = [&](const auto& x) {
            using T = decay_t<decltype(x)>;
            string name = i < ns.size() ? ns[i] : "?";
            ++i;
            if constexpr (vector_depth<T>::value >= 2) {
                if (on_line) cerr << endl;
                on_line = false;
                cerr << name << " =" << endl;
                value(x);
            } else {
                if (on_line) cerr << ", ";
                cerr << name << " = ";
                one(x);
                on_line = true;
            }
        };
        (emit(xs), ...);
        if (on_line) cerr << endl;
    }
    template<class... Ts> void print_values(const Ts&... xs) {
        (value(xs), ...);
    }
}

#define PRINT(...) dbg::print_named(#__VA_ARGS__, __VA_ARGS__)
#define print(...) dbg::print_values(__VA_ARGS__)
