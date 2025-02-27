#pragma once

//----------------------------------------------------------------
//! player ui.h
//----------------------------------------------------------------

class PlayerUI {
private:

	//-------------------------------
	//目を開ける演出用
	//-------------------------------
	void OpenEyes();	//目を開ける処理
	void Eyes_Draw();	//瞼表現の描画
	bool Eye_Open;	//目を開ける処理が終わっているかどうか
	//初期座標
	Vector3 OpenEyes1;
	Vector3 OpenEyes2;
#define EYES 2
	Vector2 OpenEye[EYES];
#define HEAF_EYE  30	//半開き
	//開く時間管理用
	int eye_counter;

	//動画データの保存場所
	int Wallrun_movie;
	int Wire_movie;
	int WireToWall_movie;


	//色情報
	int Black = GetColor(0,0,0);

	//時間
	//int minute,time, conma;
	int time_x, time_y;
	int fontsize;
	int color;



	#define save  4

public:
	PlayerUI();

	int minute, time, conma;
	bool timeOver;

	void Update(float cooltime, bool& eyeOpen, bool ClearFlag[], int& MoviePlay, bool tutrial_mode, bool tutrial_eria);
	void Draw();
	void Exit();
};