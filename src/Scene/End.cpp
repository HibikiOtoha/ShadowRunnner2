#include "WinMain.h"
#include "GameMain.h"
#include "Dxlib.h"
#include "Vector2.h"
#include "Vector3.h"
#include "End.h"

Ending::Ending()
{

}

void Ending::Init(int player_model)
{
	Scene_Change = false;
	//�摜�w��
	End_image = LoadGraph("data/img/END.jpg");
	player_image = LoadGraph("data/img/Player.png");

	bEnd_image = LoadGraph("data/img/bEND.jpg");

	clear = true;
}

void Ending::Update(bool gameclear)
{
	clear = gameclear;

	if (PushHitKey(KEY_INPUT_SPACE))
	{
		Scene_Change = true;
	}
}

void Ending::Render()
{
	if (clear) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawGraph(0, 0, End_image, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		int SizeX, SizeY;
		GetGraphSize(player_image, &SizeX, &SizeY);
		DrawExtendGraph(SCREEN_W / 2 - 60, SCREEN_H / 2 - 60, SCREEN_W / 2 - 60 + SizeX / 2, SCREEN_H / 2 - 60 + SizeY / 2, player_image, TRUE);
		SetFontSize(128);
		DrawString(80, 160, "�N���A�I�I", GetColor(255, 255, 255));
		SetFontSize(16);
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawGraph(0, 0, bEnd_image, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		int SizeX, SizeY;
		GetGraphSize(player_image, &SizeX, &SizeY);
		//DrawExtendGraph(SCREEN_W / 2 - 60, SCREEN_H / 2 - 60, SCREEN_W / 2 - 60 + SizeX / 2, SCREEN_H / 2 - 60 + SizeY / 2, player_image, TRUE);
		SetFontSize(128);
		DrawString(80, 160, "Defeat", GetColor(255, 255, 255));
		SetFontSize(64);
		DrawString(80, 300, "�������|�󂵂Ă��܂���", GetColor(255, 255, 255));
		SetFontSize(16);
	}
}

void Ending::Exit()
{

}
