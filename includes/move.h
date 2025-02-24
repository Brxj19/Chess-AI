#ifndef MOVE_H
#define MOVE_H

#include <map>
#include <string>
#include <vector>
#include "GameState.h"

class Gamestate;
class Move {
public:
    int startRow;
    int startCol;
    int endRow;
    int endCol;
    std::string pieceMoved;
    std::string pieceCaptured;
    int moveID;
    bool isPawnPromotion;
    bool isEnpassant;
    bool castle;
    bool castle_done;
   

    Move(std::pair<int, int> startSq, std::pair<int, int> endSq, std::vector<std::vector<std::string>> board, bool isEnpassant = false,bool isPawnPromotion=false,bool castle = false);

    static std::map<std::string, int> ranksToRows;
    static std::map<int, std::string> rowsToRanks;
    static std::map<std::string, int> filesToCols;
    static std::map<int, std::string> colsToFiles;

    std::string getChessNotation();
    std::string getRanksFiles(int row, int col);
    bool operator==(const Move& other) const; 
};

#endif // MOVE_H
