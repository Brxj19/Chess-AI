# Chess AI

This project is a chess game with an AI opponent, built using C++ and the SFML library for graphics and audio. The game allows a human player to play against an AI, with features such as move validation, move animation, and sound effects.

## Project Structure
```
chessAI
|-- assets
|    |-- audio
|    |    |-- move.wav
|    |    |-- capture.wav
|    |    |-- check.wav
|    |    |-- checkmate.wav
|    |-- piece_sets
|-- fonts
|    |--helvetica.ttf
|-- includes
|    |-- AIMove.h
|    |-- castlingRights.h
|    |-- GameState.h
|    |-- move.h
├── src
│   ├── AIMove.cpp       # Implementation of ai move generator
│   ├── castlingRights.cpp  # Implementation of castling rights
│   ├── GameState.cpp     # Implementation of game state
│   ├── move.cpp     # Implementation of game state
├── tests
│   ├── AIMoveTest.cpp    # Unit tests for the AI move generated
│   ├── GameStateTest.cpp  # Unit tests for Gamestate
├── CMakeLists.txt       # CMake configuration file
└── README.md            # Documentation for the project
└── run.txt            # instructions for running this in windows mac and linux system
└── windows-toolchain.cmake

```

## Dependencies

- SFML (Simple and Fast Multimedia Library)
- C++17

## Installation

### Step 1: Install GCC/g++ Compiler

1. Download MinGW from the MinGW-w64 project download page.
2. Run the downloaded installer and follow the instructions to install MinGW.
3. Add MinGW to the system PATH.

### Step 2: Download and Install SFML

1. Download SFML from the SFML download page.
2. Extract the downloaded SFML package to a directory of your choice.
3. Set up your development environment to use SFML.

### Step 3: Clone the Repository

```sh
git clone https://github.com/yourusername/chessAI.git
cd chessAI

## Step 4: Build the Project

```sh
mkdir build
cd build
cmake ..
make
```

## Running the Game

After building the project, you can run the game using the following command:

```sh
./window
```

## Features

- **Move Validation**: Ensures that only legal moves are allowed.
- **Move Animation**: Animates the movement of pieces on the board.
- **Sound Effects**: Plays sound effects for moves, captures, castling, and check.
- **AI Opponent**: Play against an AI that can make moves based on a simple algorithm.

## Usage

- **Mouse Click**: Click on a piece to select it, then click on a destination square to move it.
- **Undo Move**: Press the `Z` key to undo the last move.
- **Drag and Drop**: Drag a piece to move it (currently commented out in the code).

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
```
