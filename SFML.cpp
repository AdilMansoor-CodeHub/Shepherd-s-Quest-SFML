#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text gameName;
    sf::Text playText;
    sf::Text exitText;

public:
    Menu(sf::RenderWindow& windowRef) : window(windowRef) {
        if (!font.loadFromFile("UbuntuSans-VariableFont_wdth,wght.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }

        // Set up play text
        gameName.setFont(font);
        gameName.setString("SHEPHERD'S QUEST");
        gameName.setCharacterSize(100);
        gameName.setFillColor(sf::Color::Green);
        gameName.setPosition(250, 100);

        // Set up play text
        playText.setFont(font);
        playText.setString("PLAY");
        playText.setCharacterSize(80);
        playText.setFillColor(sf::Color::White);
        playText.setPosition(550, 300);

        // Set up exit text
        exitText.setFont(font);
        exitText.setString("EXIT");
        exitText.setCharacterSize(80);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(550, 450);
    }

    // Draw the menu
    void draw() {
        window.draw(gameName);
        window.draw(playText);
        window.draw(exitText);
    }

    // Check if play button is clicked
    bool isPlayClicked(sf::Vector2f mousePos) {
        return playText.getGlobalBounds().contains(mousePos);
    }

    // Check if exit button is clicked
    bool isExitClicked(sf::Vector2f mousePos) {
        return exitText.getGlobalBounds().contains(mousePos);
    }
};

class Player {
private:
    float speed;


    int currentFrame; // Index of the current frame
    std::vector<sf::IntRect> frames; // Vector to store the rectangles representing each frame
    sf::Clock animationClock; // Clock to control the animation speed
    float animationSpeed; // Speed of the animation
public:
    sf::Vector2f playerPos;
    sf::Sprite playerSprite;
    Player() {}
    Player(sf::Texture& texture, int frameWidth, int frameHeight) : speed(1.0f), currentFrame(0), animationSpeed(0.1f) {
        playerPos = sf::Vector2f(200, 200);
        playerSprite.setTexture(texture);

        // Define rectangles for each frame and add them to the frames vector
        for (int i = 0; i < texture.getSize().x / frameWidth; ++i) {
            sf::IntRect frameRect(i * frameWidth, 0, frameWidth, frameHeight);
            frames.push_back(frameRect);
        }

        playerSprite.setTextureRect(frames[currentFrame]);
        playerSprite.setPosition(playerPos);
        playerSprite.setScale(3, 3);
    }

    void updatePos() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && playerSprite.getPosition().y > 5)
            playerPos.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerSprite.getPosition().y < 480)
            playerPos.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && playerSprite.getPosition().x > 205)
            playerPos.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && playerSprite.getPosition().x < 900)
            playerPos.x += speed;

        // Update animation frame
        if (animationClock.getElapsedTime().asSeconds() > animationSpeed) {
            currentFrame = (currentFrame + 1) % frames.size();
            playerSprite.setTextureRect(frames[currentFrame]);
            animationClock.restart();
        }

        playerSprite.setPosition(playerPos);
    }
};

class Sheep {
private:
    float speed;
    sf::Vector2f avoidDistance;
    sf::Vector2f distance;

    //Animation Shit
    int currentFrame; // Index of the current frame
    std::vector<sf::IntRect> frames; // Vector to store the rectangles representing each frame
    sf::Clock animationClock; // Clock to control the animation speed
    float animationSpeed; // Speed of the animation

public:
    bool sheepInBarn;
    sf::Vector2f sheepPos;
    sf::Sprite sheepSprite;

    Sheep(sf::Texture& texture, int frameWidth, int frameHeight) : speed(0.5f), avoidDistance(sf::Vector2f(100, 100)),
        sheepInBarn(false), currentFrame(0), animationSpeed(0.1f) {
        sheepPos = sf::Vector2f(350, 350);
        sheepSprite.setTexture(texture);

        // Define rectangles for each frame and add them to the frames vector
        for (int i = 0; i < texture.getSize().x / frameWidth; ++i) {
            sf::IntRect frameRect(i * frameWidth, 0, frameWidth, frameHeight);
            frames.push_back(frameRect);
        }

        sheepSprite.setTextureRect(frames[currentFrame]);
        sheepSprite.setPosition(sheepPos);
        sheepSprite.setScale(0.6, 0.6);
    }

    void sheepLogic(sf::Vector2f playerPos) {
        distance = playerPos - sheepPos;

        if (std::abs(distance.x) < avoidDistance.x && std::abs(distance.y) < avoidDistance.y) {
            if (distance.x > 0 && sheepPos.x > 205 && sheepPos.x < 900)
                sheepPos.x -= speed;
            else if (distance.x < 0 && sheepPos.x > 205 && sheepPos.x < 900)
                sheepPos.x += speed;

            if (distance.y > 0 && sheepPos.y > 40 && sheepPos.y < 500)
                sheepPos.y -= speed;
            else if (distance.y < 0 && sheepPos.y > 40 && sheepPos.y < 500)
                sheepPos.y += speed;

            if (sheepPos.x >= 900 && sheepPos.y > 270 && sheepPos.y < 370) {
                sheepInBarn = true;
            }

            // Update animation frame
            if (animationClock.getElapsedTime().asSeconds() > animationSpeed) {
                currentFrame = (currentFrame + 1) % frames.size();
                sheepSprite.setTextureRect(frames[currentFrame]);
                animationClock.restart();
            }

            sheepSprite.setPosition(sheepPos);
        }
    }
};

class Game {
public:
    Game() : window(sf::VideoMode(1307, 702), "SFML Square Controller", sf::Style::Close), showMenu(true), timerDuration(30.f), timerRunning(false), gameEnded(false) {
        initializeVariables();
    }

    void run() {
        while (window.isOpen()) {
            if (showMenu) {
                menuLoop();
            }
            else {
                gameLoop();
            }
        }
    }

private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture playerTexture;
    sf::Texture sheepTexture;

    Player player;
    std::vector<Sheep> sheeps;

    std::unique_ptr<Menu> menu;
    bool showMenu;

    float timerDuration;
    sf::Clock timerClock;
    bool timerRunning;
    sf::Text timerText;
    sf::Font font;

    sf::Text sheepCountText; // UI for displaying number of sheep in barn
    sf::Text winText; // "You Win" text
    sf::Text loseText; // "You Lose" text
    bool gameEnded;

    void initializeVariables() {
        backgroundTexture.loadFromFile("mincraft.png");
        backgroundSprite.setTexture(backgroundTexture);

        playerTexture.loadFromFile("Old_man_walk.png");
        player = Player(playerTexture, 48, 48);

        sheepTexture.loadFromFile("Sheepfinal.png");

        srand(static_cast<unsigned>(time(NULL)));

        for (int i = 0; i < 5; ++i) {
            Sheep sheep(sheepTexture, 167, 167);
            sheep.sheepPos = sf::Vector2f(rand() % (865 - 205) + 205, rand() % (505 - 85) + 85);
            sheep.sheepSprite.setPosition(sheep.sheepPos);
            sheeps.push_back(sheep);
        }

        showMenu = true;
        menu = std::make_unique<Menu>(window);

        if (!font.loadFromFile("UbuntuSans-VariableFont_wdth,wght.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }

        // Initialize timer UI
        timerText.setFont(font);
        timerText.setCharacterSize(40);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(20.f, 20.f);

        // Initialize sheep count UI
        sheepCountText.setFont(font);
        sheepCountText.setCharacterSize(30);
        sheepCountText.setFillColor(sf::Color::White);
        sheepCountText.setPosition(20.f, 70.f);

        // Initialize "You Win" text
        winText.setFont(font);
        winText.setString("You Win!");
        winText.setCharacterSize(100);
        winText.setFillColor(sf::Color::Green);
        sf::FloatRect winBounds = winText.getLocalBounds();
        winText.setOrigin(winBounds.left + winBounds.width / 2, winBounds.top + winBounds.height / 2);
        winText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        // Initialize "You Lose" text
        loseText.setFont(font);
        loseText.setString("You Lose!");
        loseText.setCharacterSize(100);
        loseText.setFillColor(sf::Color::Red);
        sf::FloatRect loseBounds = loseText.getLocalBounds();
        loseText.setOrigin(loseBounds.left + loseBounds.width / 2, loseBounds.top + loseBounds.height / 2);
        loseText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        gameEnded = false;
    }

    void menuLoop() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (menu->isPlayClicked(mousePos)) {
                    showMenu = false;
                    timerRunning = true;
                    timerClock.restart();
                    return;
                }
                else if (menu->isExitClicked(mousePos)) {
                    window.close();
                    return;
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        menu->draw();
        window.display();
    }

    void gameLoop() {
        if (timerRunning) {
            sf::Time elapsedTime = timerClock.getElapsedTime();
            float remainingTime = timerDuration - elapsedTime.asSeconds();
            if (remainingTime <= 0) {
                std::cout << "Time's up! You lose." << std::endl;
                gameEnded = true;
                timerRunning = false;
            }

            timerText.setString("Time: " + std::to_string(static_cast<int>(remainingTime)));
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.updatePos();
        int sheepInBarn = 0;
        for (auto& sheep : sheeps) {
            sheep.sheepLogic(player.playerPos);
            if (sheep.sheepInBarn) {
                sheepInBarn++;
            }
        }
        if (sheepInBarn == sheeps.size()) {
            std::cout << "Win\n";
            gameEnded = true;
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(player.playerSprite);
        for (auto& sheep : sheeps) {
            if (!sheep.sheepInBarn) {
                window.draw(sheep.sheepSprite);
            }
        }

        if (timerRunning) {
            window.draw(timerText);
        }

        // Update sheep count UI
        sheepCountText.setString("Sheep in barn: " + std::to_string(sheepInBarn));
        window.draw(sheepCountText);

        // Draw "You Win" or "You Lose" text if game has ended
        if (gameEnded) {
            if (sheepInBarn == sheeps.size()) {
                window.draw(winText);
            }
            else {
                window.draw(loseText);
            }
        }

        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
