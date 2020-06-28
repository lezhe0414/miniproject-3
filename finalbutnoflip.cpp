#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>


struct Point {
    int x, y;
    Point() : Point(0, 0) {}
    Point(float x, float y) : x(x), y(y) {}
    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const {
        return !operator==(rhs);
    }
    Point operator+(const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
};

int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::vector<Point> next_valid_spots;

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({ x, y });
    }
}






bool valid_spots(std::array<std::array<int, SIZE>, SIZE> board_, int player_, int i, int j) {

    if (board_[i][j] != 0) return false;

    int x, y, count;
    int oplayer = ((player_ == 1) ? 2 : 1);

    //move up
    x = i - 1;
    y = j;
    count = 0;
    while (x > 0 && board_[x][y] == oplayer) {
        x--;
        count++;
    }
    if (x >= 0 && board_[x][y] == player_ && count > 0) return true;


    //move down
    x = i + 1;
    y = j;
    count = 0;
    while (x < 7 && board_[x][y] == oplayer) {
        x++;
        count++;
    }
    if (x <= 7 && board_[x][y] == player_ && count > 0) return true;

    //move left
    x = i;
    y = j - 1;
    count = 0;
    while (x > 0 && board_[x][y] == oplayer) {
        y--;
        count++;
    }
    if (y >= 0 && board_[x][y] == player_ && count > 0) return true;

    //move right
    x = i;
    y = j + 1;
    count = 0;
    while (y < 7 && board_[x][y] == oplayer) {
        y++;
        count++;
    }
    if (y <= 7 && board_[x][y] == player_ && count > 0) return true;

    //move up left
    x = i - 1;
    y = j - 1;
    count = 0;
    while (x > 0 && y > 0 && board_[x][y] == oplayer) {
        x--;
        y--;
        count++;
    }
    if (x >= 0 && y >= 0 && board_[x][y] == player_ && count > 0) return true;

    //move up right
    x = i - 1;
    y = j + 1;
    count = 0;
    while (x > 0 && y < 7 && board_[x][y] == oplayer) {
        x--;
        y++;
        count++;
    }
    if (x >= 0 && y <= 7 && board_[x][y] == player_ && count > 0) return true;

    //move down left
    x = i + 1;
    y = j - 1;
    count = 0;
    while (x < 7 && y>0 && board_[x][y] == oplayer) {
        x++;
        y--;
        count++;
    }
    if (x <= 7 && y >= 0 && board_[x][y] == player_ && count > 0) return true;

    //move down right
    x = i + 1;
    y = j + 1;
    count = 0;
    while (x < 7 && y < 7 && board_[x][y] == oplayer) {
        x++;
        y++;
        count++;
    }
    if (x <= 7 && y <= 7 && board_[x][y] == player_ && count > 0) return true;

    return false;
}


std::vector<Point> getAllPossibleMoves(std::array<std::array<int, SIZE>, SIZE> board_, int player_) {
    std::vector<Point> result;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (valid_spots(board_, player_, i, j)) {
                result.push_back(Point(i, j));
            }
        }
    }
    return result;
}


int eval_mobility(std::array<std::array<int, SIZE>, SIZE> board_, int player_) {
    int oplayer = (player_ == 1) ? 2 : 1;

    int p_disc = getAllPossibleMoves(board_, player_).size();
    int op_disc = getAllPossibleMoves(board_, oplayer).size();

    return p_disc - op_disc;
}
int eval_disc_diff(std::array<std::array<int, SIZE>, SIZE> board_, int player_) {
    int oplayer = (player_ == 1) ? 2 : 1;

    int p_disc = 0;
    int op_disc = 0;

    for (int i = 0;i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board_[i][j] == player_) {
                p_disc++;
            }
            else if (board_[i][j] == oplayer) {
                op_disc++;
            }
        }
    }

    return p_disc - op_disc;
}
int eval_corner(std::array<std::array<int, SIZE>, SIZE> board_, int player_) {
    int oplayer = (player_ == 1) ? 2 : 1;

    int p_score = 0;
    int op_score = 0;


    if (board_[0][0] == player_) {
        p_score++;
    }
    else if (board_[0][0] == oplayer) {
        op_score++;
    }

    if (board_[0][7] == player_) {
        p_score++;
    }
    else if (board_[0][7] == oplayer) {
        op_score++;
    }

    if (board_[7][0] == player_) {
        p_score++;
    }
    else if (board_[7][0] == oplayer) {
        op_score++;
    }

    if (board_[7][7] == player_) {
        p_score++;
    }
    else if (board_[7][7] == oplayer) {
        op_score++;
    }

    return p_score - op_score;
}
int eval_map(std::array<std::array<int, SIZE>, SIZE> board_, int player_) {
    int oplayer = (player_ == 1) ? 2 : 1;

    int map[8][8] = {
        { 5 , -1,  2,   1,   1,   2,  -1,   5},
        {-1 , -2, -1,  -1,  -1,  -1,  -2,  -1},
        { 2 , -1,  2,   0,   0,   2,  -1,   2},
        { 1 , -1,  0,   0,   0,   0,  -1,   1},
        { 1 , -1,  0,   0,   0,   0,  -1,   1},
        { 2 , -1,  2,   0,   0,   2,  -1,   2},
        {-1 , -2, -1,  -1,  -1,  -1,  -2,  -1},
        { 5 , -1,  2,   1,   1,   2,  -1,   5} };

    if (board_[0][0] != 0) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                map[i][j] = 0;
            }
        }
    }

    if (board_[0][7] != 0) {
        for (int i = 0; i < 3; i++) {
            for (int j = 5; j < 8; j++) {
                map[i][j] = 0;
            }
        }
    }

    if (board_[7][0] != 0) {
        for (int i = 5; i < 8; i++) {
            for (int j = 0; j < 3; j++) {
                map[i][j] = 0;
            }
        }
    }

    if (board_[7][7] != 0) {
        for (int i = 5; i < 8; i++) {
            for (int j = 5; j < 8; j++) {
                map[i][j] = 0;
            }
        }
    }

    int p_score;
    int op_score;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board_[i][j] == player_) {
                p_score += map[i][j];
            }
            else if (board_[i][j] == oplayer) {
                op_score += map[i][j];
            }
        }
    }

    return p_score - op_score;
}


int eval(std::array<std::array<int, SIZE>, SIZE> board_, int player_) {
    int mobility = eval_mobility(board_, player_);
    int disc_diff = eval_disc_diff(board_, player_);
    int corner = eval_corner(board_, player_);
    int map = eval_map(board_, player_);
    int total = 0;
    for (int i = 0;i < 8; i++) {
        for (int j = 0;j < 8;j++) {
            if (board_[i][j] != 0) {
                total++;
            }
        }
    }
    if (total <= 30) {
        return mobility * 50 + disc_diff * 20 + corner * 1000 + map * 30;
    }
    else if (total > 30 && total <= 50) {
        return mobility * 25 + disc_diff * 25 + corner * 1000 + map * 30;
    }
    else {
        return mobility * 20 + disc_diff * 50 + corner * 100 + map * 30;
    }
}



int alpha_beta(std::array<std::array<int, SIZE>, SIZE> board_, int  player_, int  depth, bool max, int alpha, int beta) {
    int oplayer = (player_ == 1) ? 2 : 1;

    if (depth == 0 || ((getAllPossibleMoves(board_, player_)).size() == 0 && (getAllPossibleMoves(board_, oplayer)).size() == 0)) {
        return eval(board_, player_);
    }

    if ((max && (getAllPossibleMoves(board_, player_)).size() == 0) || (!max && (getAllPossibleMoves(board_, oplayer)).size() == 0)) {
        return alpha_beta(board_, player_, depth - 1, !max, alpha, beta);
    }
    int score;
    if (max) {

        score = INT_MIN;
        for (auto move : getAllPossibleMoves(board_, player_)) {


            std::array<std::array<int, SIZE>, SIZE> new_board;
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 8; k++) {
                    new_board[j][k] = board_[j][k];
                }
            }
            new_board[move.x][move.y] = player_;


            int childScore = alpha_beta(new_board, player_, depth - 1, false, alpha, beta);
            if (childScore > score) score = childScore;

            if (score > alpha) alpha = score;
            if (beta <= alpha) break;
        }
    }
    else {

        score = INT_MAX;
        for (Point move : getAllPossibleMoves(board_, oplayer)) {

            std::array<std::array<int, SIZE>, SIZE> new_board;
            for (int j = 0; j < 8; j++) {
                for (int k = 0; k < 8; k++) {
                    new_board[j][k] = board_[j][k];
                }
            }
            new_board[move.x][move.y] = oplayer;
            //recursive call
            int childScore = alpha_beta(new_board, player_, depth - 1, true, alpha, beta);
            if (childScore < score) score = childScore;
            //update beta
            if (score < beta) beta = score;
            if (beta <= alpha) break; //Alpha Cutoff
        }
    }
    return score;
}



int minimax(std::array<std::array<int, SIZE>, SIZE> board_, int player_, int depth) {
    int bestScore = INT_MIN;
    unsigned int i;
    int ans;
    for (i = 0; i < next_valid_spots.size(); i++) {
        std::array<std::array<int, SIZE>, SIZE> newboard;
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                newboard[j][k] = board_[j][k];
            }
        }
        newboard[next_valid_spots[i].x][next_valid_spots[i].y] = player_;

        int childScore = alpha_beta(newboard, player_, depth - 1, false, INT_MIN, INT_MAX);
        if (childScore > bestScore) {
            bestScore = childScore;
            ans = i;
        }
    }
    return ans;
}










void write_valid_spot(std::ofstream& fout) {

    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    int fuck = 0;
    fuck = minimax(board, player, 3);
    Point p = next_valid_spots[fuck];

    // Remember to flush the output to ensure the last action is written to file.
    fout << p.x << " " << p.y << std::endl;
    fout.flush();
}



int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
