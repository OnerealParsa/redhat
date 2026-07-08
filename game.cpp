#include "game.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <climits>
#include <random>

Game::Game(GameMap* gm, const string& user)
    : game_map(gm), username(user), score(0), turn_score(0), turn(0), game_over(false), won(false) {
    compute_dijkstra();
}

void Game::compute_dijkstra() {
    auto result = game_map->get_graph()->dijkstra(game_map->get_player_pos());
    auto& dist = result.first;
    auto& prev = result.second;
    Node home = game_map->get_home_pos();
    if (dist.count(home) && dist[home] != INT_MAX) {
        dijkstra_path.clear();
        Node cur = home;
        while (cur != Node{-1, -1}) {
            dijkstra_path.push_back(cur);
            if (cur == game_map->get_player_pos()) break;
            cur = prev[cur];
        }
        reverse(dijkstra_path.begin(), dijkstra_path.end());
    } else {
        dijkstra_path.clear();
    }
}

void Game::save_state() {
    GameState state;
    state.player_pos = game_map->get_player_pos();
    state.wolf_pos = game_map->get_wolf_pos();
    state.home_pos = game_map->get_home_pos();
    state.score = score;
    state.turn_score = turn_score;
    state.traps = game_map->get_traps();
    state.grid_snapshot = game_map->get_grid();
    state.turn = turn;
    undo_stack.push(state);
}

Node Game::get_dijkstra_suggestion() {
    if (dijkstra_path.size() < 2) return {-1, -1};
    for (size_t i = 0; i < dijkstra_path.size(); i++) {
        if (dijkstra_path[i] == game_map->get_player_pos() && i + 1 < dijkstra_path.size()) {
            return dijkstra_path[i + 1];
        }
    }
    if (dijkstra_path.size() >= 2) return dijkstra_path[1];
    return {-1, -1};
}

pair<bool, string> Game::player_move(Node new_pos) {
    if (game_over) return {false, "Game is already over."};
    if (!game_map->is_valid_move(new_pos)) return {false, "Invalid move."};

    auto adj = game_map->get_adjacent_cells(game_map->get_player_pos());
    bool found = false;
    for (auto& cell : adj) {
        if (cell == new_pos) { found = true; break; }
    }
    if (!found) return {false, "You can only move to adjacent cells."};

    save_state();

    turn_score = 0;
    Node suggestion = get_dijkstra_suggestion();
    if (new_pos == suggestion) {
        turn_score += SCORE_FOLLOW_DIJKSTRA;
    } else {
        turn_score += SCORE_DIFFERENT_MOVE;
    }

    if (game_map->is_trap(new_pos)) {
        turn_score = 0;
    }

    score += turn_score;
    game_map->move_player(new_pos);

    if (new_pos == game_map->get_home_pos()) {
        score += SCORE_REACH_HOME;
        score += SCORE_BONUS_WIN;
        game_over = true;
        won = true;
        return {true, "You reached home! You win!"};
    }

    if (new_pos == game_map->get_wolf_pos()) {
        game_over = true;
        won = false;
        return {true, "Wolf caught you! You lose!"};
    }

    int dice = (rand() % 6) + 1;
    if (dice % 2 == 0) {
        wolf_turn();
    }

    turn++;
    compute_dijkstra();

    if (game_map->get_player_pos() == game_map->get_wolf_pos()) {
        game_over = true;
        won = false;
        return {true, "Wolf caught you! You lose!"};
    }

    return {true, "Move completed."};
}

void Game::wolf_turn() {
    Node wolf = game_map->get_wolf_pos();
    Node player = game_map->get_player_pos();

    auto adj = game_map->get_adjacent_cells(wolf);
    if (adj.empty()) return;

    auto path = game_map->get_graph()->bfs_shortest_path(wolf, player);
    if (path.size() >= 2) {
        Node next_pos = path[1];
        if (game_map->is_valid_move(next_pos)) {
            game_map->move_wolf(next_pos);
            return;
        }
    }

    Node best = {-1, -1};
    int best_dist = INT_MAX;
    for (auto& cell : adj) {
        auto result = game_map->get_graph()->dijkstra(cell);
        auto& dist = result.first;
        int d = dist.count(player) ? dist[player] : INT_MAX;
        if (d < best_dist) {
            best_dist = d;
            best = cell;
        }
    }
    if (best != Node{-1, -1}) {
        game_map->move_wolf(best);
    }
}

pair<bool, string> Game::undo() {
    if (turn == 0 && undo_stack.is_empty()) {
        return {false, "Cannot undo on first turn."};
    }
    if (undo_stack.is_empty()) {
        return {false, "No moves to undo."};
    }
    if (game_over) {
        return {false, "Game is over, cannot undo."};
    }

    GameState state = undo_stack.pop();
    game_map->set_player_pos(state.player_pos);
    game_map->set_wolf_pos(state.wolf_pos);
    game_map->set_grid(state.grid_snapshot);
    game_map->set_traps(state.traps);
    score = state.score;
    turn_score = state.turn_score;
    score = max(0, score + SCORE_UNDO);
    turn = state.turn;
    compute_dijkstra();
    return {true, "Last move undone."};
}

int Game::get_score() { return score; }
bool Game::is_game_over() { return game_over; }
bool Game::is_won() { return won; }

void Game::display() {
    game_map->display();
    cout << "\nScore: " << score << "  |  Turn: " << turn << "  |  Player: " << username << "\n";
    Node suggestion = get_dijkstra_suggestion();
    if (suggestion != Node{-1, -1}) {
        cout << "Dijkstra suggestion: Move to (" << suggestion.first << ", " << suggestion.second << ")\n";
    }
    if (game_over) {
        if (won) {
            cout << " Congratulations! You won!\n";
        } else {
            cout << " You lost!\n";
        }
    }
}