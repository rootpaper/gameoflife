#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

const int CELL_SIZE = 10;
const int WIDTH = 80;
const int HEIGHT = 60;
const int WINDOW_WIDTH = WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = HEIGHT * CELL_SIZE;

class GameOfLife {
private:
    std::vector<std::vector<bool>> grid;
    std::vector<std::vector<bool>> nextGrid;

public:
    GameOfLife() : grid(HEIGHT, std::vector<bool>(WIDTH, false)),
                  nextGrid(HEIGHT, std::vector<bool>(WIDTH, false)) {
        randomize();
    }

    void randomize() {
        srand(time(nullptr));
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                grid[y][x] = rand() % 4 == 0;
            }
        }
    }

    void update() {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int neighbors = countNeighbors(x, y);
                
                if (grid[y][x]) {
                    nextGrid[y][x] = (neighbors == 2 || neighbors == 3);
                } else {
                    nextGrid[y][x] = (neighbors == 3);
                }
            }
        }
        
        grid.swap(nextGrid);
    }

    int countNeighbors(int x, int y) {
        int count = 0;
        
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                
                int nx = (x + dx + WIDTH) % WIDTH;
                int ny = (y + dy + HEIGHT) % HEIGHT;
                
                if (grid[ny][nx]) count++;
            }
        }
        
        return count;
    }

    bool isAlive(int x, int y) const {
        return grid[y][x];
    }

    void setCell(int x, int y, bool state) {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            grid[y][x] = state;
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Conway's Game of Life");
    window.setFramerateLimit(15);
    
    GameOfLife game;
    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
    bool paused = false;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    paused = !paused;
                } else if (event.key.code == sf::Keyboard::R) {
                    game.randomize();
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x / CELL_SIZE;
                    int y = event.mouseButton.y / CELL_SIZE;
                    game.setCell(x, y, true);
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    int x = event.mouseButton.x / CELL_SIZE;
                    int y = event.mouseButton.y / CELL_SIZE;
                    game.setCell(x, y, false);
                }
            }
        }
        
        if (!paused) {
            game.update();
        }
        
        window.clear(sf::Color(50, 50, 50));
        
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (game.isAlive(x, y)) {
                    cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    cellShape.setFillColor(sf::Color::White);
                    window.draw(cellShape);
                }
            }
        }
        
        window.display();
    }
    
    return 0;
}