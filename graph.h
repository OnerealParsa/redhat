#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <deque>
#include <climits>
#include <functional>
#include <iostream>

using namespace std;

typedef pair<int, int> Node;

class Graph {
private:
    int rows;
    int cols;
    map<Node, vector<pair<Node, int>>> adj;

public:
    Graph(int r, int c);
    void add_edge(Node u, Node v, int w = 1);
    void remove_edge(Node u, Node v);
    vector<Node> neighbors(Node node);
    int get_weight(Node u, Node v);
    pair<map<Node, int>, map<Node, Node>> dijkstra(Node start);
    vector<Node> dijkstra_path(Node start, Node goal);
    vector<Node> bfs(Node start);
    vector<Node> bfs_shortest_path(Node start, Node goal);
    pair<vector<Node>, int> a_star(Node start, Node goal, function<int(Node, Node)> heuristic);
    static int manhattan(Node a, Node b);
};

#endif