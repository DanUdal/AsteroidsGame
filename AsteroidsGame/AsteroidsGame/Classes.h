#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string_cast.hpp>
#include <transform.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Player.h"
#include <sstream>
#include <string>

void waitForFrame();

class Asteroid
{
protected:
	glm::vec4 m_position;
	glm::vec4 m_velocity;
	SDL_Texture* m_texture;
public:
	Asteroid(glm::vec4 _startPos, float _velocity, SDL_Renderer* _renderer);
	~Asteroid()
	{

	}
	glm::vec4 getPosition() //getters for base asteroid
	{
		return m_position;
	}
	glm::vec4 getVelocity()
	{
		return m_velocity;
	}
	void movement()
	{
		m_position += m_velocity;
	}
	virtual int collision(Player _player);
	virtual int getRadius()
	{
		return 16;
	}
	SDL_Texture* getTexture()
	{
		return m_texture;
	}
};

class SpinAsteroid : public Asteroid
{
public:
	SpinAsteroid(glm::vec4 _startPos, float _velocity, glm::vec4 _direction, SDL_Renderer* _renderer) : Asteroid(_startPos, _velocity, _renderer)
	{
		m_velocity += _direction;
	}
};

class PointAsteroid : public Asteroid
{
public:
	int collision(Player _player);
	PointAsteroid(glm::vec4 _startPos, float _velocity, SDL_Renderer* _renderer) : Asteroid(_startPos, _velocity, _renderer)
	{
		SDL_Surface* m_surface;
		const char* filePtr = "Assets/PointAsteroid.bmp";
		m_surface = SDL_LoadBMP(filePtr);
		m_texture = SDL_CreateTextureFromSurface(_renderer, m_surface);
		SDL_FreeSurface(m_surface);
	}
};

class LargeAsteroid : public Asteroid
{
public:
	int collision(Player _player);
	LargeAsteroid(glm::vec4 _startPos, float _velocity, SDL_Renderer* _renderer) : Asteroid(_startPos, _velocity, _renderer)
	{
		SDL_Surface* m_surface;
		const char* filePtr = "Assets/LargeAsteroid.bmp";
		m_surface = SDL_LoadBMP(filePtr);
		m_texture = SDL_CreateTextureFromSurface(_renderer, m_surface);
		SDL_FreeSurface(m_surface);
	}
	int getRadius()
	{
		return 32;
	}
};

class GameClass
{
private:
	Player m_player;
	std::vector<Asteroid*> m_asteroids;
	int m_score;
	float timer;
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	TTF_Font* font;
public:
	GameClass()
	{
		m_score = 1000; //starting score for the player
		timer = 4.0f; //gives player 1 second before first asteroid spawn
		std::srand(std::time(NULL)); //generates random seed
		m_window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
		if (m_window == NULL)
		{

		}
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED); //sets up game window and renderer
		m_player.initialiseTexture(m_renderer); //initialises the player's texture
		font = TTF_OpenFont("arial.ttf", 25);
	}
	void gameLoop();
	void pointsChange()
	{
		m_score += std::round((500.0f - m_player.getDistance()) / 10.0f); //adds points each frame
	}
	void addPoints(int _points)
	{
		m_score += _points; //adds bonus points
	}
	void spawnAsteroids();
	void pause();
	void drawWindow();
	void mainMenu();
	void drawMainMenu();
	void drawDeathScreen();
};