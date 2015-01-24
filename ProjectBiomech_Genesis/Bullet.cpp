#include "Bullet.h"
#include <random>
#include <Runengine2D/ResourceManager.h>

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime):
	_lifeTime(lifeTime),
	_position(position),
	_direction(direction),
	_speed(speed)
{
}


Bullet::~Bullet()
{
}

void Bullet::draw(Runengine2D::SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Runengine2D::GLTexture texture = Runengine2D::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Runengine2D::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update()
{
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0)
	{
		return true;
	}
	return false;
}
