#ifndef CASTLINGRIGHTS_H
#define CASTLINGRIGHTS_H
class CastlingRights {

public:
bool white_king_side;
bool white_queen_side;
bool black_king_side;
bool black_queen_side;

CastlingRights(bool white_king_side,bool white_queen_side,bool black_king_side,bool black_queen_side);
};
#endif // CASTLINGRIGHTS_H