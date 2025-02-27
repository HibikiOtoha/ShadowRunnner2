#pragma once
//----------------------------------------------------------------
//! Stage.h
//----------------------------------------------------------------

class Stage {
private:

public:
	Stage();

	//���f��������ϐ�
	int m_model_;	//���f��
	int sky_model;		//�X�J�C�{�b�N�X�̃��f��

	Vector3 m_pos_;   //�X�e�[�W���f���̈ʒu
	Vector3 sky_rot;	// �󃂃f���̌���

	Vector3 goal_pos;	//�S�[���̈ʒu
	const Vector3 goal_size = {-80,20,110};
	bool goalflag;

#define GIMMICK_MAX 5
	Vector3 Gimmick_pos[GIMMICK_MAX];

	Vector3 player_pos;

	void ReSet();

	void Init(int model,int skymodel);
	void Update(Vector3 player_pos, float* player_rot_y, Vector3* JumpBoard_mov, Vector3* WireAction_mov, Vector3 cam_rot, bool GimmickMov_Off);
	void Render();
	void Exit();
};