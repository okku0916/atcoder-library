struct Point {
    ll x, y;
};

// 上半分か下半分かどちらの領域か判定
int half(const Point& p) {
    if (p.x == 0 && p.y == 0) // 原点
        return -1;
    if (p.y > 0 || (p.y == 0 && p.x > 0)) // [0°, 180°)
        return 0;
    else // [180°, 360°)
        return 1;
}

// a < b　のboolを返す比較関数
bool cmp(const Point& a, const Point& b) {
    int a_half = half(a);
    int b_half = half(b);

    if (a_half != b_half) return (a_half < b_half); // 領域が違う場合
    if (a_half == -1) return false; // 同一点はfalseを返す

    // 外積で比較
    __int128 cross = (__int128)a.x * b.y
                   - (__int128)a.y * b.x;
    if (cross != 0) return cross > 0; // 外積が正なら a の方が小さい

    // 同じ方向なら原点から近い順という順序をつける場合はコメントアウトを解除
    // __int128 da = (__int128)a.x * a.x
    //             + (__int128)a.y * a.y;
    // __int128 db = (__int128)b.x * b.x
    //             + (__int128)b.y * b.y;
    // return da < db;
}