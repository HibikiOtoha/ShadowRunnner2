#pragma once

//----------------------------------------------------------------
//! option.h
//----------------------------------------------------------------

class Option {
private:

	//ボリュームバー
	int val_x;
	//感度バー
	int sens_x;

	//色データ
	int White = GetColor(255, 255, 255);
	int Black = GetColor(0,0,0);
	int Yellow = GetColor(255, 255, 0);
	int Red = GetColor(255,100, 100);

	//Vector2 click_pos = {SCREEN_W,}

	//マウスを押したとき一度反応させるためのディレイ
#define MOUSE_DERAYMAX 30
	int mouse_deray;

public:
	Option();

	//オプションを使用するかどうか
	bool OptionMode_ON;

	//外部参照
	float camera_pos;
	int BGM_vol;
	bool tutorial_on;
	int Sens_vol;


	void Update(int Scene_num, int& Sound);
	void Draw();
	void Exit();
};