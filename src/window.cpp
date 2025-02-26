#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include "GameState.h"
#include "move.h"
#include "AIMove.h"

std::vector<sf::Color> colors {sf::Color(235,236,211),sf::Color(125, 148, 93)};//gray 128,128,128
int width = 640;
int height = width;
int log_panel_width = 250;
int log_panel_height = height;
int dimension = 8;
int sq_size = width/dimension;

void load_images(std::map<std::string, sf::Texture> &images, sf::Texture &wp, sf::Texture &wR, sf::Texture &wN, sf::Texture &wB, sf::Texture &wQ, sf::Texture &wK, sf::Texture &bp, sf::Texture &bR, sf::Texture &bN, sf::Texture &bB, sf::Texture &bQ, sf::Texture &bK) {
    std::vector<std::pair<std::string, sf::Texture*>> pieces = {
        {"wp", &wp}, {"wR", &wR}, {"wN", &wN}, {"wB", &wB}, {"wQ", &wQ}, {"wK", &wK},
        {"bp", &bp}, {"bR", &bR}, {"bN", &bN}, {"bB", &bB}, {"bQ", &bQ}, {"bK", &bK}
    };

    for (auto& p : pieces) {
        if (!p.second->loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/assets/piece_sets/neo_set/" + p.first + ".png")) {
            std::cerr << "Error loading image: " << "images/" + p.first + ".png" << std::endl;
        }
        images[p.first] = *p.second;
    }
}
void drawBoard(sf::RenderWindow &window, int squareSize){
    //std::vector<sf::Color> colors {sf::Color::White,sf::Color(128, 128, 128)};
    for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
                square.setPosition(col * squareSize, row * squareSize);
                square.setFillColor((row + col) % 2 == 0 ? colors[0] : colors[1]);
                window.draw(square);
            }
        }
}
void drawPiece(sf::RenderWindow &window,GameState gs,int sq_size,std::map<std::string, sf::Texture> &images){
    for(int row = 0; row < 8; ++row){
        for(int col = 0; col < 8; ++col){
            std::string piece = gs.board[row][col];
            if(piece != "--"){
                //add the piece to the position
                sf::Sprite sprite;
                sprite.setTexture(images.at(piece)); // Get the texture for the piece
                sprite.setPosition(col * sq_size, row * sq_size); // Set the position on the board
                window.draw(sprite); // Draw the piece
            }
        }
    }
}

//highlights the squares that are valid moves
void highlightSquares(sf::RenderWindow &window, GameState &gs, const std::vector<Move> &validMoves, std::pair<int, int> sqSelected, int sq_size) {
    if (sqSelected != std::make_pair(-1, -1)) {
        int row = sqSelected.first;
        int col = sqSelected.second;

        // Check if the selected square contains a piece of the current player
        if (gs.board[row][col][0] == (gs.whiteToMove ? 'w' : 'b')) {
            // Highlight selected square in blue
            sf::RectangleShape square(sf::Vector2f(sq_size, sq_size));
            square.setFillColor(sf::Color(0, 0, 255, 100)); // RGBA: blue, semi-transparent
            square.setPosition(col * sq_size, row * sq_size);
            window.draw(square);

            // Highlight valid moves from the selected square in yellow
            square.setFillColor(sf::Color(255, 255, 0, 100)); // RGBA: yellow, semi-transparent
            for (const auto &move : validMoves) {
                if (move.startRow == row && move.startCol == col) {
                    square.setPosition(move.endCol * sq_size, move.endRow * sq_size);
                    window.draw(square);
                }
            }
        }
    }
}

//highlight the last move
void highlightLastMove(sf::RenderWindow &window, const Move &lastMove, int sq_size) {
    sf::RectangleShape square(sf::Vector2f(sq_size, sq_size));
    square.setFillColor(sf::Color(255, 0, 0, 100)); // RGBA: red, semi-transparent

    // Highlight the start square of the last move
    square.setPosition(lastMove.startCol * sq_size, lastMove.startRow * sq_size);
    window.draw(square);

    // Highlight the end square of the last move
    square.setPosition(lastMove.endCol * sq_size, lastMove.endRow * sq_size);
    window.draw(square);
}

////animate the moved piece
void Animate(Move move,sf::RenderWindow &window,GameState &gs,sf::Clock clock,int sq_size,std::map<std::string, sf::Texture> &images){
    int dR = move.endRow - move.startRow;
    int dC = move.endCol - move.startCol;
    int framesPerSquare = 2;  // frames to move one square
    int frameCount = (std::abs(dR) + std::abs(dC)) * framesPerSquare;

    for (int frame = 0; frame <= frameCount; ++frame) {
        float r = move.startRow + dR * (frame / (float)frameCount);
        float c = move.startCol + dC * (frame / (float)frameCount);

        window.clear();
        drawBoard(window, sq_size);
        drawPiece(window,gs, sq_size, images);

        // Erase the piece moved from its ending square
        sf::Color color = colors[(move.endRow + move.endCol) % 2];
        sf::RectangleShape endSquare(sf::Vector2f(sq_size, sq_size));
        endSquare.setPosition(move.endCol * sq_size, move.endRow * sq_size);
        endSquare.setFillColor(color);
        window.draw(endSquare);

        // Draw captured piece onto rectangle
        if (move.pieceCaptured != "--") {
            if(move.isEnpassant){
                int enPassantPawnRow = (move.pieceCaptured[0] == 'b') ? move.endRow + 1 : move.endRow - 1;
                endSquare.setPosition(move.endCol * sq_size, enPassantPawnRow * sq_size);
                sf::Sprite captured(images.at((move.pieceMoved[0] == 'w') ? "bp" : "wp"));
                captured.setPosition(move.endCol * sq_size, enPassantPawnRow * sq_size);
                window.draw(captured);
            }
            else{
                sf::Sprite captured(images.at(move.pieceCaptured));
                captured.setPosition(move.endCol * sq_size, move.endRow * sq_size);
                window.draw(captured);
            }
            //sf::Sprite captured(images.at(move.pieceCaptured));
            //captured.setPosition(move.endCol * sq_size, move.endRow * sq_size);
            //window.draw(captured);
        }

        // Draw moving piece
        sf::Sprite moving(images.at(move.pieceMoved));
        moving.setPosition(c * sq_size, r * sq_size);
        window.draw(moving);

        window.display();
        clock.restart();
        sf::sleep(sf::seconds(1.f / 1200));  // 60 FPS
    }
}

void drawEndGameText(sf::RenderWindow &window,std::string text){
    sf::Font font;
    if (!font.loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/fonts/helvetica.ttf")) {
        // Handle error
        return;
    }

    // Create a text object
    sf::Text textObject;
    textObject.setFont(font);
    textObject.setString(text);
    textObject.setCharacterSize(32); // in pixels, not points
    textObject.setStyle(sf::Text::Bold);
    textObject.setFillColor(sf::Color::Black);

    // Calculate text position
    sf::FloatRect textRect = textObject.getLocalBounds();
    textObject.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textObject.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));

    // Draw the text
    window.draw(textObject);
}

void DrawMoveLog(sf::RenderWindow &window,GameState gs){
    sf::Font font;
    if (!font.loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/fonts/helvetica.ttf")) {
        // Handle error
        return;
    }
    sf::RectangleShape logPanel(sf::Vector2f(log_panel_width, log_panel_height));
    int x = width+12;
    int y = 0;
    int charSize = 12;
    for(int i=0;i<gs.movelogs.size();i++){
        sf::Text text;
        text.setFont(font);
        text.setString(gs.movelogs[i].getChessNotation());
        text.setCharacterSize(charSize);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        text.setPosition(x,y);
        window.draw(text);
        
        if(x < width + 190){
            x += 34;
        }else{
            x = width + 12;
            y+=charSize;
        }
    
    }
}

void DrawGameState(sf::RenderWindow &window,GameState gs,int sq_size,std::map<std::string, sf::Texture> &images,const std::vector<Move> &validMoves,std::pair<int,int> sqSelected){
    drawBoard(window,sq_size);
    highlightSquares(window,gs,validMoves,sqSelected,sq_size);
    DrawMoveLog(window,gs);
    if(!gs.movelogs.empty()){
        highlightLastMove(window,gs.movelogs.back(),sq_size);
    }
    drawPiece(window,gs,sq_size,images);
}

#ifndef UNIT_TEST
int main() {

   

    sf::Texture wp,wR,wN,wB,wQ,wK,bp,bR,bN,bB,bQ,bK;
    std::map<std::string,sf::Texture> images;
    GameState gs;
    std::vector<std::vector<std::string>> board = gs.board;

    std::vector<Move> validMoves = gs.getValidMoves();
    bool moveMade = false;
    bool animate = false;
    bool gameOver = false;
    load_images(images,wp,wR,wN,wB,wQ,wK,bp,bR,bN,bB,bQ,bK);

    sf::RenderWindow window(sf::VideoMode(width + log_panel_width ,height), "CHESS");
    sf::Clock clock;
    std::pair<int,int> sqSelected = {NULL,NULL};
    std::vector<std::pair<int,int>> playerClicks = {};
    //sf::Clock clock;
    bool playerOne = true;//it is true when a human is playing as white and false when AI playes white
    bool playerTwo = !true; // its same as above but for black 
    bool humanTurn;
    AIMove ai;

    sf::SoundBuffer moveBuffer, captureBuffer, castleBuffer, checkBuffer, notifyBuffer;
    if (!moveBuffer.loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/assets/audio/move.wav") ||
        !captureBuffer.loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/assets/audio/capture.wav") ||
        !castleBuffer.loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/assets/audio/castle.wav") ||
        !checkBuffer.loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/assets/audio/check.wav") ||
        !notifyBuffer.loadFromFile("/Users/deepak/Brajs_workspace/Project_Workspace/cpp_Projects/chessAI/assets/audio/notify.wav")) {
        std::cerr << "Error loading sound files" << std::endl;
        return -1;
    }

    sf::Sound moveSound(moveBuffer);
    sf::Sound captureSound(captureBuffer);
    sf::Sound castleSound(castleBuffer);
    sf::Sound checkSound(checkBuffer);
    sf::Sound notifySound(notifyBuffer);

    notifySound.setVolume(100);
    notifySound.play();

    // Dragging variables
    bool draggingPiece = false;
    sf::Sprite draggedSprite;
    std::pair<int, int> dragStartSquare = {-1, -1};
    std::string draggedPiece = "--";
    sf::Vector2f dragOffset;

    while (window.isOpen()) {
        humanTurn = (gs.whiteToMove && playerOne) || (!gs.whiteToMove && playerTwo);
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            // Mouse Pressed: Start Dragging
            // if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            //     if (!gameOver && humanTurn) {
            //         sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            //         int col = mousePos.x / sq_size;
            //         int row = mousePos.y / sq_size;

            //         if (gs.board[row][col] != "--" && gs.board[row][col][0] == (gs.whiteToMove ? 'w' : 'b')) {
            //             draggingPiece = true;
            //             dragStartSquare = {row, col};
            //             draggedPiece = gs.board[row][col];
            //             draggedSprite.setTexture(images.at(draggedPiece));
            //             draggedSprite.setScale((float)sq_size / 100, (float)sq_size / 100);
            //             dragOffset = sf::Vector2f(mousePos.x - col * sq_size, mousePos.y - row * sq_size);
            //         }
            //     }
            // }

            // // Mouse Moved: Dragging Motion
            // if (e.type == sf::Event::MouseMoved && draggingPiece) {
            //     sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            //     draggedSprite.setPosition(mousePos.x - dragOffset.x, mousePos.y - dragOffset.y);
            // }

            // // Mouse Released: Drop the Piece
            // if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left && draggingPiece) {
            //     sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            //     int endCol = mousePos.x / sq_size;
            //     int endRow = mousePos.y / sq_size;

            //     Move move(dragStartSquare, {endRow, endCol}, gs.board);

            //     for(int i=0 ; i<validMoves.size();++i){
            //         // Check if the move is valid
            //         if (move == validMoves[i]) {
            //             std::cout << "Move played: " << move.getChessNotation() << std::endl;
            //             gs.makeMove(validMoves[i]);
            //             gs.print_board(); // print the board after making move
            //             std::cout<<endl;
            //             moveMade = true; // Flag that a move was made
            //             animate = true;
            //             sqSelected = std::make_pair(-1, -1);
            //             playerClicks.clear();

            //             if (validMoves[i].castle) {
            //                 castleSound.setVolume(100);
            //                 castleSound.play();
                            
            //             }else if (validMoves[i].pieceCaptured != "--" && gs.inCheck()) {
            //                 checkSound.setVolume(100);
            //                 checkSound.play();
            //             }else if (validMoves[i].pieceCaptured != "--" || validMoves[i].isEnpassant) {
            //                 captureSound.setVolume(100);
            //                 captureSound.play();
            //             }else if (gs.inCheck()) {
            //                 checkSound.setVolume(100);
            //                 checkSound.play();
            //             } else {
            //                 moveSound.setVolume(100);
            //                 moveSound.play();
            //             }
            //             break;
            //         }
            //         // Reset the selection and clicks after the move
            //     }

            //     // Reset Dragging State
            //     draggingPiece = false;
            //     draggedPiece = "--";
            // }
            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                if(!gameOver && humanTurn){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int col = mousePos.x / sq_size;
                    int row = mousePos.y / sq_size;
                    
                    // Check if the same square is clicked twice (deselect)
                    if (sqSelected == std::make_pair(row, col)) {
                        sqSelected = std::make_pair(-1, -1); // Reset selection
                        playerClicks.clear(); // Clear player clicks
                    } else {
                        sqSelected = {row, col};
                        playerClicks.push_back(sqSelected);
                    }

                    // If two clicks (start and end squares) are recorded
                    if (playerClicks.size() == 2) {
                        Move move(playerClicks[0], playerClicks[1], gs.board);

                        // Check if the selected piece exists at the start square
                        if (gs.board[move.startRow][move.startCol] == "--") {
                            std::cout << "Invalid move: No piece at selected square." << std::endl;
                            sqSelected = std::make_pair(-1, -1); // Reset selection
                            playerClicks.clear();
                        } else {
                            
                            for(int i=0 ; i<validMoves.size();++i){
                                // Check if the move is valid
                                if (move == validMoves[i]) {
                                    std::cout << "Move played: " << move.getChessNotation() << std::endl;
                                    gs.makeMove(validMoves[i]);
                                    gs.print_board(); // print the board after making move
                                    std::cout<<endl;
                                    moveMade = true; // Flag that a move was made
                                    animate = true;
                                    sqSelected = std::make_pair(-1, -1);
                                    playerClicks.clear();

                                    if (validMoves[i].castle) {
                                        castleSound.setVolume(100);
                                        castleSound.play();
                                        
                                    }else if (validMoves[i].pieceCaptured != "--" && gs.inCheck()) {
                                        checkSound.setVolume(100);
                                        checkSound.play();
                                    }else if (validMoves[i].pieceCaptured != "--" || validMoves[i].isEnpassant) {
                                        captureSound.setVolume(100);
                                        captureSound.play();
                                    }else if (gs.inCheck()) {
                                        checkSound.setVolume(100);
                                        checkSound.play();
                                    } else {
                                        moveSound.setVolume(100);
                                        moveSound.play();
                                    }
                                    
                                    break;
                                }
                                // Reset the selection and clicks after the move
                            }
                            if(!moveMade){
                                //std::cout << "Invalid move: This move is not allowed." << std::endl;
                                playerClicks = {sqSelected};
                            }
                            
                        }
                    }
                }
            }
            else if(e.type == sf::Event::KeyPressed){
                if(e.key.code == sf::Keyboard::Z){
                    gs.undo_move();
                    moveMade = true;
                    animate=false;
                    cout<<"undo done: "<<endl;
                    gs.print_board();
                    gameOver = false;
                }
                
            }
        }
        if(!gameOver && !humanTurn){
            Move AIMove = ai.findBestMove(gs,validMoves);
            if(AIMove == Move(std::make_pair(0,0),std::make_pair(0,0),gs.board)){
                AIMove = ai.findRandomMove(validMoves);
            }
            gs.makeMove(AIMove);
            std::cout << "Move played: " << AIMove.getChessNotation() << std::endl;
            gs.print_board();
            moveMade=true;
            animate=true;

            // Play sound based on move type
            if (AIMove.castle) {
                castleSound.setVolume(100);
                castleSound.play();
            } 
            else if (AIMove.pieceCaptured != "--" && gs.inCheck()) {
                checkSound.setVolume(100);
                checkSound.play();
            }else if(AIMove.pieceCaptured != "--" || AIMove.isEnpassant){
                captureSound.setVolume(100);
                captureSound.play();
            }else if(gs.inCheck()){
                checkSound.setVolume(100);
                checkSound.play();
            }else {
                moveSound.setVolume(100);
                moveSound.play();
            }
            
        }
        if(moveMade){
            if(animate)
                Animate(gs.movelogs.back(),window,gs,clock,sq_size,images);
            validMoves = gs.getValidMoves();
            if(validMoves.size()==0 && !gs.incheck)
                gs.staleMate= true;
            else if(validMoves.size()==0 && gs.incheck){
                gs.checkMate = true;
            }
            moveMade = false;
            animate = false;

            // if (gs.inCheck()) {
            //     checkSound.play();
            // }
        }

        
        sf::Time elapsed = clock.restart();
        float deltatime = elapsed.asSeconds();
        //cout<<endl;
        //cout<<deltatime<<endl;
        window.clear();
        // Add your drawing code here
        DrawGameState(window,gs,sq_size,images,validMoves,sqSelected);
        if(gs.checkMate){
            gameOver = true;
            if(gs.whiteToMove){
                drawEndGameText(window,"Black Wins By Checkmate");
            }
            else{
                drawEndGameText(window,"White Wins By Checkmate");
            }
        }
        if(gs.staleMate){
            gameOver = true;
            drawEndGameText(window,"Stalemate");
        }
        if (draggingPiece) {
            window.draw(draggedSprite);
        }
        window.display();
    }
    return 0;
}
#endif