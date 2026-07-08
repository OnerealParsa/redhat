#ifndef GAME_H
#define GAME_H

#include "game_map.h"
#include "stack.h"
#include <string>
#include <set>
#include <vector>
#include <utility>

using namespace std;

struct GameState {
    Node player_pos;
    Node wolf_pos;
    Node home_pos;
    int score;
    int turn_score;
    set<Node> traps;
    vector<vector<char>> grid_snapshot;
    int turn;
};

class Game {
private:
    GameMap* game_map;
    string username;
    int score;
    int turn_score;
    int turn;
    bool game_over;
    bool won;
    Stack<GameState> undo_stack;
    vector<Node> dijkstra_path;
    void compute_dijkstra();
    void save_state();
    void wolf_turn();

public:
    static const int SCORE_FOLLOW_DIJKSTRA = 3;
    static const int SCORE_DIFFERENT_MOVE = 1;
    static const int SCORE_UNDO = -2;
    static const int SCORE_REACH_HOME = 5;
    static const int SCORE_BONUS_WIN = 5;

    Game(GameMap* gm, const string& user);
    pair<bool, string> player_move(Node new_pos);
    pair<bool, string> undo();
    Node get_dijkstra_suggestion();
    int get_score();
    bool is_game_over();
    bool is_won();
    void display();
};

#endif