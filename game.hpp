#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <memory>
#include <random>

#include "board.hpp"
#include "tetromino.hpp"

struct Scorer {
    std::string initials;
    int value;
};

class Game {
    private:
        static int PIECE_DROP_BONUS;
        static int ROW_CLEAR_BONUS;
        static int TILESIZE;
        static int OFFSET;

        std::vector<sf::Color> colors;
        std::vector<Scorer> scoreboard;
        std::vector<char> pieces;
        std::vector<char> upcoming;
        sf::Texture block_texture;
        sf::Font font;
        sf::Text scoreText;
        std::vector<sf::Text> allScores;
        std::mt19937 rng;
        std::unique_ptr<Board> well;
        std::unique_ptr<Tetromino> fallingShape;
        int score;
        int top;
        int left;
        bool no_texture;
        bool no_font;
        bool gameover;
        bool paused;
    public:
        /* Constructors/Destructor */
        Game();
        ~Game();

        void startNewGame();

        /* Viewing */
        void drawDebugBoard();
        void drawBoard(sf::RenderWindow& win);
        void drawNextBlock(sf::RenderWindow& win);
        void drawScore(sf::RenderWindow& win);

        /* Shape creation */
        void createNextShape();
        void createNextShape(char shape);
        void createUpcomingShapes(int amount);
        bool hasFallingShape() const { return fallingShape != nullptr; }

        /* Shape movement */
        void rotatePiece();
        void movePieceHorizontal(int pos);
        void movePieceDown();

        /* Scoring */
        void clearFilledRows();
        int getScore() const { return score; }
        void addToScoreboard(std::string nm, int sc);
        int validateScoreboard();
        void setInitials(int i, std::string nm);
        void displayScoreboard(sf::RenderWindow& win);

        /* Play modes */
        void pause() { paused = true; }
        void resume() { paused = false; }
        bool isPaused() const { return paused; }
        void checkForEndgame();
        bool isGameOver() { return gameover; }

        /* Resource loading */
        bool failedToLoadTexture() { return no_texture; }
        bool failedToLoadFont() { return no_font; }
};
