#pragma once

class Ending {
private:

	//�G���h�V�[���̉摜
	int End_image;
	//�v���C���[�̉摜
	int player_image;

	int bEnd_image;

	//�N���A���A���s��
	bool clear;
	

public:
	Ending();

	//�؂�ւ��\���ǂ���
	bool Scene_Change;


	void Init(int player_model);
	void Update(bool gameclear);
	void Render();
	void Exit();
};