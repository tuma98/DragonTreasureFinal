#pragma once
#include "Sprite2D.h"
#include "Application.h"

enum class BULLET_TYPELEFT
{
	None,
	Player,
	Enermy
};

class BulletLeft  : public Sprite2D
{
public:
	BulletLeft(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture);
	~BulletLeft();

	void		Update(GLfloat deltatime) override;
	void		SetSpeed(float speed);

	bool		IsActive();
	void		SetActive(bool status);

	void		SetType(BULLET_TYPELEFT type);
	BULLET_TYPELEFT	GetType();

	void		SetColliderSize(float size);
	float		GetColliderSize();

	void		SetDamage(float damage);
	float		GetDamage();
			
private:
	BULLET_TYPELEFT m_type;
	bool	m_active;
	float	m_speed;
	float	m_SizeCollider;
	float	m_Damage;
};
