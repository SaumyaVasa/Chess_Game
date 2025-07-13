#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <set>
#include <iomanip>
#include <limits>

using namespace std;

enum Color { WHITE, BLACK };

// Base Piece class
class Piece {
public:
    Color color;
    string name;
    string symbol;
    bool hasMoved;
    int value;

    Piece(Color c, string n, string s, int v) : color(c), name(n), symbol(s), hasMoved(false), value(v) {}
    virtual ~Piece() {}
    virtual bool isValidMove(pair<int,int> from, pair<int,int> to, Piece* board[8][8]) = 0;
    virtual vector<pair<int,int>> getPossibleMoves(pair<int,int> pos, Piece* board[8][8]) = 0;
    virtual string getType() = 0;
};

// Pawn class
class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, c == WHITE ? "WP" : "BP", c == WHITE ? "P" : "p", 1) {}
    
    bool isValidMove(pair<int,int> from, pair<int,int> to, Piece* board[8][8]) override {
        int fromRow = from.first, fromCol = from.second;
        int toRow = to.first, toCol = to.second;
        int direction = (color == WHITE) ? -1 : 1;
        
        // Forward move
        if (fromCol == toCol) {
            if (toRow == fromRow + direction && board[toRow][toCol] == nullptr) {
                return true;
            }
            // Double move on first move
            if (!hasMoved && toRow == fromRow + 2 * direction && 
                board[toRow][toCol] == nullptr && 
                board[fromRow + direction][fromCol] == nullptr) {
                return true;
            }
        }
        // Diagonal capture
        else if (abs(fromCol - toCol) == 1 && toRow == fromRow + direction) {
            if (board[toRow][toCol] != nullptr && board[toRow][toCol]->color != color) {
                return true;
            }
        }
        return false;
    }
    
    vector<pair<int,int>> getPossibleMoves(pair<int,int> pos, Piece* board[8][8]) override {
        vector<pair<int,int>> moves;
        int row = pos.first, col = pos.second;
        int direction = (color == WHITE) ? -1 : 1;
        
        // Forward moves
        if (row + direction >= 0 && row + direction < 8 && board[row + direction][col] == nullptr) {
            moves.push_back({row + direction, col});
            if (!hasMoved && row + 2 * direction >= 0 && row + 2 * direction < 8 && 
                board[row + 2 * direction][col] == nullptr) {
                moves.push_back({row + 2 * direction, col});
            }
        }
        
        // Diagonal captures
        for (int c : {col - 1, col + 1}) {
            if (c >= 0 && c < 8 && row + direction >= 0 && row + direction < 8) {
                if (board[row + direction][c] != nullptr && board[row + direction][c]->color != color) {
                    moves.push_back({row + direction, c});
                }
            }
        }
        
        return moves;
    }
    
    string getType() override { return "Pawn"; }
};

// Knight class - Fixed naming to avoid confusion with King
class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, c == WHITE ? "WN" : "BN", c == WHITE ? "N" : "n", 3) {}
    
    bool isValidMove(pair<int,int> from, pair<int,int> to, Piece* board[8][8]) override {
        int rowDiff = abs(from.first - to.first);
        int colDiff = abs(from.second - to.second);
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
    
    vector<pair<int,int>> getPossibleMoves(pair<int,int> pos, Piece* board[8][8]) override {
        vector<pair<int,int>> moves;
        int row = pos.first, col = pos.second;
        int knightMoves[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
        
        for (int i = 0; i < 8; i++) {
            int newRow = row + knightMoves[i][0];
            int newCol = col + knightMoves[i][1];
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol] == nullptr || board[newRow][newCol]->color != color) {
                    moves.push_back({newRow, newCol});
                }
            }
        }
        return moves;
    }
    
    string getType() override { return "Knight"; }
};

// Bishop class
class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, c == WHITE ? "WB" : "BB", c == WHITE ? "B" : "b", 3) {}
    
    bool isValidMove(pair<int,int> from, pair<int,int> to, Piece* board[8][8]) override {
        int rowDiff = abs(from.first - to.first);
        int colDiff = abs(from.second - to.second);
        return rowDiff == colDiff && rowDiff > 0;
    }
    
    vector<pair<int,int>> getPossibleMoves(pair<int,int> pos, Piece* board[8][8]) override {
        vector<pair<int,int>> moves;
        int row = pos.first, col = pos.second;
        int directions[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};
        
        for (int i = 0; i < 4; i++) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol] == nullptr) {
                    moves.push_back({newRow, newCol});
                } else {
                    if (board[newRow][newCol]->color != color) {
                        moves.push_back({newRow, newCol});
                    }
                    break;
                }
                newRow += directions[i][0];
                newCol += directions[i][1];
            }
        }
        return moves;
    }
    
    string getType() override { return "Bishop"; }
};

// Rook class
class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, c == WHITE ? "WR" : "BR", c == WHITE ? "R" : "r", 5) {}
    
    bool isValidMove(pair<int,int> from, pair<int,int> to, Piece* board[8][8]) override {
        return (from.first == to.first && from.second != to.second) || 
               (from.second == to.second && from.first != to.first);
    }
    
    vector<pair<int,int>> getPossibleMoves(pair<int,int> pos, Piece* board[8][8]) override {
        vector<pair<int,int>> moves;
        int row = pos.first, col = pos.second;
        int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        
        for (int i = 0; i < 4; i++) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol] == nullptr) {
                    moves.push_back({newRow, newCol});
                } else {
                    if (board[newRow][newCol]->color != color) {
                        moves.push_back({newRow, newCol});
                    }
                    break;
                }
                newRow += directions[i][0];
                newCol += directions[i][1];
            }
        }
        return moves;
    }
    
    string getType() override { return "Rook"; }
};

// Queen class
class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, c == WHITE ? "WQ" : "BQ", c == WHITE ? "Q" : "q", 9) {}
    
    bool isValidMove(pair<int,int> from, pair<int,int> to, Piece* board[8][8]) override {
        int rowDiff = abs(from.first - to.first);
        int colDiff = abs(from.second - to.second);
        return (from.first == to.first && from.second != to.second) || 
               (from.second == to.second && from.first != to.first) ||
               (rowDiff == colDiff && rowDiff > 0);
    }
    
    vector<pair<int,int>> getPossibleMoves(pair<int,int> pos, Piece* board[8][8]) override {
        vector<pair<int,int>> moves;
        int row = pos.first, col = pos.second;
        int directions[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
        
        for (int i = 0; i < 8; i++) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol] == nullptr) {
                    moves.push_back({newRow, newCol});
                } else {
                    if (board[newRow][newCol]->color != color) {
                        moves.push_back({newRow, newCol});
                    }
                    break;
                }
                newRow += directions[i][0];
                newCol += directions[i][1];
            }
        }
        return moves;
    }
    
    string getType() override { return "Queen"; }
};

// King class
class King : public Piece {
public:
    King(Color c) : Piece(c, c == WHITE ? "WKG" : "BKG", c == WHITE ? "K" : "k", 0) {}
    
    bool isValidMove(pair<int,int> from, pair<int,int> to, Piece* board[8][8]) override {
        int rowDiff = abs(from.first - to.first);
        int colDiff = abs(from.second - to.second);
        return rowDiff <= 1 && colDiff <= 1 && (rowDiff != 0 || colDiff != 0);
    }
    
    vector<pair<int,int>> getPossibleMoves(pair<int,int> pos, Piece* board[8][8]) override {
        vector<pair<int,int>> moves;
        int row = pos.first, col = pos.second;
        int directions[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
        
        for (int i = 0; i < 8; i++) {
            int newRow = row + directions[i][0];
            int newCol = col + directions[i][1];
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                if (board[newRow][newCol] == nullptr || board[newRow][newCol]->color != color) {
                    moves.push_back({newRow, newCol});
                }
            }
        }
        return moves;
    }
    
    string getType() override { return "King"; }
};

// Game class
class Game {
private:
    Piece* board[8][8];
    Color currentTurn;
    map<string, pair<int,int>> piecePositions;
    vector<string> moveHistory;
    int movesSinceCapture;
    bool whiteKingMoved, blackKingMoved;
    bool whiteRookLeftMoved, whiteRookRightMoved;
    bool blackRookLeftMoved, blackRookRightMoved;
    
public:
    Game() : currentTurn(WHITE), movesSinceCapture(0), 
             whiteKingMoved(false), blackKingMoved(false),
             whiteRookLeftMoved(false), whiteRookRightMoved(false),
             blackRookLeftMoved(false), blackRookRightMoved(false) {
        initializeBoard();
    }
    
    ~Game() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                delete board[i][j];
            }
        }
    }
    
    void initializeBoard() {
        // Initialize empty board
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = nullptr;
            }
        }
        
        // Place pieces (Black pieces at top, White pieces at bottom)
        board[0][0] = new Rook(BLACK);
        board[0][1] = new Knight(BLACK);
        board[0][2] = new Bishop(BLACK);
        board[0][3] = new Queen(BLACK);
        board[0][4] = new King(BLACK);
        board[0][5] = new Bishop(BLACK);
        board[0][6] = new Knight(BLACK);
        board[0][7] = new Rook(BLACK);
        
        for (int i = 0; i < 8; i++) {
            board[1][i] = new Pawn(BLACK);
        }
        
        for (int i = 0; i < 8; i++) {
            board[6][i] = new Pawn(WHITE);
        }
        
        board[7][0] = new Rook(WHITE);
        board[7][1] = new Knight(WHITE);
        board[7][2] = new Bishop(WHITE);
        board[7][3] = new Queen(WHITE);
        board[7][4] = new King(WHITE);
        board[7][5] = new Bishop(WHITE);
        board[7][6] = new Knight(WHITE);
        board[7][7] = new Rook(WHITE);
        
        // Assign numbered names to pieces
        assignPieceNumbers();
        updatePiecePositions();
    }
    
    void assignPieceNumbers() {
        // Assign numbers to pawns
        int whitePawnCount = 1, blackPawnCount = 1;
        int whiteRookCount = 1, blackRookCount = 1;
        int whiteKnightCount = 1, blackKnightCount = 1;
        int whiteBishopCount = 1, blackBishopCount = 1;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr) {
                    string pieceType = board[i][j]->getType();
                    if (pieceType == "Pawn") {
                        if (board[i][j]->color == WHITE) {
                            board[i][j]->name = "WP" + to_string(whitePawnCount++);
                        } else {
                            board[i][j]->name = "BP" + to_string(blackPawnCount++);
                        }
                    } else if (pieceType == "Rook") {
                        if (board[i][j]->color == WHITE) {
                            board[i][j]->name = "WR" + to_string(whiteRookCount++);
                        } else {
                            board[i][j]->name = "BR" + to_string(blackRookCount++);
                        }
                    } else if (pieceType == "Knight") {
                        if (board[i][j]->color == WHITE) {
                            board[i][j]->name = "WN" + to_string(whiteKnightCount++);
                        } else {
                            board[i][j]->name = "BN" + to_string(blackKnightCount++);
                        }
                    } else if (pieceType == "Bishop") {
                        if (board[i][j]->color == WHITE) {
                            board[i][j]->name = "WB" + to_string(whiteBishopCount++);
                        } else {
                            board[i][j]->name = "BB" + to_string(blackBishopCount++);
                        }
                    }
                    // Queen and King keep their original names (WQ, BQ, WKG, BKG)
                }
            }
        }
    }
    
    void updatePiecePositions() {
        piecePositions.clear();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr) {
                    piecePositions[board[i][j]->name] = {i, j};
                }
            }
        }
    }
    
    void printBoard() {
        cout << "\n";
        cout << "  +-----+-----+-----+-----+-----+-----+-----+-----+\n";
        for (int i = 0; i < 8; i++) {
            cout << (8 - i) << " |";
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr) {
                    cout << setw(4) << board[i][j]->name << " |";
                } else {
                    cout << "     |";
                }
            }
            cout << "\n";
            cout << "  +-----+-----+-----+-----+-----+-----+-----+-----+\n";
        }
        cout << "     a     b     c     d     e     f     g     h\n\n";
    }
    
    void showAlivePieces(Color color) {
        cout << "\n*** " << (color == WHITE ? "White's" : "Black's") << " alive pieces: ***\n";
        bool first = true;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr && board[i][j]->color == color) {
                    if (!first) cout << ", ";
                    cout << board[i][j]->name;
                    first = false;
                }
            }
        }
        cout << "\n";
    }
    
    bool isPathClear(pair<int,int> from, pair<int,int> to) {
        int rowDiff = to.first - from.first;
        int colDiff = to.second - from.second;
        
        int rowDir = (rowDiff == 0) ? 0 : (rowDiff > 0) ? 1 : -1;
        int colDir = (colDiff == 0) ? 0 : (colDiff > 0) ? 1 : -1;
        
        int currentRow = from.first + rowDir;
        int currentCol = from.second + colDir;
        
        while (currentRow != to.first || currentCol != to.second) {
            if (board[currentRow][currentCol] != nullptr) {
                return false;
            }
            currentRow += rowDir;
            currentCol += colDir;
        }
        
        return true;
    }
    
    bool isInCheck(Color color) {
        pair<int,int> kingPos = findKing(color);
        if (kingPos.first == -1) return false;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr && board[i][j]->color != color) {
                    if (board[i][j]->isValidMove({i, j}, kingPos, board) &&
                        (board[i][j]->getType() == "Knight" || isPathClear({i, j}, kingPos))) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    pair<int,int> findKing(Color color) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr && 
                    board[i][j]->getType() == "King" && 
                    board[i][j]->color == color) {
                    return {i, j};
                }
            }
        }
        return {-1, -1};
    }
    
    bool wouldBeInCheck(pair<int,int> from, pair<int,int> to, Color color) {
        Piece* temp = board[to.first][to.second];
        board[to.first][to.second] = board[from.first][from.second];
        board[from.first][from.second] = nullptr;
        
        bool inCheck = isInCheck(color);
        
        board[from.first][from.second] = board[to.first][to.second];
        board[to.first][to.second] = temp;
        
        return inCheck;
    }
    
    bool isCheckmate(Color color) {
        if (!isInCheck(color)) return false;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr && board[i][j]->color == color) {
                    vector<pair<int,int>> moves = board[i][j]->getPossibleMoves({i, j}, board);
                    for (auto move : moves) {
                        if (!wouldBeInCheck({i, j}, move, color)) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    
    bool isStalemate(Color color) {
        if (isInCheck(color)) return false;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] != nullptr && board[i][j]->color == color) {
                    vector<pair<int,int>> moves = board[i][j]->getPossibleMoves({i, j}, board);
                    for (auto move : moves) {
                        if (!wouldBeInCheck({i, j}, move, color)) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    
    bool isValidMove(pair<int,int> from, pair<int,int> to) {
        if (from.first < 0 || from.first >= 8 || from.second < 0 || from.second >= 8 ||
            to.first < 0 || to.first >= 8 || to.second < 0 || to.second >= 8) {
            return false;
        }
        
        Piece* piece = board[from.first][from.second];
        if (piece == nullptr) return false;
        
        if (board[to.first][to.second] != nullptr && 
            board[to.first][to.second]->color == piece->color) {
            return false;
        }
        
        if (!piece->isValidMove(from, to, board)) {
            return false;
        }
        
        // FIXED: Only check path for sliding pieces, not knights
        if (piece->getType() != "Knight" && !isPathClear(from, to)) {
            return false;
        }
        
        return true;
    }
    
    string expandDirection(string shortDir) {
        map<string, string> directionMap = {
            {"U", "up"}, {"D", "down"}, {"L", "left"}, {"R", "right"},
            {"SLU", "slantleftup"}, {"SLD", "slantleftdown"},
            {"SRU", "slantrightup"}, {"SRD", "slantrightdown"},
            {"UL", "upleft"}, {"UR", "upright"}, {"LU", "leftup"}, {"LD", "leftdown"},
            {"RU", "rightup"}, {"RD", "rightdown"}, {"DL", "downleft"}, {"DR", "downright"},
            {"CL", "castle-left"}, {"CR", "castle-right"}
        };
        
        auto it = directionMap.find(shortDir);
        return (it != directionMap.end()) ? it->second : shortDir;
    }
    
    pair<int,int> parseDirection(string direction, pair<int,int> from, int steps) {
        pair<int,int> to = from;
        
        if (direction == "up") {
            to.first -= steps;
        } else if (direction == "down") {
            to.first += steps;
        } else if (direction == "left") {
            to.second -= steps;
        } else if (direction == "right") {
            to.second += steps;
        } else if (direction == "slantrightup") {
            to.first -= steps; to.second += steps;
        } else if (direction == "slantleftup") {
            to.first -= steps; to.second -= steps;
        } else if (direction == "slantrightdown") {
            to.first += steps; to.second += steps;
        } else if (direction == "slantleftdown") {
            to.first += steps; to.second -= steps;
        } else if (direction == "upleft") {
            to.first -= 2; to.second--;
        } else if (direction == "upright") {
            to.first -= 2; to.second++;
        } else if (direction == "leftup") {
            to.first--; to.second -= 2;
        } else if (direction == "leftdown") {
            to.first++; to.second -= 2;
        } else if (direction == "rightup") {
            to.first--; to.second += 2;
        } else if (direction == "rightdown") {
            to.first++; to.second += 2;
        } else if (direction == "downleft") {
            to.first += 2; to.second--;
        } else if (direction == "downright") {
            to.first += 2; to.second++;
        }
        
        return to;
    }
    
    bool canCastle(Color color, bool kingside) {
        if (color == WHITE) {
            if (whiteKingMoved) return false;
            if (kingside && whiteRookRightMoved) return false;
            if (!kingside && whiteRookLeftMoved) return false;
            
            int rookCol = kingside ? 7 : 0;
            if (board[7][rookCol] == nullptr || board[7][rookCol]->getType() != "Rook") return false;
            
            // Check if path is clear
            int start = kingside ? 5 : 1;
            int end = kingside ? 6 : 3;
            for (int i = start; i <= end; i++) {
                if (board[7][i] != nullptr) return false;
            }
            
            // Check if king would be in check during castling
            for (int i = 4; i <= (kingside ? 6 : 2); i++) {
                if (wouldBeInCheck({7, 4}, {7, i}, color)) return false;
            }
        } else {
            if (blackKingMoved) return false;
            if (kingside && blackRookRightMoved) return false;
            if (!kingside && blackRookLeftMoved) return false;
            
            int rookCol = kingside ? 7 : 0;
            if (board[0][rookCol] == nullptr || board[0][rookCol]->getType() != "Rook") return false;
            
            // Check if path is clear
            int start = kingside ? 5 : 1;
            int end = kingside ? 6 : 3;
            for (int i = start; i <= end; i++) {
                if (board[0][i] != nullptr) return false;
            }
            
            // Check if king would be in check during castling
            for (int i = 4; i <= (kingside ? 6 : 2); i++) {
                if (wouldBeInCheck({0, 4}, {0, i}, color)) return false;
            }
        }
        
        return true;
    }

    bool makeMove(string pieceCode, string direction, int steps) {
        if (piecePositions.find(pieceCode) == piecePositions.end()) {
            cout << "\n*** ERROR: Piece '" << pieceCode << "' not found! ***\n";
            return false;
        }
        
        pair<int,int> from = piecePositions[pieceCode];
        Piece* piece = board[from.first][from.second];
        
        if (piece->color != currentTurn) {
            cout << "\n*** ERROR: It's not your turn! ***\n";
            return false;
        }
        
        pair<int,int> to;
        
        // Handle castling
        if (piece->getType() == "King" && (direction == "castle-left" || direction == "castle-right")) {
            bool kingside = (direction == "castle-right");
            if (!canCastle(currentTurn, kingside)) {
                cout << "\n*** ERROR: Cannot castle! ***\n";
                return false;
            }
            
            if (currentTurn == WHITE) {
                to = {7, kingside ? 6 : 2};
                // Move rook first
                if (kingside) {
                    board[7][5] = board[7][7];
                    board[7][7] = nullptr;
                } else {
                    board[7][3] = board[7][0];
                    board[7][0] = nullptr;
                }
            } else {
                to = {0, kingside ? 6 : 2};
                // Move rook first
                if (kingside) {
                    board[0][5] = board[0][7];
                    board[0][7] = nullptr;
                } else {
                    board[0][3] = board[0][0];
                    board[0][0] = nullptr;
                }
            }
            
            // Move king to castling position
            board[to.first][to.second] = piece;
            board[from.first][from.second] = nullptr;
            piece->hasMoved = true;
            
            // Update castling flags
            if (currentTurn == WHITE) {
                whiteKingMoved = true;
                if (kingside) whiteRookRightMoved = true;
                else whiteRookLeftMoved = true;
            } else {
                blackKingMoved = true;
                if (kingside) blackRookRightMoved = true;
                else blackRookLeftMoved = true;
            }
            
            updatePiecePositions();
            
            // Check for check/checkmate
            Color oppositeColor = (currentTurn == WHITE) ? BLACK : WHITE;
            if (isInCheck(oppositeColor)) {
                cout << "\n*** CHECK! ***\n";
                if (isCheckmate(oppositeColor)) {
                    cout << "\n*** CHECKMATE! " << (currentTurn == WHITE ? "White" : "Black") << " wins! ***\n";
                    return true;
                }
            } else if (isStalemate(oppositeColor)) {
                cout << "\n*** STALEMATE! It's a draw! ***\n";
                return true;
            }
            
            // Check for 50-move rule
            if (movesSinceCapture >= 100) {
                cout << "\n*** DRAW by 50-move rule! ***\n";
                return true;
            }
            
            currentTurn = oppositeColor;
            return false;
        } else {
            to = parseDirection(direction, from, steps);
        }
        
        if (to.first < 0 || to.first >= 8 || to.second < 0 || to.second >= 8) {
            cout << "\n*** ERROR: Invalid move - out of bounds! ***\n";
            return false;
        }
        
        if (!isValidMove(from, to)) {
            cout << "\n*** ERROR: Invalid move! ***\n";
            return false;
        }
        
        // Only check path for sliding pieces, not knights
        if (piece->getType() != "Knight" && !isPathClear(from, to)) {
            cout << "\n*** ERROR: Path blocked! ***\n";
            return false;
        }
        
        if (wouldBeInCheck(from, to, currentTurn)) {
            cout << "\n*** ERROR: Move would leave king in check! ***\n";
            return false;
        }
        
        // Execute move
        Piece* captured = board[to.first][to.second];
        if (captured != nullptr) {
            cout << "\n*** SUCCESS: You captured " << captured->name << " (" << captured->getType() << ")! ***\n";
            movesSinceCapture = 0;
        } else {
            movesSinceCapture++;
        }
        
        board[to.first][to.second] = piece;
        board[from.first][from.second] = nullptr;
        piece->hasMoved = true;
        
        // Update castling flags
        if (piece->getType() == "King") {
            if (currentTurn == WHITE) whiteKingMoved = true;
            else blackKingMoved = true;
        } else if (piece->getType() == "Rook") {
            if (currentTurn == WHITE) {
                if (from.second == 0) whiteRookLeftMoved = true;
                else if (from.second == 7) whiteRookRightMoved = true;
            } else {
                if (from.second == 0) blackRookLeftMoved = true;
                else if (from.second == 7) blackRookRightMoved = true;
            }
        }
        
        updatePiecePositions();
        
        // Check for check/checkmate
        Color oppositeColor = (currentTurn == WHITE) ? BLACK : WHITE;
        if (isInCheck(oppositeColor)) {
            cout << "\n*** CHECK! ***\n";
            if (isCheckmate(oppositeColor)) {
                cout << "\n*** CHECKMATE! " << (currentTurn == WHITE ? "White" : "Black") << " wins! ***\n";
                return true;
            }
        } else if (isStalemate(oppositeColor)) {
            cout << "\n*** STALEMATE! It's a draw! ***\n";
            return true;
        }
        
        // Check for 50-move rule
        if (movesSinceCapture >= 100) {
            cout << "\n*** DRAW by 50-move rule! ***\n";
            return true;
        }
        
        currentTurn = oppositeColor;
        return false;
    }
    
    void play() {
        printBoard();
        
        while (true) {
            cout << "\n*** " << (currentTurn == WHITE ? "White's" : "Black's") << " turn ***\n";
            showAlivePieces(currentTurn);
            
            cout << "\n*** Available directions (short forms): ***\n";
            cout << "For Pawns: U, SLU, SRU\n";
            cout << "For Knights: UL, UR, LU, LD, RU, RD, DL, DR\n";
            cout << "For others: U, D, L, R, SLU, SLD, SRU, SRD\n";
            cout << "For King: CL, CR (in addition to above)\n\n";
            
            string pieceCode, direction;
            int steps;
            
            cout << "Enter piece code (or 'quit' to exit): ";
            cin >> pieceCode;
            
            if (pieceCode == "quit") {
                cout << "\n*** Game ended by user. ***\n";
                break;
            }
            
            // Clear input buffer to prevent infinite loop
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "Enter direction (short form): ";
            cin >> direction;
            
            // Clear input buffer
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            // Expand short direction to full form
            direction = expandDirection(direction);
            
            // For knight moves and castling, steps are not needed
            if (direction == "upleft" || direction == "upright" || 
                direction == "leftup" || direction == "leftdown" ||
                direction == "rightup" || direction == "rightdown" ||
                direction == "downleft" || direction == "downright" ||
                direction == "castle-left" || direction == "castle-right") {
                steps = 1; // Default for knight moves and castling
            } else {
                cout << "Enter number of steps: ";
                cin >> steps;
                
                // Handle invalid input for steps
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n*** ERROR: Please enter a valid number! ***\n";
                    cout << "Enter number of steps: ";
                    cin >> steps;
                }
            }
            
            bool gameOver = makeMove(pieceCode, direction, steps);
            printBoard();
            
            if (gameOver) {
                cout << "\n*** Game Over! ***\n";
                break;
            }
        }
    }
};

int main() {
    cout << "\n*** Welcome to Chess Game! ***\n";
    cout << "Piece codes: WP1-8/BP1-8 (Pawns), WN1-2/BN1-2 (Knights), WB1-2/BB1-2 (Bishops), WR1-2/BR1-2 (Rooks), WQ/BQ (Queen), WKG/BKG (King)\n";
    cout << "Type 'quit' as piece code to exit\n\n";
    
    Game game;
    game.play();
    
    return 0;
}
