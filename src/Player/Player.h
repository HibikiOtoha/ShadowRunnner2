#pragma once
//----------------------------------------------------------------
//! player.h
//----------------------------------------------------------------

class Player {
public:
#define RUN_SPEED  1.0f			//�O�ړ��̃X�s�[�h

#define BACK_SPEED 0.1f		//���ɉ�����X�s�[�h
#define JUMP_POWER	1.0f		//�W�����v��
#define DOWN_SPEED	0.05f		//�d��
#define AUTOJUMP_POWER	1.0f		//�����̃W�����v��
#define AUTOJUMP_DOWN_SPEED	0.05f		//������
#define WALLKICK_POWER 2.0f		//�ǏR��̗�

#define BLINK_FAR 1.5f			//�u�����N�̑��x
#define BLINK_TIME 10			//�u�����N���ԏ��(�t���[��)
#define BLINK_COOLTIME 120			//�u�����N���ԏ��(�t���[��)

#define PLAYER_SIZE	0.05f
#define RADIAN 1.5f	//���a

private:
	//�����l
	
	Vector3 DefaultPos{625,30,0 };

	//test(�ǁ[�ǁ[���C���[��)
	//Vector3 DefaultPos{ -155,30,0 };

	float DefaultRot = 270.0f;

	//�v���C���[��
	enum STATE {
		STAND,		//����
		RUN,		//����
		LEFT,		//���ړ�
		RIGHT,		//�E�ړ�
		BACK,		//���
		JUMP,		//�W�����v
		LEFTWALL,	//�Ǒ���(���ɕǂ����鎞)
		RIGHTWALL,	//�Ǒ���(���ɕǂ����鎞)
		AUTOJUMP,	//�Ǒ��肹���ǃW�����v

		STATE_MAX	//����
	};

	//�A�j���[�V�����֘A
	int playing_anim_;					//���Đ����Ă���A�j���[�V�����̎�ނ̓��ꕨ
	int anim_model_[STATE_MAX];			//�A�j���[�V�����f�[�^�p�ϐ�
	int anim_attach_[STATE_MAX];		//���f���̃A�^�b�`�ԍ�������p�̕ϐ�
	float anim_total_[STATE_MAX];		//�A�j���[�V������1���[�v����t���[����
	float anim_frame_[STATE_MAX];		//�A�j���[�V�����̐i�s�t���[����
	float anim_rate_[STATE_MAX];		//���ꂼ��̃A�j���[�V��������(0.0f ~ 1.0f)

	//�v���C���[��(state)
	int NowState = 0;			//���ݏ�
	int BeforeState = NULL;		//�O���
	int Before2State = NULL;		//�O�O���
	
	
	//���ׂ𑖂��Ă��邩�ǂ���
	bool leftwallrun;
	bool rightwallrun;

	//�ق���STATE����W�����v�ɐ؂�ւ��Ƃ�
	bool EnterJump() { return (BeforeState != NULL && BeforeState != NowState && NowState == JUMP) ? true : false; }

	//�A�j���[�V�����̐i�s�Ɛ؂�ւ�(STATE�̕ω����܂�)
	void AnimUpdate();

	//�ړ��ʂ̓��ꕨ
	Vector3 Enter_mov_ = { 0, 0, 0 };	//�ړ����͗�(�v���C���[���ւ̉�]�O)
	//      �ړ����͑��ʁ@�W�����v���͑���
	Vector3 Enter_mov_all_, Jump_mov_ = { 0,0,0 };

	//STATE�̐؂�ւ�����
	bool SwitchStand() { return (Enter_mov_.x == 0.0f && Enter_mov_.z == 0.0f && OnGround) ? true : false; }		//�ړ��Ȃ��̎�
	bool SwitchRun() { return (Enter_mov_.z > 0.0f && OnGround) ? true : false; }									//�O�Ɉړ����鎞
	bool SwitchBack() { return(Enter_mov_.z < 0.0f && OnGround) ? true : false; }									//���Ɉړ����鎞
	bool SwitchLeft() { return (Enter_mov_.x < 0.0f && Enter_mov_.z == 0.0f && OnGround) ? true : false; }				//���Ɉړ����鎞
	bool SwitchRight() { return(Enter_mov_.x > 0.0f && Enter_mov_.z == 0.0f && OnGround) ? true : false; }				//�E�Ɉړ����鎞
	bool SwitchJump() { return(!OnGround && Jump_mov_.y > 0) ? true : false; }								//��Ɉړ��ʂ��ł��A�n�ʂɂ���Ƃ�
	bool SwitchLeftwall() { return (leftwallrun && !rightwallrun); }
	bool SwitchRightwall() { return (rightwallrun && !leftwallrun); }

	//�A�j���[�V�����̊e�؂�ւ�����
	void AnimSwitch();


	//�X�L��
	Vector3 Blink(Vector3 mov);
	//�u�����N���g�p���鎞�Ԃ��v��ϐ�
	int blink_time_;
	//�u�����N�̃N�[���^�C�����v��ϐ�
	int blink_cooltime_;

	//�ړ��Ȃǂ̓��͂ɂ��ړ���
	Vector3 EnterMove(Vector3 camera_rot);

	

	//�W�����v�Ȃǂ̈ړ���
	Vector3 JumpMove(int stage_model, Vector3 cam_rot);
	
	//�ǂɓ����������ɂ������_��Ԃ�
	Vector3 ReturnHitPos(Vector3 next_pos, int stage_model);

	//�ǂ��莞�̏���
	void Wall_Rubbing(int loop_num, MV1_COLL_RESULT_POLY_DIM hit_info, Vector3 *Key_mov, Vector3 next_pos, Vector3 HitPosition, int stage_model);

	//�v���C���[�̕�������ǂ����ɕǂ����邩�ǂ�������
	void CheckWhichWall(int stage_model);

	//----------------------
	//�Ǒ��肵�Ă���Ƃ��̈ړ������ƕǂ̓���
	float wallrun_dot;

	//���ǂ̈ړ����͂ł��邩
	int NowMode;
	enum MODE {
		NORMAL,
		WALLRUN,
		GIMMICK,

		MODE_MAX
	};

	Vector3 autojump_mov;

	

	//�n�ʂ̏�ɗ����Ă��邩�ǂ���
	bool OnGround;
	//�ǂɐG���Ă��邩�ǂ���
	bool TouchWall;

	//�ǂŎO�p�W�����v�ł��邩
	bool CanAutoJump;

	int light_handle;

public:
	Player();

	//��������`�F�b�N�|�C���g�܂Ń��Z�b�g
	void Reset(int CheckPoint);


	int m_model_;
	Vector3 m_pos_;
	Vector3 m_rot_;

	//�T�E���h�p�n���h��
	int BGM_Handle;
	int Tutrial_Handle;
	int Wallrun_Handle;
	int InG_Handle;
	int Run_Handle;
	int Wire_Handle;
#define HANDLE_MAX 6
	int time[HANDLE_MAX];


	//�O���Q�Ɨp(�M�~�b�N)
	bool GimmickMov_Off;

	//�O��̈ړ�����
	Vector3 BeforeMove;

	//�M�~�b�N�ɂ��ړ��ʂ̓��ꕨ
	Vector3 Gimmick_JumpBoard_mov_;

	Vector3 Gimmick_WireAction_mov_;

	void Init(int model);
	void Update(int stage_model, Vector3 cam_rot, int Sound);
	void Render();
	void Exit();
	
	int ReturnCoolDown() { return blink_cooltime_; }

	bool ReturnWallrun() { return (NowState == LEFTWALL || NowState == RIGHTWALL); }

	bool ReturnRightWall() { return (NowState == RIGHT) ? true : false; }

};