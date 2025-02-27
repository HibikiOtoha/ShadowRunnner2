#pragma once

//----------------------------------------------------------------
//! WireActions.h
//----------------------------------------------------------------

class WireActions :public GimmickBase {
private:

	//�M�~�b�N�̌��ʔ͈�(����͈�)
#define GIMMICK_RANGE 85.0f
#define GIMMICK_CENTER_RANGE 5.0f

#define WIREMOVE_SPEED_RATE 3.0f

#define RESISTANCE_POWER 0.0001f;

#define COOLTIME_MAX 2

	bool actionflag = false;

	// �v���C���[�Ƃ��̃M�~�b�N�̋���
	float LinkLine;

	//Y���̈ړ���
	float Y_mov;

	//�N�[���^�C��
	float cooltime;
	//�t���[�����𐔂���
	float frame_time;

	//(�N�[���^�C��)���Ԍv���p
	void SetCoolTime();

	//���莞�̃v���C���[����M�~�b�N�܂ł̋���
	float mov_length = 0;

	//�`��p
	Vector3 cam_rot;

	//�ЂƂO�̃t���[���ŃL�[�����Ă��邩�ǂ���
	bool beforeUse = false;

public:
	WireActions();

	
	//�v���C���[���M�~�b�N�̓K�����ɂ��邩�ǂ���
	bool CheckOnHitPlayer(Vector3 player_pos);

	//�v���C���[���M�~�b�N�̒��S���ɂ��邩�ǂ���
	bool CheckOnHitCenter(Vector3 player_pos);

	//�ړ��ʂ����ׂč폜
	void MoveClear();


	void Update(Vector3 player_pos, Vector3* Gimmick_mov, float* player_rot, Vector3 cam_rot, bool Move_Off);
	void Render(Vector3 player_pos);
};