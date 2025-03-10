#ifndef AIMOVE_H
#define AIMOVE_H

#include <vector>
#include<string>
#include "move.h"


class AIMove{
public:
std::map<char,int> pieceScore = {{'K',0},{'Q',10},{'R',5},{'B',3},{'N',3},{'p',1}};
int CHECKMATE = 1000;
int STALEMATE = 0;
int DEPTH = 4;

int scoreTheMaterial(std::vector<std::vector<std::string>> &board);
int scoreBoard(GameState gs);

Move findRandomMove(const std::vector<Move>& validMoves);
Move findBestMove(GameState gs,const std::vector<Move> validMoves);
//Move findBestMoveMinMax(GameState gs,const std::vector<Move> validMoves);
int findMoveMinMax(GameState gs,const std::vector<Move> validMoves,int depth, bool whiteToMove);
int findMoveNegamax(GameState gs,const std::vector<Move> validMoves,int depth, int turnMultiplier);
int findMoveNegamaxAlphaBeta(GameState gs,const std::vector<Move> validMoves,int depth, int turnMultiplier,int alpha,int beta);
};



#endif // AIMOVE_H
