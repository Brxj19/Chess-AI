#include <gtest/gtest.h>
#include "GameState.h"
#include "move.h"

// Test fixture for GameState
class GameStateTest : public ::testing::Test {
protected:
    GameState gs;

    void SetUp() override {
        // Initialize the game state if needed
        gs = GameState();
    }
};

TEST_F(GameStateTest, InitialSetup) {
    // Test initial setup of the board
    EXPECT_EQ(gs.board[0][0], "bR");
    EXPECT_EQ(gs.board[7][7], "wR");
    EXPECT_EQ(gs.whiteToMove, true);
}

TEST_F(GameStateTest, MakeMove) {
    // Test making a move
    Move move = Move(std::make_pair(6, 4), std::make_pair(4, 4), gs.board); // e2 to e4
    gs.makeMove(move);
    EXPECT_EQ(gs.board[4][4], "wp");
    EXPECT_EQ(gs.board[6][4], "--");
    EXPECT_EQ(gs.whiteToMove, false);
}

TEST_F(GameStateTest, UndoMove) {
    // Test undoing a move
    Move move = Move(std::make_pair(6, 4), std::make_pair(4, 4), gs.board); // e2 to e4
    gs.makeMove(move);
    gs.undo_move();
    EXPECT_EQ(gs.board[6][4], "wp");
    EXPECT_EQ(gs.board[4][4], "--");
    EXPECT_EQ(gs.whiteToMove, true);
}

TEST_F(GameStateTest, UpdateCastleRights) {
    // Test updating castling rights
    Move move = Move(std::make_pair(7, 4), std::make_pair(7, 6), gs.board); // e1 to g1 (king move)
    gs.UpdateCastleRights(move);
    EXPECT_FALSE(gs.currentCastlingRights.white_king_side);
    EXPECT_FALSE(gs.currentCastlingRights.white_queen_side);
}

TEST_F(GameStateTest, PrintBoard) {
    // Test printing the board (just ensure it runs without error)
    gs.print_board();
}

TEST_F(GameStateTest, InCheck) {
    // Test inCheck function
    gs.board[0][4] = "--"; // Remove black king for simplicity
    gs.board[7][4] = "wK"; // Place white king on e1
    gs.board[1][4] = "bQ"; // Place black queen on e2
    gs.whitekingLocation = std::make_pair(7, 4);
    gs.blackkingLocation = std::make_pair(0, 4);
    gs.whiteToMove = true;
    EXPECT_TRUE(gs.inCheck());
}

TEST_F(GameStateTest, AllPossibleMoves) {
    // Test all possible moves generation
    std::vector<Move> moves = gs.allPossibleMoves();
    EXPECT_GT(moves.size(), 0); // Ensure there are possible moves
}

TEST_F(GameStateTest, GetValidMoves) {
    // Test valid moves generation
    std::vector<Move> moves = gs.getValidMoves();
    EXPECT_GT(moves.size(), 0); // Ensure there are valid moves
}

TEST_F(GameStateTest, CheckForPinsAndChecks) {
    // Test checking for pins and checks
    auto [inCheck, pins, checks] = gs.checkForPinsAndChecks();
    EXPECT_FALSE(inCheck); // Ensure the king is not in check initially
    EXPECT_EQ(pins.size(), 0); // Ensure there are no pins initially
    EXPECT_EQ(checks.size(), 0); // Ensure there are no checks initially
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}