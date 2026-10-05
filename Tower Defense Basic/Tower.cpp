#include  "Tower.h"
#include <cmath>

Tower::Tower(sf::Vector2f pos, float rng, float dmg, float rate, int cst)
    : position(pos), range(rng), damage(dmg), fireRate(rate), cost(cst),
    fireTimer(0), currentTarget(nullptr) {

    shape.setSize(sf::Vector2f(40.f, 40.f));
    shape.setOrigin(20.f, 20.f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(100, 100, 100));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    rangeCircle.setRadius(range);
    rangeCircle.setOrigin(range, range);
    rangeCircle.setPosition(position);
    rangeCircle.setFillColor(sf::Color::Transparent);
    rangeCircle.setOutlineThickness(2.f);
    rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 100));
}

void Tower::update(float dt, std::vector<std::unique_ptr<Enemy>>& enemies,
    std::vector<std::unique_ptr<Bullet>>& bullets) {

    fireTimer += dt;

    // Find target
    currentTarget = findTarget(enemies);

    // Shoot if has target and ready
    if (currentTarget && fireTimer >= fireRate) {
        shoot(bullets);
        fireTimer = 0;
    }
}

void Tower::draw(sf::RenderWindow& window, bool showRange) {
    if (showRange) {
        window.draw(rangeCircle);
    }
    window.draw(shape);
}

Enemy* Tower::findTarget(std::vector<std::unique_ptr<Enemy>>& enemies) {
    Enemy* closest = nullptr;
    float closestDist = range + 1;

    for (auto& enemy : enemies) {
        if (enemy->isDead() || enemy->reachedEnd()) {
            continue;
        }

        sf::Vector2f enemyPos = enemy->getPosition();
        float dx = enemyPos.x - position.x;
        float dy = enemyPos.y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range && dist < closestDist) {
            closest = enemy.get();
            closestDist = dist;
        }
    }

    return closest;
}

void Tower::shoot(std::vector<std::unique_ptr<Bullet>>& bullets) {
    if (currentTarget) {
        bullets.push_back(std::make_unique<Bullet>(position, currentTarget, damage, 300.f));
    }
}

// MachineGunTower
MachineGunTower::MachineGunTower(sf::Vector2f pos)
    : Tower(pos, 150.f, 10.f, 0.2f, 100) {
    shape.setFillColor(sf::Color(150, 150, 150));
}

// CannonTower
CannonTower::CannonTower(sf::Vector2f pos)
    : Tower(pos, 200.f, 50.f, 1.0f, 200) {
    shape.setFillColor(sf::Color(200, 100, 50));
}

// SniperTower
SniperTower::SniperTower(sf::Vector2f pos)
    : Tower(pos, 300.f, 100.f, 2.0f, 300) {
    shape.setFillColor(sf::Color(50, 100, 200));
}

// GlacioTower
GlacioTower::GlacioTower(sf::Vector2f pos)
    : Tower(pos, 150.f, 10.f, 0.5f, 150), slowPercent(0.5f), slowDuration(1.0f) {
    shape.setFillColor(sf::Color(100, 200, 255)); // Màu xanh băng
}

void GlacioTower::shoot(std::vector<std::unique_ptr<Bullet>>& bullets) {
    if (currentTarget) {
        auto bullet = std::make_unique<Bullet>(position, currentTarget, damage, 300.f);
        // Đặt màu cho đạn băng (màu xanh)
        bullet->setColor(sf::Color(100, 200, 255));
        bullets.push_back(std::move(bullet));
    }
}
