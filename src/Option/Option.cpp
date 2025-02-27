#include "WinMain.h"
#include "GameMain.h"
#include "Dxlib.h"
#include <iostream>
#include <string>
#include "Vector2.h"
#include "Vector3.h"
#include "Scene/Game.h"

#include "Option/Option.h"
using namespace std;


Option::Option()
{
	OptionMode_ON = false;

	camera_pos = 5;
	tutorial_on = true;
	BGM_vol = 50;
	Sens_vol = 5;

	mouse_deray = 0;

	val_x = SCREEN_W / 20 * 10.5 + (((SCREEN_W / 20 * 15.2) - (SCREEN_W / 20 * 10.5)) / 2);
	sens_x = SCREEN_W / 20 * 10.5 + (((SCREEN_W / 20 * 15.2) - (SCREEN_W / 20 * 10.5)) / 2);
}

void Option::Update(int Scene_num, int& Sound)
{
	//�I�v�V������ʂ̃I���I�t
	if (PushHitKey(KEY_INPUT_ESCAPE) && OptionMode_ON == false)
	{
		SetMouseDispFlag(TRUE);	//�}�E�X�J�[�\���̕\��(�S���)
		OptionMode_ON = true;
	}
	else if(PushHitKey(KEY_INPUT_ESCAPE) && OptionMode_ON == true)
	{
		OptionMode_ON = false;
		SetMouseDispFlag(FALSE);	//�}�E�X�J�[�\���̕\��(�S���)
	}
	if (!OptionMode_ON)
	{
		//�}�E�X�J�[�\���Œ�
		SetMousePoint(SCREEN_W / 2, SCREEN_H / 2);
	}

	//�}�E�X�ł̐ݒ葀��
	if (OptionMode_ON) {
		mouse_deray++;
		if (mouse_deray >= MOUSE_DERAYMAX)
		{
			mouse_deray = MOUSE_DERAYMAX;
		}

		int mouse_x, mouse_y = 0;
		GetMousePoint(&mouse_x, &mouse_y);

		if (GetMouseInput())
		{
			//�I���I�t
			if (mouse_deray == MOUSE_DERAYMAX) {
				mouse_deray = 0;
				//�{�^��
				if (mouse_x >= (SCREEN_W / 20 * 13) && mouse_x <= (SCREEN_W / 20 * 15) &&
					mouse_y >= ((SCREEN_H / 20 * 6.5) - (SCREEN_W / 40)) && mouse_y <= ((SCREEN_H / 20 * 6.5) + (SCREEN_W / 40)))
				{
					if (tutorial_on)	tutorial_on = false;
					else tutorial_on = true;
				}
			}
			//���l����(����)
			if (mouse_x >= val_x - SCREEN_H / 40 && mouse_x <= val_x + SCREEN_H / 40 &&
				mouse_y >= SCREEN_H / 20 * 10 && mouse_y <= SCREEN_H / 20 * 11)
			{
				val_x = mouse_x;
				//�{�����[���P�̍��W������
				int par = (SCREEN_W / 20 * 5) / 100;

				if (val_x <= SCREEN_W / 20 * 10.5)
				{
					val_x = SCREEN_W / 20 * 10.5;
				}
				if (val_x >= SCREEN_W / 20 * 15.2)
				{
					val_x = SCREEN_W / 20 * 15.2;
				}
				
				BGM_vol = (val_x - (SCREEN_W / 20 * 10.5)) / par;
			}

			//���l����(���x)
			if (mouse_x >= sens_x - SCREEN_H / 40 && mouse_x <= sens_x + SCREEN_H / 40 &&
				mouse_y >= SCREEN_H / 20 * 13 && mouse_y <= SCREEN_H / 20 * 14)
			{
				sens_x = mouse_x;
				//�{�����[���P�̍��W������
				int par = (SCREEN_W / 20 * 5) / 10;

				if (sens_x <= SCREEN_W / 20 * 10.5)
				{
					sens_x = SCREEN_W / 20 * 10.5;
				}
				if (sens_x >= SCREEN_W / 20 * 15.2)
				{
					sens_x = SCREEN_W / 20 * 15.2;
				}

				//0~9�ɂȂ�̂łP�^�X
				Sens_vol = (sens_x - (SCREEN_W / 20 * 10.5)) / par + 1;
			}
		}


	}
	Sound = 255 / 100 * BGM_vol;
}

void Option::Draw()
{
	if (OptionMode_ON)
	{
		//�������̍��w�i
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, Black, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//
		DrawBox(SCREEN_W / 8, SCREEN_H / 8, SCREEN_W / 8 * 7, SCREEN_H / 8 * 7, Black, TRUE);

		SetFontSize(48);
		DrawString(SCREEN_W / 20 * 3, SCREEN_H / 20 * 3, "�I�v�V����", White);
		SetFontSize(32);
		//�`���[�g���A���ݒ�
		DrawString(SCREEN_W / 20 * 4, SCREEN_H / 20 * 6, "�`���[�g���A�����[�r�[", White);
		if (tutorial_on)
		{
			DrawOval(SCREEN_W / 20 * 14, SCREEN_H / 20 * 6.5, SCREEN_W / 20 * 1, SCREEN_W / 40, Yellow, true);
			DrawCircle(SCREEN_W / 20 * 14.5, SCREEN_H / 20 * 6.5, SCREEN_H / 40, Red, TRUE);
		}
		else{
			DrawOval(SCREEN_W / 20 * 14, SCREEN_H / 20 * 6.5, SCREEN_W / 20 * 1, SCREEN_W / 40, Yellow, tutorial_on, 3);
			DrawCircle(SCREEN_W / 20 * 13.5, SCREEN_H / 20 * 6.5, SCREEN_H / 40, Red, TRUE);
		}

		SetFontSize(32);
		//���ʐݒ�
		DrawString(SCREEN_W / 20 * 4, SCREEN_H / 20 * 10, "����", White);
		DrawLine(SCREEN_W / 20 * 10.5, SCREEN_H / 20 * 10.5, SCREEN_W / 20 * 15.2, SCREEN_H / 20 * 10.5, White, 8);
		DrawBox(val_x, SCREEN_H / 20 * 10, val_x + SCREEN_H / 60, SCREEN_H / 20 * 11, Red, TRUE);
		SetFontSize(16);
		DrawFormatString(val_x - 16, SCREEN_H / 40 * 22, Red, "%d", BGM_vol);

		SetFontSize(32);
		//���x�ݒ�
		DrawString(SCREEN_W / 20 * 4, SCREEN_H / 20 * 13, "����", White);
		DrawLine(SCREEN_W / 20 * 10.5, SCREEN_H / 20 * 13.5, SCREEN_W / 20 * 15.2, SCREEN_H / 20 * 13.5, White, 8);
		DrawBox(sens_x, SCREEN_H / 20 * 13, sens_x + SCREEN_H / 60, SCREEN_H / 20 * 14, Red, TRUE);
		SetFontSize(16);
		DrawFormatString(sens_x - 16, SCREEN_H / 40 * 28, Red, "%d", Sens_vol);


	}
}

void Option::Exit()
{

}