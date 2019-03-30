#include "tetromino.hpp"

int Tetromino::TILESIZE = 32;
int Tetromino::OFFSET = 100;

Tetromino::Tetromino(char shape, sf::Texture& t, int center) {
    setColor(shape);
    createSprites(t);
    prepareOrientations(shape,center);
}

Tetromino::~Tetromino() {
    /*
    for(size_t i = 0; i < 4; ++i) {
        delete sprites.at(i);
    }
    */
}

void Tetromino::setColor(char shape) {
    switch (shape) {
        case 'T':
            color = sf::Color::Red;
            break;
        case 'L':
            color = sf::Color::Cyan;
            break;
        case 'J':
            color = sf::Color::Magenta;
            break;
        case 'Z':
            color = sf::Color::Green;
            break;
        case 'S':
            color = sf::Color::White;
            break;
        case 'I':
            color = sf::Color::Blue;
            break;
        default: // 'O'
            color = sf::Color::Yellow;
            break;
    }
}

void Tetromino::createSprites(sf::Texture& t) {
    for(size_t i = 0; i < 4; i++) {
        std::unique_ptr<sf::Sprite> s(new sf::Sprite(t));
        s->setColor(color);
        s->setPosition(0,0);
        sprites.push_back(std::move(s));
    }
}

void Tetromino::prepareOrientations(char shape, int left) {
    currentRotation = 0;
    leftmost = left-2;
    topmost = 0;
    tempLeft = 0;
    tempTop = 0;
    tempRot = 0;
    switch (shape) {
        case 'T':
            rotations = 4;
            orientation.push_back({
                    {0,0,0},
                    {1,1,1},
                    {0,1,0}});
            orientation.push_back({
                    {0,1,0},
                    {1,1,0},
                    {0,1,0}});
            orientation.push_back({
                    {0,1,0},
                    {1,1,1},
                    {0,0,0}});
            orientation.push_back({
                    {0,1,0},
                    {0,1,1},
                    {0,1,0}});
            break;
        case 'L':
            rotations = 4;
            orientation.push_back({
                    {0,1,0},
                    {0,1,0},
                    {0,1,1}});
            orientation.push_back({
                    {0,0,0},
                    {1,1,1},
                    {1,0,0}});
            orientation.push_back({
                    {1,1,0},
                    {0,1,0},
                    {0,1,0}});
            orientation.push_back({
                    {0,0,1},
                    {1,1,1},
                    {0,0,0}});
            break;
        case 'J':
            rotations = 4;
            orientation.push_back({
                    {0,1,0},
                    {0,1,0},
                    {1,1,0}});
            orientation.push_back({
                    {1,0,0},
                    {1,1,1},
                    {0,0,0}});
            orientation.push_back({
                    {0,1,1},
                    {0,1,0},
                    {0,1,0}});
            orientation.push_back({
                    {0,0,0},
                    {1,1,1},
                    {0,0,1}});
            break;
        case 'Z':
            rotations = 2;
            orientation.push_back({
                    {0,1,0},
                    {1,1,0},
                    {1,0,0}});
            orientation.push_back({
                    {0,0,0},
                    {1,1,0},
                    {0,1,1}});
            break;
        case 'S':
            rotations = 2;
            orientation.push_back({
                    {0,1,0},
                    {0,1,1},
                    {0,0,1}});
            orientation.push_back({
                    {0,0,0},
                    {0,1,1},
                    {1,1,0}});
        case 'I':
            rotations = 2;
            orientation.push_back({
                    {1},
                    {1},
                    {1},
                    {1}});
            orientation.push_back({
                    {1,1,1,1}});
            break;
        default: // 'O'
            rotations = 1;
            orientation.push_back({
                    {1,1},
                    {1,1}});
            break;

    }
} 

void Tetromino::repositionSprite(size_t i, size_t x, size_t y) {
    sprites.at(i)->setPosition(x*TILESIZE+OFFSET,y*TILESIZE);
}

void Tetromino::incrementBlock(size_t i, size_t j) {
    if(orientation.at(currentRotation).at(i).at(j) != 0) {
        orientation.at(currentRotation).at(i).at(j)++; 
    }
}
