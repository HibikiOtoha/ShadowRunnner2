#pragma once

//----------------------------------------------------------------
//! option.h
//----------------------------------------------------------------

class Option {
private:

	//�{�����[���o�[
	int val_x;
	//���x�o�[
	int sens_x;

	//�F�f�[�^
	int White = GetColor(255, 255, 255);
	int Black = GetColor(0,0,0);
	int Yellow = GetColor(255, 255, 0);
	int Red = GetColor(255,100, 100);

	//Vector2 click_pos = {SCREEN_W,}

	//�}�E�X���������Ƃ���x���������邽�߂̃f�B���C
#define MOUSE_DERAYMAX 30
	int mouse_deray;

public:
	Option();

	//�I�v�V�������g�p���邩�ǂ���
	bool OptionMode_ON;

	//�O���Q��
	float camera_pos;
	int BGM_vol;
	bool tutorial_on;
	int Sens_vol;


	void Update(int Scene_num, int& Sound);
	void Draw();
	void Exit();
};