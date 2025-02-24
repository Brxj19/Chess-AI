#include <gtest/gtest.h>
#include "AIMove.h"
#include "GameState.h"
#include "move.h"

// Test fixture for AIMove
class AIMoveTest : public ::testing::Test {
protected:
    AIMove aiMove;
    GameState gs;
    // Move nextMove(std::make_pair(0,0),std::make_pair(0,0),{
    //     {"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
    //     {"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp"},
    //     {"--", "--", "--", "--", "--", "--", "--", "--"},
    //     {"--", "--", "--", "--", "--", "--", "--", "--"},
    //     {"--", "--", "--", "--", "--", "--", "--", "--"},
    //     {"--", "--", "--", "--", "--", "--", "--", "--"},
    //     {"wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp"},
    //     {"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"}
    // });

    void SetUp() override {
        // Initialize the game state if needed
        gs = GameState();
    }
};

TEST_F(AIMoveTest, FindRandomMove) {
    // Test finding a random move
    std::vector<Move> validMoves = gs.allPossibleMoves();
    Move randomMove = aiMove.findRandomMove(validMoves);
    EXPECT_NE(std::find(validMoves.begin(), validMoves.end(), randomMove), validMoves.end());
}

TEST_F(AIMoveTest, ScoreTheMaterial) {
    // Test scoring the material on the board
    int score = aiMove.scoreTheMaterial(gs.board);
    EXPECT_EQ(score, 0); // Initial board should have a score of 0
}

TEST_F(AIMoveTest, ScoreBoard) {
    // Test scoring the board
    int score = aiMove.scoreBoard(gs);
    EXPECT_EQ(score, 0); // Initial board should have a score of 0
}

TEST_F(AIMoveTest, FindBestMove) {
    // Test finding the best move
    std::vector<Move> validMoves = gs.getValidMoves();
    Move bestMove = aiMove.findBestMove(gs, validMoves);
    EXPECT_NE(std::find(validMoves.begin(), validMoves.end(), bestMove), validMoves.end());
}

TEST_F(AIMoveTest, FindMoveMinMax) {
    // Test finding the best move using MinMax
    std::vector<Move> validMoves = gs.getValidMoves();
    int score = aiMove.findMoveMinMax(gs, validMoves, aiMove.DEPTH, gs.whiteToMove);
    EXPECT_NE(score, 0); // Ensure the score is not zero
}

TEST_F(AIMoveTest, FindMoveNegamax) {
    // Test finding the best move using Negamax
    std::vector<Move> validMoves = gs.getValidMoves();
    int turnMultiplier = gs.whiteToMove ? 1 : -1;
    int score = aiMove.findMoveNegamax(gs, validMoves, aiMove.DEPTH, turnMultiplier);
    EXPECT_NE(score, 0); // Ensure the score is not zero
}

TEST_F(AIMoveTest, FindMoveNegamaxAlphaBeta) {
    // Test finding the best move using Negamax with Alpha-Beta Pruning
    std::vector<Move> validMoves = gs.getValidMoves();
    int turnMultiplier = gs.whiteToMove ? 1 : -1;
    int score = aiMove.findMoveNegamaxAlphaBeta(gs, validMoves, aiMove.DEPTH, turnMultiplier, -aiMove.CHECKMATE, aiMove.CHECKMATE);
    EXPECT_NE(score, 0); // Ensure the score is not zero
}

