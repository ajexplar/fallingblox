#include "game.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

int Game::PIECE_DROP_BONUS = 1;
int Game::ROW_CLEAR_BONUS = 100;
int Game::TILESIZE = 32;
int Game::OFFSET = 100;

Game::Game() {
    rng.seed(std::random_device()());

    std::unique_ptr<Board> b(new Board(12,16));
    well = std::move(b);
    score = 0;

    no_texture = false;
    no_font = false;

    if(!block_texture.loadFromFile("cube.png")) {
        std::cerr << "Unable to load texture. Closing...";
        no_texture = true;
    }

    if(!font.loadFromFile("gamefont.ttf")) {
        std::cerr << "Unable to load font. Closing...";
        no_font = true;
    }

    //fallingShape = nullptr;

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setColor(sf::Color::White);
    scoreText.setPosition(500,120);
    scoreText.setString(std::to_string(score));

    colors.push_back(sf::Color::Black);
    colors.push_back(sf::Color::White);
    colors.push_back(sf::Color::Red);
    colors.push_back(sf::Color::Green);
    colors.push_back(sf::Color::Blue);
    colors.push_back(sf::Color::Cyan);
    colors.push_back(sf::Color::Magenta);
    colors.push_back(sf::Color::Yellow);

    pieces.push_back('I');
    pieces.push_back('L');
    pieces.push_back('J');
    pieces.push_back('T');
    pieces.push_back('S');
    pieces.push_back('Z');
    pieces.push_back('O');

    paused = false;
    gameover = false;
}

Game::~Game() {
    /*delete well;
    if(fallingShape != nullptr) {
        delete fallingShape;
        fallingShape = nullptr;
    }*/
}

void Game::startNewGame() {
    //delete well;
    std::unique_ptr<Board> b(new Board(12,16));
    well = std::move(b);
    score = 0;
    scoreText.setString(std::to_string(score));
    /*
    if(fallingShape != nullptr) {
        delete fallingShape;
        fallingShape = nullptr;
    }
    */
    fallingShape.reset();
    upcoming.clear();
    paused = false;
    gameover = false;
    allScores.clear();
}

void Game::createNextShape() {
    if(!upcoming.empty()) {
        createNextShape(upcoming.at(upcoming.size()-1));
        upcoming.pop_back();
    }
}

void Game::createNextShape(char shape) {
    if(fallingShape == nullptr) {
        if(shape == 'I' || shape == 'L' || shape == 'T' || shape == 'S' || shape == 'J' || shape == 'Z') {
            std::unique_ptr<Tetromino> fs(new Tetromino(shape, block_texture, well->getWidth(0) / 2));
            fallingShape = std::move(fs);
        } else {
            std::unique_ptr<Tetromino> fs(new Tetromino('O', block_texture, well->getWidth(0) / 2));
            fallingShape = std::move(fs);
        }

        int zeroes = 0;
        for(size_t j = 0; j < fallingShape->getWidth(0); ++j) {
            if(fallingShape->getBlock(0,j) == 0) {
                zeroes++;
            }
        }
        if((unsigned) zeroes == fallingShape->getWidth(0)) {
            fallingShape->reduceTopmost();
        }

        zeroes = 0;
        for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
            if(fallingShape->getBlock(i,0) == 0) {
                zeroes++;
            }
        }
        if((unsigned) zeroes == fallingShape->getHeight()) {
            fallingShape->reduceLeftmost();
        }

        checkForEndgame();
    }
}

void Game::createUpcomingShapes(int amount) {
    if(upcoming.empty()) {
        if(amount == 0) {
            std::uniform_int_distribution<std::mt19937::result_type> dstr(0,7);
            upcoming.push_back(pieces.at(dstr(rng)));
        } else {
            while(amount > 0) {
                std::random_shuffle(pieces.begin(), pieces.end());
                for(size_t i = 0; i < pieces.size(); ++i) {
                    upcoming.push_back(pieces.at(i));
                }
                amount--;
            }
        }
    }
}

void Game::drawDebugBoard() {
    bool found = false;
    size_t horizontal = 0;
    size_t vertical = 0;

    if(fallingShape != nullptr) {
        top = fallingShape->getTopmost();
        left = fallingShape->getLeftmost();
        if(top < 0) {
            vertical = 1;
        } else {
            vertical = 0;
        }
        if(left < 0) {
            horizontal = 1;
        } else {
            horizontal = 0;
        }
    }

    for(size_t i = 0; i < well->getHeight(); ++i) {
        for(size_t j = 0; j < well->getWidth(i); ++j) {
            if(fallingShape != nullptr) {
                if(top+vertical == i && vertical < fallingShape->getHeight()) {
                    found = true;
                    if(left+horizontal == j && horizontal < fallingShape->getWidth(vertical)) {
                        if(fallingShape->getBlock(vertical, horizontal) != 0) {
                            std::cout << fallingShape->getBlock(vertical,horizontal) << " ";
                        } else {
                            std::cout << well->atIndex(i,j) << " ";
                        }
                        horizontal++;
                    } else {
                        std::cout << well->atIndex(i,j) << " ";
                    }
                } else {
                    std::cout << well->atIndex(i,j) << " ";
                }
            } else {
                std::cout << well->atIndex(i,j) << " ";
            }
        }

        if(found) {
            vertical++;
            if(left < 0) {
                horizontal = 1;
            } else {
                horizontal = 0;
            }
        }

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Game::drawBoard(sf::RenderWindow& win) {
    bool found = false;
    size_t horizontal = 0;
    size_t vertical = 0;
    sf::Color c;

    if(fallingShape != nullptr) {
        top = fallingShape->getTopmost();
        left = fallingShape->getLeftmost();
        c = fallingShape->getColor();
        if(top < 0) {
            vertical = 1;
        } else {
            vertical = 0;
        }
        if(left < 0) {
            horizontal = 1;
        } else {
            horizontal = 0;
        }
    }

    size_t blocks = 0;
    for(size_t i = 0; i < well->getHeight(); ++i) {
        for(size_t j = 0; j < well->getWidth(i); ++j) {
            if(fallingShape != nullptr) {
                if(top+vertical == i && vertical < fallingShape->getHeight()) {
                    found = true;
                    if(left+horizontal == j && horizontal < fallingShape->getWidth(vertical)) {
                        if(fallingShape->getBlock(vertical, horizontal) != 0) {
                            fallingShape->repositionSprite(blocks, j, i);
                            win.draw(*fallingShape->getSprite(blocks));
                            blocks++;
                        } else {
                            if(well->atIndex(i,j) != 0) {
                                sf::Sprite s;
                                s.setTexture(block_texture);
                                s.setPosition(j*TILESIZE+OFFSET,i*TILESIZE);
                                s.setColor(colors.at(well->atIndex(i,j)));
                                win.draw(s);
                            }
                        }
                        horizontal++;
                    } else {
                        if(well->atIndex(i,j) != 0) {
                            sf::Sprite s;
                            s.setTexture(block_texture);
                            s.setPosition(j*TILESIZE+OFFSET,i*TILESIZE);
                            s.setColor(colors.at(well->atIndex(i,j)));
                            win.draw(s);
                        }
                    }
                } else {
                    if(well->atIndex(i,j) != 0) {
                        sf::Sprite s;
                        s.setTexture(block_texture);
                        s.setPosition(j*TILESIZE+OFFSET,i*TILESIZE);
                        s.setColor(colors.at(well->atIndex(i,j)));
                        win.draw(s);
                    }
                }
            } else {
                if(well->atIndex(i,j) != 0) {
                    sf::Sprite s;
                    s.setTexture(block_texture);
                    s.setPosition(j*TILESIZE+OFFSET,i*TILESIZE);
                    s.setColor(colors.at(well->atIndex(i,j)));
                    win.draw(s);
                }
            }
        }

        if(found) {
            vertical++;
            if(left < 0) {
                horizontal = 1;
            } else {
                horizontal = 0;
            }
        }
    }
}

void Game::drawNextBlock(sf::RenderWindow& win) {
    if(!upcoming.empty()) {
        Tetromino t(upcoming.at(upcoming.size()-1), block_texture, 0);
        int blocks = 0;
        for(size_t i = 0; i < t.getHeight(); ++i) {
            for(size_t j = 0; j < t.getWidth(i); ++j) {
                if(t.getBlock(i,j) == 1) {
                    t.repositionSprite(blocks,14+i,8+j);
                    win.draw(*t.getSprite(blocks));
                    blocks++;
                }
            }
        }
    }
}

void Game::drawScore(sf::RenderWindow& win) {
    win.draw(scoreText);
}

void Game::rotatePiece() {
    if(fallingShape != nullptr) {
        bool fail = false;

        top = fallingShape->getTopmost();
        left = fallingShape->getLeftmost();

        fallingShape->tempRotate();

        for(size_t i = 0; i < fallingShape->getTempHeight(); ++i) {
            for(size_t j = 0; j < fallingShape->getTempWidth(i); ++j) {
                if(fallingShape->getTempBlock(i,j) == 1) {
                    int ti = i+top;
                    int lj = j+left;
                    if(ti < 0 || lj < 0 || (unsigned) ti >= well->getHeight() || (unsigned) lj >= well->getWidth(0)) {
                        fail = true;
                        break;
                    } else {
                        if((unsigned) ti < well->getHeight() && (unsigned) lj < well->getWidth(ti) && well->atIndex(ti,lj) >= 1) {
                            fail = true;
                            break;
                        }
                    }
                }
            }

            if(fail) {
                break;
            }
        }

        if(!fail) {
            fallingShape->nextRotation();
        }
        fallingShape->resetTemp();
    }
}

void Game::movePieceHorizontal(int pos) {
    if(fallingShape != nullptr) {
        bool halt = false;

        top = fallingShape->getTopmost();
        left = fallingShape->getLeftmost();

        for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
            for(size_t j = 0; j < fallingShape->getWidth(i); ++j) {
                if(fallingShape->getBlock(i,j) == 1) {
                    int ti = i+top;
                    int ljp = j+left;
                    if(ti >= 0 && (unsigned) ti < well->getHeight() && ljp >= 0) {
                        ljp += pos;
                        if(pos > 0) {
                            if((unsigned) ljp < well->getWidth(ti) && well->atIndex(ti,ljp) != 0) {
                                halt = true;
                                break;
                            }
                        } else {
                            if(ljp >= 0 && well->atIndex(ti,ljp) != 0) {
                                halt = true;
                                break;
                            }
                        }
                    }
                }
            }

            if(halt) {
                break;
            }
        }

        if(!halt) {
            int hpos = 0;
            int zeroes = 0;
            if(pos < 0) {
                fallingShape->tempLmove();
                hpos = fallingShape->getTempLeft();
                for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
                    if(fallingShape->getBlock(i,0) == 0) {
                        zeroes++;
                    }
                }
                if((unsigned) zeroes == fallingShape->getHeight()) {
                    hpos++;
                }
                if(hpos >= 0) {
                    fallingShape->moveLeft();
                }
            } else {
                fallingShape->tempRmove();
                hpos = fallingShape->getTempLeft();
                for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
                    if(fallingShape->getBlock(i,fallingShape->getWidth(0)-1) == 0) {
                        zeroes++;
                    }
                }
                if((unsigned) zeroes == fallingShape->getHeight()) {
                    hpos--;
                }
                if(hpos+fallingShape->getWidth(0) <= well->getWidth(0)) {
                    fallingShape->moveRight();
                }
            }
            fallingShape->resetTemp();
        }
    }
}

void Game::movePieceDown() {
    if(fallingShape != nullptr) {
        bool halt = false;

        top = fallingShape->getTopmost();
        left = fallingShape->getLeftmost();

        for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
            for(size_t j = 0; j < fallingShape->getWidth(i); ++j) {
                if(fallingShape->getBlock(i,j) == 1) {
                    int tip = i+top;
                    int lj = j+left;
                    if(lj >= 0 && (unsigned) lj < well->getWidth(i) && tip >= 0) {
                        tip++;
                        if((unsigned) tip < well->getHeight() && well->atIndex(tip, lj) != 0) {
                            halt = true;
                            break;
                        }
                    }
                }
            }

            if(halt) {
                break;
            }
        }

        fallingShape->tempDrop();

        int vpos = fallingShape->getTempTop();

        for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
            for(size_t j = 0; j < fallingShape->getWidth(i); ++j) {
                int vi = i+vpos;
                int lj = j+left;
                if(lj >= 0 && (unsigned) lj < well->getWidth(i) && vi >= 0) {
                    if(fallingShape->getBlock(i,j) == 1 && (unsigned) vi >= well->getHeight()) {
                        halt = true;
                        break;
                    }
                }
            }

            if(halt) {
                break;
            }
        }

        if(halt) {
            bool colorize;
            for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
                for(size_t j = 0; j < fallingShape->getWidth(i); ++j) {
                    if(fallingShape->getBlock(i,j) != 0) {
                        colorize = false;
                        for(size_t k = 0; k < colors.size(); ++k) {
                            if(colors.at(k) == fallingShape->getColor()) {
                                colorize = true;
                                well->setIndex(top+i,left+j,k);
                                break;
                            }
                        }
                        if(!colorize) { // shouldn't happen
                            well->setIndex(top+i,left+j,1);
                        }
                    }
                }
            }
            /*delete fallingShape;
            fallingShape = nullptr;
            */
            fallingShape.reset();
            score += Game::PIECE_DROP_BONUS;
            scoreText.setString(std::to_string(score));
        } else {
            fallingShape->resetTemp();
            fallingShape->dropBlock();
        }
    }
}

void Game::clearFilledRows() {
    for(size_t i = well->getHeight(); i-- > 0;) {
        size_t filled = 0;
        for(size_t j = 0; j < well->getWidth(i); ++j) {
            if(well->atIndex(i,j) >= 1) {
                filled++;
            }
        }
        if(filled == well->getWidth(i)) {
            score += Game::ROW_CLEAR_BONUS;
            scoreText.setString(std::to_string(score));
            int si = i;
            while(si > 0) {
                well->swapRow(si,si-1);
                si--;
            }
            well->clearRow(0);
        }
    }
}

void Game::addToScoreboard(std::string nm, int sc) {
    Scorer sx;
    sx.initials = nm;
    sx.value = sc;
    /*
    bool found = false;
    for(size_t i = 0; i < scoreboard.size(); ++i) {
        if(sx.value >= scoreboard.at(i).value) {
            scoreboard.insert(scoreboard.begin()+i,sx);
            found = true;
            break;
        }
    }
    if(!found) {
    }*/
    scoreboard.push_back(sx);
}

int Game::validateScoreboard() {
    Scorer snew;
    snew.initials = "   ";
    snew.value = score;
    int index = -1;
    for(size_t i = 0; i < scoreboard.size(); ++i) {
        if(i >= 5) {
            break;
        }
        if(snew.value >= scoreboard.at(i).value) {
            scoreboard.insert(scoreboard.begin()+i,snew);
            index = i;
            break;
        }
    }

    if(index == -1 && scoreboard.size() < 5) {
        scoreboard.push_back(snew);
        index = scoreboard.size()-1;
    }
    return index;
}

void Game::setInitials(int i, std::string nm) {
    scoreboard.at(i).initials = nm;
}

void Game::displayScoreboard(sf::RenderWindow& win) {
    if(allScores.empty()) {
        std::ofstream outFile;
        outFile.open("scoreboard.txt", std::ofstream::out | std::ofstream::trunc);
        for(size_t i = 0; i < scoreboard.size(); ++i) {
            if(i >= 5) {
                break;
            }
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(20);
            t.setColor(sf::Color::White);
            t.setPosition(250,240+i*20);
            std::string s = scoreboard.at(i).initials + " " + std::to_string(scoreboard.at(i).value);
            t.setString(s);
            allScores.push_back(t);
            outFile << s << "\n";
        }
        outFile.close();
    }
    for(size_t i = 0; i < allScores.size(); ++i) {
        if(i >= 5) {
            break;
        }
        win.draw(allScores.at(i));
    }

}

void Game::checkForEndgame() {
    top = fallingShape->getTopmost();
    left = fallingShape->getLeftmost();
    for(size_t i = 0; i < fallingShape->getHeight(); ++i) {
        for(size_t j = 0; j < fallingShape->getWidth(i); ++j) {
            if(fallingShape->getBlock(i,j) == 1) {
                int tip = i+top;
                int lj = j+left;
                if(lj >= 0 && (unsigned) lj < well->getWidth(i) && tip >= 0) {
                    tip++;
                    if((unsigned) tip < well->getHeight() && well->atIndex(tip, lj) != 0) {
                        gameover = true;
                        return;
                    }
                }
            }
        }
    }
}
