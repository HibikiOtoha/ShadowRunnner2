#include "WinMain.h"
#include "GameMain.h"
#include "Dxlib.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Scene/Game.h"
#include "PlayerUI.h"


PlayerUI::PlayerUI()
{
	
	OpenEye[0] = { 0,0 };
	OpenEye[1] = { 0,SCREEN_H / 2 };
	
	eye_counter = 0;

	Eye_Open = false;

	time = 0;
	conma = 0;

	fontsize = 128;
	time_x = SCREEN_W / 7 * 2;
	time_y = SCREEN_H / 2 - fontsize;
	color = GetColor(200, 50, 50);
	timeOver = false;
}

void PlayerUI::Update(float cooltime, bool& eyeOpen, bool ClearFlag[], int& MoviePlay, bool tutrial_mode,bool tutrial_eria)
{
	//目を閉じる処理
	OpenEyes();

	eyeOpen = Eye_Open;
	if (tutrial_mode) {
		//チュートリアル動画再生
		if (ClearFlag[Game::WALL] && MoviePlay == Game::WALL - 1 && CheckHitKey(KEY_INPUT_P)) {
			SeekMovieToGraph(Wallrun_movie, 0);
			Wallrun_movie = PlayMovie("data/movie/WallRun.mp4", 1, DX_MOVIEPLAYTYPE_NORMAL);
			if (GetMovieStateToGraph(Wallrun_movie))
			{
				MoviePlay++;
				PauseMovieToGraph(Wallrun_movie);
			}
			
		}
		else if (ClearFlag[Game::WIRE] && MoviePlay == Game::WIRE - 1 && CheckHitKey(KEY_INPUT_P)) {
			SeekMovieToGraph(Wire_movie, 0);
			Wire_movie = PlayMovie("data/movie/WireAction.ogv", 1, DX_MOVIEPLAYTYPE_NORMAL);
			if (GetMovieStateToGraph(Wire_movie))
			{
				MoviePlay++;
				PauseMovieToGraph(Wire_movie);
			}
		}
		else if (ClearFlag[Game::WIRE2] && MoviePlay == Game::WIRE2 - 1 && CheckHitKey(KEY_INPUT_P)) {
			SeekMovieToGraph(WireToWall_movie, 0);
			WireToWall_movie = PlayMovie("data/movie/WireToWall.ogv", 1, DX_MOVIEPLAYTYPE_NORMAL);
			if (GetMovieStateToGraph(WireToWall_movie))
			{
				MoviePlay++;
				PauseMovieToGraph(WireToWall_movie);
			}
		}
	}

	//三分を切ったら真ん中にもう一度表示
	if (minute >= 5 && time == 0)
	{
		fontsize = 128;
		time_x = SCREEN_W / 7 * 2;
		time_y = SCREEN_H / 2 - fontsize;
		color= GetColor(200, 0, 0);
	}
	//時間切れ
	if (7 - minute <= 0 && 59 - time <= 0)
	{
		timeOver = true;
	}
}

void PlayerUI::Draw()
{	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
	DrawBox(SCREEN_W / 4 * 3, SCREEN_H / 8 * 7 - 8, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetFontSize(16);
	DrawString(SCREEN_W / 4 * 3 + 16, SCREEN_H / 8 * 7, "W A S D  移動",GetColor(255,255,255));
	DrawString(SCREEN_W / 4 * 3 + 16, SCREEN_H / 8 * 7 + 22, "マウス移動　カメラ移動", GetColor(255, 255, 255));
	DrawString(SCREEN_W / 4 * 3 + 16, SCREEN_H / 8 * 7 + 44, "右クリック  ワイヤーアクション", GetColor(255, 255, 255));
	DrawString(SCREEN_W / 4 * 3 + 16, SCREEN_H / 8 * 7 + 66, "P    チュートリアルムービー再生", GetColor(255, 255, 255));

	//最初の目を開く処理
	Eyes_Draw();
}

void PlayerUI::Exit()
{

}

void PlayerUI::OpenEyes()
{
	eye_counter++;

	//最初半分目を開ける
	if (eye_counter <= HEAF_EYE)
	{
		//開く
		OpenEye[0].y -= 2;
		OpenEye[1].y += 2;
	}
	//半分開けた目を倍の速さで閉じる
	else if (eye_counter <= (HEAF_EYE + (HEAF_EYE / 2)))
	{
		//閉じる
		OpenEye[0].y += 4;
		OpenEye[1].y -= 4;
	}
	else if (eye_counter <= (HEAF_EYE + ((HEAF_EYE / 2)) + HEAF_EYE / 4))
	{
		OpenEye[0].y --;
		OpenEye[1].y ++;
	}
	else if (eye_counter <= (HEAF_EYE + (HEAF_EYE)))
	{
		//閉じる
		OpenEye[0].y ++;
		OpenEye[1].y --;
	}
	else if (eye_counter <= (HEAF_EYE *5))
	{
		//完全に開く
		OpenEye[0].y -= 5;
		OpenEye[1].y += 5;
	}
	else {

		//開ける処理が終わった
		Eye_Open = true;
	}
	//タイム計測
	if (Eye_Open)
	{
		conma++;
		if (time >= 2) {
			fontsize--;
			time_x += 5;
			time_y -= 2;
		}
		if (fontsize <= 64)
		{
			fontsize = 64;
		}
		if (time_x >= SCREEN_W - 33 * 8) time_x = SCREEN_W - 33 * 8;
		if (time_y <= 0) time_y = 0;
	}
	if (conma == 60)
	{
		conma = 0;
		time++;
	}
	if (time == 60 )
	{
		time = 0;
		minute++;
	}
}

void PlayerUI::Eyes_Draw()
{
	//瞼
	DrawBox(OpenEye[0].x, OpenEye[0].y,
		OpenEye[0].x + SCREEN_W, OpenEye[0].y + (SCREEN_H / 2),
		Black, TRUE);
	DrawBox(OpenEye[1].x, OpenEye[1].y,
		OpenEye[1].x + SCREEN_W, OpenEye[1].y + (SCREEN_H / 2),
		Black, TRUE);

	//目を開けたら制限時間を表示
	if (Eye_Open)
	{
		SetFontSize(fontsize);
		DrawFormatString(time_x, time_y, color, "%02d:%02d:%01d", 7 - minute, 59 - time, 99 - (conma % 60) * (5 / 3));
	}

	//目を開いた時の少しぼやけた感じを黒の半透明のボックスで表現
	int blend = 180 - eye_counter;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend);
	DrawBox(0, 0, SCREEN_W, SCREEN_H, Black, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

}

