#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <array>
#include <fstream>

#include "game.hpp"

int main() {
    int width = 800;
    int height = 600;
    sf::RenderWindow window(sf::VideoMode(width,height), "Falling Blox");
    window.setFramerateLimit(60);

    sf::Event event;

    /*
    sf::Sprite sp1;
    sp1.setTexture(t1);
    sp1.setPosition(width / 3, 0);

    sf::Texture t2;
    if(!t2.loadFromFile("stage.png")) {
        std::cerr << "Unable to load texture. Closing...";
        return 1;
    }

    sf::Sprite sp2;
    sp2.setTexture(t2);
    sp2.scale(0.40,0.40);
    sp2.setPosition(width / 4, height - 100);
    */

    sf::Texture score_texture;
    if(!score_texture.loadFromFile("score_label.png")) {
        std::cerr << "Unable to load texture. Closing...";
        return 1;
    }

    sf::Sprite scr_lab;
    scr_lab.setTexture(score_texture);
    scr_lab.setPosition(width-300, 50);

    sf::Texture gameover_texture;
    if(!gameover_texture.loadFromFile("gameover.png")) {
        std::cerr << "Unable to load texture. Closing...";
        return 1;
    }

    sf::Sprite gopanel;
    gopanel.setTexture(gameover_texture);
    gopanel.setPosition(width/3-180, height/4-115);

    sf::Texture paused_texture;
    if(!paused_texture.loadFromFile("paused.png")) {
        std::cerr << "Unable to load texture. Closing...";
        return 1;
    }

    sf::Sprite pausepanel;
    pausepanel.setTexture(paused_texture);
    pausepanel.setPosition(width/3-180, height/4-115);

    sf::Texture highscore_texture;
    if(!highscore_texture.loadFromFile("highscores.png")) {
        std::cerr << "Unable to load texture. Closing...";
        return 1;
    }

    sf::Sprite highscore_sp;
    highscore_sp.setTexture(highscore_texture);
    highscore_sp.setPosition(120, 175);

    sf::Texture nm_input;
    if(!nm_input.loadFromFile("name_input.png")) {
        std::cerr << "Unable to load texture. Closing...";
        return 1;
    }

    sf::Sprite input_box;
    input_box.setTexture(nm_input);
    input_box.setPosition(120, 220);

    sf::VertexArray wellPerimeter(sf::LinesStrip, 4);
    wellPerimeter[0].position = sf::Vector2f(100,0);
    wellPerimeter[1].position = sf::Vector2f(100,510);
    wellPerimeter[2].position = sf::Vector2f(485,510);
    wellPerimeter[3].position = sf::Vector2f(485,0);

    sf::Music bgm;
    if(!bgm.openFromFile("b.wav")) {
        std::cerr << "Unable to load music. Closing...";
        return 1;
    }
    bgm.setLoop(true);

    sf::SoundBuffer sbuf;
    if(!sbuf.loadFromFile("lb.wav")) {
        std::cerr << "Unable to load sound. Closing...";
        return 1;
    }

    sf::Sound clearRowSound;
    clearRowSound.setBuffer(sbuf);

    Game game;

    if(game.failedToLoadTexture() || game.failedToLoadFont()) {
        return 1;
    }

    sf::Font font;
    if(!font.loadFromFile("gamefont.ttf")) {
        std::cerr << "Unable to load font. Closing...";
    }

    std::ifstream inFile;
    inFile.open("scoreboard.txt", std::ifstream::in);

    if(!inFile) {
        std::cerr << "Cannot open scoreboard file. It may not exist." << std::endl;
    } else {
        std::string nm;
        int sc;
        int scores = 0;
        inFile >> nm;
        while(!inFile.eof() && scores < 5) {
            if(nm != "\n") {
                inFile >> sc;
                game.addToScoreboard(nm,sc);
                scores++;
            }
            inFile >> nm;
        }
        inFile.close();
    }

    bool clicked = false;
    char shape = '_';

    sf::Clock clock;
    size_t level = 0;
    std::array<double, 7> dropTimes = { 1.5, 1, 1.25, 1.1, 0.9, 0.8, 0.7 };
    std::array<int, 7> shuffles = { 5, 4, 4, 3, 2, 1, 0 };
    std::array<int, 7> difficulty = { 500, 1000, 1400, 1900, 2300, 2600, 3000};
    std::string mode = "release";
    std::string initials = "";
    int addedScore = -10;

    sf::Text name;
    name.setFont(font);
    name.setCharacterSize(20);
    name.setColor(sf::Color::White);
    name.setPosition(325, 250);
    name.setString(initials);

    while(window.isOpen()) {
        clicked = false;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(!clicked) {
                if(!game.isGameOver() && !game.isPaused()) {
                    if(mode == "debug") {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
                            clicked = true;
                            shape = 'I';
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                            clicked = true;
                            shape = 'L';
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                            clicked = true;
                            shape = 'J';
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                            clicked = true;
                            shape = 'T';
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                            clicked = true;
                            shape = 'S';
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                            clicked = true;
                            shape = 'Z';
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
                            clicked = true;
                            shape = 'O';
                        }
                    } 

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        clicked = true;
                        game.rotatePiece();
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                        clicked = true;
                        game.pause();
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        clicked = true;
                        game.movePieceHorizontal(-1);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        clicked = true;
                        game.movePieceHorizontal(1);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        clicked = true;
                        game.movePieceDown();
                    }
                } else {
                    if(addedScore >= 0) {
                        if(initials.length() == 3) {
                            game.setInitials(addedScore, initials);
                            addedScore = -1;
                            initials = "";
                        } else {
                            if(event.type == sf::Event::TextEntered) {
                                if(static_cast<char>(event.text.unicode) == '\b') {
                                    initials.pop_back();
                                }
                                if(event.text.unicode > 32 && event.text.unicode < 128) {
                                    initials += static_cast<char>(event.text.unicode);
                                }
                                name.setString(initials);
                            }
                        }
                    } else {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                            if(!game.isGameOver()) {
                                clicked = true;
                                game.resume();
                            }
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                            clicked = true;
                            game.startNewGame();
                            level = 0;
                            addedScore = -10;
                            initials = "";
                            name.setString(initials);
                        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                            clicked = true;
                            window.close();
                        }
                    }
                }
            }
        }

        if(!game.isGameOver() && !game.isPaused()) {
            if(bgm.getStatus() != sf::Music::Playing) {
                bgm.play();
            }
            if(mode == "debug" && shape != '_') {
                game.createNextShape(shape);
                shape = '_';
            }
            if(mode == "debug" && clicked) {
                game.drawDebugBoard();
            }

            int oscore = game.getScore();
            game.clearFilledRows();
            if(game.getScore() > oscore) {
                clearRowSound.play();
            }
            game.createUpcomingShapes(shuffles[level]);

            sf::Time elapsed = clock.getElapsedTime();
            if(elapsed.asSeconds() > dropTimes[level]) {
                game.movePieceDown();
                clock.restart();
            }

            if(!game.hasFallingShape()) {
                game.createNextShape();
            }

            if(game.getScore() > difficulty[level] && level < difficulty.size()) {
                level++;
            }
        } else {
            bgm.pause();
        }

        window.clear(sf::Color::Black);
        game.drawBoard(window);
        game.drawNextBlock(window);
        window.draw(scr_lab);
        game.drawScore(window);
        if(game.isGameOver()) {
            bgm.stop();
            window.draw(gopanel);
            int sidx = -1;
            if(addedScore == -10) {
                sidx = game.validateScoreboard();
                addedScore = sidx;
            }

            if(sidx >= 0 || addedScore >= 0) {
                window.draw(input_box);
                window.draw(name);
            } 
            if(addedScore < 0) {
                window.draw(highscore_sp);
                game.displayScoreboard(window);
            }
        }
        if(game.isPaused()) {
            window.draw(pausepanel);
        }
        window.draw(wellPerimeter);
        window.display();
    }
    return 0;
}
