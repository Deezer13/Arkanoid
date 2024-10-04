# Arkanoid Game

Arkanoid is a classic arcade-style game where the objective is to complete levels by destroying targets with a bouncing ball. This game currently features 3 levels and is developed for Linux x64.

## Features

- **Levels**: 3 unique levels to challenge your skills.
- **Controls**:
  - Move the racket left: `a`
  - Move the racket right: `d`
  - Launch the ball: `w`
  - Quit the game: `q`

## Requirements

- Linux x64
- GCC compiler

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/Deezer13/Arkanoid.git
   cd Arkanoid/
2. Compile the program using GCC:

   ```bash
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/bin
   g++ -o arkanoid arkanoid.c irrKlangWrapper.cpp -Lbin -lIrrKlang
3. Run the game:

   ```bash
   ./arkanoid

## Gameplay
Aim to hit the targets and clear the levels. If you miss the ball, it will fall.

## License

This project is licensed under the GNU General Public License (GPL) v3. See the [LICENSE](LICENSE) file for more information.

## Contributing

If you'd like to contribute to the project, feel free to open issues or submit pull requests.
Acknowledgements

    Inspired by the classic Arkanoid game.
    Thanks to all contributors and the open-source community.

Enjoy the game!
