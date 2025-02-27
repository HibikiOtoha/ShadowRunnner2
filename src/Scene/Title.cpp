#include "WinMain.h"
#include "GameMain.h"
#include "Dxlib.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Title.h"
#include <cstdlib>

Title::Title()
{

}

void Title::Init()
{
	Eyes[0] = { 0, -SCREEN_H / 2 };
	Eyes[1] = { 0,SCREEN_H };

	Scene_Change = false;
	close = false;

	BGM_Handle = LoadSoundMem("data/Sounds/title.mp3");
	IN_Handle = LoadSoundMem("data/Sounds/title_G.mp3");

	Title_image = LoadGraph("data/img/Title_BackScreen.jpg");
	Title_image_1 = LoadGraph("data/img/1.png");
	Title_image_2 = LoadGraph("data/img/2.png");
	Title_image_3 = LoadGraph("data/img/3.png");
	Title_image_4 = LoadGraph("data/img/4.png");
	Title_image_5 = LoadGraph("data/img/5.png");
	Title_image_6 = LoadGraph("data/img/6.png");
	srand((unsigned)time(NULL));
	for (int i = 0; i < 6; i++)
	{
		rand_[i] = rand() % 90 + 0;
	}
}

void Title::Update(int Sound)
{
	ChangeVolumeSoundMem(Sound, BGM_Handle);
	ChangeVolumeSoundMem(Sound, IN_Handle);
	if (time_ == 0) {
		PlaySoundMem(BGM_Handle, DX_PLAYTYPE_BACK);
	}
	time_++;

	if (PushHitKey(KEY_INPUT_SPACE))
	{
		close = true;
	}
	if (close) {
		CloseEyes();
	}
}

void Title::Render()
{
	//タイトル画像描画
	//DrawGraph(0, 0, Title_image, TRUE);

	
	DrawRotaGraph(SCREEN_W / 6 * 3, SCREEN_H / 4 * 1, 1.0f, 0, Title_image_2, true);
	DrawRotaGraph(SCREEN_W / 6 * 1, SCREEN_H / 4 * 1, 1.0f, -60, Title_image_1, true);
	DrawRotaGraph(SCREEN_W / 6 * 5, SCREEN_H / 4 * 1, 1.0f, 60, Title_image_3, true);
	DrawRotaGraph(SCREEN_W / 6 * 1, SCREEN_H / 4 * 3, 1.0f, -60, Title_image_4, true);
	DrawRotaGraph(SCREEN_W / 6 * 3, SCREEN_H / 4 * 3, 1.0f, 0, Title_image_5, true);
	DrawRotaGraph(SCREEN_W / 6 * 5, SCREEN_H / 4 * 3, 1.0f, 60, Title_image_6, true);

	SetFontSize(128);

	DrawString(80, 160, "ShadowRunnner", GetColor(0, 0, 0));
	SetFontSize(120);
	DrawString(82, 162, "ShadowRunnner", GetColor(255, 255, 255));
	SetFontSize(64);
	DrawString(80, 500, "SPACEキーを押してスタート", GetColor(255, 255, 255));
	SetFontSize(16);


	//瞼
	DrawBox(Eyes[0].x, Eyes[0].y,
		Eyes[0].x + SCREEN_W, Eyes[0].y + (SCREEN_H / 2),
		GetColor(0,0,0), TRUE);
	DrawBox(Eyes[1].x, Eyes[1].y,
		Eyes[1].x + SCREEN_W, Eyes[1].y + (SCREEN_H / 2),
		GetColor(0, 0, 0), TRUE);
}

void Title::Exit()
{

}

void Title::CloseEyes()
{
	Eyes[0].y += CLOSE_SPEED;
	Eyes[1].y -= CLOSE_SPEED;

	if (Eyes[0].y >= 0 || Eyes[1].y <= SCREEN_H / 2)
	{
		PlaySoundMem(IN_Handle, DX_PLAYTYPE_BACK);

		StopSoundMem(BGM_Handle);
		//閉じたらシーン切り替え
		Scene_Change = true;
	}
}