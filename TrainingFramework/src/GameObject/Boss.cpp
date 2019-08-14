#include "Boss.h"
#include "GameManager/ResourceManagers.h"
#include <GameStates\GSPlay.h>

Boss::Boss(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture)
	:Sprite2D(model, shader, texture)
{
	m_TargetPosition = Vector2(0, 0);
	m_active = false;
	m_MaxCooldown = 0.3;
	m_Cooldown = 0.0;
	m_speed = 200;
	m_MaxSpeed = 500;
	m_Heal = 1050;
	m_Damage = 10000000;
	m_Explosive = false;
	m_SizeCollider = 200;
}

Boss::~Boss()
{
}


void Boss::Update(float deltaTime)
{
	if (!m_active)
		return;

	if (m_Heal <= 0 || m_Explosive)
	{
		SoundManager::GetInstance()->PlaySound("explosive");
		m_Explosive = true;
		GSPlay::m_score + 500;
		return;
	}

	if (m_Cooldown > 0)
	{
		m_Cooldown -= deltaTime;
	}

	Vector2 pos = Get2DPosition();
	if (pos.x == Application::screenWidth -100 && pos.y == 200) {
		SetTargetPosition(Vector2(Application::screenWidth - 900, 200));
	}
	if (pos.x == Application::screenWidth -900 && pos.y == 200) {
		SetTargetPosition(Vector2(Application::screenWidth - 100, 200));
	}
	if (pos.x < m_TargetPosition.x)
	{
		pos.x += m_speed * deltaTime;
		if (pos.x > m_TargetPosition.x)
			pos.x = m_TargetPosition.x;
	}

	if (pos.x > m_TargetPosition.x)
	{
		pos.x -= m_speed * deltaTime;
		if (pos.x < m_TargetPosition.x)
			pos.x = m_TargetPosition.x;
	}

	if (pos.y < m_TargetPosition.y)
	{
		pos.y += m_speed * deltaTime;
		if (pos.y > m_TargetPosition.y)
			pos.y = m_TargetPosition.y;
	}

	if (pos.y > m_TargetPosition.y)
	{
		pos.y -= m_speed * deltaTime;
		if (pos.y < m_TargetPosition.y)
			pos.y = m_TargetPosition.y;
	}

	Set2DPosition(pos);
}

bool Boss::CanShoot()
{
	return (m_Cooldown <= 0);
}

void Boss::Shoot(std::vector<std::shared_ptr<Bullet>>& listBullet)
{
	m_Cooldown = m_MaxCooldown;
	for (auto bullet : listBullet)
	{
		if (!bullet->IsActive())
		{
			bullet->SetActive(true);
			bullet->Set2DPosition(Get2DPosition());
			bullet->SetSpeed(-500);
			bullet->SetType(BULLET_TYPE::Enermy);
			return;
		}
	}

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bullet");

	std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(model, shader, texture);
	bullet->SetSize(20, 20);
	bullet->Set2DPosition(Get2DPosition());
	bullet->SetSpeed(-500);
	bullet->SetType(BULLET_TYPE::Enermy);
	listBullet.push_back(bullet);
}

void Boss::ShootLeft(std::vector<std::shared_ptr<BulletLeft>>& listBullet)
{
	m_Cooldown = m_MaxCooldown;
	for (auto bullet : listBullet)
	{
		if (!bullet->IsActive())
		{
			bullet->SetActive(true);
			bullet->Set2DPosition(Get2DPosition());
			bullet->SetSpeed(-500);
			bullet->SetType(BULLET_TYPELEFT::Enermy);
			return;
		}
	}

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bullet");

	std::shared_ptr<BulletLeft> bullet = std::make_shared<BulletLeft>(model, shader, texture);
	bullet->SetSize(20, 20);
	bullet->Set2DPosition(Get2DPosition());
	bullet->SetSpeed(-500);
	bullet->SetType(BULLET_TYPELEFT::Enermy);

	listBullet.push_back(bullet);
}

void Boss::ShootRight(std::vector<std::shared_ptr<BulletRight>>& listBullet)
{
	m_Cooldown = m_MaxCooldown;
	for (auto bullet : listBullet)
	{
		if (!bullet->IsActive())
		{
			bullet->SetActive(true);
			bullet->Set2DPosition(Get2DPosition());
			bullet->SetSpeed(-500);
			bullet->SetType(BULLET_TYPERIGHT::Enermy);
			return;
		}
	}

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bullet");

	std::shared_ptr<BulletRight> bullet = std::make_shared<BulletRight>(model, shader, texture);
	bullet->SetSize(20, 20);
	bullet->Set2DPosition(Get2DPosition());
	bullet->SetSpeed(-500);
	bullet->SetType(BULLET_TYPERIGHT::Enermy);

	listBullet.push_back(bullet);
}

float Boss::distance(Vector2 pos, Vector2 target)
{
	return sqrt((pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y));
}


void Boss::CheckCollider(std::vector<std::shared_ptr<Bullet>>& listBullet)
{
	Vector2 pos = Get2DPosition();

	for (auto bullet : listBullet)
	{
		if (bullet->IsActive() && bullet->GetType() == BULLET_TYPE::Player)
		{
			if (distance(pos, bullet->Get2DPosition()) < m_SizeCollider + bullet->GetColliderSize())
			{
				bullet->SetActive(false);
				m_Heal -= bullet->GetDamage();
				if (m_Heal <= 0)
					return;
			}
		}
	}
}

void Boss::CheckColliderLeft(std::vector<std::shared_ptr<BulletLeft>>& listBulletLeft)
{
	Vector2 pos = Get2DPosition();

	for (auto bullet : listBulletLeft)
	{
		if (bullet->IsActive() && bullet->GetType() == BULLET_TYPELEFT::Player)
		{
			if (distance(pos, bullet->Get2DPosition()) < m_SizeCollider + bullet->GetColliderSize())
			{
				bullet->SetActive(false);
				m_Heal -= bullet->GetDamage();
				if (m_Heal <= 0)
					return;
			}
		}
	}
}

void Boss::CheckColliderRight(std::vector<std::shared_ptr<BulletRight>>& listBulletRight)
{
	Vector2 pos = Get2DPosition();

	for (auto bullet : listBulletRight)
	{
		if (bullet->IsActive() && bullet->GetType() == BULLET_TYPERIGHT::Player)
		{
			if (distance(pos, bullet->Get2DPosition()) < m_SizeCollider + bullet->GetColliderSize())
			{
				bullet->SetActive(false);
				m_Heal -= bullet->GetDamage();
				if (m_Heal <= 0)
					return;
			}
		}
	}
}


bool Boss::IsActive()
{
	return m_active;
}

void Boss::SetActive(bool status)
{
	m_active = status;
	m_Explosive = false;
	m_Heal = 3000;
}

void Boss::SetDamage(float damage)
{
	m_Damage = damage;
}

float Boss::GetDamage()
{
	return m_Damage;
}

void Boss::SetMSpeed(float speed)
{
	m_speed = speed;
}

float Boss::GetMSpeed()
{
	return m_speed;
}

void Boss::Explosive()
{
	m_Explosive = true;
}

bool Boss::IsExplosive()
{
	return m_Explosive;
}


void Boss::SetColliderSize(float size)
{
	m_SizeCollider = size;
}
float Boss::GetColliderSize()
{
	return m_SizeCollider;
}

void Boss::SetTargetPosition(Vector2 pos) 
{
	m_TargetPosition = pos;
}