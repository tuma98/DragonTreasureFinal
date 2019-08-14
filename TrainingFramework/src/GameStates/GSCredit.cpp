#include "GSCredit.h"

#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"

GSCredit::GSCredit()
{

}


GSCredit::~GSCredit()
{
}



void GSCredit::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Credit_Scene_Dragon");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	m_BackGround->SetSize(Application::screenWidth, Application::screenHeight);
	m_BackGround->SetRotation(180);

	//menu button
	texture = ResourceManagers::GetInstance()->GetTexture("button_menu");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Application::screenWidth/2, 900);
	button->SetSize(200, 200);
	button->SetOnClick([]() {
		Sleep(500);
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		});
	m_listButton.push_back(button);


	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_CreditName = std::make_shared< Text>(shader, font, "CREDIT", TEXT_COLOR::GREEN, 1.0);
	m_CreditName->Set2DPosition(Vector2(Application::screenWidth / 2-50, 120));

	m_Credit = std::make_shared< Text>(shader, font, "MADE BY NGUYEN ANH TU", TEXT_COLOR::GREEN, 1.0);
	m_Credit->Set2DPosition(Vector2(Application::screenWidth / 2 - 150, 150));

	m_SpecialThankName = std::make_shared< Text>(shader, font, "SPECIAL THANK", TEXT_COLOR::GREEN, 1.0);
	m_SpecialThankName->Set2DPosition(Vector2(Application::screenWidth / 2 - 100, 180));

	m_SpecialThank = std::make_shared< Text>(shader, font, "ANH CUONG VA GAMELOFT STUDIO HAN", TEXT_COLOR::GREEN, 1.0);
	m_SpecialThank->Set2DPosition(Vector2(Application::screenWidth / 2 - 250, 210));


}

void GSCredit::Exit()
{
}


void GSCredit::Pause()
{

}

void GSCredit::Resume()
{

}


void GSCredit::HandleEvents()
{

}

void GSCredit::HandleKeyEvents(int key, bool bIsPressed)
{

}
void GSCredit::HandleMouseEvents(int x, int y)
{
}
void GSCredit::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSCredit::Update(float deltaTime)
{
	m_BackGround->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSCredit::Draw()
{
	m_BackGround->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_CreditName->Draw();
	m_Credit->Draw();
	m_SpecialThankName->Draw();
	m_SpecialThank->Draw();
}
