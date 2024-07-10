#pragma once
//Game Engine Class
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>


class Game
{
private:
	//Variables

	//Windows
	sf::RenderWindow window;
	sf::Event ev;
	sf::VideoMode VideoMode;

	sf::Texture backGorundText;
	sf::Sprite backGround;
	//private functions
	void initializeVariables();
	void initializeWindow();
public:

	//Constructors / Destructors
	Game();
	virtual ~Game();


	//Accessors
	const bool gameRunning() const;


	//public functions
	void PollEvents();
	void Update();
	void Render();
};