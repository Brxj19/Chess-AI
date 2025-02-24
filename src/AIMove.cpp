#include<vector>
#include <cstdlib> 
#include <ctime>
#include<string>
#include<algorithm>
#include<random>
#include"AIMove.h"
#include"move.h"
#include<iostream>
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
int counter = 0;
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
Move AIMove::findBestMove(GameState gs, vector<Move> validMoves){
    int score =0;
    std::random_device rd;
    std::mt19937 g(rd());

    // Shuffle the valid moves using the random number generator
    std::vector<Move> shuffledMoves = validMoves;
    std::shuffle(shuffledMoves.begin(), shuffledMoves.end(), g);
    
    //findMoveMinMax(gs,shuffledMoves,DEPTH,gs.whiteToMove);
     int TurnMultiplier;
    if(gs.whiteToMove){
        TurnMultiplier = 1;
    }else{
        TurnMultiplier = -1;
    }
    //findMoveNegamax(gs,shuffledMoves,DEPTH,TurnMultiplier);
    score = findMoveNegamaxAlphaBeta(gs,shuffledMoves,DEPTH,TurnMultiplier,-CHECKMATE,CHECKMATE);
    std::cout<<"No.of counter played:: "<<counter<<" :score"<<score<<std::endl;
    return nextMove;
}

int AIMove::findMoveMinMax(GameState gs , vector<Move> validMoves, int depth, bool whiteToMove){
    counter += 1;
    // if(depth == 0)
    //     return scoreTheMaterial(gs.board);

    // if(whiteToMove){
    //     int maxScore = -CHECKMATE;
    //     for(auto pmove : validMoves){
    //         gs.makeMove(pmove);
    //         vector<Move> nextMoves = gs.getValidMoves();
    //         int score = findMoveMinMax(gs,nextMoves,depth-1,false);
    //         if(score > maxScore){
    //             maxScore = score;
    //             if(depth == DEPTH){
    //                 nextMove = pmove;
    //             }
    //         }
    //         gs.undo_move();
    //     }
    //     return maxScore;
    // }
    // else{
    //     int minScore = CHECKMATE;
    //     for(auto pmove : validMoves){
    //         gs.makeMove(pmove);
    //         vector<Move> nextMoves = gs.getValidMoves();
    //         int score = findMoveMinMax(gs,nextMoves,depth-1,true);

    //         if(score<minScore){
    //             minScore = score;
    //             if(depth == DEPTH){
    //                 nextMove = pmove;
    //             }
    //         }
    //         gs.undo_move();
    //     }
    //     return minScore;
    // }
    if (depth == 0) {
        return scoreBoard(gs);
    }
    int maxScore = -CHECKMATE;
    for (const auto &move : validMoves) {
        gs.makeMove(move);
        int score = -findMoveMinMax(gs, gs.getValidMoves(), depth - 1, !whiteToMove);
        gs.undo_move();
        if (score > maxScore) {
            maxScore = score;
        }
    }
    return maxScore;
}

int AIMove::findMoveNegamax(GameState gs, vector<Move> validMoves, int depth, int turnMultiplier){
    counter += 1;
    // if(depth == 0)
    //     return turnMultiplier * scoreBoard(gs);

    // int maxScore = -CHECKMATE;
    // for(auto pmove : validMoves){
    //     gs.makeMove(pmove);
    //     vector<Move> nextMoves = gs.getValidMoves();
    //     int score = -findMoveNegamax(gs,nextMoves,depth-1,-turnMultiplier);
    //     if(score > maxScore){
    //         maxScore = score;
    //         if(depth == DEPTH){
    //             nextMove = pmove;
    //         }
    //     }
    //     gs.undo_move();
    // }
    // return maxScore;
    if (depth == 0) {
        return turnMultiplier * scoreBoard(gs);
    }
    int maxScore = -CHECKMATE;
    for (const auto &move : validMoves) {
        gs.makeMove(move);
        int score = -findMoveNegamax(gs, gs.getValidMoves(), depth - 1, -turnMultiplier);
        gs.undo_move();
        if (score > maxScore) {
            maxScore = score;
        }
    }
    return maxScore;
}

int AIMove::findMoveNegamaxAlphaBeta(GameState gs, vector<Move> validMoves, int depth, int turnMultiplier, int alpha, int beta){
    counter += 1;
    if(depth == 0)
        return turnMultiplier * scoreBoard(gs);

    //move ordering - implement later
    int maxScore = -CHECKMATE;
    for(auto pmove : validMoves){
        gs.makeMove(pmove);
        vector<Move> nextMoves = gs.getValidMoves();
        int score = -findMoveNegamaxAlphaBeta(gs,nextMoves,depth-1,-turnMultiplier,-beta,-alpha);
        if(score > maxScore){
            maxScore = score;
            if(depth == DEPTH){
                nextMove = pmove;
            }
        }
        gs.undo_move();
        alpha = std::max(alpha,maxScore);
        if(alpha >= beta)
            break;
    }
    return maxScore;
    // if (depth == 0) {
    //     return turnMultiplier * scoreBoard(gs);
    // }
    // int maxScore = -CHECKMATE;
    // for (const auto &move : validMoves) {
    //     gs.makeMove(move);
    //     int score = -findMoveNegamaxAlphaBeta(gs, gs.getValidMoves(), depth - 1, -turnMultiplier, -beta, -alpha);
    //     gs.undo_move();
    //     if (score > maxScore) {
    //         maxScore = score;
    //         if(depth == DEPTH){
    //             nextMove = move;
    //         }
    //     }
    //     if (maxScore > alpha) {
    //         alpha = maxScore;

    //     }
    //     if (alpha >= beta) {
    //         break;
    //     }
    // }
    // return maxScore;
}

