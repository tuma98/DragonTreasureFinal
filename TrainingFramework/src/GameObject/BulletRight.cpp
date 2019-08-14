#include "BulletRight.h"


BulletRight::BulletRight(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture)
	:Sprite2D(model, shader, texture)
{
	m_speed = 500.0;
	m_active = true;
	m_type = BULLET_TYPERIGHT::None;
	m_SizeCollider = 10;
	m_Damage = 5;
}

BulletRight::~BulletRight()
{
}

void BulletRight::Update(GLfloat deltatime)
{
	if (!m_active)
		return;
	Vector2 pos = Get2DPosition();
	pos.y = pos.y - m_speed * deltatime;
	pos.x = pos.x + 75 * deltatime;
	Set2DPosition(pos);

	if (pos.y <= 0 || pos.y > Application::screenHeight)
		m_active = false;
}

bool BulletRight::IsActive()
{
	return m_active;
}

void BulletRight::SetActive(bool status)
{
	m_active = status;
}

void BulletRight::SetSpeed(float speed)
{
	m_speed = speed;
}

void BulletRight::SetType(BULLET_TYPERIGHT type)
{
	m_type = type;
}

BULLET_TYPERIGHT BulletRight::GetType()
{
	return m_type;
}

void BulletRight::SetColliderSize(float size)
{
	m_SizeCollider = size;
}
float BulletRight::GetColliderSize()
{
	return m_SizeCollider;
}

void BulletRight::SetDamage(float damage)
{
	m_Damage = damage;
}

float BulletRight::GetDamage()
{
	return m_Damage;
}

