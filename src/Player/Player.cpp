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

	//初期値を設定
	m_pos_.set(DefaultPos);
	m_rot_.y = DefaultRot;

	//ステートをスタンドから開始
	NowState = STAND;
	anim_rate_[STAND] = 1.0f;
	TouchWall = false;
	autojump_mov = { 0,0,0 };
	CanAutoJump = false;
}

void Player::Init(int model)
{
	//モデル情報を保存
	m_model_ = model;
	//アニメーションの読み込み
	anim_model_[STAND] = MV1LoadModel("data/Player/Anim_idle.mv1");
	anim_model_[RUN] = MV1LoadModel("data/Player/Anim_mainrun.mv1");
	anim_model_[LEFT] = MV1LoadModel("data/Player/Anim_leftmove.mv1");
	anim_model_[RIGHT] = MV1LoadModel("data/Player/Anim_rightmove.mv1");
	anim_model_[BACK] = MV1LoadModel("data/Player/Anim_back.mv1");
	anim_model_[JUMP] = MV1LoadModel("data/Player/Anim_jump.mv1");
	anim_model_[LEFTWALL] = MV1LoadModel("data/Player/Anim_wallrun.mv1");
	anim_model_[RIGHTWALL] = MV1LoadModel("data/Player/Anim_wallrun2.mv1");
	anim_model_[AUTOJUMP] = MV1LoadModel("data/Player/Anim_backflip.mv1");

	//動画用
	//anim_model_[STAND] = MV1LoadModel("data/movie/model/Idle.mv1");
	//anim_model_[RUN] = MV1LoadModel("data/movie/model/Running.mv1");

	//アニメーションをアタッチして割合を初期化
	for (int i = 0; i < STATE_MAX; i++) {
		anim.AnimInit(m_model_, anim_model_[i], &anim_attach_[i], &anim_total_[i], &anim_frame_[i], &anim_rate_[i]);
	}
	//STANDから
	NowState = STAND;
	anim_rate_[STAND] = 1.0f;

	//初期値を設定
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
	//リセット
	if ((PushHitKey(KEY_INPUT_M) && PushHitKey(KEY_INPUT_A)) || m_pos_.y <= -100.0f)
	{
		Reset(time[1]);
	}
	SetLightPositionHandle(light_handle, m_pos_);

	//アニメーション進行
	AnimUpdate();

	//入力移動量を保存
	//壁走りの時
	if (TouchWall && !OnGround)
	{
		//前回の移動量のまま足す
		Enter_mov_all_ = BeforeMove;
		CanAutoJump = false;
	}
	//それ以外の時は画面の方向に移動する
	else {
		Enter_mov_all_ = EnterMove(cam_rot);
	}

	//ジャンプと重力
	Jump_mov_ += JumpMove(stage_model, cam_rot);


	//壁走りしているとき
	if (TouchWall && !OnGround)
	{
		//重力を無効
		Jump_mov_.y = 0.0f;
	}

	//キーなどでの移動入力の移動量をまとめる
	Vector3 key_mov(0,0,0);
	key_mov = (Enter_mov_all_ + Jump_mov_ + Gimmick_WireAction_mov_);
	if(TouchWall && !OnGround)
	{
		key_mov = (Enter_mov_all_ + Jump_mov_);
	}
	//-------------------------------------
	// 壁ずり,壁ジャンプ
	//-------------------------------------
	{
		//次の座標
		Vector3 next_m_pos_ = m_pos_ + key_mov;

		// 当たり判定をとり、一点に絞る
		Vector3 HitPosition = ReturnHitPos(next_m_pos_, stage_model);

		//------------------------------------------------------------------
		// Capsule当たり判定
		//------------------------------------------------------------------
		MV1_COLL_RESULT_POLY_DIM hit_info = MV1CollCheck_Capsule(
			stage_model,    // モデルのハンドル
			-1,                    // コリジョンの情報を更新するフレームの番号
			next_m_pos_ + Vector3{ 0,3,0 },
			next_m_pos_ + Vector3{ 0,5,0 },
			1.5f
		);
		//当たっているとき
		if (hit_info.HitNum >= 1) {

			//壁に触れている
			TouchWall = true;

			//壁→壁の時に壁ジャンプの移動速度がなくなるように
			Jump_mov_ = { 0.0f,Jump_mov_.y,0.0f };
			

			int loop_num = hit_info.HitNum;
			//3回以上参照する必要がないので制限
			if (loop_num >= 3)
			{
				loop_num = 3;
			}

			//壁ずり時の処理
			Wall_Rubbing(loop_num, hit_info, &key_mov, next_m_pos_, HitPosition, stage_model);

		}

		//当たっていないとき
		else
		{
			//地面にいるなら三角飛び用の移動量をゼロに固定
			if (OnGround) {
				autojump_mov = { 0,0,0 };
				NowMode = NORMAL;
				CanAutoJump = true;	//三角ジャンプ可能に
			}
			TouchWall = false;
			leftwallrun = false;
			rightwallrun = false;
		}


	}

	//移動量を一つにして
	Vector3 all_movement = key_mov;

	GimmickMov_Off = (OnGround || TouchWall);

	//ワイヤーアクション
	if (Gimmick_WireAction_mov_.y != 0 )
	{
		//上限スピードを決めて
		Gimmick_WireAction_mov_.x = max(min(Gimmick_WireAction_mov_.x, 2), -1);
		Gimmick_WireAction_mov_.y = max(min(Gimmick_WireAction_mov_.y, 2), -1);
		Gimmick_WireAction_mov_.z = max(min(Gimmick_WireAction_mov_.z, 2), -1);
		m_pos_ += Gimmick_WireAction_mov_;
		Jump_mov_.y = 0;
		CanAutoJump = false;
	}
	//三角飛び
	else if ((autojump_mov.x != 0 ||autojump_mov.y != 0 || autojump_mov.z != 0))
	{
		m_pos_ += autojump_mov;
		m_pos_.y += all_movement.y;
		CanAutoJump = false;	//そのあともう一度起こらないように
	}

	//ギミックの自由落下を含む
	else {
		//プレイヤーの座標に移動量を足す
		m_pos_ += all_movement;
	}

	//ひとつ前のプレイヤー状況を保存
	Before2State = BeforeState;
	BeforeState = NowState;


	//	当たり判定の情報の更新を行う
	MV1RefreshCollInfo(m_model_);
}


void Player::Render()
{
	//モデルの描画
	MV1SetPosition(m_model_, VGet(m_pos_.x, m_pos_.y, m_pos_.z));
	//Y軸に180度回転
	MV1SetRotationXYZ(m_model_, VGet(TO_RADIAN(m_rot_.x), TO_RADIAN(m_rot_.y - 180), TO_RADIAN(m_rot_.z)));
	//モデルのサイズを小さくする
	MV1SetScale(m_model_, VGet(PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE));
	MV1DrawModel(m_model_);


	//確認用
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


//アニメーションの進行と切り替え(STATEの変化も含む)
void Player::AnimUpdate()
{
	//アニメーションが終わったかどうか(ジャンプ用)
	bool anim_end = false;

	//各状態の処理更新
	switch (NowState)
	{
	case STAND:
		StopSoundMem(Run_Handle);
		StopSoundMem(Wallrun_Handle);
		//アニメーションの進行
		anim.AnimProgress(0.5f, &anim_total_[STAND], &anim_frame_[STAND], &anim_rate_[STAND], true, &anim_end);
		//切り替えするかどうか
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

//アニメーションの切り替え
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

	//前進
	if (CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_S))
	{
		Enter_mov_.z = +RUN_SPEED;

		m_rot_.y = camera_rot.y;

		//前進
		move.x = RUN_SPEED * sinf(TO_RADIAN(m_rot_.y));
		move.z = RUN_SPEED * cosf(TO_RADIAN(m_rot_.y));
	}
	//後退
	if (CheckHitKey(KEY_INPUT_S) && !CheckHitKey(KEY_INPUT_W))
	{
		Enter_mov_.z = -BACK_SPEED;
		if (NowState != LEFTWALL && NowState != RIGHTWALL) {
			m_rot_.y = camera_rot.y;
		}

		//後退
		move.x = -BACK_SPEED * sinf(TO_RADIAN(m_rot_.y));
		move.z = -BACK_SPEED * cosf(TO_RADIAN(m_rot_.y));
	}
	//アイドル
	else if ((!CheckHitKey(KEY_INPUT_W)) && (!CheckHitKey(KEY_INPUT_S)))
	{
		Enter_mov_.z = 0.0f;
	}

	//ブリンクの中で計算が完了したものを移動量に入れる
	Vector3 all_mov = Blink(move);

	BeforeMove = all_mov;
	return all_mov;
}

Vector3 Player::Blink(Vector3 mov)
{
	//ブリンクできないとき
	if (blink_cooltime_ <= BLINK_COOLTIME)
	{
		//クールダウン計算
		blink_cooltime_++;
	}

	//ブリンクできるとき
	else {
		//前に進んでいるときに左シフトを押したら
		if ((NowState == RUN || NowState == LEFTWALL || NowState == RIGHTWALL) && (CheckHitKey(KEY_INPUT_LSHIFT)))
		{
			mov.x = BLINK_FAR * sinf(TO_RADIAN(m_rot_.y));
			mov.z = BLINK_FAR * cosf(TO_RADIAN(m_rot_.y));
			//押している間その位置からまっすぐ前に一定距離移動する
			blink_time_++;
			if (blink_time_ >= BLINK_TIME)
			{
				blink_time_ = 0;
				blink_cooltime_ = 0;
			}
		}
		//前フレームブリンクしていて今回シフトを押していないとき
		else if (blink_time_ >= 0.1f && !(CheckHitKey(KEY_INPUT_LSHIFT)))
		{
			//離した瞬間移動量の増加は止まりクールダウン
			blink_time_ = 0;
			blink_cooltime_ = 0;
		}
	}

	return mov;
}

//ジャンプと重力
Vector3 Player::JumpMove(int stage_model, Vector3 cam_rot)
{
	//カプセル判定(プレイヤーの足元にカプセルを作って判定)
	//	カプセルの開始座標（プレイヤー座標）
	Vector3 start_pos = m_pos_;
	//	カプセルのゴール座標（プレイヤー座標の少し下）
	Vector3 goal_pos = m_pos_ - Vector3{0.0f,-3.0f,0.0f};
	//	当たり判定をした結果を当たり情報として取得します
	MV1_COLL_RESULT_POLY_DIM jump_hit_info =
		MV1CollCheck_Capsule(stage_model, -1, start_pos.VGet(), goal_pos.VGet(),RADIAN);

	 Vector3 ground_pos;	//地面の高さ
	//	地面と当たっていたら
	if (jump_hit_info.HitNum != 0) {
		ground_pos = ReturnHitPos(goal_pos, stage_model);	//地面座標登録
	}
	//地面の上にいるかどうか
				//地面にいて				//プレイヤー座標が地面に当たっている(or 埋まっている)
	OnGround = (jump_hit_info.HitNum !=0  && m_pos_.y <= ground_pos.y);


	//--
	//地面からのジャンプの移動量の入れ物
	Vector3 jump_mov;
	//地面の上にいるとき
	if (OnGround) {

		Vector3 mov_Normal = goal_pos - m_pos_;
		Vector3 hit_Normal = jump_hit_info.Dim->Normal;
		float mov_dot = GetVector3Dot(mov_Normal, hit_Normal);

		//重力を０にし、プレイヤー座標を地面の座標に
		jump_mov = { 0.0f,0.0f,0.0f };
		Jump_mov_ = { 0.0f,0.0f,0.0f };
		//m_pos_.y = ground_pos.y;
		m_pos_.y +=  ground_pos.y - m_pos_.y ;

		//ジャンプに切り替わるとき
		if (PushHitKey(KEY_INPUT_SPACE))
		{
			jump_mov = jump_hit_info.Dim->Normal;
			jump_mov.SetLength(JUMP_POWER);

			//ギミックの変化移動分足す
			jump_mov += Gimmick_JumpBoard_mov_;
		}
	}

	jump_mov.y = max(min(jump_mov.y, 2), -1);

	//重力分を足す
	jump_mov.y -= DOWN_SPEED;

	//--

	//--
	//壁ジャンプの移動量の入れ物
	Vector3 walljump_mov;
	bool iswalljump = false;

	//壁ジャンプの時(ジャンプ中でないときに)
	//壁に触っていてジャンプが開始できない(空中の)時
	if (TouchWall && NowState != JUMP)
	{
		walljump_mov = { 0.0f,0.0f,0.0f };
		Jump_mov_ = { 0.0f,0.0f,0.0f };

		//	カメラが向いている方に回転させるための回転行列作成
		MATRIX mat_walljump = MGetRotY(TO_RADIAN(cam_rot.y));

		if (PushHitKey(KEY_INPUT_SPACE))
		{
			//	ジャンプ開始
			walljump_mov = Vector3{ 0,-1.0,0 };

			iswalljump = true;
		}
		CanAutoJump == false;
	}
	if (iswalljump)
	{
		//カメラ方向に移動
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
	//移動量を足して返す
	return jump_mov + walljump_mov;
}

// 当たり判定をとり、一点に絞る
Vector3 Player::ReturnHitPos(Vector3 next_pos, int stage_model)
{
	Vector3 PosBase = next_pos;
	PosBase.y += 5;
	//今の座標と次の座標でカプセルを作り当たり判定
	MV1_COLL_RESULT_POLY_DIM line_hit_info = MV1CollCheck_Capsule(stage_model, -1, PosBase, m_pos_, RADIAN);
	int HitNum = line_hit_info.HitNum;
	MV1CollResultPolyDimTerminate(line_hit_info);	//次のループで使うので後始末
	//当たっていれば(当たった座標が一つ以上なら)
	if (HitNum != 0)
	{
		float NotHitLength;	//当たらない長さ
		float HitLength;	//当たった長さ
		float TestLength;		//長さの入れ物(仮)
		VECTOR TestPosition;	//当たるかどうかのテスト用
		VECTOR Direction;		//プレイヤーからカメラへの方向ベクトルの入れ物

		//ポリゴンに当たらない距離をセット
		NotHitLength = 0.0f;

		//ポリゴンに当たる距離をセット
		HitLength = VSize(VSub(m_pos_, PosBase));

		//プレイヤーの移動方向を算出
		Direction = VNorm(VSub(m_pos_, PosBase));
		do
		{
			//当たるかどうかテストする距離をセット(当たる距離と当たらない距離の中間)
			TestLength = NotHitLength + (HitLength - NotHitLength) / 2.0f;

			//テスト用の座標を算出
			TestPosition = VAdd(VScale(Direction, TestLength), PosBase);

			//新しい座標で壁に当たるかテスト
			line_hit_info = MV1CollCheck_Capsule(stage_model, -1, PosBase, TestPosition, RADIAN);
			HitNum = line_hit_info.HitNum;
			MV1CollResultPolyDimTerminate(line_hit_info);
			if (HitNum != 0)
			{
				//当たったら当たる距離をTestLengthに変更
				HitLength = TestLength;
			}
			else
			{
				//当たらなかったら当たらない距離をTestLengthに変更する
				NotHitLength = TestLength;
			}
			//十分近づくまでループ
		} while (HitLength - NotHitLength > 0.001f);


		//一点を返す
		return TestPosition;
	}
}

//壁ずりの判定
void Player::Wall_Rubbing(int loop_num, MV1_COLL_RESULT_POLY_DIM hit_info, Vector3* Key_mov, Vector3 next_pos, Vector3 HitPosition, int stage_model)
{
	//元の移動量を保持
	Vector3 before_mov = *Key_mov;
	for (int i = 0; i < loop_num; i++) {

		Vector3 mov_Normal = next_pos - m_pos_;
		Vector3 hit_Normal = hit_info.Dim[i].Normal;
		float mov_dot = GetVector3Dot(mov_Normal, hit_Normal);

		wallrun_dot = mov_dot;

		//壁の法線方向と逆側に向かっているとき(壁に向かって移動しているとき)
		if (mov_dot < 0)
		{
			//壁走りせずその場で(壁への入射角が浅かったらその場で自動壁ジャンプ)
			if ((mov_dot >= -1.0f && mov_dot <= -0.7f) && !OnGround && !leftwallrun && !rightwallrun && (BeforeState != WALLRUN && Before2State != WALLRUN) && CanAutoJump == true)
			{
				//切り替え始めの時
				if (autojump_mov.x == 0 && autojump_mov.z == 0) {
					int before_state = NowState;

					anim_rate_[before_state] = 0.0f;
					NowState = AUTOJUMP;
					anim.AnimChanger(m_model_, anim_model_[AUTOJUMP], &anim_attach_[AUTOJUMP], &anim_rate_[AUTOJUMP],
						&anim_frame_[AUTOJUMP], &NowState, AUTOJUMP);
				}

				//壁と反対側に飛ばす
				autojump_mov = hit_Normal;
				autojump_mov.y = 0.5f;
				autojump_mov.SetLength(AUTOJUMP_POWER / 2);
			}

			if (hit_Normal.x != 0) {
				//めり込む前に移動量をあたったところまでに戻す
				Key_mov->x = (next_pos.x + autojump_mov.x) - HitPosition.x;
			}
			if (hit_Normal.z != 0) {
				//めり込む前に移動量をあたったところまでに戻す
				Key_mov->z = (next_pos.z + autojump_mov.z) - HitPosition.z;
			}			
			//プレイヤーの左右に法線を引き、右の壁に当たったか左の壁に当たったかを判定
			CheckWhichWall(stage_model);
		}
	}

}

//プレイヤーの左右に法線を引き、右の壁に当たったか左の壁に当たったかを判定
void Player::CheckWhichWall(int stage_model)
{
	//確認に使うポイントの数
	const int checkpoint = 2;
	float check_lot[checkpoint] =
	{
		m_rot_.y + (45 * 5),
		m_rot_.y + (45 * 7),
	};
	//当たり判定関係保存用のものを確認ポイント数分用意
	MV1_COLL_RESULT_POLY wallhit_info[checkpoint];

	for (int j = 0; j < checkpoint; j++) {
		//プレイヤーの壁に当たっている方向を取得するためのライン作成
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
	//右か左どちらに壁があるか
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