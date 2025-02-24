#include<vector>
#include<string>
#include"GameState.h"
#include<iostream>
#include<algorithm>
#include"move.h"
#include"castlingRights.h"
using namespace std;

class Move;
class CastlingRights;
void GameState::makeMove(Move move){
    
    this->board[move.startRow][move.startCol] = "--";
    this->board[move.endRow][move.endCol] = move.pieceMoved;
    
    // if (move.pieceMoved == "wK") {
    //     this->whitekingLocation = std::make_pair(move.endRow, move.endCol);
    //     if (abs(move.endCol - move.startCol) == 2) {
    //         move.castle_done = true;
    //     }
    // } else if (move.pieceMoved == "bK") {
    //     this->blackkingLocation = std::make_pair(move.endRow, move.endCol);
    //     if (abs(move.endCol - move.startCol) == 2) {
    //         move.castle_done = true;
    //     }
    // }
    
    this->movelogs.push_back(move);
    this->whiteToMove = !this->whiteToMove;
    
    if(move.pieceMoved == "wK")
        this->whitekingLocation = std::make_pair(move.endRow,move.endCol);
    if(move.pieceMoved == "bK")
        this->blackkingLocation = std::make_pair(move.endRow,move.endCol);
    
    if(move.isPawnPromotion){
        //cout<<"choose a piece to promot your pawn 'Q','N','R','B' "<<endl;
        //string promotedpiece;
        //cin>>promotedpiece;
        this->board[move.endRow][move.endCol] = std::string(1,move.pieceMoved[0]) +  "Q";
    }
    if (move.isEnpassant) {
        
        this->board[move.startRow][move.endCol] = "--";  // Capturing the pawn via en passant
    }

    if (move.pieceMoved[1] == 'p' && abs(move.startRow - move.endRow) == 2) {
        this->enPassantPossible = std::make_pair((move.startRow + move.endRow) / 2, move.startCol);
    } else {
        this->enPassantPossible = {};
    }

    //castle move
    if(move.castle){
    
        if(move.endCol - move.startCol == 2){//its king side castle 
            this->board[move.endRow][move.endCol-1] = this->board[move.endRow][move.endCol+1];
            this->board[move.endRow][move.endCol+1] = "--";
        }
        else{
            this->board[move.endRow][move.endCol+1] = this->board[move.endRow][move.endCol-2];
            this->board[move.endRow][move.endCol-2] = "--";
        }
    }

    this->enPassantPossibleLogs.push_back(this->enPassantPossible);
    this->UpdateCastleRights(move);
    this->castlingRightsLogs.push_back(CastlingRights(this->currentCastlingRights.white_king_side,this->currentCastlingRights.white_queen_side,
        this->currentCastlingRights.black_king_side,this->currentCastlingRights.black_queen_side));


}

void GameState::UpdateCastleRights(Move move){
    if(move.pieceMoved == "wK"){
        this->currentCastlingRights.white_king_side = false;
        this->currentCastlingRights.white_queen_side = false;
    }
    else if(move.pieceMoved == "bK"){
        this->currentCastlingRights.black_king_side = false;
        this->currentCastlingRights.black_queen_side = false;
    }
    else if(move.pieceMoved == "wR"){
        if(move.startRow == 7){
            if(move.startCol == 0){
                this->currentCastlingRights.white_queen_side = false;
            }
            else if(move.startCol == 7){
                this->currentCastlingRights.white_king_side = false;
            }
        }
    }
    else if(move.pieceMoved == "bR"){
        if(move.startRow == 0){
            if(move.startCol == 0){
                this->currentCastlingRights.black_queen_side = false;
            }
            else if(move.startCol == 7){
                this->currentCastlingRights.black_king_side = false;
            }
        }
    }
    //if rook is captured
    if(move.pieceCaptured == "wR"){
        if(move.endRow == 7){
            if(move.endCol == 0){
                this->currentCastlingRights.white_queen_side = false;
            }
            else if(move.endCol == 7){
                this->currentCastlingRights.white_king_side = false;
            }
        }
    }
    else if(move.pieceCaptured == "bR"){
        if(move.endRow == 0){
            if(move.endCol == 0){
                this->currentCastlingRights.black_queen_side = false;
            }
            else if(move.endCol == 7){
                this->currentCastlingRights.black_king_side = false;
            }
        }
    }
    
}
void GameState::print_board(){
    for(int r=0;r<8;++r){
        for(int c=0;c<8;++c){
            cout<<this->board[r][c]<<" ";
        }
        cout<<endl;
    }
}

// void GameState::undo_move(){
//     if(this->movelogs.size() != 0){
//         Move move = this->movelogs.back(); 
//         this->movelogs.pop_back(); 
//         this->board[move.startRow][move.startCol] = move.pieceMoved;
//         this->board[move.endRow][move.endCol] = move.pieceCaptured;
//         this->whiteToMove = !this->whiteToMove;

//         if(move.pieceMoved == "wK")
//             this->whitekingLocation = std::make_pair(move.startRow,move.startCol);
//         else if(move.pieceMoved == "bK")
//             this->blackkingLocation = std::make_pair(move.startRow,move.startCol);

//         if (move.isEnpassant) {
//             this->board[move.endRow][move.endCol] = "--";
//             this->board[move.startRow][move.endCol] = (this->whiteToMove) ? "bp" : "wp";  // Restoring captured pawn
//             //this->enPassantPossible = std::make_pair(move.endRow, move.endCol);
//         }

//         //if (move.pieceMoved[1] == 'p' && abs(move.startRow - move.endRow) == 2) {
//         //    this->enPassantPossible = {};
//         //}
//         this->enPassantPossibleLogs.pop_back();
//         this->enPassantPossible = this->enPassantPossibleLogs.back();

//         this->castlingRightsLogs.pop_back();
//         this->currentCastlingRights = this->castlingRightsLogs.back();

//         //undo castle move
//         if(move.castle){
//             if(move.endCol - move.startCol == 2){
//                 this->board[move.endRow][move.endCol+1] = this->board[move.endRow][move.endCol-1];
//                 this->board[move.endRow][move.endCol-1] = "--";
//             }
//             else{
//                 this->board[move.endRow][move.endCol-2] = this->board[move.endRow][move.endCol+1];
//                 this->board[move.endRow][move.endCol+1] = "--";
//             }
//         }
//         this->checkMate = false;
//         this->staleMate = false;
//     }
// }
void GameState::undo_move() {
    if (this->movelogs.empty()) {
        return;  // No move to undo
    }

    Move lastMove = this->movelogs.back();
    this->movelogs.pop_back();

    // Revert the board state
    this->board[lastMove.startRow][lastMove.startCol] = lastMove.pieceMoved;
    this->board[lastMove.endRow][lastMove.endCol] = lastMove.pieceCaptured;

    // Revert the king's position if the king was moved
    if (lastMove.pieceMoved == "wK") {
        this->whitekingLocation = std::make_pair(lastMove.startRow, lastMove.startCol);
    } else if (lastMove.pieceMoved == "bK") {
        this->blackkingLocation = std::make_pair(lastMove.startRow, lastMove.startCol);
    }

    // Revert castling rights
    if (lastMove.castle) {
        if (lastMove.endCol - lastMove.startCol == 2) {  // Kingside castle
            this->board[lastMove.endRow][lastMove.endCol + 1] = this->board[lastMove.endRow][lastMove.endCol - 1];
            this->board[lastMove.endRow][lastMove.endCol - 1] = "--";
        } else {  // Queenside castle
            this->board[lastMove.endRow][lastMove.endCol - 2] = this->board[lastMove.endRow][lastMove.endCol + 1];
            this->board[lastMove.endRow][lastMove.endCol + 1] = "--";
        }
    }

    // Revert en passant
   if (lastMove.isEnpassant) {
            this->board[lastMove.endRow][lastMove.endCol] = "--";
            this->board[lastMove.startRow][lastMove.endCol] = !(this->whiteToMove) ? "bp" : "wp";  // Restoring captured pawn
            //this->enPassantPossible = std::make_pair(move.endRow, move.endCol);
        }

        if (lastMove.pieceMoved[1] == 'p' && abs(lastMove.startRow - lastMove.endRow) == 2) {
           this->enPassantPossible = {};
        }
        this->enPassantPossibleLogs.pop_back();
        this->enPassantPossible = this->enPassantPossibleLogs.back();;

    // Revert castling rights
    
    this->castlingRightsLogs.pop_back();
    this->currentCastlingRights = this->castlingRightsLogs.back();

    // Revert half-move clock
    //this->halfMoveClock = this->halfMoveClockLog.back();
    //this->halfMoveClockLog.pop_back();

    // Revert full-move number
    // if (!this->whiteToMove) {
    //     this->fullMoveNumber--;
    // }

    // Switch turn back
    this->whiteToMove = !this->whiteToMove;
}
void GameState::getPawnMoves(int row, int col, std::vector<Move> &moves) {
    bool piecePinned = false;
    std::tuple<int, int> pinDirection;
    auto &pins = std::get<1>(this->checkStatus);

    // Loop through the 'pins' vector in reverse order
    for (int i = pins.size() - 1; i >= 0; --i) {
        if (std::get<0>(pins[i]) == row && std::get<1>(pins[i]) == col) {
            piecePinned = true;
            pinDirection = std::make_tuple(std::get<2>(pins[i]), std::get<3>(pins[i]));
            pins.erase(pins.begin() + i);
            break;
        }
    }

    // Declare variables outside the conditional blocks to keep their scope
    int moveAMT, startRow, endRow,kingRow,kingCol;
    char enemyColor;
    vector<int> insideRange;
    vector<int> outsideRange;

    if (this->whiteToMove) {
        moveAMT = -1;
        startRow = 6;
        endRow = 0;
        enemyColor = 'b';
        kingRow = this->whitekingLocation.first;
        kingCol = this->whitekingLocation.second;

    } else {
        moveAMT = 1;
        startRow = 1;
        endRow = 7;
        enemyColor = 'w';
        kingRow = this->blackkingLocation.first;
        kingCol = this->blackkingLocation.second;
    }

    bool pawnPromotion = false;
    bool attackingPiece = false;
    bool blockingPiece = false;
    // Check if the square in front is empty
    if (row + moveAMT >= 0 && row + moveAMT <= 7 && this->board[row + moveAMT][col] == "--") {
        if (!piecePinned || pinDirection == std::make_tuple(moveAMT, 0)) {
            if (row + moveAMT == endRow) pawnPromotion = true;
            Move m(std::make_pair(row, col), std::make_pair(row + moveAMT, col), this->board, false, pawnPromotion);
            moves.push_back(m);

            // Check for two-square pawn move
            if (row == startRow && this->board[row + 2 * moveAMT][col] == "--") {
                Move m(std::make_pair(row, col), std::make_pair(row + 2 * moveAMT, col), this->board);
                moves.push_back(m);
            }
        }
    }

    // Check capture to the left
    if (col - 1 >= 0) {
        if (!piecePinned || pinDirection == std::make_tuple(moveAMT, -1)) {
            if (this->board[row + moveAMT][col - 1][0] == enemyColor) {
                if (row + moveAMT == endRow) pawnPromotion = true;
                Move m(std::make_pair(row, col), std::make_pair(row + moveAMT, col - 1), this->board, false, pawnPromotion);
                moves.push_back(m);
            }
        
            // En passant capture to the left
            if (std::make_pair(row + moveAMT, col - 1) == this->enPassantPossible) {
                //if (!piecePinned || pinDirection == std::make_tuple(moveAMT, -1)) {
                if(kingRow == row){
                    if(kingCol < col){
                        insideRange = range(kingCol+1,col-1);
                        outsideRange = range(col+1,8);
                    }else{
                        insideRange = range(kingCol-1,col,-1);
                        outsideRange = range(col-2,-1,-1);
                    }
                    for(auto i:insideRange){
                        if(this->board[row][i] != "--"){
                            blockingPiece = true;
                            break;
                        }
                    }
                    for(auto i : outsideRange){
                        if(this->board[row][i][0] == enemyColor && (this->board[row][i][1] == 'R' || this->board[row][i][1] == 'Q')){
                            attackingPiece = true;
                            break;
                        }else if(this->board[row][i] != "--"){
                            blockingPiece = true;
                            break;
                        }
                    }
                }
                if(!attackingPiece || blockingPiece){
                    Move m(std::make_pair(row, col), std::make_pair(row + moveAMT, col - 1), this->board, true);
                    moves.push_back(m);
                }
                //Move m(std::make_pair(row, col), std::make_pair(row + moveAMT, col - 1), this->board, true);
               // moves.push_back(m);
            }
        }
    }

    // Check capture to the right
    if (col + 1 <= 7) {
        if (!piecePinned || pinDirection == std::make_tuple(moveAMT, 1)) {
            if (this->board[row + moveAMT][col + 1][0] == enemyColor) {
                if (row + moveAMT == endRow) pawnPromotion = true;
                Move m(std::make_pair(row, col), std::make_pair(row + moveAMT, col + 1), this->board, false, pawnPromotion);
                moves.push_back(m);
            }
        
            // En passant capture to the right
            if (std::make_pair(row + moveAMT, col + 1) == this->enPassantPossible) {
                //if (!piecePinned || pinDirection == std::make_tuple(moveAMT, 1)) {
                if(kingRow == row){
                    if(kingCol < col){
                        insideRange = range(kingCol+1,col);
                        outsideRange = range(col+2,8);
                    }else{
                        insideRange = range(kingCol-1,col+1,-1);
                        outsideRange = range(col-1,-1,-1);
                    }
                    for(auto i:insideRange){
                        if(this->board[row][i] != "--"){
                            blockingPiece = true;
                            break;
                        }
                    }
                    for(auto i : outsideRange){
                        if(this->board[row][i][0] == enemyColor && (this->board[row][i][1] == 'R' || this->board[row][i][1] == 'Q')){
                            attackingPiece = true;
                            break;
                        }else if(this->board[row][i] != "--"){
                            blockingPiece = true;
                            break;
                        }
                    }
                }
                if(!attackingPiece || blockingPiece){
                    Move m(std::make_pair(row, col), std::make_pair(row + moveAMT, col + 1), this->board, true);
                    moves.push_back(m);
                }
                //Move m(std::make_pair(row, col), std::make_pair(row + moveAMT, col + 1), this->board, true);
                //moves.push_back(m);
            }
        }
    }
}

void GameState::getRookMoves(int row,int col,std::vector<Move> &moves){
    bool piecePinned = false;  // Equivalent to piecePinned = False in Python
    std::tuple<int, int> pinDirection;  // Equivalent to pinDirection = () in Python
    auto & pins = std::get<1>(this->checkStatus);
    // Loop through the 'pins' vector in reverse order
    for (int i = pins.size() - 1; i >= 0; --i) {
        // Assuming this->pins[i][0] is the condition for pinning (replace <condition> accordingly)
        if (std::get<0>(pins[i]) == row && std::get<1>(pins[i]) == col) {
            piecePinned = true;
            pinDirection = std::make_tuple(std::get<2>(pins[i]), std::get<3>(pins[i]));

            // Remove the pin from the 'pins' vector
            if(this->board[row][col][1] != 'Q')
                pins.erase(pins.begin() + i);

            // Exit the loop once the pin is found
            break;
        }
    }
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    std::string enemyColor = (this->whiteToMove) ? "b" : "w";  // Determine enemy color based on current turn

    // Iterate over each direction
    for (auto d : directions) {
        for (int i = 1; i < 8; ++i) {  // Rook can move maximum of 7 squares
            int endRow = row + d.first * i;
            int endCol = col + d.second * i;

            // Check if the square is within the board limits
            if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
                if(!piecePinned ||  pinDirection ==  std::make_tuple(d.first,d.second)|| pinDirection == std::make_tuple(-d.first,-d.second)){
                    std::string endPiece = this->board[endRow][endCol];

                    if (endPiece == "--") {  // Empty space
                        moves.push_back(Move({row, col}, {endRow, endCol}, this->board));
                    }
                    else if (endPiece[0] == enemyColor[0]) {  // Enemy piece
                        moves.push_back(Move({row, col}, {endRow, endCol}, this->board));
                        break;  // Stop searching in this direction after capturing
                    }
                    else {  // Friendly piece
                        break;  // Stop searching in this direction
                    }
                }
            }
            else {
                break;  // Off the board
            }
        }
    }
}
void GameState::getKnightMoves(int row,int col, std::vector<Move> &moves){
    bool piecePinned = false;  // Equivalent to piecePinned = False in Python
    //std::tuple<int, int> pinDirection;  // Equivalent to pinDirection = () in Python
    auto & pins = std::get<1>(this->checkStatus);
    // Loop through the 'pins' vector in reverse order
    for (int i = pins.size() - 1; i >= 0; --i) {
        // Assuming this->pins[i][0] is the condition for pinning (replace <condition> accordingly)
        if (std::get<0>(pins[i]) == row && std::get<1>(pins[i]) == col) {
            piecePinned = true;
            //pinDirection = std::make_tuple(std::get<2>(pins[i]), std::get<3>(pins[i]));

            // Remove the pin from the 'pins' vector
            pins.erase(pins.begin() + i);

            // Exit the loop once the pin is found
            break;
        }
    }
    std::vector<std::pair<int, int>> knightMoves = {
        {-2, 1}, {-2, -1}, {-1, -2}, {-1, 2}, 
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    // Determine ally color based on the current turn
    std::string allyColor = (this->whiteToMove) ? "w" : "b";

    // Loop through each of the knight's possible moves
    for (auto m : knightMoves) {
        int endRow = row + m.first;
        int endCol = col + m.second;

        // Check if the move is within the bounds of the board
        if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
            if(!piecePinned){
                std::string endPiece = this->board[endRow][endCol];

                // Check if the destination square is either empty or contains an enemy piece
                if (endPiece[0] != allyColor[0]) {  // Not an ally piece
                    moves.push_back(Move({row, col}, {endRow, endCol}, this->board));
                }
            }
        }
    }
}
void GameState::getBishopMoves(int row,int col, std::vector<Move> &moves){
    bool piecePinned = false;  // Equivalent to piecePinned = False in Python
    std::tuple<int, int> pinDirection;  // Equivalent to pinDirection = () in Python
    auto & pins = std::get<1>(this->checkStatus);
    // Loop through the 'pins' vector in reverse order
    for (int i = pins.size() - 1; i >= 0; --i) {
        // Assuming this->pins[i][0] is the condition for pinning (replace <condition> accordingly)
        if (std::get<0>(pins[i]) == row && std::get<1>(pins[i]) == col) {
            piecePinned = true;
            pinDirection = std::make_tuple(std::get<2>(pins[i]), std::get<3>(pins[i]));

            // Remove the pin from the 'pins' vector
            pins.erase(pins.begin() + i);

            // Exit the loop once the pin is found
            break;
        }
    }
    std::vector<std::pair<int, int>> directions = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
    std::string enemyColor = (this->whiteToMove) ? "b" : "w";  // Determine enemy color based on current turn

    // Iterate over each direction
    for (auto d : directions) {
        for (int i = 1; i < 8; ++i) {  // Rook can move maximum of 7 squares
            int endRow = row + d.first * i;
            int endCol = col + d.second * i;

            // Check if the square is within the board limits
            if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
                if(!piecePinned || pinDirection == std::make_tuple(d.first,d.second) || pinDirection == std::make_tuple(-d.first,-d.second)){
                    std::string endPiece = this->board[endRow][endCol];

                    if (endPiece == "--") {  // Empty space
                        moves.push_back(Move({row, col}, {endRow, endCol}, this->board));
                    }
                    else if (endPiece[0] == enemyColor[0]) {  // Enemy piece
                        moves.push_back(Move({row, col}, {endRow, endCol}, this->board));
                        break;  // Stop searching in this direction after capturing
                    }
                    else {  // Friendly piece
                        break;  // Stop searching in this direction
                    }
                }
            }
            else {
                break;  // Off the board
            }
        }
    }
}
void GameState::getQueenMoves(int row,int col, std::vector<Move> &moves){
   this->getRookMoves(row,col,moves);
   this->getBishopMoves(row,col,moves);
}
void GameState::getKingMoves(int row,int col, std::vector<Move> &moves){
    std::vector<int> RowMoves = {-1,-1,-1,0,0,1,1,1};
    std::vector<int> ColMoves = {-1,0,1,-1,1,-1,0,1};
    //std::vector<std::pair<int, int>> kingMoves = {
    //    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    //};

    // Determine ally color based on the current turn
    std::string allyColor = (this->whiteToMove) ? "w" : "b";

    // Loop through each of the king's possible moves
    for (int i = 0; i < 8; i++) {
        int endRow = row + RowMoves[i];
        int endCol = col + ColMoves[i];

        // Check if the move is within the bounds of the board
        if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
            std::string endPiece = this->board[endRow][endCol];

            // Check if the destination square is either empty or contains an enemy piece
            if (endPiece[0] != allyColor[0]) {  // Not an ally piece
                
                if(allyColor[0] == 'w'){
                    this->whitekingLocation = std::make_pair(endRow,endCol);
                }
                else{
                    this->blackkingLocation = std::make_pair(endRow,endCol);
                }
                std::tuple<bool, std::vector<std::tuple<int, int, int, int>>, std::vector<std::tuple<int, int, int, int>>> tempcheckStatus;
                tempcheckStatus = this->checkForPinsAndChecks();
                if(!std::get<0>(tempcheckStatus)){
                    moves.push_back(Move({row, col}, {endRow, endCol}, this->board));
                }
                if(allyColor[0] == 'w'){
                    this->whitekingLocation = std::make_pair(row,col);
                }
                else{
                    this->blackkingLocation = std::make_pair(row,col);
                }
            }
        }
    }
    //this->getCastleMoves(row,col,moves,allyColor);
}

void GameState::getCastleMoves(int row,int col,std::vector<Move> &moves){
    if(this->squareUnderAttack(row,col))
        return;
    if((this->whiteToMove && this->currentCastlingRights.white_king_side) || (!this->whiteToMove && this->currentCastlingRights.black_king_side)){
        this->getKingSideCastle(row,col,moves);
    }
    if((this->whiteToMove && this->currentCastlingRights.white_queen_side) || (!this->whiteToMove && this->currentCastlingRights.black_queen_side)){
        this->getQueenSideCastle(row,col,moves);
    }
    
}

void GameState::getKingSideCastle(int row, int col,std::vector<Move> &moves){
    if(this->board[row][col+1]=="--" && this->board[row][col+2] == "--"){
        if(!this->squareUnderAttack(row,col+1) && !this->squareUnderAttack(row,col+2)){
            //moves.push_back(Move({row, col}, {row,col+1}, this->board,false,false,true));
            moves.push_back(Move({row, col}, {row,col+2}, this->board,false,false,true));
        }
    }
}
void GameState::getQueenSideCastle(int row, int col,std::vector<Move> &moves){
    if(this->board[row][col-1]=="--" && this->board[row][col-2]=="--" && this->board[row][col-3]=="--"){
        if(!this->squareUnderAttack(row,col-1) && !this->squareUnderAttack(row,col-2)){
            moves.push_back(Move({row, col}, {row,col-2}, this->board,false,false,true));
        }
    }
}


std::vector<Move>  GameState::allPossibleMoves(){
    
    std::vector<Move> moves = {};
    for (int r=0 ;r<this->board.size();++r){
        for(int c=0; c<this->board[r].size();++c){
            char turn = this->board[r][c][0];
            if((turn == 'w' && this->whiteToMove ) || (turn == 'b' && !this->whiteToMove)){
                char piece = this->board[r][c][1];
                if (piece == 'p'){
                    this->getPawnMoves(r,c,moves);
                }
                else if(piece =='R'){
                    this->getRookMoves(r,c,moves);
                }
                else if(piece == 'N'){
                    this->getKnightMoves(r,c,moves);
                }
                else if(piece == 'B'){
                    this->getBishopMoves(r,c,moves);
                }
                else if(piece == 'Q'){
                    this->getQueenMoves(r,c,moves);
                }
                else if(piece == 'K'){
                    this->getKingMoves(r,c,moves);
                } 
            }
        }
    }

    return moves;

}

std::vector<Move> GameState::getValidMoves() {
    std::vector<Move> moves;
    this->checkStatus = this->checkForPinsAndChecks();
    this->incheck = std::get<0>(this->checkStatus);
    this->pins = std::get<1>(this->checkStatus);
    this->checks = std::get<2>(this->checkStatus);

    int kingRow, kingCol;
    if (this->whiteToMove) {
        kingRow = this->whitekingLocation.first;
        kingCol = this->whitekingLocation.second;
    } else {
        kingRow = this->blackkingLocation.first;
        kingCol = this->blackkingLocation.second;
    }

    if (this->incheck) {
        if (checks.size() == 1) {  // Only 1 check, block or move the king
            moves = this->allPossibleMoves();

            // To block a check, move a piece into one of the squares between the enemy
            auto check = checks[0];  // Check information
            int checkRow = std::get<0>(check);
            int checkCol = std::get<1>(check);
            std::string checkingPiece = this->board[checkRow][checkCol];
            std::vector<std::tuple<int, int>> validSquares;

            // If knight is checking, no blocking, must capture the knight or move the king
            if (checkingPiece[1] == 'N') {
                validSquares.push_back(std::make_tuple(checkRow, checkCol));
            } else {  // Otherwise, move in the direction of check
                for (int i = 1; i < 8; i++) {
                    int validRow = kingRow + std::get<2>(check) * i;
                    int validCol = kingCol + std::get<3>(check) * i;
                    validSquares.push_back(std::make_tuple(validRow, validCol));
                    if (validRow == checkRow && validCol == checkCol) {
                        break;
                    }
                }
            }

            // Get rid of moves that don't block check or move king
            for(int i = moves.size()-1 ; i>=0 ; i--){
                if(moves[i].pieceMoved[1] != 'K' ){
                    bool validMove = false;
                    for(auto square : validSquares){
                        if(moves[i].endRow == std::get<0>(square) && moves[i].endCol == std::get<1>(square)){
                            validMove = true;
                            break;
                        }
                    }
                    if(!validMove){
                        moves.erase(moves.begin()+i);
                    }
                }
            }
            // moves.erase(std::remove_if(moves.begin(), moves.end(), [&](Move move) {
            //     if (move.pieceMoved == "wK" || move.pieceMoved == "bK") {
            //         return false;  // King's moves are allowed
            //     }
            //     for (auto square : validSquares) {
            //         if (std::get<0>(square) == move.endRow && std::get<1>(square) == move.endCol) {
            //             return false;  // Valid move
            //         }
            //     }
            //     return true;  // Not a valid move to block check
            // }), moves.end());

        } else {  // Double check, king must move
            this->getKingMoves(kingRow, kingCol, moves);
        }
    } else {
        moves = this->allPossibleMoves();
        if(this->whiteToMove){
            this->getCastleMoves(this->whitekingLocation.first,this->whitekingLocation.second,moves);
        }
        else{
            this->getCastleMoves(this->blackkingLocation.first,this->blackkingLocation.second,moves);
        }
    }

    return moves;
}

bool GameState::inCheck(){
    // if(this->whiteToMove){
    //     return squareUnderAttack(this->whitekingLocation.first,this->whitekingLocation.second);
    // }
    // else{
    //     return squareUnderAttack(this->blackkingLocation.first, this->whitekingLocation.second);
    // }
    std::tuple<bool, std::vector<std::tuple<int, int, int, int>>, std::vector<std::tuple<int, int, int, int>>> tempcheckStatus;
    tempcheckStatus = this->checkForPinsAndChecks();
    return std::get<0>(tempcheckStatus);
}

bool GameState::squareUnderAttack(int row, int col){
    this->whiteToMove = !(this->whiteToMove);
    std::vector<Move> oppMoves = this->allPossibleMoves();
    this->whiteToMove = !(this->whiteToMove);
    for(auto move : oppMoves){
       if(move.endRow == row && move.endCol == col){
           return true;
        }
    }

    return false;
}

std::tuple<bool, std::vector<std::tuple<int, int, int, int>>, std::vector<std::tuple<int, int, int, int>>> 
GameState::checkForPinsAndChecks() {
    std::vector<std::tuple<int, int, int, int>> pins;  // squares where the allied pinned piece is and direction pinned from
    std::vector<std::tuple<int, int, int, int>> checks;  // squares where enemy is applying a check
    bool inCheck = false;

    std::string enemyColor, allyColor;
    int startRow, startCol;

    if (this->whiteToMove) { 
        enemyColor = "b";
        allyColor = "w";
        startRow = this->whitekingLocation.first;
        startCol = this->whitekingLocation.second;
    } else {
        enemyColor = "w";
        allyColor = "b";
        startRow = blackkingLocation.first;
        startCol = blackkingLocation.second;
    }

    // Check outward from king for pins and checks
    std::vector<std::tuple<int, int>> directions = {
        {-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (int j = 0; j < directions.size(); j++) {
        auto [dRow, dCol] = directions[j];
        std::tuple<int, int, int, int> possiblePin = std::make_tuple(-1, -1, 0, 0);  // Reset possible pin
        bool pieceBetween = false;
        for (int i = 1; i < 8; i++) {
            int endRow = startRow + dRow * i;
            int endCol = startCol + dCol * i;

            if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {  // Check within bounds
                std::string endPiece = board[endRow][endCol];
                if (endPiece[0] == allyColor[0] && endPiece[1]!='K') {
                    if (!pieceBetween) {  // First allied piece could be pinned
                        possiblePin = std::make_tuple(endRow, endCol, dRow, dCol);
                        pieceBetween = true;  // A piece between king and enemy found
                    } else {  // Second allied piece, no pin possible
                        break;
                    }
                } else if (endPiece[0] == enemyColor[0]) {
                    char type = endPiece[1];
                    // Check if the piece is attacking the king in the current direction
                    if ((0 <= j && j <= 3 && type == 'R') ||  // Rook check
                        (4 <= j && j <= 7 && type == 'B') ||  // Bishop check
                        (i == 1 && type == 'p' && ((enemyColor == "w" && (j == 6 || j == 7)) || (enemyColor == "b" && (j == 4 || j == 5)))) ||  // Pawn check
                        (type == 'Q') ||  // Queen can check like a rook or bishop
                        (i == 1 && type == 'K')) {  // King can give check from 1 square away
                        if (!pieceBetween) {  // No piece blocking, it's a check
                            inCheck = true;
                            checks.push_back(std::make_tuple(endRow, endCol, dRow, dCol));
                            break;
                        } else {  // Piece between, it's a pin
                            pins.push_back(possiblePin);
                            break;
                        }
                        
                    } else {  // Other enemy pieces (Knight, etc.)
                        break;
                    }
                }
            
            } else {
                break;  // Out of bounds
            }
        }
    }

    // Knight checks
    std::vector<std::tuple<int, int>> knightMoves = {{-2, 1}, {-2, -1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    for (auto move : knightMoves) {
        int endRow = startRow + std::get<0>(move);
        int endCol = startCol + std::get<1>(move);
        if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
            std::string endPiece = board[endRow][endCol];
            if (endPiece[0] == enemyColor[0] && endPiece[1] == 'N') {  // Knight check
                inCheck = true;
                checks.push_back(std::make_tuple(endRow, endCol, 0, 0));
            }
        }
    }

    return std::make_tuple(inCheck, pins, checks);
}

std::vector<int> GameState::range(int start, int stop, int step) {
    if (step == 0) {
        throw std::invalid_argument("Step must not be zero.");
    }

    std::vector<int> result;

    if (start < stop && step > 0) {
        for (int value = start; value < stop; value += step) {
            result.push_back(value);
        }
    } else if (start > stop && step < 0) {
        for (int value = start; value > stop; value += step) {
            result.push_back(value);
        }
    }

    return result;
}