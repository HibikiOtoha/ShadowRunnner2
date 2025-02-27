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
	//	カメラ設定（とりあえず）
	cam_pos_.set(0.0f, 25.0f, -30.0f);
	cam_look_.clear();
	SetCameraPositionAndTarget_UpVecY(cam_pos_.VGet(), cam_look_.VGet());

	//	プレイヤーが回転していないときのカメラ座標（見る座標からどれくらい離れるか）
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
	// カメラの初期水平角度は１８０度
	AngleH = DX_PI_F;

	// 垂直角度は０度
	AngleV = 0.0f;
}

// 入力処理
void Camera::Input_Process(void)
{
	int Old;

	// ひとつ前のフレームの入力を変数にとっておく
	Old = NowInput;

	// 現在の入力状態を取得
	NowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 今のフレームで新たに押されたボタンのビットだけ立っている値を EdgeInput に代入する
	EdgeInput = NowInput & ~Old;
}

// カメラの処理
void Camera::Camera_Process(void)
{	
	//マウスの移動量取得
	Vector2 mouse_mov;
		//デバッグ用
		//if (CheckMouseInput()) {
		//mouse_mov.x = GetMouseMoveX();
		//mouse_mov.y = GetMouseMoveY();
		
		//実装用
		mouse_mov.x = GetCenterMouseMoveX();
		mouse_mov.y = GetCenterMouseMoveY();
		mouse_mov *= 0.2f;	//補正
		cam_rot_.x += mouse_mov.y / (Camera_Sens * 0.5f);
		cam_rot_.y += mouse_mov.x / (Camera_Sens * 0.5f);
	//}
	

	VECTOR base = VGet(0.0f, 0.0f, Normal_CamPos_Max);

	MATRIX mat_x = MGetRotX(TO_RADIAN(cam_rot_.x));
	MATRIX mat_y = MGetRotY(TO_RADIAN(cam_rot_.y));
	MATRIX mat = MMult(mat_x, mat_y);
	VECTOR change_dir = VTransform(base, mat);



	//左
	if (mouse_mov.x < 0)
	{
		AngleH -= CAMERA_ANGLE_SPEED;

		// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
		if (AngleH < -DX_PI_F)
		{
			AngleH += DX_TWO_PI_F;
		}
	}
	//右
	if (mouse_mov.x > 0)
	{
		AngleH += CAMERA_ANGLE_SPEED;

		// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
		if (AngleH > -DX_PI_F)
		{
			AngleH += DX_TWO_PI_F;
		}
	}
	//上
	if (mouse_mov.y < 0)
	{
		AngleV -= CAMERA_ANGLE_SPEED;

		// ある一定角度以下にはならないようにする
		if (AngleV <= -DX_PI_F / 2.0f + 0.6f)
		{
			AngleV = -DX_PI_F / 2.0f + 0.6f;
		}
		
	}
	//下
	if (mouse_mov.y > 0)
	{
		AngleV += CAMERA_ANGLE_SPEED;

		//ある一定角度以下にはならないようにする
		if (AngleV >= DX_PI_F / 2.0f - 0.6f)
		{
			AngleV = DX_PI_F / 2.0f - 0.6f;
		}
	}

	//リセットボタン
	Camera_ReSet();

	// カメラの注視点はプレイヤー座標から規定値分高い座標
	Target = VAdd(player_pos_, VGet(0.0f, CAMERA_PLAYER_TARGET_HEIGHT, 0.0f));

	// カメラの座標を決定する
	{
		MV1_COLL_RESULT_POLY_DIM HRes;
		int HitNum;
		VECTOR RightVector;
		VECTOR ForwardVector;
		VECTOR CamPosBase;

		// カメラの奥行方向を算出
		ForwardVector.x = cos(AngleV) * cos(-AngleH);
		ForwardVector.y = sin(AngleV);
		ForwardVector.z = cos(AngleV) * sin(-AngleH);

		// カメラの水平方向を算出
		RightVector.x = cos(-AngleH - DX_PI_F / 2.0f);
		RightVector.y = 0.0f;
		RightVector.z = sin(-AngleH - DX_PI_F / 2.0f);

		// カメラの座標を算出
		//Eye = VAdd(VAdd(player_pos_, VScale(RightVector, CAMERA_PLAYER_RIGHT_DISTANCE)), VScale(ForwardVector, -CAMERA_PLAYER_BACKWARD_DISTANCE));
		Eye = VAdd(VAdd(player_pos_, VScale(RightVector, CAMERA_PLAYER_RIGHT_DISTANCE)), VScale(change_dir, -CAMERA_PLAYER_BACKWARD_DISTANCE));
		Eye.y += CAMERA_PLAYER_TARGET_HEIGHT;

		// カメラの注視点を算出
		Target = VAdd(player_pos_, VScale(RightVector, CAMERA_PLAYER_RIGHT_DISTANCE));
		Target.y += CAMERA_PLAYER_TARGET_HEIGHT;

		// カメラの座標からプレイヤーの間にステージのポリゴンがあるか調べる
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

			// あったら無い位置までプレイヤーに近づく

			// ポリゴンに当たらない距離をセット
			NotHitLength = 0.0f;

			// ポリゴンに当たる距離をセット
			HitLength = VSize(VSub(Eye, CamPosBase));

			// プレイヤーからカメラへの方向を算出
			Direction = VNorm(VSub(Eye, CamPosBase));
			do
			{
				// 当たるかどうかテストする距離をセット( 当たらない距離と当たる距離の中間 )
				TestLength = NotHitLength + (HitLength - NotHitLength) / 2.0f;

				// テスト用のカメラ座標を算出
				TestPosition = VAdd(VScale(Direction, TestLength), CamPosBase);

				// 新しい座標で壁に当たるかテスト
				HRes = MV1CollCheck_Capsule(stage_model_, -1, CamPosBase, TestPosition, CAMERA_COLLISION_SIZE);
				HitNum = HRes.HitNum;
				MV1CollResultPolyDimTerminate(HRes);
				if (HitNum != 0)
				{
					// 当たったら当たる距離を TestLength に変更する
					HitLength = TestLength;
				}
				else
				{
					// 当たらなかったら当たらない距離を TestLength に変更する
					NotHitLength = TestLength;
				}
				//カメラが近づくとターゲット座標を少し下げる
				Target.y -= CAMERA_ANGLE_CONTROL_HEIGHT;

				// HitLength と NoHitLength が十分に近づいていなかったらループ
			} while (HitLength - NotHitLength > 0.1f);

			// カメラの座標をセット
			Eye = TestPosition;
		}
	}

	// カメラの情報をライブラリのカメラに反映させる
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