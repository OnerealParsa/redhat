#include "game_map.h"
#include "game.h"
#include "user_manager.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Menu {
private:
    UserManager user_manager;
    string current_user;

    void show_main_menu() {
        while (true) {
            cout << "\n";
            cout << "========================================\n";
            cout << "   HoodQuest: The Algorithm Forest\n";
            cout << "========================================\n\n";
            cout << "1. Create new account\n";
            cout << "2. Login to existing account\n";
            cout << "3. Exit\n";
            cout << "\nChoose: ";

            string choice;
            getline(cin, choice);

            if (choice == "1") create_account();
            else if (choice == "2") login();
            else if (choice == "3") {
                cout << "Goodbye!\n";
                break;
            } else {
                cout << "Invalid choice.\n";
            }
        }
    }

    void create_account() {
        cout << "\n--- Create New Account ---\n";
        cout << "Username: ";
        string username;
        getline(cin, username);
        if (username.empty()) {
            cout << "Username cannot be empty.\n";
            return;
        }
        cout << "Password: ";
        string password;
        getline(cin, password);
        if (password.empty()) {
            cout << "Password cannot be empty.\n";
            return;
        }
        auto result = user_manager.create_user(username, password);
        cout << result.second << "\n";
        if (result.first) {
            current_user = username;
            game_menu();
        }
    }

    void login() {
        cout << "\n--- Login ---\n";
        cout << "Username: ";
        string username;
        getline(cin, username);
        if (username.empty()) {
            cout << "Username cannot be empty.\n";
            return;
        }
        cout << "Password: ";
        string password;
        getline(cin, password);
        auto result = user_manager.authenticate(username, password);
        cout << result.second << "\n";
        if (result.first) {
            current_user = username;
            game_menu();
        }
    }

    void game_menu() {
        while (true) {
            cout << "\n--- Game Menu (User: " << current_user << ") ---\n";
            cout << "1. Start new game\n";
            cout << "2. Show top player\n";
            cout << "3. Tutorial\n";
            cout << "4. Logout\n";
            cout << "\nChoose: ";

            string choice;
            getline(cin, choice);

            if (choice == "1") start_game();
            else if (choice == "2") show_top_player();
            else if (choice == "3") show_tutorial();
            else if (choice == "4") {
                current_user = "";
                cout << "Logged out.\n";
                break;
            } else {
                cout << "Invalid choice.\n";
            }
        }
    }

    void start_game() {
        GameMap* gm = new GameMap(10, 10);
        Game* game = new Game(gm, current_user);

        cout << "\nGame started!\n";
        cout << "R = You (Red)  |  G = Wolf  |  V = Home  |  T = Trap  |  # = Wall  |  . = Empty\n";
        cout << "Move by entering coordinates (row col) e.g: 1 0\n";
        cout << "Type 'undo' to undo last turn\n";
        cout << "Type 'quit' to exit game\n\n";

        game->display();

        while (!game->is_game_over()) {
            Node pp = gm->get_player_pos();
            Node wp = gm->get_wolf_pos();
            Node hp = gm->get_home_pos();
            cout << "\nYour position: (" << pp.first << "," << pp.second << ")  |  Wolf: (" << wp.first << "," << wp.second << ")  |  Home: (" << hp.first << "," << hp.second << ")\n";
            Node suggestion = game->get_dijkstra_suggestion();
            if (suggestion != Node{-1, -1}) {
                cout << "Dijkstra suggestion: (" << suggestion.first << "," << suggestion.second << ")\n";
            }

            cout << "Your move: ";
            string cmd;
            getline(cin, cmd);

            if (cmd == "quit") {
                cout << "Exited game.\n";
                break;
            } else if (cmd == "undo") {
                auto result = game->undo();
                cout << result.second << "\n";
                game->display();
                continue;
            }

            istringstream iss(cmd);
            int r, c;
            if (!(iss >> r >> c)) {
                cout << "Invalid input. Example: 1 0 or 'undo' or 'quit'\n";
                continue;
            }

            auto result = game->player_move({r, c});
            cout << result.second << "\n";
            game->display();
        }

        if (game->is_game_over()) {
            int final_score = game->get_score();
            int current_score = user_manager.get_user_score(current_user);
            if (current_score < 0) current_score = 0;
            if (final_score > current_score) {
                user_manager.update_score(current_user, final_score);
            }
            cout << "\nFinal Score: " << final_score << "\n";
        }

        delete game;
        delete gm;
    }

    void show_top_player() {
        auto top = user_manager.get_top_player();
        if (top.first.empty()) {
            cout << "\nNo players registered yet.\n";
        } else {
            cout << "\nTop Player: " << top.first << " with score " << top.second << "\n";
        }
    }

    void show_tutorial() {
        cout << "\n=== Game Tutorial ===\n\n";
        cout << "You are Little Red Riding Hood (R) lost in a forest.\n";
        cout << "Goal: Reach Grandma's house (V) without being caught by the Wolf (G).\n\n";
        cout << "Rules:\n";
        cout << "- Move one cell per turn (up/down/left/right).\n";
        cout << "- Wolf rolls a 6-sided die after your move.\n";
        cout << "- If the die is even, the Wolf moves toward you using BFS.\n";
        cout << "- If the Wolf lands on you, you lose.\n";
        cout << "- If you reach V, you win!\n\n";
        cout << "Scoring:\n";
        cout << "- Move suggested by Dijkstra: +3\n";
        cout << "- Move not suggested: +1\n";
        cout << "- Undo: -2\n";
        cout << "- Reach home: +5\n";
        cout << "- Bonus for winning: +5\n\n";
        cout << "Traps (T):\n";
        cout << "- Stepping on a trap zeroes the score earned in that turn.\n\n";
        cout << "Undo:\n";
        cout << "- Revert your last turn using a Stack.\n";
        cout << "- Undo only works for your own turns.\n\n";
        cout << "Pathfinding:\n";
        cout << "- Dijkstra: finds shortest path to Home.\n";
        cout << "- BFS: finds shortest path for Wolf to you.\n";
        cout << "- A*: also available for pathfinding.\n";
    }

public:
    void run() {
        show_main_menu();
    }
};

int main() {
    Menu menu;
    menu.run();
    return 0;
}