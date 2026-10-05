#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int health);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;

    bool isAlive() const;
    bool hasReachedEnd() const;
    void takeDamage(int damage);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

private:
    std::vector<sf::Vector2f> path;
    int currentPoint;
    sf::Vector2f position;
    float speed;
    int health;
    int maxHealth;
    sf::RectangleShape shape;
};

#endif