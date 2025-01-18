#include<SFML/Graphics.hpp>
#include<iostream>
#include<array>
#include<vector>



int main() {

    sf::RenderWindow window(sf::VideoMode(640, 640), "SFML Chess window");

    const int squareSize = 80;
    sf::Color lightColor = sf::Color::White;
    sf::Color darkColor = sf::Color(118, 150, 86);

    // Load piece textures
    sf::Texture whitePawnTexture, whiteKnightTexture, whiteBishopTexture, whiteRookTexture, whiteQueenTexture, whiteKingTexture,
        blackPawnTexture, blackKnightTexture, blackBishopTexture, blackRookTexture, blackQueenTexture, blackKingTexture;

    if (!whitePawnTexture.loadFromFile("images/white_pawn.png") ||
        !whiteKnightTexture.loadFromFile("images/white_knight.png") ||
        !whiteBishopTexture.loadFromFile("images/white_bishop.png") ||
        !whiteRookTexture.loadFromFile("images/white_rook.png") ||
        !whiteQueenTexture.loadFromFile("images/white_queen.png") ||
        !whiteKingTexture.loadFromFile("images/white_king.png") ||
        !blackRookTexture.loadFromFile("images/black_rook.png") ||
        !blackPawnTexture.loadFromFile("images/black_pawn.png") ||
        !blackKnightTexture.loadFromFile("images/black_knight.png") ||
        !blackBishopTexture.loadFromFile("images/black_bishop.png") ||
        !blackQueenTexture.loadFromFile("images/black_queen.png") ||
        !blackKingTexture.loadFromFile("images/black_king.png")) {

        std::cerr << "Error loading piece texture" << std::endl;
        return -1;
    }

    // Create pieces and assign them to starting positions
    std::vector<Piece> pieces;

    // White pieces
    for (int i = 0; i < 8; ++i) {
        pieces.push_back({ sf::Sprite(whitePawnTexture), 'P', 6, i });  // White pawns
    }
    pieces.push_back({ sf::Sprite(whiteRookTexture), 'R', 7, 0 });
    pieces.push_back({ sf::Sprite(whiteRookTexture), 'R', 7, 7 });
    pieces.push_back({ sf::Sprite(whiteKnightTexture), 'N', 7, 1 });
    pieces.push_back({ sf::Sprite(whiteKnightTexture), 'N', 7, 6 });
    pieces.push_back({ sf::Sprite(whiteBishopTexture), 'B', 7, 2 });
    pieces.push_back({ sf::Sprite(whiteBishopTexture), 'B', 7, 5 });
    pieces.push_back({ sf::Sprite(whiteQueenTexture), 'Q', 7, 3 });
    pieces.push_back({ sf::Sprite(whiteKingTexture), 'K', 7, 4 });

    // Black pieces
    for (int i = 0; i < 8; ++i) {
        pieces.push_back({ sf::Sprite(blackPawnTexture), 'p', 1, i });  // Black pawns
    }
    pieces.push_back({ sf::Sprite(blackRookTexture), 'r', 0, 0 });
    pieces.push_back({ sf::Sprite(blackRookTexture), 'r', 0, 7 });
    pieces.push_back({ sf::Sprite(blackKnightTexture), 'n', 0, 1 });
    pieces.push_back({ sf::Sprite(blackKnightTexture), 'n', 0, 6 });
    pieces.push_back({ sf::Sprite(blackBishopTexture), 'b', 0, 2 });
    pieces.push_back({ sf::Sprite(blackBishopTexture), 'b', 0, 5 });
    pieces.push_back({ sf::Sprite(blackQueenTexture), 'q', 0, 3 });
    pieces.push_back({ sf::Sprite(blackKingTexture), 'k', 0, 4 });

    bool isPieceSelected = false;
    Piece* selectedPiece = nullptr;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Detect mouse click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / squareSize;
                int row = mousePos.y / squareSize;

                // If no piece is selected, select a piece
                if (!isPieceSelected) {
                    for (auto& piece : pieces) {
                        if (piece.row == row && piece.col == col) {
                            std::cout << "Piece Selected: " << piece.type << " at (" << row << ", " << col << ")" << std::endl;
                            isPieceSelected = true;
                            selectedPiece = &piece;
                            break;
                        }
                    }
                }
                // If a piece is selected, move it to the clicked square
                else {
                    if (selectedPiece) {
                        selectedPiece->row = row;
                        selectedPiece->col = col;
                        isPieceSelected = false;
                        selectedPiece = nullptr;
                    }
                }
            }
        }

        // Draw the board
        window.clear();
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
                square.setPosition(col * squareSize, row * squareSize);
                square.setFillColor((row + col) % 2 == 0 ? lightColor : darkColor);
                window.draw(square);
            }
        }

        // Draw pieces with their updated positions
        for (auto& piece : pieces) {
            piece.sprite.setPosition(piece.col * squareSize, piece.row * squareSize);
            window.draw(piece.sprite);
        }

        window.display();
    }

    return 0;
}
