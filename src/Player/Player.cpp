//----------------------------------------------------------------
//! Player.cpp
//----------------------------------------------------------------

#include "WinMain.h"
#include "GameMain.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Player/Player.h"

#include "Anim/Animation.h"	

Animation anim;

//#define DEBUG

Player::Player()
{

	//�����l��ݒ�
	m_pos_.set(DefaultPos);
	m_rot_.y = DefaultRot;

	//�X�e�[�g���X�^���h����J�n
	NowState = STAND;
	anim_rate_[STAND] = 1.0f;
	TouchWall = false;
	autojump_mov = { 0,0,0 };
	CanAutoJump = false;
}

void Player::Init(int model)
{
	//���f������ۑ�
	m_model_ = model;
	//�A�j���[�V�����̓ǂݍ���
	anim_model_[STAND] = MV1LoadModel("data/Player/Anim_idle.mv1");
	anim_model_[RUN] = MV1LoadModel("data/Player/Anim_mainrun.mv1");
	anim_model_[LEFT] = MV1LoadModel("data/Player/Anim_leftmove.mv1");
	anim_model_[RIGHT] = MV1LoadModel("data/Player/Anim_rightmove.mv1");
	anim_model_[BACK] = MV1LoadModel("data/Player/Anim_back.mv1");
	anim_model_[JUMP] = MV1LoadModel("data/Player/Anim_jump.mv1");
	anim_model_[LEFTWALL] = MV1LoadModel("data/Player/Anim_wallrun.mv1");
	anim_model_[RIGHTWALL] = MV1LoadModel("data/Player/Anim_wallrun2.mv1");
	anim_model_[AUTOJUMP] = MV1LoadModel("data/Player/Anim_backflip.mv1");

	//����p
	//anim_model_[STAND] = MV1LoadModel("data/movie/model/Idle.mv1");
	//anim_model_[RUN] = MV1LoadModel("data/movie/model/Running.mv1");

	//�A�j���[�V�������A�^�b�`���Ċ�����������
	for (int i = 0; i < STATE_MAX; i++) {
		anim.AnimInit(m_model_, anim_model_[i], &anim_attach_[i], &anim_total_[i], &anim_frame_[i], &anim_rate_[i]);
	}
	//STAND����
	NowState = STAND;
	anim_rate_[STAND] = 1.0f;

	//�����l��ݒ�
	m_pos_.set(DefaultPos);
	m_rot_.y = DefaultRot;

	light_handle = CreatePointLightHandle(m_pos_,100,0.0f,0.002f,0.0f);
	SetLightDifColorHandle(light_handle, GetColorF(0.05f,0.05f,0.05f,0.0f));
	SetLightSpcColorHandle(light_handle, GetColorF(0.05f, 0.05f, 0.05f, 0.0f));
	SetLightAmbColorHandle(light_handle, GetColorF(0.1f, 0.1f, 0.1f, 0.0f));

	Tutrial_Handle = LoadSoundMem("data/Sounds/tutrial.mp3");
	BGM_Handle = LoadSoundMem("data/Sounds/game.mp3");
	Run_Handle = LoadSoundMem("data/Sounds/Run.mp3");
	Wallrun_Handle = LoadSoundMem("data/Sounds/WallRun.mp3");

	InG_Handle = LoadSoundMem("data/Sounds/G_.mp3");
	Wire_Handle = LoadSoundMem("data/Sounds/Wire.mp3");

}

void Player::Update(int stage_model, Vector3 cam_rot,int Sound)
{
	if (time[0] == 0) {
		PlaySoundMem(Tutrial_Handle, DX_PLAYTYPE_BACK);
	}
	time[0]++;
	if (m_pos_.x <= -120)
	{
		time[1]++;
	}
	if (time[1] == 1)
	{
		StopSoundMem(Tutrial_Handle);
		PlaySoundMem(BGM_Handle, DX_PLAYTYPE_BACK);
	}
	ChangeVolumeSoundMem(Sound-10, Tutrial_Handle);
	ChangeVolumeSoundMem(Sound -10, BGM_Handle);
	ChangeVolumeSoundMem(Sound, Run_Handle);
	ChangeVolumeSoundMem(Sound, Wallrun_Handle);
	ChangeVolumeSoundMem(Sound, Wire_Handle);
	ChangeVolumeSoundMem(Sound, InG_Handle);
	//���Z�b�g
	if ((PushHitKey(KEY_INPUT_M) && PushHitKey(KEY_INPUT_A)) || m_pos_.y <= -100.0f)
	{
		Reset(time[1]);
	}
	SetLightPositionHandle(light_handle, m_pos_);

	//�A�j���[�V�����i�s
	AnimUpdate();

	//���͈ړ��ʂ�ۑ�
	//�Ǒ���̎�
	if (TouchWall && !OnGround)
	{
		//�O��̈ړ��ʂ̂܂ܑ���
		Enter_mov_all_ = BeforeMove;
		CanAutoJump = false;
	}
	//����ȊO�̎��͉�ʂ̕����Ɉړ�����
	else {
		Enter_mov_all_ = EnterMove(cam_rot);
	}

	//�W�����v�Əd��
	Jump_mov_ += JumpMove(stage_model, cam_rot);


	//�Ǒ��肵�Ă���Ƃ�
	if (TouchWall && !OnGround)
	{
		//�d�͂𖳌�
		Jump_mov_.y = 0.0f;
	}

	//�L�[�Ȃǂł̈ړ����͂̈ړ��ʂ��܂Ƃ߂�
	Vector3 key_mov(0,0,0);
	key_mov = (Enter_mov_all_ + Jump_mov_ + Gimmick_WireAction_mov_);
	if(TouchWall && !OnGround)
	{
		key_mov = (Enter_mov_all_ + Jump_mov_);
	}
	//-------------------------------------
	// �ǂ���,�ǃW�����v
	//-------------------------------------
	{
		//���̍��W
		Vector3 next_m_pos_ = m_pos_ + key_mov;

		// �����蔻����Ƃ�A��_�ɍi��
		Vector3 HitPosition = ReturnHitPos(next_m_pos_, stage_model);

		//------------------------------------------------------------------
		// Capsule�����蔻��
		//------------------------------------------------------------------
		MV1_COLL_RESULT_POLY_DIM hit_info = MV1CollCheck_Capsule(
			stage_model,    // ���f���̃n���h��
			-1,                    // �R���W�����̏����X�V����t���[���̔ԍ�
			next_m_pos_ + Vector3{ 0,3,0 },
			next_m_pos_ + Vector3{ 0,5,0 },
			1.5f
		);
		//�������Ă���Ƃ�
		if (hit_info.HitNum >= 1) {

			//�ǂɐG��Ă���
			TouchWall = true;

			//�ǁ��ǂ̎��ɕǃW�����v�̈ړ����x���Ȃ��Ȃ�悤��
			Jump_mov_ = { 0.0f,Jump_mov_.y,0.0f };
			

			int loop_num = hit_info.HitNum;
			//3��ȏ�Q�Ƃ���K�v���Ȃ��̂Ő���
			if (loop_num >= 3)
			{
				loop_num = 3;
			}

			//�ǂ��莞�̏���
			Wall_Rubbing(loop_num, hit_info, &key_mov, next_m_pos_, HitPosition, stage_model);

		}

		//�������Ă��Ȃ��Ƃ�
		else
		{
			//�n�ʂɂ���Ȃ�O�p��їp�̈ړ��ʂ��[���ɌŒ�
			if (OnGround) {
				autojump_mov = { 0,0,0 };
				NowMode = NORMAL;
				CanAutoJump = true;	//�O�p�W�����v�\��
			}
			TouchWall = false;
			leftwallrun = false;
			rightwallrun = false;
		}


	}

	//�ړ��ʂ���ɂ���
	Vector3 all_movement = key_mov;

	GimmickMov_Off = (OnGround || TouchWall);

	//���C���[�A�N�V����
	if (Gimmick_WireAction_mov_.y != 0 )
	{
		//����X�s�[�h�����߂�
		Gimmick_WireAction_mov_.x = max(min(Gimmick_WireAction_mov_.x, 2), -1);
		Gimmick_WireAction_mov_.y = max(min(Gimmick_WireAction_mov_.y, 2), -1);
		Gimmick_WireAction_mov_.z = max(min(Gimmick_WireAction_mov_.z, 2), -1);
		m_pos_ += Gimmick_WireAction_mov_;
		Jump_mov_.y = 0;
		CanAutoJump = false;
	}
	//�O�p���
	else if ((autojump_mov.x != 0 ||autojump_mov.y != 0 || autojump_mov.z != 0))
	{
		m_pos_ += autojump_mov;
		m_pos_.y += all_movement.y;
		CanAutoJump = false;	//���̂��Ƃ�����x�N����Ȃ��悤��
	}

	//�M�~�b�N�̎��R�������܂�
	else {
		//�v���C���[�̍��W�Ɉړ��ʂ𑫂�
		m_pos_ += all_movement;
	}

	//�ЂƂO�̃v���C���[�󋵂�ۑ�
	Before2State = BeforeState;
	BeforeState = NowState;


	//	�����蔻��̏��̍X�V���s��
	MV1RefreshCollInfo(m_model_);
}


void Player::Render()
{
	//���f���̕`��
	MV1SetPosition(m_model_, VGet(m_pos_.x, m_pos_.y, m_pos_.z));
	//Y����180�x��]
	MV1SetRotationXYZ(m_model_, VGet(TO_RADIAN(m_rot_.x), TO_RADIAN(m_rot_.y - 180), TO_RADIAN(m_rot_.z)));
	//���f���̃T�C�Y������������
	MV1SetScale(m_model_, VGet(PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE));
	MV1DrawModel(m_model_);


	//�m�F�p
	//---------------------------------------------------------------------------------------------------------------------------------------
#ifdef DEBUG

	DrawFormatString(0, 0, GetColor(255, 255, 255), "NowState : %d", NowState);

	DrawFormatString(0, 16, GetColor(255, 255, 255), "player_mov_.x : %f,player_mov_.y : %f,player.z : %f", m_pos_.x, m_pos_.y, m_pos_.z);

	DrawFormatString(0, 32, GetColor(255, 255, 255), "Jump_mov.y : %f", Jump_mov_.y);

	DrawFormatString(0, 48, GetColor(255, 255, 255), "player_rot_.y : %f", m_rot_.y);

	DrawFormatString(0, 64, GetColor(255, 255, 255), "CanAutoJump : %d", CanAutoJump);

	for (int i = 0;i < STATE_MAX;i++)
	{
		DrawFormatString(0, 84 + (i * 16), GetColor(255, 255, 255), "anim_state(%d) : %2.0f", i, anim_rate_[i]);
	}


#endif
}

void Player::Exit()
{
	for (int i = 0; i < STATE_MAX; i++) {
		MV1DeleteModel(anim_model_[i]);
		anim.DetachAnim(&anim_model_[i], &anim_attach_[i]);
	}
}


//�A�j���[�V�����̐i�s�Ɛ؂�ւ�(STATE�̕ω����܂�)
void Player::AnimUpdate()
{
	//�A�j���[�V�������I��������ǂ���(�W�����v�p)
	bool anim_end = false;

	//�e��Ԃ̏����X�V
	switch (NowState)
	{
	case STAND:
		StopSoundMem(Run_Handle);
		StopSoundMem(Wallrun_Handle);
		//�A�j���[�V�����̐i�s
		anim.AnimProgress(0.5f, &anim_total_[STAND], &anim_frame_[STAND], &anim_rate_[STAND], true, &anim_end);
		//�؂�ւ����邩�ǂ���
		AnimSwitch();
		break;

	case RUN:
		if (time[2] == 0) {
			PlaySoundMem(Run_Handle, DX_PLAYTYPE_BACK);
		}
		time[2]++;
		StopSoundMem(Wallrun_Handle);
		anim.AnimProgress(0.5f, &anim_total_[RUN], &anim_frame_[RUN], &anim_rate_[RUN], true, &anim_end);
		AnimSwitch();
		break;

	case LEFT:
		StopSoundMem(Run_Handle);
		StopSoundMem(Wallrun_Handle);
		anim.AnimProgress(1.0f, &anim_total_[LEFT], &anim_frame_[LEFT], &anim_rate_[LEFT], true, &anim_end);
		AnimSwitch();
		break;

	case RIGHT:
		StopSoundMem(Run_Handle);
		StopSoundMem(Wallrun_Handle);
		anim.AnimProgress(1.0f, &anim_total_[RIGHT], &anim_frame_[RIGHT], &anim_rate_[RIGHT], true, &anim_end);
		AnimSwitch();
		break;

	case BACK:
		StopSoundMem(Run_Handle);
		StopSoundMem(Wallrun_Handle);
		anim.AnimProgress(1.0f, &anim_total_[BACK], &anim_frame_[BACK], &anim_rate_[BACK], true, &anim_end);
		AnimSwitch();
		break;

	case JUMP:
		StopSoundMem(Run_Handle);
		StopSoundMem(Wallrun_Handle);
		anim.AnimProgress(0.5f, &anim_total_[JUMP], &anim_frame_[JUMP], &anim_rate_[JUMP], false, &anim_end);
		AnimSwitch();

		break;

	case LEFTWALL:
		StopSoundMem(Run_Handle);
		if (time[3] == 0) {
			PlaySoundMem(Wallrun_Handle, DX_PLAYTYPE_BACK);
		}
		time[3]++;
		anim.AnimProgress(0.5f, &anim_total_[LEFTWALL], &anim_frame_[LEFTWALL], &anim_rate_[LEFTWALL], true, &anim_end);
		AnimSwitch();
		break;

	case RIGHTWALL:
		StopSoundMem(Run_Handle);
		if (time[3] == 0) {
			PlaySoundMem(Wallrun_Handle, DX_PLAYTYPE_BACK);
		}
		time[3]++;
		anim.AnimProgress(0.5f, &anim_total_[RIGHTWALL], &anim_frame_[RIGHTWALL], &anim_rate_[RIGHTWALL], true, &anim_end);
		AnimSwitch();
		break;

	case AUTOJUMP:
		StopSoundMem(Run_Handle);
		StopSoundMem(Wallrun_Handle);
		anim.AnimProgress(0.8f, &anim_total_[AUTOJUMP], &anim_frame_[AUTOJUMP], &anim_rate_[AUTOJUMP], false, &anim_end);
		if (anim_frame_[AUTOJUMP] >= anim_total_[AUTOJUMP] || OnGround)
		{
			AnimSwitch();
		}
		break;

	default:
		break;
	}

	for (int i = 0; i < STATE_MAX; i++) {
		anim.AnimUpdate(m_model_, &anim_attach_[i], &anim_frame_[i], &anim_rate_[i]);
	}
	MV1RefreshCollInfo(m_model_);
}

//�A�j���[�V�����̐؂�ւ�
void Player::AnimSwitch()
{
	if (SwitchStand() && NowState != STAND) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[STAND], &anim_attach_[STAND], &anim_rate_[STAND],
			&anim_frame_[STAND], &NowState, STAND);
	}
	else if (SwitchLeftwall() && NowState != LEFTWALL) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[LEFTWALL], &anim_attach_[LEFTWALL], &anim_rate_[LEFTWALL],
			&anim_frame_[LEFTWALL], &NowState, LEFTWALL);
	}
	else if (SwitchRightwall() && NowState != RIGHTWALL) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[RIGHTWALL], &anim_attach_[RIGHTWALL], &anim_rate_[RIGHTWALL],
			&anim_frame_[RIGHTWALL], &NowState, RIGHTWALL);
	}
	else if (SwitchRun() && NowState != RUN) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[RUN], &anim_attach_[RUN], &anim_rate_[RUN],
			&anim_frame_[RUN], &NowState, RUN);
	}
	else if (SwitchBack() && NowState != BACK) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[BACK], &anim_attach_[BACK], &anim_rate_[BACK],
			&anim_frame_[BACK], &NowState, BACK);
	}
	else if (SwitchLeft() && NowState != LEFT) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[LEFT], &anim_attach_[LEFT], &anim_rate_[LEFT],
			&anim_frame_[LEFT], &NowState, LEFT);
	}
	else if (SwitchRight() && NowState != RIGHT) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[RIGHT], &anim_attach_[RIGHT], &anim_rate_[RIGHT],
			&anim_frame_[RIGHT], &NowState, RIGHT);
	}
	else if (SwitchJump() && NowState != JUMP) {
		anim_rate_[NowState] = 0.0f;
		anim.AnimChanger(m_model_, anim_model_[JUMP], &anim_attach_[JUMP], &anim_rate_[JUMP],
			&anim_frame_[JUMP], &NowState, JUMP);
	}
}

Vector3 Player::EnterMove(Vector3 camera_rot)
{
	Enter_mov_ = { 0.0f,0.0f,0.0f };
	Vector3 move;

	//�O�i
	if (CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
	{
		Enter_mov_.z = +RUN_SPEED;

		m_rot_.y = camera_rot.y;

		//�O�i
		move.x = RUN_SPEED * sinf(TO_RADIAN(m_rot_.y));
		move.z = RUN_SPEED * cosf(TO_RADIAN(m_rot_.y));
	}
	//���
	if (CheckHitKey(KEY_INPUT_S) && !CheckHitKey(KEY_INPUT_W))
	{
		Enter_mov_.z = -BACK_SPEED;
		if (NowState != LEFTWALL && NowState != RIGHTWALL) {
			m_rot_.y = camera_rot.y;
		}

		//���
		move.x = -BACK_SPEED * sinf(TO_RADIAN(m_rot_.y));
		move.z = -BACK_SPEED * cosf(TO_RADIAN(m_rot_.y));
	}
	//�A�C�h��
	else if ((!CheckHitKey(KEY_INPUT_W)) && (!CheckHitKey(KEY_INPUT_S)))
	{
		Enter_mov_.z = 0.0f;
	}

	//�u�����N�̒��Ōv�Z�������������̂��ړ��ʂɓ����
	Vector3 all_mov = Blink(move);

	BeforeMove = all_mov;
	return all_mov;
}

Vector3 Player::Blink(Vector3 mov)
{
	//�u�����N�ł��Ȃ��Ƃ�
	if (blink_cooltime_ <= BLINK_COOLTIME)
	{
		//�N�[���_�E���v�Z
		blink_cooltime_++;
	}

	//�u�����N�ł���Ƃ�
	else {
		//�O�ɐi��ł���Ƃ��ɍ��V�t�g����������
		if ((NowState == RUN || NowState == LEFTWALL || NowState == RIGHTWALL) && (CheckHitKey(KEY_INPUT_LSHIFT)))
		{
			mov.x = BLINK_FAR * sinf(TO_RADIAN(m_rot_.y));
			mov.z = BLINK_FAR * cosf(TO_RADIAN(m_rot_.y));
			//�����Ă���Ԃ��̈ʒu����܂������O�Ɉ�苗���ړ�����
			blink_time_++;
			if (blink_time_ >= BLINK_TIME)
			{
				blink_time_ = 0;
				blink_cooltime_ = 0;
			}
		}
		//�O�t���[���u�����N���Ă��č���V�t�g�������Ă��Ȃ��Ƃ�
		else if (blink_time_ >= 0.1f && !(CheckHitKey(KEY_INPUT_LSHIFT)))
		{
			//�������u�Ԉړ��ʂ̑����͎~�܂�N�[���_�E��
			blink_time_ = 0;
			blink_cooltime_ = 0;
		}
	}

	return mov;
}

//�W�����v�Əd��
Vector3 Player::JumpMove(int stage_model, Vector3 cam_rot)
{
	//�J�v�Z������(�v���C���[�̑����ɃJ�v�Z��������Ĕ���)
	//	�J�v�Z���̊J�n���W�i�v���C���[���W�j
	Vector3 start_pos = m_pos_;
	//	�J�v�Z���̃S�[�����W�i�v���C���[���W�̏������j
	Vector3 goal_pos = m_pos_ - Vector3{0.0f,-3.0f,0.0f};
	//	�����蔻����������ʂ𓖂�����Ƃ��Ď擾���܂�
	MV1_COLL_RESULT_POLY_DIM jump_hit_info =
		MV1CollCheck_Capsule(stage_model, -1, start_pos.VGet(), goal_pos.VGet(),RADIAN);

	 Vector3 ground_pos;	//�n�ʂ̍���
	//	�n�ʂƓ������Ă�����
	if (jump_hit_info.HitNum != 0) {
		ground_pos = ReturnHitPos(goal_pos, stage_model);	//�n�ʍ��W�o�^
	}
	//�n�ʂ̏�ɂ��邩�ǂ���
				//�n�ʂɂ���				//�v���C���[���W���n�ʂɓ������Ă���(or ���܂��Ă���)
	OnGround = (jump_hit_info.HitNum !=0  && m_pos_.y <= ground_pos.y);


	//--
	//�n�ʂ���̃W�����v�̈ړ��ʂ̓��ꕨ
	Vector3 jump_mov;
	//�n�ʂ̏�ɂ���Ƃ�
	if (OnGround) {

		Vector3 mov_Normal = goal_pos - m_pos_;
		Vector3 hit_Normal = jump_hit_info.Dim->Normal;
		float mov_dot = GetVector3Dot(mov_Normal, hit_Normal);

		//�d�͂��O�ɂ��A�v���C���[���W��n�ʂ̍��W��
		jump_mov = { 0.0f,0.0f,0.0f };
		Jump_mov_ = { 0.0f,0.0f,0.0f };
		//m_pos_.y = ground_pos.y;
		m_pos_.y +=  ground_pos.y - m_pos_.y ;

		//�W�����v�ɐ؂�ւ��Ƃ�
		if (PushHitKey(KEY_INPUT_SPACE))
		{
			jump_mov = jump_hit_info.Dim->Normal;
			jump_mov.SetLength(JUMP_POWER);

			//�M�~�b�N�̕ω��ړ�������
			jump_mov += Gimmick_JumpBoard_mov_;
		}
	}

	jump_mov.y = max(min(jump_mov.y, 2), -1);

	//�d�͕��𑫂�
	jump_mov.y -= DOWN_SPEED;

	//--

	//--
	//�ǃW�����v�̈ړ��ʂ̓��ꕨ
	Vector3 walljump_mov;
	bool iswalljump = false;

	//�ǃW�����v�̎�(�W�����v���łȂ��Ƃ���)
	//�ǂɐG���Ă��ăW�����v���J�n�ł��Ȃ�(�󒆂�)��
	if (TouchWall && NowState != JUMP)
	{
		walljump_mov = { 0.0f,0.0f,0.0f };
		Jump_mov_ = { 0.0f,0.0f,0.0f };

		//	�J�����������Ă�����ɉ�]�����邽�߂̉�]�s��쐬
		MATRIX mat_walljump = MGetRotY(TO_RADIAN(cam_rot.y));

		if (PushHitKey(KEY_INPUT_SPACE))
		{
			//	�W�����v�J�n
			walljump_mov = Vector3{ 0,-1.0,0 };

			iswalljump = true;
		}
		CanAutoJump == false;
	}
	if (iswalljump)
	{
		//�J���������Ɉړ�
		walljump_mov.x = RUN_SPEED * sinf(TO_RADIAN(cam_rot.y));
		walljump_mov.z = RUN_SPEED * cosf(TO_RADIAN(cam_rot.y));
		walljump_mov.y = JUMP_POWER;

		walljump_mov.SetLength(WALLKICK_POWER);

		if (walljump_mov.y <= 0)
		{
			iswalljump = false;
		}
	}
	//--
	//�ړ��ʂ𑫂��ĕԂ�
	return jump_mov + walljump_mov;
}

// �����蔻����Ƃ�A��_�ɍi��
Vector3 Player::ReturnHitPos(Vector3 next_pos, int stage_model)
{
	Vector3 PosBase = next_pos;
	PosBase.y += 5;
	//���̍��W�Ǝ��̍��W�ŃJ�v�Z������蓖���蔻��
	MV1_COLL_RESULT_POLY_DIM line_hit_info = MV1CollCheck_Capsule(stage_model, -1, PosBase, m_pos_, RADIAN);
	int HitNum = line_hit_info.HitNum;
	MV1CollResultPolyDimTerminate(line_hit_info);	//���̃��[�v�Ŏg���̂Ō�n��
	//�������Ă����(�����������W����ȏ�Ȃ�)
	if (HitNum != 0)
	{
		float NotHitLength;	//������Ȃ�����
		float HitLength;	//������������
		float TestLength;		//�����̓��ꕨ(��)
		VECTOR TestPosition;	//�����邩�ǂ����̃e�X�g�p
		VECTOR Direction;		//�v���C���[����J�����ւ̕����x�N�g���̓��ꕨ

		//�|���S���ɓ�����Ȃ��������Z�b�g
		NotHitLength = 0.0f;

		//�|���S���ɓ����鋗�����Z�b�g
		HitLength = VSize(VSub(m_pos_, PosBase));

		//�v���C���[�̈ړ��������Z�o
		Direction = VNorm(VSub(m_pos_, PosBase));
		do
		{
			//�����邩�ǂ����e�X�g���鋗�����Z�b�g(�����鋗���Ɠ�����Ȃ������̒���)
			TestLength = NotHitLength + (HitLength - NotHitLength) / 2.0f;

			//�e�X�g�p�̍��W���Z�o
			TestPosition = VAdd(VScale(Direction, TestLength), PosBase);

			//�V�������W�ŕǂɓ����邩�e�X�g
			line_hit_info = MV1CollCheck_Capsule(stage_model, -1, PosBase, TestPosition, RADIAN);
			HitNum = line_hit_info.HitNum;
			MV1CollResultPolyDimTerminate(line_hit_info);
			if (HitNum != 0)
			{
				//���������瓖���鋗����TestLength�ɕύX
				HitLength = TestLength;
			}
			else
			{
				//������Ȃ������瓖����Ȃ�������TestLength�ɕύX����
				NotHitLength = TestLength;
			}
			//�\���߂Â��܂Ń��[�v
		} while (HitLength - NotHitLength > 0.001f);


		//��_��Ԃ�
		return TestPosition;
	}
}

//�ǂ���̔���
void Player::Wall_Rubbing(int loop_num, MV1_COLL_RESULT_POLY_DIM hit_info, Vector3* Key_mov, Vector3 next_pos, Vector3 HitPosition, int stage_model)
{
	//���̈ړ��ʂ�ێ�
	Vector3 before_mov = *Key_mov;
	for (int i = 0; i < loop_num; i++) {

		Vector3 mov_Normal = next_pos - m_pos_;
		Vector3 hit_Normal = hit_info.Dim[i].Normal;
		float mov_dot = GetVector3Dot(mov_Normal, hit_Normal);

		wallrun_dot = mov_dot;

		//�ǂ̖@�������Ƌt���Ɍ������Ă���Ƃ�(�ǂɌ������Ĉړ����Ă���Ƃ�)
		if (mov_dot < 0)
		{
			//�Ǒ��肹�����̏��(�ǂւ̓��ˊp���󂩂����炻�̏�Ŏ����ǃW�����v)
			if ((mov_dot >= -1.0f && mov_dot <= -0.7f) && !OnGround && !leftwallrun && !rightwallrun && (BeforeState != WALLRUN && Before2State != WALLRUN) && CanAutoJump == true)
			{
				//�؂�ւ��n�߂̎�
				if (autojump_mov.x == 0 && autojump_mov.z == 0) {
					int before_state = NowState;

					anim_rate_[before_state] = 0.0f;
					NowState = AUTOJUMP;
					anim.AnimChanger(m_model_, anim_model_[AUTOJUMP], &anim_attach_[AUTOJUMP], &anim_rate_[AUTOJUMP],
						&anim_frame_[AUTOJUMP], &NowState, AUTOJUMP);
				}

				//�ǂƔ��Α��ɔ�΂�
				autojump_mov = hit_Normal;
				autojump_mov.y = 0.5f;
				autojump_mov.SetLength(AUTOJUMP_POWER / 2);
			}

			if (hit_Normal.x != 0) {
				//�߂荞�ޑO�Ɉړ��ʂ����������Ƃ���܂łɖ߂�
				Key_mov->x = (next_pos.x + autojump_mov.x) - HitPosition.x;
			}
			if (hit_Normal.z != 0) {
				//�߂荞�ޑO�Ɉړ��ʂ����������Ƃ���܂łɖ߂�
				Key_mov->z = (next_pos.z + autojump_mov.z) - HitPosition.z;
			}			
			//�v���C���[�̍��E�ɖ@���������A�E�̕ǂɓ������������̕ǂɓ����������𔻒�
			CheckWhichWall(stage_model);
		}
	}

}

//�v���C���[�̍��E�ɖ@���������A�E�̕ǂɓ������������̕ǂɓ����������𔻒�
void Player::CheckWhichWall(int stage_model)
{
	//�m�F�Ɏg���|�C���g�̐�
	const int checkpoint = 2;
	float check_lot[checkpoint] =
	{
		m_rot_.y + (45 * 5),
		m_rot_.y + (45 * 7),
	};
	//�����蔻��֌W�ۑ��p�̂��̂��m�F�|�C���g�����p��
	MV1_COLL_RESULT_POLY wallhit_info[checkpoint];

	for (int j = 0; j < checkpoint; j++) {
		//�v���C���[�̕ǂɓ������Ă���������擾���邽�߂̃��C���쐬
		Vector3 player_pos = m_pos_ + Vector3{ 0.0f, 5.0f, 0.0f };
		MATRIX mat = MGetRotY(TO_RADIAN(check_lot[j]));
		Vector3 base(RADIAN + 0.5f, 0.0f, 0.0f);
		Vector3 move = GetVector3VTransform(base, mat);

		Vector3 hitcheck_pos = player_pos + move;

		wallhit_info[j] = MV1CollCheck_Line(stage_model, -1, player_pos, hitcheck_pos);

#ifdef DEBUG
		DrawLine3D(player_pos, hitcheck_pos, GetColor(255, 0, 0));
#endif
	}
	//�E�����ǂ���ɕǂ����邩
	if ((wallhit_info[0].HitFlag || wallhit_info[1].HitFlag) && !OnGround)
	{
		if (m_pos_.x > 0) {
			if ((m_pos_.x - wallhit_info[0].HitPosition.x) >= (m_pos_.x - wallhit_info[1].HitPosition.x))
			{
				rightwallrun = true;
				leftwallrun = false;
			}
			else if ((m_pos_.x - wallhit_info[0].HitPosition.x) <= (m_pos_.x - wallhit_info[1].HitPosition.x))
			{
				leftwallrun = true;
				rightwallrun = false;
			}
		}
		else {
			if ((m_pos_.x - wallhit_info[0].HitPosition.x) >= (m_pos_.x - wallhit_info[1].HitPosition.x))
			{
				leftwallrun = true;
				rightwallrun = false;
			}
			else if ((m_pos_.x - wallhit_info[0].HitPosition.x) <= (m_pos_.x - wallhit_info[1].HitPosition.x))
			{
				rightwallrun = true;
				leftwallrun = false;
			}
		}
	}
}

void Player::Reset(int CheckPoint)
{
	
		m_pos_.set(DefaultPos);
		m_rot_.y = DefaultRot;
		if (CheckPoint >= 1)
		{
			m_pos_.x = -120;
		}

		Jump_mov_ = { 0.0f,0.0f,0.0f };
		Gimmick_JumpBoard_mov_ = { 0.0f,0.0f,0.0f };
	
}