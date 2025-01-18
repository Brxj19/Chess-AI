#include<vector>
#include <cstdlib> 
#include <ctime>
#include<string>
#include<algorithm>
#include<random>
#include"AIMove.h"
#include"move.h"
class Move; 
Move nextMove(std::make_pair(0,0),std::make_pair(0,0),{
        {"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
        {"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"--", "--", "--", "--", "--", "--", "--", "--"},
        {"wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp"},
        {"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"}
    });

Move AIMove::findRandomMove(const std::vector<Move> &validMoves) {
    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Generate a random index within the range of validMoves
    int randomIndex = std::rand() % validMoves.size();

    // Return the move at the random index
    return validMoves[randomIndex];
}

int AIMove::scoreTheMaterial(std::vector<std::vector<std::string>> &board){
    int score = 0;
    for(auto row : board){
        for(auto square : row){
            if(square[0] == 'w'){
                score += this->pieceScore[square[1]];
            }
            else if(square[0] == 'b'){
                score -= this->pieceScore[square[1]];
            }
        }
    }
    return score;
}

Move AIMove::findBestMove(GameState gs,const std::vector<Move> validMoves){
    int TurnMultiplier;
    if(gs.whiteToMove){
        TurnMultiplier = 1;
    }else{
        TurnMultiplier = -1;
    }
    int opponentMinMaxScore = this->CHECKMATE;
    int score;
    Move bestMove(std::make_pair(0,0),std::make_pair(0,0),gs.board);
    std::random_device rd;
    std::mt19937 g(rd());

    // Shuffle the valid moves using the random number generator
    std::vector<Move> shuffledMoves = validMoves;
    std::shuffle(shuffledMoves.begin(), shuffledMoves.end(), g);
    int opponentMaxScore;
    for(auto playerMove :shuffledMoves){
        gs.makeMove(playerMove);
        std::vector<Move> opponentMoves = gs.getValidMoves();
        if(gs.staleMate){
            opponentMaxScore = STALEMATE;
        }
        else if(gs.checkMate){
            opponentMaxScore = -CHECKMATE;
        }
        else{
            opponentMaxScore = -CHECKMATE;
            for(auto opponentMove : opponentMoves){
                gs.makeMove(opponentMove);
                gs.getValidMoves();
                if(gs.checkMate)
                    score = CHECKMATE;
                else if(gs.staleMate)
                    score = STALEMATE;
                else
                    score = -TurnMultiplier * scoreTheMaterial(gs.board);
                
                if(score > opponentMaxScore){
                    opponentMaxScore = score;
                    //bestMove=playerMove;
                }
                gs.undo_move();
            }
        }
        if(opponentMaxScore < opponentMinMaxScore){
            opponentMinMaxScore = opponentMaxScore;
            bestMove = playerMove;
        }
        gs.undo_move();
    }

    return bestMove;
}

//a positive score is good for white, and negative score is good for black
int AIMove::scoreBoard(GameState gs){
    if (gs.checkMate){
        if(gs.whiteToMove)
            return -CHECKMATE;//blacks wins
        else
            return CHECKMATE;
    }else{
        if(gs.staleMate)
            return STALEMATE;
    }
    int score = 0;
    for(auto row : gs.board){
        for(auto square : row){
            if(square[0] == 'w'){
                score += this->pieceScore[square[1]];
            }
            else if(square[0] == 'b'){
                score -= this->pieceScore[square[1]];
            }
        }
    }
    return score;
}
Move AIMove::findBestMoveMinMax(GameState gs, vector<Move> validMoves){

    std::random_device rd;
    std::mt19937 g(rd());

    // Shuffle the valid moves using the random number generator
    std::vector<Move> shuffledMoves = validMoves;
    std::shuffle(shuffledMoves.begin(), shuffledMoves.end(), g);
    findMoveMinMax(gs,shuffledMoves,DEPTH,gs.whiteToMove);

    return nextMove;
}

int AIMove::findMoveMinMax(GameState gs , vector<Move> validMoves, int depth, bool whiteToMove){
    if(depth == 0)
        return scoreTheMaterial(gs.board);

    if(whiteToMove){
        int maxScore = -CHECKMATE;
        for(auto pmove : validMoves){
            gs.makeMove(pmove);
            vector<Move> nextMoves = gs.getValidMoves();
            int score = findMoveMinMax(gs,nextMoves,depth-1,false);
            if(score > maxScore){
                maxScore = score;
                if(depth == DEPTH){
                    nextMove = pmove;
                }
            }
            gs.undo_move();
        }
        return maxScore;
    }
    else{
        int minScore = CHECKMATE;
        for(auto pmove : validMoves){
            gs.makeMove(pmove);
            vector<Move> nextMoves = gs.getValidMoves();
            int score = findMoveMinMax(gs,nextMoves,depth-1,true);

            if(score<minScore){
                minScore = score;
                if(depth == DEPTH){
                    nextMove = pmove;
                }
            }
            gs.undo_move();
        }
        return minScore;
    }
}