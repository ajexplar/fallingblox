#include <SFML/Graphics.hpp>

#include <memory>
#include <utility>

class Tetromino {
    private:
        static int TILESIZE;
        static int OFFSET;

        std::vector< std::vector< std::vector<int> > > orientation;
        std::vector< std::unique_ptr<sf::Sprite> > sprites;
        sf::Color color;
        int currentRotation;
        int rotations;
        size_t topmost;
        size_t leftmost;
        int tempRot;
        int tempTop;
        int tempLeft;

        void setColor(char shape);
        void createSprites(sf::Texture& t);
        void prepareOrientations(char shape, int left);

    public:
        /* Constructor/Destructor */
        Tetromino(char shape, sf::Texture& t, int center);
        ~Tetromino();

        /* primitive access */
        sf::Color getColor() const { return color; }

        void repositionSprite(size_t i, size_t x, size_t y);
        sf::Sprite* getSprite(size_t i) { return sprites.at(i).get(); }

        void nextRotation() { currentRotation = (currentRotation + 1) % rotations; }
        int getCurrentRotation() const { return currentRotation; }

        size_t getLeftmost() const { return leftmost; }
        size_t getTopmost() const { return topmost; }
        void reduceTopmost() { topmost--; }
        void reduceLeftmost() { leftmost--; }
        void dropBlock() { topmost++; }
        void moveLeft() { leftmost--; }
        void moveRight() { leftmost++; }

        void tempRotate() { tempRot = (currentRotation + 1) % rotations; }
        int getTempRot() const { return tempRot; }
        void tempDrop() { tempTop = topmost + 1; }
        int getTempTop() const { return tempTop; }
        void tempLmove() { tempLeft = leftmost - 1; }
        void tempRmove() { tempLeft = leftmost + 1; }
        int getTempLeft() const { return tempLeft; }
        void resetTemp() { tempTop = tempLeft = tempRot = 0; }

        /* vector access */
        size_t getHeight() const { return orientation.at(currentRotation).size(); }
        size_t getWidth(size_t i) const { return orientation.at(currentRotation).at(i).size(); }
        void incrementBlock(size_t i, size_t j);
        int getFirst() const { return orientation.at(currentRotation).at(0).at(leftmost); }
        int getBlock(size_t i, size_t j) { return orientation.at(currentRotation).at(i).at(j); }
        size_t getTempHeight() const { return orientation.at(tempRot).size(); }
        size_t getTempWidth(size_t i) const { return orientation.at(tempRot).at(i).size(); }
        int getTempBlock(size_t i, size_t j) { return orientation.at(tempRot).at(i).at(j); }

};
