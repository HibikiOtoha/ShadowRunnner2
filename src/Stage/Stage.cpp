//----------------------------------------------------------------
//! Stage.cpp
//----------------------------------------------------------------
#include "WinMain.h"
#include "GameMain.h"
#include "Vector3.h"
#include "Player/Player.h"
#include "Stage.h"

#include "Gimmick/Base/GimmickBase.h"
#include "Gimmick/JumpBoard.h"
#include "Gimmick/WireActions.h"

JumpBoard jumpboard;
WireActions wireactions[GIMMICK_MAX];

void Stage::ReSet()
{

	m_pos_ = Vector3{ 0.0f,0.0f,0.0f };

	goal_pos = Vector3{ -650.0f,50.0f,-55.0f };

	goalflag = false;


	Gimmick_pos[0] = Vector3{ 270,55,0 };
	Gimmick_pos[1] = Vector3{ -25,50,31 };
	Gimmick_pos[2] = Vector3{ -25,50,-31 };
	Gimmick_pos[3] = Vector3{ -360,65,-30 };
	Gimmick_pos[4] = Vector3{ -470,100,0 };
	//Gimmick_pos[5] = Vector3{ -530,70,0 };
}

Stage::Stage()
{
	ReSet();
}

void Stage::Init(int model, int skymodel)
{
	//ステージモデルデータの読み込み
	m_model_ = model;
	sky_model = skymodel;
	for (int i = 0; i < GIMMICK_MAX; i++) {
		wireactions[i].m_pos_ = Gimmick_pos[i];
	}
	goalflag = false;
}

void Stage::Update(Vector3 player_pos,float* player_rot_y, Vector3* JumpBoard_mov,Vector3* WireAction_mov,Vector3 cam_rot,bool GimmickMov_Off)
{
	//	当たり判定の情報の更新を行う
	MV1RefreshCollInfo(m_model_);
	this->player_pos = player_pos;

	// 空モデルの向きの変更
	sky_rot.y += 0.1f;



	//ゴールの中にいたら
	if ((player_pos.x <= goal_pos.x && player_pos.x >= (goal_pos.x + goal_size.x)) && (player_pos.z >= goal_pos.x && player_pos.z <= (goal_pos.z + goal_size.z)))
	{
		//クリア
		//DrawString(0, 0, "NextStage", GetColor(255, 255, 255));
		goalflag = true;
	}


	jumpboard.Update(player_pos,JumpBoard_mov);

	//ワイヤーアクションの加算
	int nonactive_cunter = 0;	//数値が入っていない非アクティブのものを数える用の変数
	for (int i = 0; i < GIMMICK_MAX; i++) {
		Vector3 wire_mov = Vector3{ 0,0,0 };
		
		//移動量をオフにするかどうか(壁走りが始まるか、地面についたら数値をゼロにするため)
		wireactions[i].Update(player_pos, &wire_mov, player_rot_y, cam_rot, GimmickMov_Off);

		if (wire_mov.x != 0 || wire_mov.z != 0)
		{
			*WireAction_mov = wire_mov;
			break;
		}
		else {
			nonactive_cunter++;
		}

		if (nonactive_cunter == GIMMICK_MAX) {
			*WireAction_mov = wire_mov;
		}
	}


	
}


void Stage::Render()
{
	// 空もモデルはライトの影響はない状態にしたい
	SetUseLighting(FALSE);
	// 空モデルの座標をプレイヤー座標にするとプレイヤーがどれだけ移動しても空がついてくる
	// MV1SetPosition(sky_model,player_pos)

	MV1SetScale(sky_model, VGet(15.0f, 10.0f, 10.0f));

	// 空モデルの向きを設定
	MV1SetRotationXYZ(sky_model, VGet(TO_RADIAN(sky_rot.x), TO_RADIAN(sky_rot.x), TO_RADIAN(sky_rot.x)));

	// 空モデルの描画
	MV1DrawModel(sky_model);
	// ライトを元に戻す
	// 地面やプレイヤーはいつもどうりライトの影響を受けたい
	SetUseLighting(TRUE);

	//モデルの描画
	MV1SetPosition(m_model_, VGet(m_pos_.x, m_pos_.y, m_pos_.z));
	//モデルのサイズを調整
	MV1SetScale(m_model_, VGet(0.5f, 0.5f, 0.5f));
	MV1DrawModel(m_model_);

	DrawCube3D(goal_pos, (goal_pos + goal_size),
		GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);

	jumpboard.Render(player_pos);
	for (int i = 0; i < GIMMICK_MAX; i++) {
		wireactions[i].Render(player_pos);
	}
	MV1DrawFrame(m_model_, 1);
}

void Stage::Exit()
{

}

