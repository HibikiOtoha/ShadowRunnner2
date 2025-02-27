//----------------------------------------------------------------
//! Camera.cpp
//----------------------------------------------------------------

#include "WinMain.h"
#include "GameMain.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Camera.h"

#include <algorithm>


Camera::Camera()
{
	//	�J�����ݒ�i�Ƃ肠�����j
	cam_pos_.set(0.0f, 25.0f, -30.0f);
	cam_look_.clear();
	SetCameraPositionAndTarget_UpVecY(cam_pos_.VGet(), cam_look_.VGet());

	//	�v���C���[����]���Ă��Ȃ��Ƃ��̃J�������W�i������W����ǂꂭ�炢����邩�j
	cambase.set(0.0f, 2.0f, Normal_CamPos_Max);

	Camera_Initialize();

	cam_rot_.y = 270;
}



void Camera::Update(Vector3 player_pos,  int stage_model,int Sens)
{
		player_pos_ = player_pos;
		stage_model_ = stage_model;

		Camera_Sens = 11 - Sens;

		Input_Process();
		Camera_Process();
}

void Camera::Render()
{
	SetCameraPositionAndTarget_UpVecY(SetEye, SetTarget);
}

void Camera::Exit()
{

}

void Camera::Camera_Initialize(void)
{
	// �J�����̏��������p�x�͂P�W�O�x
	AngleH = DX_PI_F;

	// �����p�x�͂O�x
	AngleV = 0.0f;
}

// ���͏���
void Camera::Input_Process(void)
{
	int Old;

	// �ЂƂO�̃t���[���̓��͂�ϐ��ɂƂ��Ă���
	Old = NowInput;

	// ���݂̓��͏�Ԃ��擾
	NowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ���̃t���[���ŐV���ɉ����ꂽ�{�^���̃r�b�g���������Ă���l�� EdgeInput �ɑ������
	EdgeInput = NowInput & ~Old;
}

// �J�����̏���
void Camera::Camera_Process(void)
{	
	//�}�E�X�̈ړ��ʎ擾
	Vector2 mouse_mov;
		//�f�o�b�O�p
		//if (CheckMouseInput()) {
		//mouse_mov.x = GetMouseMoveX();
		//mouse_mov.y = GetMouseMoveY();
		
		//�����p
		mouse_mov.x = GetCenterMouseMoveX();
		mouse_mov.y = GetCenterMouseMoveY();
		mouse_mov *= 0.2f;	//�␳
		cam_rot_.x += mouse_mov.y / (Camera_Sens * 0.5f);
		cam_rot_.y += mouse_mov.x / (Camera_Sens * 0.5f);
	//}
	

	VECTOR base = VGet(0.0f, 0.0f, Normal_CamPos_Max);

	MATRIX mat_x = MGetRotX(TO_RADIAN(cam_rot_.x));
	MATRIX mat_y = MGetRotY(TO_RADIAN(cam_rot_.y));
	MATRIX mat = MMult(mat_x, mat_y);
	VECTOR change_dir = VTransform(base, mat);



	//��
	if (mouse_mov.x < 0)
	{
		AngleH -= CAMERA_ANGLE_SPEED;

		// �|�P�W�O�x�ȉ��ɂȂ�����p�x�l���傫���Ȃ肷���Ȃ��悤�ɂR�U�O�x�𑫂�
		if (AngleH < -DX_PI_F)
		{
			AngleH += DX_TWO_PI_F;
		}
	}
	//�E
	if (mouse_mov.x > 0)
	{
		AngleH += CAMERA_ANGLE_SPEED;

		// �|�P�W�O�x�ȉ��ɂȂ�����p�x�l���傫���Ȃ肷���Ȃ��悤�ɂR�U�O�x�𑫂�
		if (AngleH > -DX_PI_F)
		{
			AngleH += DX_TWO_PI_F;
		}
	}
	//��
	if (mouse_mov.y < 0)
	{
		AngleV -= CAMERA_ANGLE_SPEED;

		// ������p�x�ȉ��ɂ͂Ȃ�Ȃ��悤�ɂ���
		if (AngleV <= -DX_PI_F / 2.0f + 0.6f)
		{
			AngleV = -DX_PI_F / 2.0f + 0.6f;
		}
		
	}
	//��
	if (mouse_mov.y > 0)
	{
		AngleV += CAMERA_ANGLE_SPEED;

		//������p�x�ȉ��ɂ͂Ȃ�Ȃ��悤�ɂ���
		if (AngleV >= DX_PI_F / 2.0f - 0.6f)
		{
			AngleV = DX_PI_F / 2.0f - 0.6f;
		}
	}

	//���Z�b�g�{�^��
	Camera_ReSet();

	// �J�����̒����_�̓v���C���[���W����K��l���������W
	Target = VAdd(player_pos_, VGet(0.0f, CAMERA_PLAYER_TARGET_HEIGHT, 0.0f));

	// �J�����̍��W�����肷��
	{
		MV1_COLL_RESULT_POLY_DIM HRes;
		int HitNum;
		VECTOR RightVector;
		VECTOR ForwardVector;
		VECTOR CamPosBase;

		// �J�����̉��s�������Z�o
		ForwardVector.x = cos(AngleV) * cos(-AngleH);
		ForwardVector.y = sin(AngleV);
		ForwardVector.z = cos(AngleV) * sin(-AngleH);

		// �J�����̐����������Z�o
		RightVector.x = cos(-AngleH - DX_PI_F / 2.0f);
		RightVector.y = 0.0f;
		RightVector.z = sin(-AngleH - DX_PI_F / 2.0f);

		// �J�����̍��W���Z�o
		//Eye = VAdd(VAdd(player_pos_, VScale(RightVector, CAMERA_PLAYER_RIGHT_DISTANCE)), VScale(ForwardVector, -CAMERA_PLAYER_BACKWARD_DISTANCE));
		Eye = VAdd(VAdd(player_pos_, VScale(RightVector, CAMERA_PLAYER_RIGHT_DISTANCE)), VScale(change_dir, -CAMERA_PLAYER_BACKWARD_DISTANCE));
		Eye.y += CAMERA_PLAYER_TARGET_HEIGHT;

		// �J�����̒����_���Z�o
		Target = VAdd(player_pos_, VScale(RightVector, CAMERA_PLAYER_RIGHT_DISTANCE));
		Target.y += CAMERA_PLAYER_TARGET_HEIGHT;

		// �J�����̍��W����v���C���[�̊ԂɃX�e�[�W�̃|���S�������邩���ׂ�
		CamPosBase = player_pos_;
		CamPosBase.y += CAMERA_PLAYER_TARGET_HEIGHT;
		HRes = MV1CollCheck_Capsule(stage_model_, -1, CamPosBase, Eye, CAMERA_COLLISION_SIZE);
		HitNum = HRes.HitNum;
		MV1CollResultPolyDimTerminate(HRes);
		if (HitNum != 0)
		{
			float NotHitLength;	
			float HitLength;
			float TestLength;
			VECTOR TestPosition;
			VECTOR Direction;

			// �������疳���ʒu�܂Ńv���C���[�ɋ߂Â�

			// �|���S���ɓ�����Ȃ��������Z�b�g
			NotHitLength = 0.0f;

			// �|���S���ɓ����鋗�����Z�b�g
			HitLength = VSize(VSub(Eye, CamPosBase));

			// �v���C���[����J�����ւ̕������Z�o
			Direction = VNorm(VSub(Eye, CamPosBase));
			do
			{
				// �����邩�ǂ����e�X�g���鋗�����Z�b�g( ������Ȃ������Ɠ����鋗���̒��� )
				TestLength = NotHitLength + (HitLength - NotHitLength) / 2.0f;

				// �e�X�g�p�̃J�������W���Z�o
				TestPosition = VAdd(VScale(Direction, TestLength), CamPosBase);

				// �V�������W�ŕǂɓ����邩�e�X�g
				HRes = MV1CollCheck_Capsule(stage_model_, -1, CamPosBase, TestPosition, CAMERA_COLLISION_SIZE);
				HitNum = HRes.HitNum;
				MV1CollResultPolyDimTerminate(HRes);
				if (HitNum != 0)
				{
					// ���������瓖���鋗���� TestLength �ɕύX����
					HitLength = TestLength;
				}
				else
				{
					// ������Ȃ������瓖����Ȃ������� TestLength �ɕύX����
					NotHitLength = TestLength;
				}
				//�J�������߂Â��ƃ^�[�Q�b�g���W������������
				Target.y -= CAMERA_ANGLE_CONTROL_HEIGHT;

				// HitLength �� NoHitLength ���\���ɋ߂Â��Ă��Ȃ������烋�[�v
			} while (HitLength - NotHitLength > 0.1f);

			// �J�����̍��W���Z�b�g
			Eye = TestPosition;
		}
	}

	// �J�����̏������C�u�����̃J�����ɔ��f������
	SetEye = VAdd(SetEye, VScale(VSub(Eye, SetEye), 0.2f));
	SetTarget = VAdd(SetTarget, VScale(VSub(Target, SetTarget), 0.2f));
}

void Camera::Camera_ReSet()
{
	if (PushHitKey(KEY_INPUT_R))
	{
		AngleH = DX_PI_F;
		AngleV = 0;

		cam_rot_.y = 270;
	}
}