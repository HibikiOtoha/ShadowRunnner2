#include "WinMain.h"
#include "GameMain.h"
#include "Vector3.h"
#include <cmath>

#include "Base/GimmickBase.h"
#include "WireActions.h"

//���C���[�A�N�V����

//#define DEBUG

WireActions::WireActions()
{
	m_pos_ = { 0,0,0 };

	Y_mov = 0;

	mov = { 0,0,0 };

	actionflag = false;

	cooltime = COOLTIME_MAX;
}

bool WireActions::CheckOnHitPlayer(Vector3 player_pos)
{
	bool flag = false;

	// �v���C���[�Ƃ��̃M�~�b�N�̋���
	LinkLine = GetVector3Distance(m_pos_ , player_pos);

	//�v���C���[����M�~�b�N�܂ł̒�������
	if (LinkLine <= GIMMICK_RANGE)
	{
		flag = true;
	}
	return flag;	
}

bool WireActions::CheckOnHitCenter(Vector3 player_pos)
{
	bool flag = false;

	// �v���C���[�Ƃ��̃M�~�b�N���S�܂ł̋���
	LinkLine = GetVector3Distance(m_pos_, player_pos);

	//�v���C���[����M�~�b�N���S�܂ł̒�������
	if (LinkLine <= GIMMICK_CENTER_RANGE)
	{
		flag = true;
	}
	return flag;
}


void WireActions::Update(Vector3 player_pos, Vector3* Gimmick_mov,float* player_rot_y,Vector3 cam_rot,bool Move_Off)
{
	//�N�[���^�C���v�Z
	SetCoolTime();

	//�M�~�b�N���g�p�\���ǂ���
	bool Usable = (!(CheckCameraViewClip(m_pos_)) && cooltime == 0);

	// �v���C���[����M�~�b�N���W�Ɍ������x�N�g�����Z�o
	Vector3 SubVector = VSub(m_pos_, player_pos);

	//�J�����̌�����ۑ�
	cam_rot = this->cam_rot;

	

	if (CheckOnHitPlayer(player_pos) && CheckHitKey(KEY_INPUT_E) && Usable)
	{
		//�A�N�V�����X�^�[�g
		actionflag = true;

		//�v���C���[���M�~�b�N�Ɍ������邽�߂̕����x�N�g��
		Vector3 RotSubVector = VSub(player_pos, m_pos_);

		//�v���C���[�����̃M�~�b�N���Ɍ�����
		// atan2 ���g�p���Ċp�x���擾
		float Angle = atan2(RotSubVector.x, RotSubVector.z) + DX_PI_F;

		//�v���C���[�̌�����ύX����(�M�~�b�N���Ɍ�����)
		*player_rot_y = TO_DEGREE(Angle);

		//���ړ��ʌv�Z
		mov_length = sqrt((SubVector.x * SubVector.x) + (SubVector.y * SubVector.y) + (SubVector.z * SubVector.z));

		beforeUse = true;
	}
	else
	{
		if (beforeUse == true) {
			cooltime = COOLTIME_MAX;
			
		}
		beforeUse = false;
		actionflag = false;
	}
	

	if (actionflag) {
		//�v���C���[���炱�̃M�~�b�N�܂ł̋����Z�o
		float Length = GetVector3Distance(m_pos_, player_pos);
		Natura_numbers(&Length);

		//�v���C���[����M�~�b�N�܂ł̋������K��X�s�[�h�ȏ�Ȃ�
		if (!CheckOnHitCenter(player_pos)) {
			//�K��X�s�[�h���M�~�b�N�����Ɉړ���
			mov.x = (SubVector.x / mov_length) * WIREMOVE_SPEED_RATE;
			mov.z = (SubVector.z / mov_length) * WIREMOVE_SPEED_RATE;
			mov.y = (SubVector.y / mov_length) * WIREMOVE_SPEED_RATE;			
		}
		//�v���C���[����M�~�b�N�܂ł̋��������̂܂܈ړ����I������ꍇ
		else {
			//mov = VSub(player_pos, m_pos_);
			//�N�[���^�C��������
			cooltime = COOLTIME_MAX;
			actionflag = false;
		}
	}
	else {
		mov.y = 0.0f;
		if (Move_Off) {
			mov.clear();
		}
	}

	*Gimmick_mov += mov;
}

void WireActions::Render(Vector3 player_pos)
{
#ifdef DEBUG 
		DrawCapsule3D(m_pos_, m_pos_, GIMMICK_RANGE, 1, GetColor(255, 0, 0), GetColor(255, 255, 255), false);
#endif
		//�g����
		Vector3 length = { 0,5,0 };
		bool active = CheckCameraViewClip(m_pos_);
	DrawCapsule3D(m_pos_ + length, m_pos_ + length, GIMMICK_CENTER_RANGE, 1, GetColor(255, 255, 0), GetColor(200, 200, 200), !active);


	if (actionflag) {
		DrawLine3D(m_pos_, player_pos + Vector3(0.0f, 7.0f, 0.0f), GetColor(100, 100, 255));
	}
}

void WireActions::SetCoolTime()
{
	frame_time++;
	if (frame_time >= 60)
	{
		cooltime--;
		frame_time = 0;
	}

	if (cooltime <= 0) {
		cooltime = 0;
	}
}

void WireActions::MoveClear()
{
	mov.clear();
}