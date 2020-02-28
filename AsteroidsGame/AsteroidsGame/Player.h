#pragma once
#include <SDL.h>
#include <string_cast.hpp>
#include <transform.hpp>

class Player
{
private:
	glm::vec4 m_position;
	glm::mat4 m_movement;
	float m_velocity;
	float m_distance;
	int m_health;
	
	SDL_Texture* m_texture;
public:
	Player();
	
	~Player();
	
	void initialiseTexture(SDL_Renderer* _renderer);
	
	glm::vec4 getPosition(); //getters for player. setters are not needed
	
	float getDistance();
	float getVelocity();
	int getHealth();
	void movement();
	void damage();
	void jump();
	void dive();
	SDL_Texture* getTexture();
	void reset();
};


