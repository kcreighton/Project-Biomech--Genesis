#pragma once

#include <GLM/glm.hpp>
#include <Runengine2D/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 direction, float speed, int lifeTime);
	~Bullet();

	void draw(Runengine2D::SpriteBatch& spriteBatch);
	// returns true when we are out of life
	bool update();

private:
	int _lifeTime;
	float _speed;
	glm::vec2 _position;
	glm::vec2 _direction;
};

