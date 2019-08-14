#include <sstream>
#include <iomanip>
#include <thread>

#include "GSPlay.h"
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Text.h"
#include "Player.h"
#include "Enermy.h"
#include "Bullet.h"
#include "BulletLeft.h"
#include "BulletRight.h"
#include "Boss.h"

#include "ExplosiveEffect.h"

int GSPlay::m_score = 0;
GSPlay::GSPlay()
{
	m_SpawnCooldown = 0.7;
	m_SpawnCooldownBoss = 0.3;
	m_score = 0;
	m_time = 0;
}


GSPlay::~GSPlay()
{

}


void GSPlay::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("blue");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	m_BackGround->SetSize(Application::screenWidth, Application::screenHeight);

	texture = ResourceManagers::GetInstance()->GetTexture("Player");
	m_Player = std::make_shared<Player >(model, shader, texture);
	m_Player->Set2DPosition(Application::screenWidth / 2, Application::screenHeight - 100);
	m_Player->MoveToPossition(Vector2(Application::screenWidth / 2, Application::screenHeight - 100));
	m_Player->SetSize(50, 50);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_scoreText = std::make_shared< Text>(shader, font, "SCORE: ", TEXT_COLOR::RED, 1.0);
	m_scoreText->Set2DPosition(Vector2(5, 25));
	m_playerHealText = std::make_shared< Text>(shader, font, "HP: ", TEXT_COLOR::RED, 1.0);
	m_playerHealText->Set2DPosition(Vector2(5, 50));

	//init effect
	model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	texture = ResourceManagers::GetInstance()->GetTexture("explosive");
	shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	std::shared_ptr<ExplosiveEffect> exp = std::make_shared<ExplosiveEffect>(model, shader, texture, Vector2(960, 768), Vector2(192, 192), 0, 19, 1);
	exp->SetSize(100, 100);
	exp->SetActive(false);
	m_listExplosiveEffect.push_back(exp);

	//init sound
	SoundManager::GetInstance()->AddSound("explosive");
	SoundManager::GetInstance()->AddSound("explosive_2");
	SoundManager::GetInstance()->AddSound("bground");
	SoundManager::GetInstance()->AddSound("fire");
	SoundManager::GetInstance()->AddSound("fire_enemy");
	SoundManager::GetInstance()->PlaySound("PlayLoop");

}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSPlay::HandleMouseEvents(int x, int y)
{
	m_Player->MoveToPossition(Vector2(x, y));
}


void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSPlay::Update(float deltaTime)
{
	m_time += deltaTime;
	if (m_time>700)
	{
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Credit);
	}

	if (m_SpawnCooldown > 0)
	{
		m_SpawnCooldown -= deltaTime;
	}
	if (m_SpawnCooldown <= 0)
	{
		CreateRandomEnermy();
		m_SpawnCooldown = 0.3;
	}

	if (m_SpawnCooldownBoss > 0)
	{
		m_SpawnCooldownBoss -= deltaTime;
	}
	if (m_SpawnCooldownBoss <= 0)
	{
		SoundManager::GetInstance()->PlaySound("BossSound");
		CreateBoss();
		m_SpawnCooldownBoss = 1000000;
	}

	//update player
	if (m_Player->IsAlive())
	{
		m_Player->Update(deltaTime);
		if (m_Player->CanShoot()) {
			if (m_score < 15) {
				m_Player->Shoot(m_listBullet);
			}
			else if (m_score >= 15 && m_score < 51) {
				m_Player->ShootRight(m_listBulletRight);
				m_Player->ShootLeft(m_listBulletLeft);
			}
			else {
				m_Player->Shoot(m_listBullet);
				m_Player->ShootLeft(m_listBulletLeft);
				m_Player->ShootRight(m_listBulletRight);
			}
		}
		m_Player->CheckCollider(m_listBullet, m_listEnermy,m_listEnermy2,m_listBulletLeft,m_listBulletRight);
	}
	else
	{
		SoundManager::GetInstance()->PauseSound("PlayLoop");
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_GameOver);
	}

	//update enermies
	for (auto enermy : m_listEnermy)
	{
		if (enermy->IsActive())
		{
			if (enermy->IsExplosive())
			{
				enermy->SetActive(false);
				SpawnExplosive(enermy->Get2DPosition());
				continue;
			}
			enermy->Update(deltaTime);
			if (enermy->CanShoot())
				enermy->Shoot(m_listBullet);
			enermy->CheckCollider(m_listBullet);
			enermy->CheckColliderLeft(m_listBulletLeft);
			enermy->CheckColliderRight(m_listBulletRight);
		}
	}


	for (auto exp : m_listExplosiveEffect)
	{
		if (exp->IsActive())
		{
			exp->Update(deltaTime);
		}
	}

	//update bullets
	for (auto bullet : m_listBullet)
	{
		if (bullet->IsActive())
			bullet->Update(deltaTime);
	}

	for (auto bullet : m_listBulletRight)
	{
		if (bullet->IsActive())
			bullet->Update(deltaTime);
	}

	for (auto bullet : m_listBulletLeft)
	{
		if (bullet->IsActive())
			bullet->Update(deltaTime);
	}

	//update boss 
	if (m_score == 51)
	{
		m_SpawnCooldown = 10000000;
		for (auto m_Boss : m_listEnermy2)
		{
			if (m_Boss->IsActive())
			{
				if (m_Boss->IsExplosive())
				{
					m_Boss->SetActive(false);
					SpawnExplosiveBoss(m_Boss->Get2DPosition());
					continue;
				}
				m_Boss->Update(deltaTime);

				if (m_Boss->CanShoot()) {
					m_Boss->Shoot(m_listBullet);
					m_Boss->ShootLeft(m_listBulletLeft);
					m_Boss->ShootRight(m_listBulletRight);
				}
				m_Boss->CheckCollider(m_listBullet);
				m_Boss->CheckColliderLeft(m_listBulletLeft);
				m_Boss->CheckColliderRight(m_listBulletRight);
			}
			if (m_Boss->IsActive() == false)
			{
				SoundManager::GetInstance()->PauseSound("PlayLoop");
				GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Victory);
			}
		}
	}



	//update Score
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << m_score;
	std::string score = "SCORE: " + stream.str();
	m_scoreText->setText(score);
	std::stringstream stream2;
	stream2 << std::fixed << std::setprecision(0) << m_Player->GetHeal();
	std::string heal = "HP: " + stream2.str();
	m_playerHealText->setText(heal);
}

void GSPlay::Draw()
{
	//ground
	m_BackGround->Draw();

	for (auto enermy : m_listEnermy)
		if (enermy->IsActive())
			enermy->Draw();

	if (m_score == 51) {
		for (auto enermy : m_listEnermy)
			if (enermy->IsActive())
				enermy->SetActive(0);
		for (auto Boss : m_listEnermy2)
			if (Boss->IsActive())
				Boss->Draw();
	}

	if (m_Player->IsAlive())
		m_Player->Draw();

	for (auto bullet : m_listBullet)
		if (bullet->IsActive())
			bullet->Draw();

	for (auto bullet : m_listBulletRight)
		if (bullet->IsActive())
			bullet->Draw();

	for (auto bullet : m_listBulletLeft)
		if (bullet->IsActive())
			bullet->Draw();

	for (auto exp : m_listExplosiveEffect)
	{
		if (exp->IsActive())
		{
			exp->Draw();
		}
	}

	//UI
	m_scoreText->Draw();
	m_playerHealText->Draw();
}

void GSPlay::CreateBoss()
{

	Vector2 pos;
	pos.x = Application::screenWidth / 2;
	pos.y = 200;


	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Boss");

	std::shared_ptr<Boss> enermy = std::make_shared<Boss>(model, shader, texture);

	enermy->SetActive(true);

	enermy->Set2DPosition(pos);
	enermy->SetSize(500, 500);
	enermy->SetTargetPosition(Vector2(Application::screenWidth -100, 200));
	m_listEnermy2.push_back(enermy);
}

void GSPlay::CreateRandomEnermy()
{

	int range = Application::screenWidth - 10 + 1;
	int num = rand() % range + 10;

	Vector2 pos;
	pos.x = num;
	pos.y = 10;

	for (auto enermy : m_listEnermy)
	{
		if (!enermy->IsActive())
		{
			enermy->SetActive(true);
			enermy->Set2DPosition(pos);
			return;
		}
	}
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Enermy_1");

	std::shared_ptr<Enermy> enermy = std::make_shared<Enermy>(model, shader, texture);
	enermy->Set2DPosition(pos);
	enermy->SetSize(40, 40);
	m_listEnermy.push_back(enermy);
}

void GSPlay::SpawnExplosive(Vector2 pos)
{
	for (auto exp : m_listExplosiveEffect)
	{
		if (!exp->IsActive())
		{
			exp->SetActive(true);
			exp->Set2DPosition(pos);
			return;
		}
	}

	//animation
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("explosive");
	auto shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	std::shared_ptr<ExplosiveEffect> exp = std::make_shared<ExplosiveEffect>(model, shader, texture, Vector2(960, 768), Vector2(192, 192), 0, 19, 1.0);
	exp->SetSize(100, 100);
	exp->Set2DPosition(pos);
	m_listExplosiveEffect.push_back(exp);
}

void GSPlay::SpawnExplosiveBoss(Vector2 pos)
{
	for (auto exp : m_listExplosiveEffect)
	{
		if (!exp->IsActive())
		{
			exp->SetActive(true);
			exp->Set2DPosition(pos);
			return;
		}
	}
	//GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Victory);
	//animation
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("explosive");
	auto shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	std::shared_ptr<ExplosiveEffect> exp = std::make_shared<ExplosiveEffect>(model, shader, texture, Vector2(960, 768), Vector2(192, 192), 0, 19, 2.5);
	exp->SetSize(500, 500);
	exp->Set2DPosition(pos);
	m_listExplosiveEffect.push_back(exp);
}
