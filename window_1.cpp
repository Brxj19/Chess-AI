#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include "GameState.h"
#include "move.h"
#include "AIMove.h"

std::vector<sf::Color> colors {sf::Color::White, sf::Color(0, 128, 120)};
int width = 640;
int height = width;
int dimension = 8;
int sq_size = width / dimension;

void load_images(std::map<std::string, sf::Texture> &images) {
    std::vector<std::string> pieceNames = {"wp", "wR", "wN", "wB", "wQ", "wK", "bp", "bR", "bN", "bB", "bQ", "bK"};

    for (const auto& piece : pieceNames) {
        sf::Texture texture;
        if (!texture.loadFromFile("images/" + piece + ".png")) {
            std::cerr << "Error loading image: " << "images/" + piece + ".png" << std::endl;
        }
        images[piece] = std::move(texture);
    }
}

void drawBoard(sf::RenderWindow &window) {
    for (int row = 0; row < dimension; ++row) {
        for (int col = 0; col < dimension; ++col) {
            sf::RectangleShape square(sf::Vector2f(sq_size, sq_size));
            square.setPosition(col * sq_size, row * sq_size);
            square.setFillColor((row + col) % 2 == 0 ? colors[0] : colors[1]);
            window.draw(square);
        }
    }
}

void drawPiece(sf::RenderWindow &window, const GameState &gs, const std::map<std::string, sf::Texture> &images) {
    for (int row = 0; row < dimension; ++row) {
        for (int col = 0; col < dimension; ++col) {
            std::string piece = gs.board[row][col];
            if (piece != "--") {
                sf::Sprite sprite;
                sprite.setTexture(images.at(piece));
                sprite.setPosition(col * sq_size, row * sq_size);
                window.draw(sprite);
            }
        }
    }
}

void highlightSquares(sf::RenderWindow &window, const GameState &gs, const std::vector<Move> &validMoves, const std::pair<int, int> &sqSelected) {
    if (sqSelected != std::make_pair(-1, -1)) {
        int row = sqSelected.first;
        int col = sqSelected.second;

        if (gs.board[row][col][0] == (gs.whiteToMove ? 'w' : 'b')) {
            sf::RectangleShape square(sf::Vector2f(sq_size, sq_size));
            square.setFillColor(sf::Color(0, 0, 255, 100));
            square.setPosition(col * sq_size, row * sq_size);
            window.draw(square);

            square.setFillColor(sf::Color(255, 255, 0, 100));
            for (const auto &move : validMoves) {
                if (move.startRow == row && move.startCol == col) {
                    square.setPosition(move.endCol * sq_size, move.endRow * sq_size);
                    window.draw(square);
                }
            }
        }
    }
}

void DrawGameState(sf::RenderWindow &window, const GameState &gs, const std::map<std::string, sf::Texture> &images, const std::vector<Move> &validMoves, const std::pair<int, int> &sqSelected) {
    drawBoard(window);
    highlightSquares(window, gs, validMoves, sqSelected);
    drawPiece(window, gs, images);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(width, height), "CHESS");
    std::map<std::string, sf::Texture> images;
    GameState gs;
    std::vector<Move> validMoves = gs.getValidMoves();
    std::pair<int, int> sqSelected = {-1, -1};

    load_images(images);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / sq_size;
                int row = mousePos.y / sq_size;

                if (sqSelected == std::make_pair(row, col)) {
                    sqSelected = std::make_pair(-1, -1);
                } else {
                    sqSelected = {row, col};
                }
            }
        }

        window.clear();
        DrawGameState(window, gs, images, validMoves, sqSelected);
        window.display();
    }

    return 0;
}
