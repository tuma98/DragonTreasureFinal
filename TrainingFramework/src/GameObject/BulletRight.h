#pragma once
#include "Sprite2D.h"
#include "Application.h"

enum class BULLET_TYPERIGHT
{
	None,
	Player,
	Enermy
};

class BulletRight  : public Sprite2D
{
public:
	BulletRight(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture);
	~BulletRight();

	void		Update(GLfloat deltatime) override;
	void		SetSpeed(float speed);

	bool		IsActive();
	void		SetActive(bool status);

	void		SetType(BULLET_TYPERIGHT type);
	BULLET_TYPERIGHT	GetType();

	void		SetColliderSize(float size);
	float		GetColliderSize();

	void		SetDamage(float damage);
	float		GetDamage();
			
private:
	BULLET_TYPERIGHT m_type;
	bool	m_active;
	float	m_speed;
	float	m_SizeCollider;
	float	m_Damage;
};
