#pragma once
//----------------------------------------------------------------
//! player.h
//----------------------------------------------------------------

class Player {
public:
#define RUN_SPEED  1.0f			//前移動のスピード

#define BACK_SPEED 0.1f		//後ろに下がるスピード
#define JUMP_POWER	1.0f		//ジャンプ力
#define DOWN_SPEED	0.05f		//重力
#define AUTOJUMP_POWER	1.0f		//自動のジャンプ力
#define AUTOJUMP_DOWN_SPEED	0.05f		//減少力
#define WALLKICK_POWER 2.0f		//壁蹴りの力

#define BLINK_FAR 1.5f			//ブリンクの速度
#define BLINK_TIME 10			//ブリンク時間上限(フレーム)
#define BLINK_COOLTIME 120			//ブリンク時間上限(フレーム)

#define PLAYER_SIZE	0.05f
#define RADIAN 1.5f	//半径

private:
	//初期値
	
	Vector3 DefaultPos{625,30,0 };

	//test(壁ー壁ーワイヤー壁)
	//Vector3 DefaultPos{ -155,30,0 };

	float DefaultRot = 270.0f;

	//プレイヤー状況
	enum STATE {
		STAND,		//立ち
		RUN,		//走り
		LEFT,		//左移動
		RIGHT,		//右移動
		BACK,		//後退
		JUMP,		//ジャンプ
		LEFTWALL,	//壁走り(左に壁がある時)
		RIGHTWALL,	//壁走り(左に壁がある時)
		AUTOJUMP,	//壁走りせず壁ジャンプ

		STATE_MAX	//総数
	};

	//アニメーション関連
	int playing_anim_;					//今再生しているアニメーションの種類の入れ物
	int anim_model_[STATE_MAX];			//アニメーションデータ用変数
	int anim_attach_[STATE_MAX];		//モデルのアタッチ番号を入れる用の変数
	float anim_total_[STATE_MAX];		//アニメーションが1ループするフレーム数
	float anim_frame_[STATE_MAX];		//アニメーションの進行フレーム数
	float anim_rate_[STATE_MAX];		//それぞれのアニメーション割合(0.0f ~ 1.0f)

	//プレイヤー状況(state)
	int NowState = 0;			//現在状況
	int BeforeState = NULL;		//前回状況
	int Before2State = NULL;		//前前回状況
	
	
	//かべを走っているかどうか
	bool leftwallrun;
	bool rightwallrun;

	//ほかのSTATEからジャンプに切り替わるとき
	bool EnterJump() { return (BeforeState != NULL && BeforeState != NowState && NowState == JUMP) ? true : false; }

	//アニメーションの進行と切り替え(STATEの変化も含む)
	void AnimUpdate();

	//移動量の入れ物
	Vector3 Enter_mov_ = { 0, 0, 0 };	//移動入力量(プレイヤー軸への回転前)
	//      移動入力総量　ジャンプ入力総量
	Vector3 Enter_mov_all_, Jump_mov_ = { 0,0,0 };

	//STATEの切り替え条件
	bool SwitchStand() { return (Enter_mov_.x == 0.0f && Enter_mov_.z == 0.0f && OnGround) ? true : false; }		//移動なしの時
	bool SwitchRun() { return (Enter_mov_.z > 0.0f && OnGround) ? true : false; }									//前に移動する時
	bool SwitchBack() { return(Enter_mov_.z < 0.0f && OnGround) ? true : false; }									//後ろに移動する時
	bool SwitchLeft() { return (Enter_mov_.x < 0.0f && Enter_mov_.z == 0.0f && OnGround) ? true : false; }				//左に移動する時
	bool SwitchRight() { return(Enter_mov_.x > 0.0f && Enter_mov_.z == 0.0f && OnGround) ? true : false; }				//右に移動する時
	bool SwitchJump() { return(!OnGround && Jump_mov_.y > 0) ? true : false; }								//上に移動量ができ、地面にいるとき
	bool SwitchLeftwall() { return (leftwallrun && !rightwallrun); }
	bool SwitchRightwall() { return (rightwallrun && !leftwallrun); }

	//アニメーションの各切り替え処理
	void AnimSwitch();


	//スキル
	Vector3 Blink(Vector3 mov);
	//ブリンクを使用する時間を計る変数
	int blink_time_;
	//ブリンクのクールタイムを計る変数
	int blink_cooltime_;

	//移動などの入力による移動量
	Vector3 EnterMove(Vector3 camera_rot);

	

	//ジャンプなどの移動量
	Vector3 JumpMove(int stage_model, Vector3 cam_rot);
	
	//壁に当たった時にあたる一点を返す
	Vector3 ReturnHitPos(Vector3 next_pos, int stage_model);

	//壁ずり時の処理
	void Wall_Rubbing(int loop_num, MV1_COLL_RESULT_POLY_DIM hit_info, Vector3 *Key_mov, Vector3 next_pos, Vector3 HitPosition, int stage_model);

	//プレイヤーの方向からどっちに壁があるかどうか見る
	void CheckWhichWall(int stage_model);

	//----------------------
	//壁走りしているときの移動方向と壁の内積
	float wallrun_dot;

	//今どの移動入力できるか
	int NowMode;
	enum MODE {
		NORMAL,
		WALLRUN,
		GIMMICK,

		MODE_MAX
	};

	Vector3 autojump_mov;

	

	//地面の上に立っているかどうか
	bool OnGround;
	//壁に触っているかどうか
	bool TouchWall;

	//壁で三角ジャンプできるか
	bool CanAutoJump;

	int light_handle;

public:
	Player();

	//落ちたらチェックポイントまでリセット
	void Reset(int CheckPoint);


	int m_model_;
	Vector3 m_pos_;
	Vector3 m_rot_;

	//サウンド用ハンドル
	int BGM_Handle;
	int Tutrial_Handle;
	int Wallrun_Handle;
	int InG_Handle;
	int Run_Handle;
	int Wire_Handle;
#define HANDLE_MAX 6
	int time[HANDLE_MAX];


	//外部参照用(ギミック)
	bool GimmickMov_Off;

	//前回の移動総量
	Vector3 BeforeMove;

	//ギミックによる移動量の入れ物
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