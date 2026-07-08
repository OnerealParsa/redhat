#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "graph.h"
#include <set>
#include <vector>

using namespace std;

class GameMap {
public:
    static const char EMPTY = '.';
    static const char WALL = '#';
    static const char PLAYER = 'R';
    static const char WOLF = 'G';
    static const char HOME = 'V';
    static const char TRAP = 'T';

private:
    int rows;
    int cols;
    vector<vector<char>> grid;
    Graph* graph_ptr;
    Node player_pos;
    Node wolf_pos;
    Node home_pos;
    set<Node> traps;
    void generate();

public:
    GameMap(int r = 10, int c = 10);
    ~GameMap();
    char get_cell(int r, int c);
    bool is_valid_move(Node pos);
    vector<Node> get_adjacent_cells(Node pos);
    void move_player(Node new_pos);
    void move_wolf(Node new_pos);
    bool is_trap(Node pos);
    void display();
    Graph* get_graph();
    Node get_player_pos();
    Node get_wolf_pos();
    Node get_home_pos();
    set<Node> get_traps();
    vector<vector<char>> get_grid();
    void set_player_pos(Node pos);
    void set_wolf_pos(Node pos);
    void set_grid(vector<vector<char>> g);
    void set_traps(set<Node> t);
};

#endif