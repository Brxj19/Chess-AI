#include "move.h"
#include "GameState.h"
#include <iostream>

Move::Move(std::pair<int, int> startSq, std::pair<int, int> endSq, std::vector<std::vector<std::string>> board, bool isEnpassant,bool isPawnPromotion ,bool castle)
    : startRow(startSq.first), startCol(startSq.second), endRow(endSq.first), endCol(endSq.second), isEnpassant(isEnpassant),isPawnPromotion(isPawnPromotion),castle(castle) {
    this->startRow = startSq.first;
    this->startCol = startSq.second;
    this->endRow = endSq.first;
    this->endCol = endSq.second;
    this->pieceMoved = board[this->startRow][this->startCol];
    this->pieceCaptured = board[this->endRow][this->endCol];
    this->moveID = this->startRow*1000 + this->startCol*100 + this->endRow*10 + this->endCol;
    this->isPawnPromotion = isPawnPromotion;
    //if((this->pieceMoved == "wp" && this->endRow==0) || (this->pieceMoved == "bp" && this->endRow == 7)){
    //    this->isPawnPromotion =true;
    //}
    this->isEnpassant = isEnpassant;
    this->pieceCaptured = isEnpassant ? (this->pieceMoved[0] == 'w' ? "bp" : "wp") : board[this->endRow][this->endCol];
    this->castle = castle;
    
    
}

// Initialize static members
std::map<std::string, int> Move::ranksToRows = {
    {"1", 7}, {"2", 6}, {"3", 5}, {"4", 4},
    {"5", 3}, {"6", 2}, {"7", 1}, {"8", 0}
};

std::map<int, std::string> Move::rowsToRanks = {
    {7, "1"}, {6, "2"}, {5, "3"}, {4, "4"},
    {3, "5"}, {2, "6"}, {1, "7"}, {0, "8"}
};

std::map<std::string, int> Move::filesToCols = {
    {"a", 0}, {"b", 1}, {"c", 2}, {"d", 3},
    {"e", 4}, {"f", 5}, {"g", 6}, {"h", 7}
};

std::map<int, std::string> Move::colsToFiles = {
    {0, "a"}, {1, "b"}, {2, "c"}, {3, "d"},
    {4, "e"}, {5, "f"}, {6, "g"}, {7, "h"}
};

std::string Move::getRanksFiles(int row, int col){
    return this->colsToFiles[col] + this->rowsToRanks[row];
}
std::string Move::getChessNotation(){
    return this->getRanksFiles(this->startRow,this->startCol) + this->getRanksFiles(this->endRow,this->endCol);
}

bool Move::operator==(const Move& other) const {
    return moveID == other.moveID;
}