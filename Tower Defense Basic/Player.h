#ifndef PLAYER_H
#define PLAYER_H 

#include <SFML/Graphics.hpp> 
#include <string>         

class Player {
private:
    int money;    // Số tiền hiện tại của người chơi (dùng để mua tháp)
    int score;    // Điểm số hiện tại của người chơi
    int lives;    // Máu còn lại của người chơi (Thường là máu của căn cứ)
    int wave;     // Số màn (wave) hiện tại đang chơi
    int bestScore;// Điểm số cao nhất đã đạt được

public:
    // Hàm khởi tạo (Constructor)
    // Đặt giá trị mặc định cho tiền (200) và mạng (5)
    Player(int startMoney = 200, int startLives = 5);

    void addMoney(int amount);      // Thêm tiền (khi tiêu diệt kẻ thù)
    bool spendMoney(int amount);    // Trừ tiền (khi mua tháp). Trả về true nếu đủ tiền và chi tiêu thành công
    int getMoney() const { return money; } // Trả về số tiền hiện tại (const: hàm không thay đổi trạng thái lớp)

    void addScore(int points);      // Thêm điểm (khi tiêu diệt kẻ thù)
    int getScore() const { return score; }      // Trả về điểm hiện tại
    int getBestScore() const { return bestScore; } // Trả về điểm cao nhất
    void updateBestScore();         // Cập nhật điểm cao nhất nếu score hiện tại lớn hơn

    // Các hàm liên quan đến mạng
    void loseLife(int amount = 1);  // Giảm máu (mặc định giảm 1). Xảy ra khi kẻ thù lọt qua đích
    int getLives() const { return lives; } // Trả về số máu còn lại
    bool isGameOver() const { return lives <= 0; } // Kiểm tra xem trò chơi đã kết thúc chưa

    void nextWave(); // Tăng số màn lên 1 (chuyển sang màn tiếp theo)
    int getWave() const { return wave; } // Trả về số màn hiện tại

    // Các hàm liên quan đến Giao diện (UI)
    void drawUI(sf::RenderWindow& window, sf::Font& font); // Hàm vẽ giao diện người chơi (Tiền, Máu, Màn, Điểm)

    void saveBestScore(const std::string& filename = "best_score.txt"); // Lưu điểm cao nhất vào file
    void loadBestScore(const std::string& filename = "best_score.txt"); // Tải điểm cao nhất từ file

    void reset(); // Đặt lại các thuộc tính về giá trị ban đầu để bắt đầu game mới
};

#endif