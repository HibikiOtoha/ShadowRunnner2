#pragma once

//----------------------------------------------------------------
//! player ui.h
//----------------------------------------------------------------

class PlayerUI {
private:

	//-------------------------------
	//�ڂ��J���鉉�o�p
	//-------------------------------
	void OpenEyes();	//�ڂ��J���鏈��
	void Eyes_Draw();	//�ٕ\���̕`��
	bool Eye_Open;	//�ڂ��J���鏈�����I����Ă��邩�ǂ���
	//�������W
	Vector3 OpenEyes1;
	Vector3 OpenEyes2;
#define EYES 2
	Vector2 OpenEye[EYES];
#define HEAF_EYE  30	//���J��
	//�J�����ԊǗ��p
	int eye_counter;

	//����f�[�^�̕ۑ��ꏊ
	int Wallrun_movie;
	int Wire_movie;
	int WireToWall_movie;


	//�F���
	int Black = GetColor(0,0,0);

	//����
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