// indexもグラフに持たせる
vector<bool> seen(n);
vector<bool> finished(n);
vl vertex_history;
vl edge_history;
vl ans_vertex;
vl ans_edge;
auto dfs = [&](auto self, ll v, ll pidx, bool directed /*有向グラフかどうか*/) -> bool {
    seen[v] = true;
    vertex_history.push_back(v);
    for (auto [nv, nidx] : g[v]) {
        if (!directed && nidx == pidx) continue; // 無向グラフなら戻る遷移は排除する必要がある
        
        if (finished[nv]) continue; // nvが探索済みならスキップ
        if (seen[nv] && !finished[nv]) { // nv探索中なら検知
            ll pos = 0;
            while (vertex_history[pos] != nv) pos++; // posをvertex_history[pos]==nvまで進める。初めの方のいらない部分を無視する。

            // 閉路の長さ(条件に合う閉路を見つけたいなら、条件に合うならansに入れ、合わないなら探索続行)
            ll cycle_len = vertex_history.size() - pos;

            // 閉路の頂点
            for (ll i = pos; i < (ll)vertex_history.size(); i++) {
                ans_vertex.push_back(vertex_history[i]);
            }

            // 閉路の辺
            for (ll i = pos; i < (ll)edge_history.size(); i++) {
                ans_edge.push_back(edge_history[i]);
            }
            ans_edge.push_back(nidx);

            return true;
        }
        edge_history.push_back(nidx);
        if (self(self, nv, nidx, directed)) return true; // dfsで検知
        edge_history.pop_back();
    }
    finished[v] = true;
    vertex_history.pop_back();
    return false;
};

rep(i, n) {
    if (!finished[i]) {
        if (dfs(dfs, i, -1, true)) { // 有向
            // ans_vertexが閉路の頂点リスト
            // ans_edgeが閉路の辺番号リスト
            return 0;
        }
    }
}