#pragma once
#include "Sprite2D.h"
#include "Bullet.h"
#include "BulletLeft.h"
#include "BulletRight.h"
#include <Enermy.h>
#include <Boss.h>

class Player : public Sprite2D
{
	
public:
	Player(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture);
	~Player();

	void MoveToPossition(Vector2 pos);
	void Update(GLfloat deltatime) override;

	bool CanShoot();
	void Shoot(std::vector<std::shared_ptr<Bullet>>& listBullet);
	void ShootLeft(std::vector<std::shared_ptr<BulletLeft>>& listBullet);
	void ShootRight(std::vector<std::shared_ptr<BulletRight>>& listBullet);

	void CheckCollider(std::vector<std::shared_ptr<Bullet>>& listBullet, std::vector<std::shared_ptr<Enermy>> listEnermy, std::vector<std::shared_ptr<Boss>> listEnermy2, std::vector<std::shared_ptr<BulletLeft>>& listBulletLeft, std::vector<std::shared_ptr<BulletRight>>& listBulletRight);

	void		SetColliderSize(float size);
	float		GetColliderSize();
	int			GetHeal();
	bool		IsAlive();
private:
	Vector2 m_TargetPosition;
	int		m_Heal;
	bool	m_isAlive;
	float	m_speed;
	float	m_MaxSpeed;
	float	m_MaxCooldown;
	float	m_Cooldown;
	float	m_SizeCollider;


	float distance(Vector2 pos, Vector2 target);
};
