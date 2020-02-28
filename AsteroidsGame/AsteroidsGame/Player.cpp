#include "Player.h"
#include <iostream>


	Player::Player()
	{
		m_position = glm::vec4(470, 240, 0, 1); //starting position is 150 from the star in the x axis
		m_health = 10; //the number of hits the player can take
		m_velocity = 30.0f; //initial velocity. velocity is in degrees per second
		m_distance = 150.0f;
		m_movement = glm::rotate(m_movement, glm::radians(m_velocity / 60.0f), glm::vec3(0, 0, 1)); //generates the first rotation matrix
	}
	Player::~Player()
	{

	}
	void Player::initialiseTexture(SDL_Renderer* _renderer)
	{
		SDL_Surface* m_surface;
		const char* filePtr = "Assets/Player.bmp";
		m_surface = SDL_LoadBMP(filePtr);
		if (m_surface == NULL)
		{
			std::cout << "Error in getting player texture";
		}
		m_texture = SDL_CreateTextureFromSurface(_renderer, m_surface);
		SDL_FreeSurface(m_surface);
		if (m_texture == NULL)
		{
			std::cout << "CreateTextureFromSurface failed: \n" << SDL_GetError();
		} //loads in the player's bitmap
	}
	glm::vec4 Player::getPosition() //getters for player. setters are not needed
	{
		return m_position;
	}
	float Player::getDistance()
	{
		return m_distance;
	}
	float Player::getVelocity()
	{
		return m_velocity;
	}
	int Player::getHealth()
	{
		return m_health;
	}
	void Player::movement()
	{
		m_position -= glm::vec4(320, 240, 0, 1);
		m_position = m_movement * m_position;
		m_position += glm::vec4(320, 240, 0, 1); //moves the player relative to the origin, rotates them then moves them back
		m_position *= 1 - (0.02f / 60.0f);
		m_distance *= 1 - (0.02f / 60.0f);
		m_velocity += 1.5f / 60.0f; //reducing distance from star and increasing velocity each frame
		m_movement = glm::rotate(glm::mat4(1), glm::radians(m_velocity / 60.0f), glm::vec3(0, 0, 1)); //generates new rotation matrix
		if (glm::distance(m_position, glm::vec4(320, 240, 0, 1)) < 56.0f)
		{
			m_health = 0; //kills the player if they hit the star
		}
		if (m_velocity < 0.0f)
		{
			m_velocity = 0.0f;
		}
	}
	void Player::damage()
	{
		m_health -= 1;
	}
	void Player::jump()
	{
		if (m_distance < 240.0f)
		{
			m_distance += 20.0f / 60.0f;
			m_position += glm::normalize((m_position - glm::vec4(320, 240, 0, 1))) * 20.0f / 60.0f;
			m_velocity -= 15.0f / 60.0f; //adds distance each frame to make the jump last 1 second
		}
		if (m_velocity < 0.0f)
		{
			m_velocity = 0.0f;
		}
	}
	void Player::dive()
	{
		m_distance -= 20.0f / 60.0f;
		m_position -= glm::normalize((m_position - glm::vec4(320, 240, 0, 1))) * 20.0f / 60.0f;
		m_velocity += 15.0f / 60.0f; //subtracts distance each frame to make the dive last 1 second
	}
	SDL_Texture* Player::getTexture()
	{
		return m_texture;
	}
	void Player::reset()
	{
		m_position = glm::vec4(470, 240, 0, 1); //starting position is 150 from the star in the x axis
		m_health = 10; //the number of hits the player can take
		m_velocity = 30.0f; //initial velocity. velocity is in degrees per second
		m_distance = 150.0f;
		m_movement = glm::rotate(m_movement, glm::radians(m_velocity / 60.0f), glm::vec3(0, 0, 1));
	}
