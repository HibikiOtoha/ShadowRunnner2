#include "WinMain.h"
#include "GameMain.h"
#include "Dxlib.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Player/PlayerUI.h"
#include "Camera/Camera.h"
#include "Stage/Stage.h"
#include "Player/Player.h"
#include "Game.h"

Player player;
Camera camera;
Stage stage;
PlayerUI playerui;

Game::Game()
{
	//決められた場所を設定(X座標)
	SavePoint[0] = 490.0f;
	SavePoint[1] = 345.0f;
	SavePoint[2] = 85.0f;
	SavePoint[3] = -130.0f;


}

void Game::Init(int M_Stage, int M_Sky, int M_Player)
{
	stage_model = M_Stage;
	sky_model = M_Sky;
	player_model = M_Player;

	Scene_Change = false;
	stage.Init(stage_model, sky_model);
	player.Init(player_model);

	for (int i = 0; i < IMAGE_MAX; i++) {
		blur_image[i] = 0;
	}

	eyeOpen = false;

	for (int i = 0; i < SAVE_MAX; i++)
	{
		ClearFlag[i] = false;
	}
	PlayMovie = -1;

	// 3D描画を書き込むための画像を作成
	for (int i = 0; i < IMAGE_MAX; i++) {
		screen_image[i] = MakeScreen(SCREEN_W, SCREEN_H);
	}
	Game_Clear = false;
}

void Game::Update(bool tutrial_mode, int Sound,int Sens)
{
	stage.Update(player.m_pos_, &player.m_rot_.y, &player.Gimmick_JumpBoard_mov_, &player.Gimmick_WireAction_mov_,
		camera.cam_rot_, player.GimmickMov_Off);


	//座標確認してセーブポイントまで来ているか確認
	CheckSave(player.m_pos_.x);

	playerui.Update(player.ReturnCoolDown(), eyeOpen, ClearFlag, PlayMovie, tutrial_mode,player.time[1] == 1);
	if (eyeOpen) {
		player.Update(stage_model, camera.cam_rot_,Sound);
	}
	camera.Update(player.m_pos_, stage_model, Sens);




	//エンド画面に移動できるようにする
	if (stage.goalflag == true)
	{
		Game_Clear = true;
		Scene_Change = true;
		playerui.minute = 0;
		playerui.time = 0;
		playerui.conma = 0;

	}
	if (playerui.timeOver)
	{
		Game_Clear = false;
		Scene_Change = true;
		playerui.timeOver = false;
		playerui.minute = 0;
		playerui.time = 0;
		playerui.conma = 0;
	}
	
}

void Game::Render()
{
	//ブラー画像用
	SetDrawScreen(screen_image[count]);
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);	// カメラの Nearクリップ面と Farクリップ面の距離を設定
	SetupCamera_Perspective(TO_RADIAN(45.0f));	// 遠近法カメラをセットアップする

	camera.Render();
	stage.Render();
	player.Render();

	//本処理
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);	// カメラの Nearクリップ面と Farクリップ面の距離を設定
	SetupCamera_Perspective(TO_RADIAN(45.0f));	// 遠近法カメラをセットアップする
	camera.Render();

	stage.Render();

	Blur();

	//画像を出す
	int blend = 5 / IMAGE_MAX;
	for (int i = 0; i < IMAGE_MAX; i++) {

		if (blur_image[i] != NULL) {
			int a = (int)(blend * (IMAGE_MAX - i));
			SetDrawBlendMode(DX_BLENDMODE_ALPHA,a );
			DrawGraphF(0, 0, blur_image[i], TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	player.Render();
	playerui.Draw();

	count++;
	if (count >= IMAGE_MAX) {
		count = 0;
	}
}

void Game::Exit()
{
	stage.Exit();
	camera.Exit();
	player.Exit();
	playerui.Exit();
}


void Game::CheckSave(float posx)
{
	for (int i = 0; i < SAVE_MAX; i++)
	{
		ClearFlag[i] = posx <= SavePoint[i];
	}
}

void Game::Blur()
{

	for (int j = 0; j < IMAGE_MAX; j++) {

		if (screen_image[j] != NULL) {
			blur_image[j] = screen_image[j];
		}

		////配列の最後じゃなくて今見てるものと次の物がNULLだったら飛ばす
		//if ((j + 1) == IMAGE_MAX && (blur_image[j] == NULL && blur_image[j + 1] == NULL))
		//{
		//	return;
		//}

		////一個次の画像をもらって保存する
		//if (j != (IMAGE_MAX - 1))
		//{
		//	blur_image[j] = blur_image[j + 1];
		//}
		////配列の最後なら今のスクリーン画像を保存
		//else
		//{
		//	blur_image[j] = screen_image[j];
		//}
	}
}

void Game::Reset()
{
	Scene_Change = false;
	player.Reset(0);
	for (int i = 0; i < HANDLE_MAX; i++) {
		player.time[i] = 0;
	}
	stage.goalflag = false;

	for (int i = 0; i < IMAGE_MAX; i++) {
		blur_image[i] = 0;
	}

	eyeOpen = false;

	for (int i = 0; i < SAVE_MAX; i++)
	{
		ClearFlag[i] = false;
	}
	PlayMovie = -1;

	Game_Clear = false;
}