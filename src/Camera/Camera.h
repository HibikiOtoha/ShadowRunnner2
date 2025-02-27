#pragma once

//----------------------------------------------------------------
//! Camera.h
//----------------------------------------------------------------

class Camera{
private:

	//�J�������WZ�̍ŏ��l�@�ő�l
	static const int Normal_CamPos_Max = -1;
	static const int WallRun_CamPos_Z_Max = -20;

	//�J�������WZ�̍ő�l
	static const int WallRun_CamPos_X_Max = 5;
	

	//	�v���C���[����]���Ă��Ȃ��Ƃ��̃J�������W�i������W����ǂꂭ�炢����邩�j
	Vector3 cambase;

	//�}�E�X�J�[�\���p(�e�X�g)
	Vector3 player_pos_;
	int stage_model_;
	Vector3 player_rot_;



	// �J�����֌W�̒�`
#define CAMERA_ANGLE_SPEED			0.05f		// ���񑬓x
#define CAMERA_PLAYER_TARGET_HEIGHT		10.0f		// �v���C���[���W����ǂꂾ�������ʒu�𒍎��_�Ƃ��邩
#define CAMERA_PLAYER_BACKWARD_DISTANCE	-20.0f		// �v���C���[�Ƃ̉��s�����̋���
#define CAMERA_PLAYER_RIGHT_DISTANCE	0.0f		// �v���C���[�Ƃ̐��������̋���
#define CAMERA_COLLISION_SIZE			1.0f		// �J�����̓����蔻��T�C�Y
	
	
	VECTOR		Eye;					// �J�������W
	VECTOR		Target;				// �����_���W

	int				NowInput;			// ���݂̓���
	int				EdgeInput;			// ���݂̃t���[���ŉ����ꂽ�{�^���̂݃r�b�g�������Ă�����͒l

#define CAMERA_ANGLE_CONTROL_HEIGHT	0.05f	//�J�����߂Â��Ƃ��ɂǂꂭ�炢�v���C���[���ɃJ�����̃^�[�Q�b�g�������邩
	 
	// �J�����̏���������
	void Camera_Initialize(void);

	// ���͏���
	void Input_Process(void);

	// �J�����̏���
	void Camera_Process(void);

	//�J�����������Z�b�g
	void Camera_ReSet();



public:
	Camera();

	int Camera_Sens;

	float		AngleH;				// �����p�x
	float		AngleV;				// �����p�x
	
	VECTOR		SetEye;
	VECTOR		SetTarget;
	
	Vector3 cam_pos_;   //�J�����̈ʒu
	Vector3 cam_rot_;   //�J�����̌���
	Vector3 cam_look_;   //�J�����̌�����W


	void Update(Vector3 player_pos, int stage_model,int Sens);
	void Render();
	void Exit();
};