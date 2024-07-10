#include <SFML/Graphics.hpp>

class Player {

private:
	sf::RectangleShape player;
	sf::Vector2f position;
	float speed;
protected:

public:
	Player(){}
	Player(float spawnX, float spawnY) : position(spawnX, spawnY){}
	void move(sf::Vector2f direction);
	
};