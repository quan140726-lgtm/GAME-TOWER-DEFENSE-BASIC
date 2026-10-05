#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
protected:
    // Hiển thị
    sf::Sprite sprite;
    sf::Texture texture;
    bool hasTexture = false;
    sf::CircleShape shape;

    // Di chuyển
    sf::Vector2f position;
    std::vector<sf::Vector2f> path;
    size_t currentWaypoint = 0;

    // Thông số
    float health;
    float maxHealth;
    float speed;
    float originalSpeed; // Tốc độ gốc trước khi bị làm chậm
    int reward;
    int damage;
    bool dead = false;

    // Slow effect
    float slowPercent = 0.0f;
    float slowDuration = 0.0f;
    float slowTimer = 0.0f;

    // Animation
    float frameWidth = 150.f;
    float frameHeight = 150.f;
    int frameCount = 8;
    float frameTime = 0.3f;
    int currentFrame = 0;
    float animationTimer = 0.f;

public:
    Enemy(const std::vector<sf::Vector2f>& pathPoints, float hp, float spd, int money, int dmg);
    virtual ~Enemy() = default;

    virtual void update(float dt);
    virtual void draw(sf::RenderWindow& window);

    void takeDamage(float dmg);
    void applySlow(float percent, float duration);
    bool loadTexture(const std::string& filename);

    bool isDead() const { return dead; }
    bool reachedEnd() const { return currentWaypoint >= path.size(); }
    sf::Vector2f getPosition() const { return position; }
    int getReward() const { return reward; }
    int getDamage() const { return damage; }
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
};

class BasicEnemy : public Enemy {
public:
    BasicEnemy(const std::vector<sf::Vector2f>& pathPoints);
};

class FastEnemy : public Enemy {
public:
    FastEnemy(const std::vector<sf::Vector2f>& pathPoints);
};

class TankEnemy : public Enemy {
public:
    TankEnemy(const std::vector<sf::Vector2f>& pathPoints);
};

#endif
