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
	//���߂�ꂽ�ꏊ��ݒ�(X���W)
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

	// 3D�`����������ނ��߂̉摜���쐬
	for (int i = 0; i < IMAGE_MAX; i++) {
		screen_image[i] = MakeScreen(SCREEN_W, SCREEN_H);
	}
	Game_Clear = false;
}

void Game::Update(bool tutrial_mode, int Sound,int Sens)
{
	stage.Update(player.m_pos_, &player.m_rot_.y, &player.Gimmick_JumpBoard_mov_, &player.Gimmick_WireAction_mov_,
		camera.cam_rot_, player.GimmickMov_Off);


	//���W�m�F���ăZ�[�u�|�C���g�܂ŗ��Ă��邩�m�F
	CheckSave(player.m_pos_.x);

	playerui.Update(player.ReturnCoolDown(), eyeOpen, ClearFlag, PlayMovie, tutrial_mode,player.time[1] == 1);
	if (eyeOpen) {
		player.Update(stage_model, camera.cam_rot_,Sound);
	}
	camera.Update(player.m_pos_, stage_model, Sens);




	//�G���h��ʂɈړ��ł���悤�ɂ���
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
	//�u���[�摜�p
	SetDrawScreen(screen_image[count]);
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);	// �J������ Near�N���b�v�ʂ� Far�N���b�v�ʂ̋�����ݒ�
	SetupCamera_Perspective(TO_RADIAN(45.0f));	// ���ߖ@�J�������Z�b�g�A�b�v����

	camera.Render();
	stage.Render();
	player.Render();

	//�{����
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);	// �J������ Near�N���b�v�ʂ� Far�N���b�v�ʂ̋�����ݒ�
	SetupCamera_Perspective(TO_RADIAN(45.0f));	// ���ߖ@�J�������Z�b�g�A�b�v����
	camera.Render();

	stage.Render();

	Blur();

	//�摜���o��
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

		////�z��̍Ōザ��Ȃ��č����Ă���̂Ǝ��̕���NULL���������΂�
		//if ((j + 1) == IMAGE_MAX && (blur_image[j] == NULL && blur_image[j + 1] == NULL))
		//{
		//	return;
		//}

		////����̉摜��������ĕۑ�����
		//if (j != (IMAGE_MAX - 1))
		//{
		//	blur_image[j] = blur_image[j + 1];
		//}
		////�z��̍Ō�Ȃ獡�̃X�N���[���摜��ۑ�
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