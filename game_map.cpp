#include "game_map.h"
#include <algorithm>
#include <iostream>
#include <random>

GameMap::GameMap(int r, int c) : rows(r), cols(c) {
    grid = vector<vector<char>>(r, vector<char>(c, EMPTY));
    graph_ptr = new Graph(r, c);
    generate();
}

GameMap::~GameMap() {
    delete graph_ptr;
}

void GameMap::generate() {
    home_pos = {rows - 1, cols - 1};
    grid[home_pos.first][home_pos.second] = HOME;

    player_pos = {0, 0};
    grid[0][0] = PLAYER;

    vector<Node> available;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (Node{r, c} != player_pos && Node{r, c} != home_pos) {
                available.push_back({r, c});
            }
        }
    }
    
    random_device rd;
    mt19937 g(rd());
    shuffle(available.begin(), available.end(), g);

    wolf_pos = available.back();
    available.pop_back();
    grid[wolf_pos.first][wolf_pos.second] = WOLF;

    int num_traps = max(3, (rows * cols) / 15);
    for (int i = 0; i < num_traps && !available.empty(); i++) {
        Node trap_cell = available.back();
        available.pop_back();
        traps.insert(trap_cell);
        grid[trap_cell.first][trap_cell.second] = TRAP;
    }

    int num_walls = max(5, (rows * cols) / 10);
    set<Node> walls;
    int wall_count = 0;
    int attempts = 0;
    int max_attempts = 500;
    
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    graph_ptr->add_edge({r, c}, {nr, nc}, 1);
                }
            }
        }
    }
    
    while (wall_count < num_walls && !available.empty() && attempts < max_attempts) {
        Node wall_cell = available.back();
        available.pop_back();
        
        walls.insert(wall_cell);
        grid[wall_cell.first][wall_cell.second] = WALL;
        
        for (int d = 0; d < 4; d++) {
            int nr = wall_cell.first + dr[d];
            int nc = wall_cell.second + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                graph_ptr->remove_edge(wall_cell, {nr, nc});
            }
        }
        
        auto path = graph_ptr->bfs_shortest_path(player_pos, home_pos);
        if (path.empty()) {
            walls.erase(wall_cell);
            grid[wall_cell.first][wall_cell.second] = EMPTY;
            for (int d = 0; d < 4; d++) {
                int nr = wall_cell.first + dr[d];
                int nc = wall_cell.second + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    graph_ptr->add_edge(wall_cell, {nr, nc}, 1);
                }
            }
        } else {
            wall_count++;
        }
        attempts++;
    }
}

char GameMap::get_cell(int r, int c) {
    return grid[r][c];
}

bool GameMap::is_valid_move(Node pos) {
    int r = pos.first;
    int c = pos.second;
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    if (grid[r][c] == WALL) return false;
    return true;
}

vector<Node> GameMap::get_adjacent_cells(Node pos) {
    int r = pos.first;
    int c = pos.second;
    vector<Node> cells;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        int nr = r + dr[d];
        int nc = c + dc[d];
        if (is_valid_move({nr, nc})) {
            cells.push_back({nr, nc});
        }
    }
    return cells;
}

void GameMap::move_player(Node new_pos) {
    grid[player_pos.first][player_pos.second] = EMPTY;
    player_pos = new_pos;
    grid[new_pos.first][new_pos.second] = (new_pos == home_pos) ? HOME : PLAYER;
}

void GameMap::move_wolf(Node new_pos) {
    grid[wolf_pos.first][wolf_pos.second] = EMPTY;
    wolf_pos = new_pos;
    grid[new_pos.first][new_pos.second] = WOLF;
}

bool GameMap::is_trap(Node pos) {
    return traps.count(pos) > 0;
}

void GameMap::display() {
    string border = "+";
    for (int c = 0; c < cols; c++) border += "---+";
    cout << border << "\n";
    for (int r = 0; r < rows; r++) {
        string row_str = "|";
        for (int c = 0; c < cols; c++) {
            char cell = grid[r][c];
            string cell_str;
            if (cell == WALL) cell_str = " # ";
            else if (cell == PLAYER) cell_str = " R ";
            else if (cell == WOLF) cell_str = " G ";
            else if (cell == HOME) cell_str = " V ";
            else if (cell == TRAP) cell_str = " T ";
            else cell_str = " . ";
            row_str += cell_str + "|";
        }
        cout << row_str << "\n";
        cout << border << "\n";
    }
}

Graph* GameMap::get_graph() { return graph_ptr; }
Node GameMap::get_player_pos() { return player_pos; }
Node GameMap::get_wolf_pos() { return wolf_pos; }
Node GameMap::get_home_pos() { return home_pos; }
set<Node> GameMap::get_traps() { return traps; }
vector<vector<char>> GameMap::get_grid() { return grid; }
void GameMap::set_player_pos(Node pos) { player_pos = pos; }
void GameMap::set_wolf_pos(Node pos) { wolf_pos = pos; }
void GameMap::set_grid(vector<vector<char>> g) { grid = g; }
void GameMap::set_traps(set<Node> t) { traps = t; }