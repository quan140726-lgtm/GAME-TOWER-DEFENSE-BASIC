#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f startPos, Enemy * targetEnemy, float dmg, float spd)
    : position(startPos), target(targetEnemy), damage(dmg), speed(spd), active(true) {

    shape.setRadius(4.f);
    shape.setOrigin(4.f, 4.f);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(position);

    // Calculate initial velocity towards target
    if (target && !target->isDead()) {
        sf::Vector2f direction = target->getPosition() - position;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            velocity = direction / length * speed;
        }
    }
}

void Bullet::update(float dt) {
    if (!active) return;

    // Update velocity to track target
    if (target && !target->isDead()) {
        sf::Vector2f direction = target->getPosition() - position;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            velocity = direction / length * speed;
        }
    }
    else {

        // Target is dead, deactivate bullet
        active = false;
        return;
    }

    position += velocity * dt;
    shape.setPosition(position);

    // Check if hit target
    if (checkHit()) {
        target->takeDamage(damage);
        // Nếu là đạn của GlacioTower (màu xanh), áp dụng hiệu ứng làm chậm
        if (shape.getFillColor() == sf::Color(100, 200, 255)) {
            target->applySlow(0.5f, 1.0f); // Làm chậm 50% trong 1 giây
        }
        active = false;
    }
}

void Bullet::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(shape);
    }
}

bool Bullet::checkHit() {
    if (!target || target->isDead()) {
        return false;
    }

    sf::Vector2f targetPos = target->getPosition();
    float dx = position.x - targetPos.x;
    float dy = position.y - targetPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance < 15.f; // Hit if within enemy radius
}
