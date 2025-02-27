#pragma once

//----------------------------------------------------------------
//! Camera.h
//----------------------------------------------------------------

class Camera{
private:

	//カメラ座標Zの最小値　最大値
	static const int Normal_CamPos_Max = -1;
	static const int WallRun_CamPos_Z_Max = -20;

	//カメラ座標Zの最大値
	static const int WallRun_CamPos_X_Max = 5;
	

	//	プレイヤーが回転していないときのカメラ座標（見る座標からどれくらい離れるか）
	Vector3 cambase;

	//マウスカーソル用(テスト)
	Vector3 player_pos_;
	int stage_model_;
	Vector3 player_rot_;



	// カメラ関係の定義
#define CAMERA_ANGLE_SPEED			0.05f		// 旋回速度
#define CAMERA_PLAYER_TARGET_HEIGHT		10.0f		// プレイヤー座標からどれだけ高い位置を注視点とするか
#define CAMERA_PLAYER_BACKWARD_DISTANCE	-20.0f		// プレイヤーとの奥行方向の距離
#define CAMERA_PLAYER_RIGHT_DISTANCE	0.0f		// プレイヤーとの水平方向の距離
#define CAMERA_COLLISION_SIZE			1.0f		// カメラの当たり判定サイズ
	
	
	VECTOR		Eye;					// カメラ座標
	VECTOR		Target;				// 注視点座標

	int				NowInput;			// 現在の入力
	int				EdgeInput;			// 現在のフレームで押されたボタンのみビットが立っている入力値

#define CAMERA_ANGLE_CONTROL_HEIGHT	0.05f	//カメラ近づくときにどれくらいプレイヤー側にカメラのターゲットを下げるか
	 
	// カメラの初期化処理
	void Camera_Initialize(void);

	// 入力処理
	void Input_Process(void);

	// カメラの処理
	void Camera_Process(void);

	//カメラ方向リセット
	void Camera_ReSet();



public:
	Camera();

	int Camera_Sens;

	float		AngleH;				// 水平角度
	float		AngleV;				// 垂直角度
	
	VECTOR		SetEye;
	VECTOR		SetTarget;
	
	Vector3 cam_pos_;   //カメラの位置
	Vector3 cam_rot_;   //カメラの向き
	Vector3 cam_look_;   //カメラの見る座標


	void Update(Vector3 player_pos, int stage_model,int Sens);
	void Render();
	void Exit();
};