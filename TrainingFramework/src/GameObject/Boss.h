#pragma once

#include "Sprite2D.h"
#include <Bullet.h>
#include <BulletLeft.h>
#include <BulletRight.h>

class Boss : public Sprite2D
{
public:
	Boss(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture);
	~Boss();
	bool		IsActive();
	void		SetActive(bool status);

	void		Update(float deltaTime) override;

	bool		CanShoot();
	void		Shoot(std::vector<std::shared_ptr<Bullet>>& listBullet);
	void		ShootLeft(std::vector<std::shared_ptr<BulletLeft>>& listBullet);
	void		ShootRight(std::vector<std::shared_ptr<BulletRight>>& listBullet);
	void		CheckCollider(std::vector<std::shared_ptr<Bullet>>& listBullet);
	void		CheckColliderLeft(std::vector<std::shared_ptr<BulletLeft>>& listBulletLeft);
	void		CheckColliderRight(std::vector<std::shared_ptr<BulletRight>>& listBulletRight);

	void		SetColliderSize(float size);
	float		GetColliderSize();

	void		SetDamage(float damage);
	float		GetDamage();

	void		SetMSpeed(float damage);
	float		GetMSpeed();

	void		Explosive();
	bool		IsExplosive();

	void		SetTargetPosition(Vector2 pos);

private:
	Vector2 m_TargetPosition;
	int		m_Heal;
	bool	m_active;
	bool	m_Explosive;
	float	m_speed;
	float	m_MaxSpeed;
	float	m_MaxCooldown;
	float	m_Cooldown;
	float	m_Damage;
	float	m_SizeCollider;

	float distance(Vector2 pos, Vector2 target);

};
