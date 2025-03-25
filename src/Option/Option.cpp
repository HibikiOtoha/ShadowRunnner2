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
	//オプション画面のオンオフ
	if (PushHitKey(KEY_INPUT_ESCAPE) && OptionMode_ON == false)
	{
		SetMouseDispFlag(TRUE);	//マウスカーソルの表示(全画面)
		OptionMode_ON = true;
	}
	else if(PushHitKey(KEY_INPUT_ESCAPE) && OptionMode_ON == true)
	{
		OptionMode_ON = false;
		SetMouseDispFlag(FALSE);	//マウスカーソルの表示(全画面)
	}
	if (!OptionMode_ON)
	{
		//マウスカーソル固定
		SetMousePoint(SCREEN_W / 2, SCREEN_H / 2);
	}

	//マウスでの設定操作
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
			//オンオフ
			if (mouse_deray == MOUSE_DERAYMAX) {
				mouse_deray = 0;
				//ボタン
				if (mouse_x >= (SCREEN_W / 20 * 13) && mouse_x <= (SCREEN_W / 20 * 15) &&
					mouse_y >= ((SCREEN_H / 20 * 6.5) - (SCREEN_W / 40)) && mouse_y <= ((SCREEN_H / 20 * 6.5) + (SCREEN_W / 40)))
				{
					if (tutorial_on)	tutorial_on = false;
					else tutorial_on = true;
				}
			}
			//数値調整(音量)
			if (mouse_x >= val_x - SCREEN_H / 40 && mouse_x <= val_x + SCREEN_H / 40 &&
				mouse_y >= SCREEN_H / 20 * 10 && mouse_y <= SCREEN_H / 20 * 11)
			{
				val_x = mouse_x;
				//ボリューム１の座標増加量
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

			//数値調整(感度)
			if (mouse_x >= sens_x - SCREEN_H / 40 && mouse_x <= sens_x + SCREEN_H / 40 &&
				mouse_y >= SCREEN_H / 20 * 13 && mouse_y <= SCREEN_H / 20 * 14)
			{
				sens_x = mouse_x;
				//ボリューム１の座標増加量
				int par = (SCREEN_W / 20 * 5) / 10;

				if (sens_x <= SCREEN_W / 20 * 10.5)
				{
					sens_x = SCREEN_W / 20 * 10.5;
				}
				if (sens_x >= SCREEN_W / 20 * 15.2)
				{
					sens_x = SCREEN_W / 20 * 15.2;
				}

				//0~9になるので１タス
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
		//半透明の黒背景
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, SCREEN_W, SCREEN_H, Black, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//
		DrawBox(SCREEN_W / 8, SCREEN_H / 8, SCREEN_W / 8 * 7, SCREEN_H / 8 * 7, Black, TRUE);

		SetFontSize(48);
		DrawString(SCREEN_W / 20 * 3, SCREEN_H / 20 * 3, "オプション", White);
		SetFontSize(32);
		//チュートリアル設定
		DrawString(SCREEN_W / 20 * 4, SCREEN_H / 20 * 6, "チュートリアルムービー", White);
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
		//音量設定
		DrawString(SCREEN_W / 20 * 4, SCREEN_H / 20 * 10, "音量", White);
		DrawLine(SCREEN_W / 20 * 10.5, SCREEN_H / 20 * 10.5, SCREEN_W / 20 * 15.2, SCREEN_H / 20 * 10.5, White, 8);
		DrawBox(val_x, SCREEN_H / 20 * 10, val_x + SCREEN_H / 60, SCREEN_H / 20 * 11, Red, TRUE);
		SetFontSize(16);
		DrawFormatString(val_x - 16, SCREEN_H / 40 * 22, Red, "%d", BGM_vol);

		SetFontSize(32);
		//感度設定
		DrawString(SCREEN_W / 20 * 4, SCREEN_H / 20 * 13, "音量", White);
		DrawLine(SCREEN_W / 20 * 10.5, SCREEN_H / 20 * 13.5, SCREEN_W / 20 * 15.2, SCREEN_H / 20 * 13.5, White, 8);
		DrawBox(sens_x, SCREEN_H / 20 * 13, sens_x + SCREEN_H / 60, SCREEN_H / 20 * 14, Red, TRUE);
		SetFontSize(16);
		DrawFormatString(sens_x - 16, SCREEN_H / 40 * 28, Red, "%d", Sens_vol);


	}
}

void Option::Exit()
{

}