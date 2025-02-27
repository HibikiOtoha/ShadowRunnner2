#pragma once

class Game {
public:
	enum SAVE
	{
		WALL,
		WIRE,
		WIRE2,
		EX,

		SAVE_MAX
	};


private:
	//�X�e�[�W���f��
	int stage_model;
	//�X�J�C�{�b�N�X���f��
	int sky_model;
	//�v���C���[���f��
	int player_model;

	//�u���[�p�̉摜�𐮗�����֐�
	void Blur();

#define IMAGE_MAX 10
	//
	int count;
	//���[�V�����u���[���̔������̃X�N���[�����������ޗp�̕ϐ�
	int screen_image[IMAGE_MAX];
	//int screen_image;
	//��t���[���̉摜��ۑ����������ŕ`�悷��p�̕ϐ�
	int blur_image[IMAGE_MAX];

	//�J�n���ڂ��󂢂Ă��邩
	bool eyeOpen = false;


	//�`�F�b�N�|�C���g�̍��W
	float SavePoint[SAVE_MAX];
	//�`�F�b�N�|�C���g���N���A����(�ʂ���)��
	bool ClearFlag[SAVE_MAX];
	//����̍Đ����I����Ă��邩�ǂ���(��x�ڂ̍Đ����Ȃ��悤��)
	int PlayMovie;

	//�v���C���[���Z�[�u�|�C���g�ɂ��邩�ǂ���
	void CheckSave( float posx);

public:
	Game();

	//�؂�ւ��\���ǂ���
	bool Scene_Change;
	bool Game_Clear;


	void Init(int M_Stage,int M_Sky,int M_Player);
	void Update(bool tutrial_mode, int Sound,int Sens);
	void Render();
	void Exit();

	void Reset();
};