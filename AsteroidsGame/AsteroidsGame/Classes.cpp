#include "Classes.h"

Asteroid::Asteroid(glm::vec4 _startPos, float _velocity, SDL_Renderer* _renderer) : m_position(_startPos)
{
	SDL_Surface* m_surface;
	const char* filePtr = "Assets/BaseAsteroid.bmp";
	m_velocity = glm::normalize(glm::vec4(320, 240, 0, 1) - _startPos);
	m_velocity *= _velocity; //sets the velocity of the asteroid using a unit vector towards the star and a magnitude
	m_surface = SDL_LoadBMP(filePtr);
	if (m_surface == NULL)
	{
		std::cout << "Error in getting base texture \n";
	}

	SDL_SetColorKey(m_surface, SDL_TRUE, SDL_MapRGB(m_surface->format, 255, 255, 255));
	m_texture = SDL_CreateTextureFromSurface(_renderer, m_surface); //loads in the asteroid's bitmap
	SDL_FreeSurface(m_surface);
	if (m_texture == NULL)
	{
		std::cout << "CreateTextureFromSurface failed: \n" << SDL_GetError();
	}
}

void GameClass::drawDeathScreen()
{
	SDL_RenderClear(m_renderer); //clears the previous frame from the screen
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect dstrect;
	SDL_Color colour = { 255,255,255 };
	std::string test;
	std::stringstream sstr;
	sstr << (m_score - 1000); //uses string stream to convert integer to a string
	test = sstr.str();
	dstrect.x = 220;
	dstrect.y = 160;
	dstrect.h = 80;
	dstrect.w = 200;
	surface = TTF_RenderText_Solid(font, "YOUR FINAL SCORE WAS", colour); //creates surface from string
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect); //copies string into renderer
	dstrect.x = 280;
	dstrect.y = 240;
	dstrect.h = 80;
	dstrect.w = 80;
	surface = TTF_RenderText_Solid(font, test.c_str(), colour);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect);
	SDL_RenderPresent(m_renderer); //draws everything copied into the renderer
}

void GameClass::drawMainMenu()
{
	SDL_RenderClear(m_renderer); //clears the previous frame from the screen
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect dstrect;
	const char* filePtr = "Assets/BeginButton.bmp";
	surface = SDL_LoadBMP(filePtr);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Surface* surface2;
	dstrect.x = 220;
	dstrect.y = 140;
	dstrect.h = 80;
	dstrect.w = 200;
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect);
	filePtr = "Assets/QuitButton.bmp";
	surface2 = SDL_LoadBMP(filePtr);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface2);
	SDL_FreeSurface(surface2);
	dstrect.x = 220;
	dstrect.y = 340;
	dstrect.h = 80;
	dstrect.w = 200;
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect);
	SDL_RenderPresent(m_renderer);
}

void GameClass::mainMenu()
{
	bool menu(true);
	SDL_Event input;
	this->drawMainMenu();
	while (menu)
	{
		while (SDL_PollEvent(&input)) //handles all events in the queue
		{
			switch (input.type)
			{
			case SDL_MOUSEBUTTONDOWN: //tests the location of the button press to find which button was clicked
				if ((input.button.x > 220) && (input.button.x < 420) && (input.button.y > 140) && (input.button.y < 220)) //begin button
				{
					this->gameLoop(); //starts the game
					SDL_Delay(1000); //freezes for 1 second on the frame the player died
					this->drawDeathScreen(); //displays the final score
					SDL_Delay(3000); //shows the final score for 3 seconds
					m_player.reset(); //resets the game to its initial values for a second play
					m_score = 1000;
					for (unsigned int i = 0; i < size(m_asteroids); i++) //unsigned to remove an annoying warning
					{
						m_asteroids.erase(m_asteroids.begin()); //deletes the asteroid from the vector
						i--;
					}
					timer = 4.0f;
					this->drawMainMenu(); //redraws the main menu
				}
				if ((input.button.x > 220) && (input.button.x < 420) && (input.button.y > 340) && (input.button.y < 420)) //quit button
				{
					menu = false; //exits the game
				}
				break;
			case SDL_QUIT: menu = false; //ends game if window is closed
				break;
			}
		}
	}
}

void GameClass::gameLoop()
{
	SDL_Event input; //event manager
	bool gameEnded(false); //game loop controller
	int test, jump(0), dive(0);
	while (!gameEnded)//main game loop
	{
		while (SDL_PollEvent(&input)) //handles all events in the queue
		{
			switch (input.type)
			{
			case SDL_KEYDOWN:
				if ((input.key.keysym.sym == SDLK_SPACE) && (jump == 0))
				{
					jump = 60; //primary gameplay function
				}
				if ((input.key.keysym.sym == SDLK_LSHIFT) && (dive == 0))
				{
					dive = 60; //primary gameplay function
				}
				if (input.key.keysym.sym == SDLK_ESCAPE)
				{
					this->pause(); //causes the game to pause
				}
				break;
			case SDL_QUIT: gameEnded = true; //ends game if window is closed
				break;
			}
		}
		if (jump > 0)
		{
			m_player.jump(); //makes the player jump 60 times, once per frame
			jump -= 1;
		}
		if (dive > 0)
		{
			m_player.dive(); //makes the player dive 60 times, once per frame
			dive -= 1;
		}
		m_player.movement(); //processes any changes for the frame after event handling
		for (unsigned int i = 0; i < size(m_asteroids); i++) //unsigned to remove an annoying warning
		{
			m_asteroids.at(i)->movement();
			test = m_asteroids.at(i)->collision(m_player);
			switch (test) //process any collision detected
			{
			case 0: m_player.damage();
				m_asteroids.at(i)->~Asteroid();
				m_asteroids.erase(m_asteroids.begin() + i); //deletes the asteroid from the vector
				i--;
				break;
			case 1: m_asteroids.at(i)->~Asteroid();
				m_asteroids.erase(m_asteroids.begin() + i);
				i--;
				break;
			case 2: addPoints(300); //adds bonus points for a point asteroid
				m_asteroids.at(i)->~Asteroid();
				m_asteroids.erase(m_asteroids.begin() + i);
				i--;
				break;
			}
		}
		if (m_player.getHealth() == 0)
		{
			gameEnded = true; //ends the game is the player has no hp left
		}
		this->spawnAsteroids(); //spawns any new asteroids for the frame
		this->drawWindow(); //draws the frame then locks at 60 FPS
		timer += 1.0f / 60.0f; //adds to the asteroid spawn timer
		waitForFrame(); //locks game at 60 FPS
	}
}

void GameClass::pause() //this function runs until the game is unpaused to hold the game loop where it is
{
	SDL_Event input;
	bool unpause(false);
	SDL_RenderClear(m_renderer); //clears the previous frame from the screen
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect dstrect;
	const char* filePtr = "Assets/Unpause.bmp"; //gets the bitmap for the unpause button
	surface = SDL_LoadBMP(filePtr);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	dstrect.x = 220;
	dstrect.y = 200;
	dstrect.h = 80;
	dstrect.w = 200;
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect); //draws the unpause button
	SDL_RenderPresent(m_renderer);
	while (!unpause) //pauses processing and event handling in game loop
	{
		while (SDL_PollEvent(&input))
		{
			switch (input.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if ((input.button.x > 220) && (input.button.x < 420) && (input.button.y > 200) && (input.button.y < 280)) //tests for clicks on the unpause button
				{
					unpause = true;
				}
				break;
			case SDL_KEYDOWN:
				if (input.key.keysym.sym == SDLK_ESCAPE) //escape unpauses the game
				{
					unpause = true; //return to game when escape is pressed
				}
				break;
			case SDL_QUIT: unpause = true; //ends game if window is closed
				break;
			}
		}
	}
}

void waitForFrame()
{
	static unsigned int lastTime = 0; //making unsigned allows for double the positive values so that ticks can be calculated higher
	unsigned int current = SDL_GetTicks();
	float deltaTs = (float)(current - lastTime) / 1000.0f;
	lastTime = current;
	if (deltaTs < (1.0f / 60.0f))
	{
		SDL_Delay((unsigned int)(((1.0f / 60.0f) - deltaTs)*1000.0f));
	}
}

void GameClass::spawnAsteroids()
{
	glm::vec4 spawnPoint(0, 0, 0, 1);
	float angle, speed;
	int type;
	if (timer > 5.0f) //limits asteroid spawns to once every 5 seconds
	{
		for (int i = 0; i < (m_score / 1000); i++) //spawns a score dependent number of asteroids
		{
			angle = (float)(rand()) / ((float)(RAND_MAX / 360.0f));
			spawnPoint.x = 320 - (300 * cos(angle));
			spawnPoint.y = 240 - (300 * sin(angle)); //generates random spawn point
			speed = 0.5 + ((float)(rand()) / ((float)(RAND_MAX / 2.0f))); //generates random speed
			type = rand() % 10; //generates random type of asteroid
			if (type < 4)
			{
				Asteroid* asteroid = new Asteroid(spawnPoint, speed, m_renderer);
				m_asteroids.push_back(asteroid);
			}
			else if (type < 8)
			{
				LargeAsteroid* asteroid = new LargeAsteroid(spawnPoint, speed, m_renderer);
				m_asteroids.push_back(asteroid);
			}
			else
			{
				PointAsteroid* asteroid = new PointAsteroid(spawnPoint, speed, m_renderer);
				m_asteroids.push_back(asteroid);
			}
		}
		timer = 0; //resets spawn timer
		this->pointsChange();
	}
}

void GameClass::drawWindow()
{
	SDL_RenderClear(m_renderer); //clears the previous frame from the screen
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect dstrect;
	const char* filePtr = "Assets/Star.bmp"; //loads in star bitmap
	for (int i = 0; i < size(m_asteroids); i++) //loads in the bitmap of each asteroid and adds them to the renderer
	{
		texture = m_asteroids.at(i)->getTexture();
		dstrect.x = m_asteroids.at(i)->getPosition().x - m_asteroids.at(i)->getRadius();
		dstrect.y = m_asteroids.at(i)->getPosition().y - m_asteroids.at(i)->getRadius();
		dstrect.h = m_asteroids.at(i)->getRadius() * 2;
		dstrect.w = m_asteroids.at(i)->getRadius() * 2;
		SDL_RenderCopy(m_renderer, texture, NULL, &dstrect);
	}
	texture = m_player.getTexture();
	dstrect.x = m_player.getPosition().x - 16;
	dstrect.y = m_player.getPosition().y - 16;
	dstrect.h = 32;
	dstrect.w = 32;
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect); //gets the bitmap for the player and adds it to the renderer
	surface = SDL_LoadBMP(filePtr);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	if (texture == NULL)
	{
		std::cout << "CreateTextureFromSurface failed: \n" << SDL_GetError();
	}
	dstrect.x = 280;
	dstrect.y = 200;
	dstrect.h = 80;
	dstrect.w = 80;
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect); //adds the star bitmap to the renderer
	SDL_Color colour = { 255,255,255 };
	std::string test;
	std::stringstream sstr;
	sstr << (m_score - 1000); //creates string from integer score (-1000 to start actual score from 0)
	test = sstr.str();
	surface = TTF_RenderText_Solid(font, test.c_str(), colour);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect); //draws the score over the star
	dstrect.x = 0;
	dstrect.y = 0;
	sstr.str(""); //empties the string stream
	sstr << m_player.getHealth(); //inserts health into string stream to convert it to string
	test = sstr.str();
	colour = { 255,0,0 };
	surface = TTF_RenderText_Solid(font, test.c_str(), colour);
	texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_RenderCopy(m_renderer, texture, NULL, &dstrect); //draws the player's health in the top left corner in red
	SDL_RenderPresent(m_renderer); //draws all the bitmaps to the screen
	SDL_FreeSurface(surface);
}

int Asteroid::collision(Player _player)
{
	if (glm::distance(m_position, _player.getPosition()) < 32.0f) //if distance between centres is less than sum of radii, collision
	{
		return 0; //return player collision
	}
	if (glm::distance(m_position, glm::vec4(320, 240, 0, 1)) < 56.0f)
	{
		return 1; //return star collision
	}
	return -1; //return no collision
}

int PointAsteroid::collision(Player _player)
{
	if (glm::distance(m_position, _player.getPosition()) < 32.0f) //if distance between centres is less than sum of radii, collision
	{
		return 2; //return player collision
	}
	if (glm::distance(m_position, glm::vec4(320, 240, 0, 1)) < 56.0f)
	{
		return 1; //return star collision
	}
	return -1; //return no collision
}

int LargeAsteroid::collision(Player _player)
{
	if (glm::distance(m_position, _player.getPosition()) < 48.0f) //if distance between centres is less than sum of radii, collision
	{
		return 0; //return player collision
	}
	if (glm::distance(m_position, glm::vec4(320, 240, 0, 1)) < 72.0f)
	{
		return 1; //return star collision
	}
	return -1; //return no collision
}