#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <string>
#include "move.h"
#include "castlingRights.h"

using namespace std;

class Move;
class CastlingRights;

class GameState {
public:
    vector<vector<string>> board = {
        {"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
        {"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp"},
        {"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"}
    };
    
    bool whiteToMove = true;
    std::vector<Move> movelogs = {};
    std::pair<int, int> whitekingLocation = {7, 4};
    std::pair<int, int> blackkingLocation = {0, 4};
    
    bool incheck = false;
    std::vector<std::tuple<int, int, int, int>> pins;  // squares where the allied pinned piece is and direction pinned from
    std::vector<std::tuple<int, int, int, int>> checks;  // squares where enemy is applying a check
    bool checkMate = false;
    bool staleMate = false;
    
    std::tuple<bool, std::vector<std::tuple<int, int, int, int>>, std::vector<std::tuple<int, int, int, int>>> checkStatus;
    std::pair<int, int> enPassantPossible = {};
    std::vector<std::pair<int, int>> enPassantPossibleLogs = {enPassantPossible};
    CastlingRights currentCastlingRights{true, true, true, true};
    std::vector<CastlingRights> castlingRightsLogs = {CastlingRights(this->currentCastlingRights.white_king_side,this->currentCastlingRights.white_queen_side,
        this->currentCastlingRights.black_king_side,this->currentCastlingRights.black_queen_side)};


    void makeMove(Move move);
    void undo_move();
    void UpdateCastleRights(Move move);
    void print_board();
    std::vector<Move> getValidMoves(); // all moves considering checks
    std::vector<Move> allPossibleMoves(); // all moves without considering checks
    void getPawnMoves(int row, int col, std::vector<Move> &moves);
    void getRookMoves(int row, int col, std::vector<Move> &moves);
    void getKnightMoves(int row, int col, std::vector<Move> &moves);
    void getBishopMoves(int row, int col, std::vector<Move> &moves);
    void getQueenMoves(int row, int col, std::vector<Move> &moves);
    void getKingMoves(int row, int col, std::vector<Move> &moves);
    void getCastleMoves(int row,int col,std::vector<Move> &moves);
    void getKingSideCastle(int row, int col,std::vector<Move> &moves);
    void getQueenSideCastle(int row, int col,std::vector<Move> &moves);
    bool inCheck();
    bool squareUnderAttack(int row, int col);
    std::tuple<bool, std::vector<std::tuple<int, int, int, int>>, std::vector<std::tuple<int, int, int, int>>> checkForPinsAndChecks();
    std::vector<int> range(int start, int end, int step = 1);
    friend class Move;
    friend class CastlingRights;
};

#endif // GAMESTATE_H
