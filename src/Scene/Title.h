#pragma once

class Title {
private:

	//�^�C�g���̉摜
	int Title_image;
	int Title_image_1;
	int Title_image_2;
	int Title_image_3;
	int Title_image_4;
	int Title_image_5;
	int Title_image_6;

	int rand_[6];

	//�ڂ���邽�߂̍��W������ϐ�
#define EYE 2
	Vector2 Eyes[EYE];
	//�ڂ����
	void CloseEyes();
#define CLOSE_SPEED 3.0f
	bool close;
	

public:
	Title();

	//�؂�ւ��\���ǂ���
	bool Scene_Change;

	int BGM_Handle;
	int IN_Handle;
	int time_ = 0;

	void Init();
	void Update(int Sound);
	void Render();
	void Exit();
};