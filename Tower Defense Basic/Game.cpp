#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, int health)
    : path(path), currentPoint(0), speed(speed), health(health), maxHealth(health) {

    if (!path.empty()) {
        position = path[0];
    }

    shape.setSize(sf::Vector2f(20, 20));
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(10, 10);
    shape.setPosition(position);
}

void Enemy::update(float deltaTime) {
    if (currentPoint >= path.size() - 1 || health <= 0) return;

    sf::Vector2f direction = path[currentPoint + 1] - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        currentPoint++;
        if (currentPoint >= path.size() - 1) return;
        direction = path[currentPoint + 1] - position;
        distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    }

    direction /= distance;
    position += direction * speed * deltaTime;
    shape.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) const {
    if (health > 0) {
        window.draw(shape);

        // Vẽ thanh máu
        if (health < maxHealth) {
            float healthRatio = static_cast<float>(health) / maxHealth;
            sf::RectangleShape healthBar(sf::Vector2f(20 * healthRatio, 3));
            healthBar.setFillColor(sf::Color::Green);
            healthBar.setPosition(position.x - 10, position.y - 15);
            window.draw(healthBar);
        }
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

bool Enemy::hasReachedEnd() const {
    return currentPoint >= path.size() - 1;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}