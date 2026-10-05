#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "Enemy.h"
#include "Player.h"
#include "Tower.h"
#include "Bullet.h"

// ========================== BEST SCORE ==========================
int loadBestScore() {
    std::ifstream file("best_score.txt");
    int score = 0;
    if (file.is_open()) file >> score;
    return score;
}

void saveBestScore(int score) {
    std::ofstream file("best_score.txt");
    if (file.is_open()) file << score;
}
// =================================================================

// Enemy spawn queue
struct SpawnQueue {
    std::vector<std::unique_ptr<Enemy>> queue;
    float spawnTimer;
    float spawnDelay;
    size_t spawnIndex;

    SpawnQueue() : spawnTimer(0), spawnDelay(1.5f), spawnIndex(0) {}
};

void prepareWave(SpawnQueue& spawnQueue, const std::vector<sf::Vector2f>& path, int wave) {
    spawnQueue.queue.clear();
    spawnQueue.spawnIndex = 0;
    spawnQueue.spawnTimer = 0;

    int basicCount = 3 + wave * 2;
    int fastCount = wave > 2 ? wave - 1 : 0;
    int tankCount = wave > 5 ? (wave - 4) : 0;

    for (int i = 0; i < basicCount; i++)
        spawnQueue.queue.push_back(std::make_unique<BasicEnemy>(path));
    for (int i = 0; i < fastCount; i++)
        spawnQueue.queue.push_back(std::make_unique<FastEnemy>(path));
    for (int i = 0; i < tankCount; i++)
        spawnQueue.queue.push_back(std::make_unique<TankEnemy>(path));
}

void updateSpawnQueue(SpawnQueue& spawnQueue, std::vector<std::unique_ptr<Enemy>>& enemies, float dt) {
    if (spawnQueue.spawnIndex >= spawnQueue.queue.size()) return;
    spawnQueue.spawnTimer += dt;

    if (spawnQueue.spawnTimer >= spawnQueue.spawnDelay) {
        enemies.push_back(std::move(spawnQueue.queue[spawnQueue.spawnIndex]));
        spawnQueue.spawnIndex++;
        spawnQueue.spawnTimer = 0;
    }
}

void updateEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, Player& player, float dt) {
    for (auto it = enemies.begin(); it != enemies.end();) {
        (*it)->update(dt);
        if ((*it)->isDead()) {
            player.addMoney((*it)->getReward());
            player.addScore((*it)->getReward() * 10);
            it = enemies.erase(it);
        }
        else if ((*it)->reachedEnd()) {
            player.loseLife((*it)->getDamage());
            it = enemies.erase(it);
        }
        else ++it;
    }
}

void updateTowers(std::vector<std::unique_ptr<Tower>>& towers,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    std::vector<std::unique_ptr<Bullet>>& bullets,
    float dt) {
    for (auto& tower : towers)
        tower->update(dt, enemies, bullets);
}

void updateBullets(std::vector<std::unique_ptr<Bullet>>& bullets, float dt) {
    for (auto it = bullets.begin(); it != bullets.end();) {
        (*it)->update(dt);
        if (!(*it)->isActive()) it = bullets.erase(it);
        else ++it;
    }
}

void drawEnemies(sf::RenderWindow& window, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (const auto& e : enemies) e->draw(window);
}

void drawTowers(sf::RenderWindow& window, const std::vector<std::unique_ptr<Tower>>& towers, bool showRange = false) {
    for (const auto& t : towers) t->draw(window, showRange);
}

void drawBullets(sf::RenderWindow& window, const std::vector<std::unique_ptr<Bullet>>& bullets) {
    for (const auto& b : bullets) b->draw(window);
}

void drawTowerShop(sf::RenderWindow& window, sf::Font& font, Player& player) {
    float startY = window.getSize().y - 120.f;
    sf::RectangleShape shopBg(sf::Vector2f(window.getSize().x, 120.f));
    shopBg.setPosition(0, startY);
    shopBg.setFillColor(sf::Color(50, 50, 50, 200));
    window.draw(shopBg);

    // Machine Gun
    sf::RectangleShape mg(sf::Vector2f(80.f, 80.f));
    mg.setPosition(50.f, startY + 20.f);
    mg.setFillColor(sf::Color(150, 150, 150));
    mg.setOutlineThickness(2.f);
    mg.setOutlineColor(sf::Color::White);
    window.draw(mg);

    sf::Text mgText;
    mgText.setFont(font);
    mgText.setString("Machine Gun\nCost: 100\nDamage: 10\nRate: Fast");
    mgText.setCharacterSize(12);
    mgText.setPosition(55.f, startY + 25.f);
    window.draw(mgText);

    // Cannon
    sf::RectangleShape cannon(sf::Vector2f(80.f, 80.f));
    cannon.setPosition(180.f, startY + 20.f);
    cannon.setFillColor(sf::Color(200, 100, 50));
    cannon.setOutlineThickness(2.f);
    cannon.setOutlineColor(sf::Color::White);
    window.draw(cannon);

    sf::Text cannonText;
    cannonText.setFont(font);
    cannonText.setString("Cannon\nCost: 200\nDamage: 50\nRate: Slow");
    cannonText.setCharacterSize(12);
    cannonText.setPosition(185.f, startY + 25.f);
    window.draw(cannonText);

    // Sniper
    sf::RectangleShape sniper(sf::Vector2f(80.f, 80.f));
    sniper.setPosition(310.f, startY + 20.f);
    sniper.setFillColor(sf::Color(50, 100, 200));
    sniper.setOutlineThickness(2.f);
    sniper.setOutlineColor(sf::Color::White);
    window.draw(sniper);

    sf::Text sniperText;
    sniperText.setFont(font);
    sniperText.setString("Sniper\nCost: 300\nDamage: 100\nRate: V.Slow");
    sniperText.setCharacterSize(12);
    sniperText.setPosition(315.f, startY + 25.f);
    window.draw(sniperText);

    // Glacio Tower (Ice Tower)
    sf::RectangleShape glacio(sf::Vector2f(80.f, 80.f));
    glacio.setPosition(440.f, startY + 20.f);
    glacio.setFillColor(sf::Color(100, 200, 255));
    glacio.setOutlineThickness(2.f);
    glacio.setOutlineColor(sf::Color::White);
    window.draw(glacio);

    sf::Text glacioText;
    glacioText.setFont(font);
    glacioText.setString("Glacio\nCost: 150\nDamage: 10\nSlow: 50%");
    glacioText.setCharacterSize(12);
    glacioText.setPosition(445.f, startY + 25.f);
    window.draw(glacioText);

    sf::Text instruction;
    instruction.setFont(font);
    instruction.setString("Press 1/2/3/4 to select tower | Click to place | ESC to cancel");
    instruction.setCharacterSize(14);
    instruction.setFillColor(sf::Color::Yellow);
    instruction.setPosition(550.f, startY + 50.f);
    window.draw(instruction);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Tower Defense Game");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/Arial.otf"))
        std::cout << "Warning: Could not load font\n";

    // PATH
    std::vector<sf::Vector2f> path = {
        {50,100},{300,100},{300,400},{600,400},
        {600,200},{900,200},{900,550},{1150,550}
    };

    sf::VertexArray pathLine(sf::LineStrip, path.size());
    for (size_t i = 0; i < path.size(); i++) {
        pathLine[i].position = path[i];
        pathLine[i].color = sf::Color(80, 80, 80);
    }

    std::vector<sf::RectangleShape> pathRects;
    for (size_t i = 0; i < path.size() - 1; i++) {
        sf::Vector2f s = path[i], e = path[i + 1];
        float w = std::abs(e.x - s.x) > std::abs(e.y - s.y) ? std::abs(e.x - s.x) : 60;
        float h = std::abs(e.y - s.y) > std::abs(e.x - s.x) ? std::abs(e.y - s.y) : 60;
        sf::RectangleShape rect(sf::Vector2f(w, h));
        rect.setPosition(std::min(s.x, e.x) - (w > 60 ? 0 : 30), std::min(s.y, e.y) - (h > 60 ? 0 : 30));
        rect.setFillColor(sf::Color(60, 60, 60));
        pathRects.push_back(rect);
    }

    Player player(200, 20);
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Bullet>> bullets;
    SpawnQueue spawnQueue;

    int selectedTower = 0;
    bool waveActive = false;
    bool towerPlacementMode = false;
    bool isPaused = false;
    int bestScore = loadBestScore();
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) isPaused = !isPaused;
                if (isPaused) continue;
                if (event.key.code == sf::Keyboard::Num1) { selectedTower = 1; towerPlacementMode = true; }
                if (event.key.code == sf::Keyboard::Num2) { selectedTower = 2; towerPlacementMode = true; }
                if (event.key.code == sf::Keyboard::Num3) { selectedTower = 3; towerPlacementMode = true; }
                if (event.key.code == sf::Keyboard::Num4) { selectedTower = 4; towerPlacementMode = true; }
                if (event.key.code == sf::Keyboard::Escape) { selectedTower = 0; towerPlacementMode = false; }
                if (event.key.code == sf::Keyboard::Space && enemies.empty() && !waveActive) {
                    prepareWave(spawnQueue, path, player.getWave());
                    waveActive = true;
                }
                if (event.key.code == sf::Keyboard::R) {
                    player.reset(); enemies.clear(); towers.clear(); bullets.clear();
                    spawnQueue.queue.clear(); spawnQueue.spawnIndex = 0;
                    waveActive = false; selectedTower = 0; towerPlacementMode = false;
                }
            }

            if (isPaused) continue;
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (towerPlacementMode && selectedTower > 0) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (mousePos.y < window.getSize().y - 120.f) {
                        bool onPath = false;
                        for (const auto& r : pathRects)
                            if (r.getGlobalBounds().contains(mousePos)) { onPath = true; break; }

                        if (!onPath) {
                            int cost = 0;
                            std::unique_ptr<Tower> newTower;
                            if (selectedTower == 1) {
                                cost = 100;
                                newTower = std::make_unique<MachineGunTower>(mousePos);
                            }
                            else if (selectedTower == 2) {
                                cost = 200;
                                newTower = std::make_unique<CannonTower>(mousePos);
                            }
                            else if (selectedTower == 3) {
                                cost = 300;
                                newTower = std::make_unique<SniperTower>(mousePos);
                            }
                            else if (selectedTower == 4) {
                                cost = 150;
                                newTower = std::make_unique<GlacioTower>(mousePos);
                            }

                            if (player.spendMoney(cost)) towers.push_back(std::move(newTower));
                        }
                    }
                }
            }
        }

        if (!player.isGameOver() && !isPaused) {
            updateSpawnQueue(spawnQueue, enemies, dt);
            updateEnemies(enemies, player, dt);
            updateTowers(towers, enemies, bullets, dt);
            updateBullets(bullets, dt);
            if (waveActive && enemies.empty() && spawnQueue.spawnIndex >= spawnQueue.queue.size()) {
                waveActive = false;
                player.nextWave();
                player.addMoney(50);
            }
        }

        window.clear(sf::Color(40, 40, 40));
        for (auto& r : pathRects) window.draw(r);
        window.draw(pathLine);
        drawTowers(window, towers, towerPlacementMode);

        drawEnemies(window, enemies);
        drawBullets(window, bullets);

        // Tower preview when placing
        if (towerPlacementMode && selectedTower > 0 && !isPaused) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::RectangleShape preview(sf::Vector2f(40.f, 40.f));
            preview.setOrigin(20.f, 20.f);
            preview.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

            // Color based on tower type
            if (selectedTower == 1) {
                preview.setFillColor(sf::Color(150, 150, 150, 150));
            }
            else if (selectedTower == 2) {
                preview.setFillColor(sf::Color(200, 100, 50, 150));
            }
            else if (selectedTower == 3) {
                preview.setFillColor(sf::Color(50, 100, 200, 150));
            }
            else if (selectedTower == 4) {
                preview.setFillColor(sf::Color(100, 200, 255, 150));
            }

            preview.setOutlineThickness(2.f);
            preview.setOutlineColor(sf::Color::White);

            // Draw range circle
            float range = 0;
            if (selectedTower == 1) range = 150.f;
            else if (selectedTower == 2) range = 200.f;
            else if (selectedTower == 3) range = 300.f;
            else if (selectedTower == 4) range = 150.f;

            sf::CircleShape rangePreview(range);
            rangePreview.setOrigin(range, range);
            rangePreview.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            rangePreview.setFillColor(sf::Color::Transparent);
            rangePreview.setOutlineThickness(2.f);
            rangePreview.setOutlineColor(sf::Color(255, 255, 255, 100));

            window.draw(rangePreview);
            window.draw(preview);
        }

        player.drawUI(window, font);
        drawTowerShop(window, font, player);

        // Best score
        sf::Text bestText;
        bestText.setFont(font);
        bestText.setString("Best: " + std::to_string(bestScore));
        bestText.setCharacterSize(18);
        bestText.setFillColor(sf::Color::Green);
        bestText.setPosition(1030.f, 110.f);
        window.draw(bestText);

        if (enemies.empty() && !waveActive && !player.isGameOver() && !isPaused) {
            sf::Text waveText;
            waveText.setFont(font);
            waveText.setString("Press SPACE to start Wave " + std::to_string(player.getWave()));
            waveText.setCharacterSize(24);
            waveText.setFillColor(sf::Color::Green);
            waveText.setPosition(400.f, 300.f);
            window.draw(waveText);
        }

        if (isPaused) {
            sf::Text paused;
            paused.setFont(font);
            paused.setString("PAUSED\nPress P to Resume");
            paused.setCharacterSize(36);
            paused.setFillColor(sf::Color::Yellow);
            paused.setStyle(sf::Text::Bold);
            paused.setPosition(400.f, 300.f);
            window.draw(paused);
        }

        if (player.isGameOver()) {
            if (player.getScore() > bestScore) {
                bestScore = player.getScore();
                saveBestScore(bestScore);
            }

            sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            sf::Text gameOver;
            gameOver.setFont(font);
            gameOver.setString(
                "GAME OVER!\n\nScore: " + std::to_string(player.getScore()) +
                "\nWave: " + std::to_string(player.getWave()) +
                "\nBest: " + std::to_string(bestScore) +
                "\n\nPress R to Restart"
            );
            gameOver.setCharacterSize(36);
            gameOver.setFillColor(sf::Color::Red);
            gameOver.setPosition(350.f, 250.f);
            window.draw(gameOver);
        }

        window.display();
    }
    return 0;
}