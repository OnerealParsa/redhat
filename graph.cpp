#include "graph.h"
#include <algorithm>
#include <tuple>

Graph::Graph(int r, int c) : rows(r), cols(c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            adj[{i, j}] = {};
        }
    }
}

void Graph::add_edge(Node u, Node v, int w) {
    bool found = false;
    for (const auto& p : adj[u]) {
        if (p.first == v) { found = true; break; }
    }
    if (!found) adj[u].push_back({v, w});
    
    found = false;
    for (const auto& p : adj[v]) {
        if (p.first == u) { found = true; break; }
    }
    if (!found) adj[v].push_back({u, w});
}

void Graph::remove_edge(Node u, Node v) {
    auto& vu = adj[u];
    vu.erase(remove_if(vu.begin(), vu.end(),
        [v](const pair<Node, int>& p) { return p.first == v; }), vu.end());
    auto& vv = adj[v];
    vv.erase(remove_if(vv.begin(), vv.end(),
        [u](const pair<Node, int>& p) { return p.first == u; }), vv.end());
}

vector<Node> Graph::neighbors(Node node) {
    vector<Node> result;
    if (adj.find(node) != adj.end()) {
        for (const auto& p : adj[node]) {
            result.push_back(p.first);
        }
    }
    return result;
}

int Graph::get_weight(Node u, Node v) {
    for (const auto& p : adj[u]) {
        if (p.first == v) return p.second;
    }
    return INT_MAX;
}

pair<map<Node, int>, map<Node, Node>> Graph::dijkstra(Node start) {
    map<Node, int> dist;
    map<Node, Node> prev;
    for (const auto& kv : adj) {
        dist[kv.first] = INT_MAX;
        prev[kv.first] = {-1, -1};
    }
    dist[start] = 0;
    priority_queue<pair<int, Node>, vector<pair<int, Node>>, greater<>> pq;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        Node u = top.second;
        if (d > dist[u]) continue;
        for (const auto& edge : adj[u]) {
            Node v = edge.first;
            int w = edge.second;
            int nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                prev[v] = u;
                pq.push({nd, v});
            }
        }
    }
    return {dist, prev};
}

vector<Node> Graph::dijkstra_path(Node start, Node goal) {
    auto result = dijkstra(start);
    auto& dist = result.first;
    auto& prev = result.second;
    if (dist[goal] == INT_MAX) return {};
    vector<Node> path;
    Node cur = goal;
    while (cur != Node{-1, -1}) {
        path.push_back(cur);
        if (cur == start) break;
        cur = prev[cur];
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<Node> Graph::bfs(Node start) {
    map<Node, bool> visited;
    vector<Node> order;
    deque<Node> q;
    q.push_back(start);
    visited[start] = true;
    while (!q.empty()) {
        Node u = q.front();
        q.pop_front();
        order.push_back(u);
        for (const auto& v : neighbors(u)) {
            if (!visited[v]) {
                visited[v] = true;
                q.push_back(v);
            }
        }
    }
    return order;
}

vector<Node> Graph::bfs_shortest_path(Node start, Node goal) {
    if (start == goal) return {start};
    map<Node, bool> visited;
    deque<pair<Node, vector<Node>>> q;
    q.push_back({start, {start}});
    visited[start] = true;
    while (!q.empty()) {
        auto front = q.front();
        q.pop_front();
        Node u = front.first;
        vector<Node> path = front.second;
        for (const auto& v : neighbors(u)) {
            if (v == goal) {
                vector<Node> result = path;
                result.push_back(v);
                return result;
            }
            if (!visited[v]) {
                visited[v] = true;
                vector<Node> new_path = path;
                new_path.push_back(v);
                q.push_back({v, new_path});
            }
        }
    }
    return {};
}

pair<vector<Node>, int> Graph::a_star(Node start, Node goal, function<int(Node, Node)> heuristic) {
    using PQEntry = tuple<int, int, Node>;
    priority_queue<PQEntry, vector<PQEntry>, greater<>> open_set;
    open_set.push(make_tuple(heuristic(start, goal), 0, start));
    map<Node, int> g_score;
    map<Node, Node> prev;
    for (auto& kv : adj) {
        g_score[kv.first] = INT_MAX;
        prev[kv.first] = {-1, -1};
    }
    g_score[start] = 0;
    map<Node, bool> visited;
    while (!open_set.empty()) {
        auto top = open_set.top();
        open_set.pop();
        int f = get<0>(top);
        int g = get<1>(top);
        Node u = get<2>(top);
        if (u == goal) {
            vector<Node> path;
            Node cur = goal;
            while (cur != Node{-1, -1}) {
                path.push_back(cur);
                if (cur == start) break;
                cur = prev[cur];
            }
            reverse(path.begin(), path.end());
            return {path, g};
        }
        if (visited[u]) continue;
        visited[u] = true;
        for (auto& edge : adj[u]) {
            Node v = edge.first;
            int w = edge.second;
            int tentative = g_score[u] + w;
            if (tentative < g_score[v]) {
                g_score[v] = tentative;
                prev[v] = u;
                int f_score = tentative + heuristic(v, goal);
                open_set.push(make_tuple(f_score, tentative, v));
            }
        }
    }
    return {{}, INT_MAX};
}

int Graph::manhattan(Node a, Node b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}