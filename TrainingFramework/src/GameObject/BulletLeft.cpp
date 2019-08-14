#include "BulletLeft.h"


BulletLeft::BulletLeft(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture)
	:Sprite2D(model, shader, texture)
{
	m_speed = 500.0;
	m_active = true;
	m_type = BULLET_TYPELEFT::None;
	m_SizeCollider = 10;
	m_Damage = 5;
}

BulletLeft::~BulletLeft()
{
}

void BulletLeft::Update(GLfloat deltatime)
{
	if (!m_active)
		return;
	Vector2 pos = Get2DPosition();
	pos.y = pos.y - m_speed * deltatime;
	pos.x = pos.x - 75 * deltatime;
	Set2DPosition(pos);

	if (pos.y <= 0 || pos.y > Application::screenHeight)
		m_active = false;
}

bool BulletLeft::IsActive()
{
	return m_active;
}

void BulletLeft::SetActive(bool status)
{
	m_active = status;
}

void BulletLeft::SetSpeed(float speed)
{
	m_speed = speed;
}

void BulletLeft::SetType(BULLET_TYPELEFT type)
{
	m_type = type;
}

BULLET_TYPELEFT BulletLeft::GetType()
{
	return m_type;
}

void BulletLeft::SetColliderSize(float size)
{
	m_SizeCollider = size;
}
float BulletLeft::GetColliderSize()
{
	return m_SizeCollider;
}

void BulletLeft::SetDamage(float damage)
{
	m_Damage = damage;
}

float BulletLeft::GetDamage()
{
	return m_Damage;
}

