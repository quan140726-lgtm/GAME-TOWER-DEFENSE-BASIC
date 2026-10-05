#include "Player.h"
#include <fstream>

// PHẦN 1: HÀM KHỞI TẠO VÀ CẬP NHẬT THUỘC TÍNH
Player::Player(int startMoney, int startLives)
    : money(200), score(0), lives(startLives), wave(1), bestScore(0) {
    loadBestScore(); // Tải điểm cao nhất khi khởi tạo game
}

// Thêm tiền
void Player::addMoney(int amount) {
    if (amount > 0) {
        money += amount;
    }
}

// Chi tiêu tiền
bool Player::spendMoney(int amount) {
    if (amount <= money && amount > 0) {
        money -= amount;
        return true;
    }
    return false;
}

// Thêm điểm
void Player::addScore(int points) {
    if (points > 0) {
        score += points;
    }
}

// Cập nhật điểm cao nhất
void Player::updateBestScore() {
    if (score > bestScore) {
        bestScore = score;
        saveBestScore(); // Tự động lưu khi cập nhật
    }
}

// Mất máu/Mạng
void Player::loseLife(int amount) {
    lives -= amount;
    if (lives < 0) {
        lives = 0;
    }
}

// Chuyển sang màn (Wave) tiếp theo
void Player::nextWave() {
    wave++;
}

// Đặt lại trạng thái trò chơi về mặc định
void Player::reset() {
    money = 200;
    score = 0;
    lives = 5;
    wave = 1;
    // Không reset bestScore vì cần giữ lại điểm cao nhất
}

// PHẦN 2: LƯU/TẢI ĐIỂM CAO NHẤT
void Player::saveBestScore(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << bestScore;
        file.close();
    }
}

void Player::loadBestScore(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> bestScore;
        file.close();
    }
    else {
        bestScore = 0; // Nếu file không tồn tại, đặt bestScore = 0
    }
}

// PHẦN 3: HÀM VẼ GIAO DIỆN NGƯỜI CHƠI (DRAW UI)
void Player::drawUI(sf::RenderWindow& window, sf::Font& font) {
    const float panelWidth = 280.f;
    const float panelHeight = 130.f;
    const float margin = 5.f;

    sf::Vector2f panelPosition(window.getSize().x - panelWidth - margin, margin);

    // 1. Vẽ khung nền UI
    sf::RectangleShape uiPanel(sf::Vector2f(panelWidth, panelHeight));
    uiPanel.setPosition(panelPosition);
    uiPanel.setFillColor(sf::Color(0, 0, 0, 180));
    uiPanel.setOutlineThickness(2.f);
    uiPanel.setOutlineColor(sf::Color::White);
    window.draw(uiPanel);

    // 2. Hiển thị Tiền
    sf::Text moneyText;
    moneyText.setFont(font);
    moneyText.setString("$ " + std::to_string(money));
    moneyText.setCharacterSize(36);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setStyle(sf::Text::Bold);
    moneyText.setPosition(panelPosition.x + 15.f, panelPosition.y + 10.f);
    window.draw(moneyText);

    // 3. Hiển thị Điểm số
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(panelPosition.x + 15.f, panelPosition.y + 55.f);
    window.draw(scoreText);

    // 4. Hiển thị Mạng
    sf::Text livesText;
    livesText.setFont(font);
    livesText.setString("Lives: " + std::to_string(lives));
    livesText.setCharacterSize(18);
    livesText.setFillColor(sf::Color::Red);
    livesText.setStyle(sf::Text::Bold);
    livesText.setPosition(panelPosition.x + 15.f, panelPosition.y + 80.f);
    window.draw(livesText);

    // 5. Hiển thị Màn chơi
    sf::Text waveText;
    waveText.setFont(font);
    waveText.setString("Wave: " + std::to_string(wave));
    waveText.setCharacterSize(18);
    waveText.setFillColor(sf::Color::Cyan);
    waveText.setPosition(panelPosition.x + 15.f, panelPosition.y + 105.f);
    window.draw(waveText);
}