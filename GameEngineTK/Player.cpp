#include "pch.h"
#include "Player.h"
#include "ADX2Le.h"
#include "Resources\Music\CueSheet_0.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* key)
{
	m_keyboard = key;
	m_FireFlag = false;

	m_debug = false;
}

void Player::Initialize()
{
	//自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_FIGHTER].LoadModel(L"Resources/Fighter.cmo");
	m_Obj[PARTS_R_WING].LoadModel(L"Resources/R_WING.cmo");
	m_Obj[PARTS_L_WING].LoadModel(L"Resources/L_WING.cmo");
	m_Obj[PARTS_BULLET].LoadModel(L"Resources/PlayerBullet.cmo");
	m_Obj[PARTS_BOOST].LoadModel(L"Resources/Boost.cmo");

	//親子関係の構築(子パールに親を設定)
	m_Obj[PARTS_R_WING].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_L_WING].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_BULLET].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_BOOST].SetObjParent(&m_Obj[PARTS_FIGHTER]);

	//スケールの指定
	m_Obj[PARTS_FIGHTER].SetScale(Vector3(1.4f));

	//座標のずれ指定
	m_Obj[PARTS_FIGHTER].SetTrans(Vector3(0, 1, 0));
	m_Obj[PARTS_BOOST].SetTrans(Vector3(0, 0, 0.7f));

	//弾は発射してないとき非表示
	m_Obj[PARTS_BULLET].SetDrawState(false);
	//加速していないとき非表示
	m_Obj[PARTS_BOOST].SetDrawState(false);

	m_cycle = 0.0f;

	//自機弾のカウント
	m_shootcount = 0;
	//ブーストのカウント、ステート
	m_boostModelCount = 0;
	m_boostModelState = false;

	//弾丸用のあたり判定ノードの設定をする
	{
		m_CollisionNodeBullet.Initialize();
		//親オブジェクトを設定
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_BULLET]);

		m_CollisionNodeBullet.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBullet.SetLocalRadius(0.3f);
	}
	//全身用のあたり判定ノードの設定をする
	{
		m_CollisionNodeBody.Initialize();
		//親オブジェクトを設定
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_FIGHTER]);

		m_CollisionNodeBody.SetTrans(Vector3(-0.1f, 0, 0));

		m_CollisionNodeBody.SetLocalRadius(0.3f);
	}

	m_isJump = false;

	m_deathstate = false;

	m_wingState = 0;

	m_respawnCount = 0;

	m_invincibleCount = 0;

	m_invincibleState = false;

	//ACFファイルふぉみこみ
	ADX2Le::Initialize("Resources/Music/Shooting3D.acf");
	ADX2Le::LoadAcb("Resources/Music/CueSheet_0.acb", "Resources/Music/CueSheet_0.awb");

	m_shootTimeCount = 0;
	m_defeatCount = 0;
	m_damageCount = 0;
}


Player::~Player()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Finalize();
	}
}

void Player::Update()
{
	//キーボードの状態取得
	Keyboard::State kb = m_keyboard->GetState();

	m_keyboardTracker.Update(kb);

	PartsGimic();
	
	PlayerMove(kb);

	//死んでから時間経過で復活
	if (GetDeathState())
	{
		m_respawnCount++;
		
		if (m_respawnTime < m_respawnCount)
		{
			m_respawnCount = 0;
			SetDeathState(false);
			SetWingState(0);
			m_Obj[PARTS_L_WING].SetDrawState(true);
			m_Obj[PARTS_R_WING].SetDrawState(true);
			SetInvincibleState(false);
			SetTrans(Vector3(0, 3, 0));
			SetRot(Vector3(0, 0, 0));
		}
	}

	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	PlayerBullet();

	//無敵状態のカウント
	if (GetInvincibleState())
	{
		m_invincibleCount++;
		if (m_invincibleCount > m_invincibleTime)
		{
			m_invincibleCount = 0;
			SetInvincibleState(false);
		}
	}

	switch (GetWingState())
	{
	case 0:
		m_Obj[PARTS_L_WING].SetDrawState(true);
		m_Obj[PARTS_R_WING].SetDrawState(true);
		break;
	case 1:
		m_Obj[PARTS_L_WING].SetDrawState(false);
		break;
	case 2:
		m_Obj[PARTS_R_WING].SetDrawState(false);
		break;
	}

	Calc();
}

void Player::Draw()
{
	//死んでいたら抜ける
	if (GetDeathState()) return;

	//無敵カウントが偶数だったら抜ける
	if (GetInvincibleCount() % 2 == 0 && GetInvincibleCount() != 0) return;

	//プレイヤーの描画
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}

	if(m_debug) m_CollisionNodeBullet.Draw();
	if (m_debug)m_CollisionNodeBody.Draw();
}

void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	m_CollisionNodeBody.Update();
	m_CollisionNodeBullet.Update();
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	return m_Obj[0].GetTrans();
}

const DirectX::SimpleMath::Matrix& Player::GetWorld()
{
	return m_Obj[0].GetWorld();
}

const DirectX::SimpleMath::Vector3 & Player::GetVelocity()
{
	return m_Velocity;
}

const bool Player::GetFireFlag()
{
	return m_FireFlag;
}

const DirectX::SimpleMath::Vector3& Player::GetScale()
{
	return m_Obj[0].GetScale();
}

const DirectX::SimpleMath::Vector3& Player::GetRot()
{
	return m_Obj[0].GetRot();
}

void Player::SetScale(DirectX::SimpleMath::Vector3& scale)
{
	m_Obj[0].SetScale(scale);
}

void Player::SetRot(DirectX::SimpleMath::Vector3& rot)
{
	m_Obj[0].SetRot(rot);
}

void Player::SetTrans(DirectX::SimpleMath::Vector3& trans)
{
	m_Obj[0].SetTrans(trans);
}


void Player::FireBullet()
{
	//発射中の場合
	if (m_FireFlag) return;

	//発射するパーツのワールド行列を取得
	Matrix worldm = m_Obj[PARTS_BULLET].GetWorld();
	//抽出した情報を格納
	Vector3 scale;			//ワールドスケーリング
	Quaternion rotation;	//ワールド回転
	Vector3 translation;	//ワールド座標

	//ワールド行列からスケーリング、回転、移動を取り出す
	worldm.Decompose(scale, rotation, translation);

	//スケール固定
	scale = Vector3(1.0f, 1.0f, 1.0f);

	//親パーツから分離、独立させる
	m_Obj[PARTS_BULLET].SetObjParent(nullptr);
	m_Obj[PARTS_BULLET].SetScale(scale);
	m_Obj[PARTS_BULLET].SetRotQ(rotation);
	m_Obj[PARTS_BULLET].SetTrans(translation);
	m_Obj[PARTS_BULLET].SetDrawState(true);

	//弾丸パーツの速度の設定
	m_BulletVel = Vector3(0, 0, -1);
	//パーツの向きに合わせて速度ベクトルを回転
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	m_FireFlag = true;

	ADX2Le::PlaySe(CRI_CUESHEET_0_LASER2);

	m_shootTimeCount++;
}

void Player::ResetBullet()
{
	//発射中ではない場合
	if (!m_FireFlag) return;
	
	//場所をリセット
	m_Obj[PARTS_BULLET].SetObjParent(&m_Obj[PARTS_FIGHTER]);
	m_Obj[PARTS_BULLET].SetTrans(Vector3(0.0f, 0.0f, 0.0f));
	m_Obj[PARTS_BULLET].SetRot(Vector3(0,0,0));
	m_Obj[PARTS_BULLET].SetDrawState(false);

	m_FireFlag = false;

}

void Player::ChangeDebug()
{
	if (m_debug)
	{
		m_debug = false;
	}
	else
	{
		m_debug = true;
	}
}

/// <summary>
/// ジャンプを開始
/// </summary>
void Player::StartJump()
{
	if (!m_isJump)
	{
		//上方向の初速を設定
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

/// <summary>
/// 落下を開始
/// </summary>
void Player::StartFall()
{
	if (!m_isJump)
	{
		//上方向の初速を設定
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

/// <summary>
/// 落下終了
/// </summary>
void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}

/// <summary>
/// プレイヤーがダメージを受けて羽が取れる
/// </summary>
void Player::Damage()
{
	//どちらかが取れる(1,左翼.2,右翼)
	int wing = rand() % 2 + 1;
	SetWingState(wing);

	SetInvincibleState(true);
	m_damageCount++;
}

void Player::PlayerDataReset()
{
	m_shootTimeCount = 0;
	m_damageCount = 0;
	m_defeatCount = 0;
}

/// <summary>
/// パーツギミック
/// </summary>
void Player::PartsGimic()
{
	//ブーストしている状態
	if (m_Obj[PARTS_BOOST].GetDrawState())
	{

		m_boostModelCount++;

		//時間経過でステート変更
		if (m_boostModelCount > m_boostModelTime)
		{
			if (m_boostModelState)
			{
				m_boostModelState = false;
			}
			else
			{
				m_boostModelState = true;
			}
			m_boostModelCount = 0;
		}

		//拡縮を繰り返す
		if (m_boostModelState)
		{
			m_Obj[PARTS_BOOST].SetScale(m_Obj[PARTS_BOOST].GetScale() + Vector3(0.05f, 0, 0.05f));
		}
		else
		{
			m_Obj[PARTS_BOOST].SetScale(m_Obj[PARTS_BOOST].GetScale() - Vector3(0.05f, 0, 0.05f));
		}
	}
}

void Player::PlayerMove(DirectX::Keyboard::State kb)
{
	//死んでいたら抜ける
	if (GetDeathState()) return;

	//常時移動
	{
		//自機のベクトル
		Vector3 moveV(0.0f, 0.0f, -0.05f);

		//左シフトキーが押されていたら加速
		if (kb.LeftShift)
		{
			moveV.z -= 0.2f;
			m_Obj[PARTS_BOOST].SetDrawState(true);
		}
		else
		{
			m_Obj[PARTS_BOOST].SetDrawState(false);
		}

		float yangle = m_Obj[0].GetRot().y;
		float xangle = m_Obj[0].GetRot().x;

		//移動ベクトルを自機の角度分回転
		Matrix yrotmat = Matrix::CreateRotationY(yangle);
		Matrix xrotmat = Matrix::CreateRotationX(xangle);

		Matrix rotmat = xrotmat * yrotmat;
		moveV = Vector3::TransformNormal(moveV, rotmat);
		//自機の座標を移動
		Vector3 pos = m_Obj[0].GetTrans();
		m_Obj[0].SetTrans(pos + moveV);

		//入力されていない場合まっすぐ
		m_Obj[0].SetRot(Vector3(xangle, yangle, 0));
	}
	//aキーを押している間左旋回
	if (kb.A)
	{
		//自機の座標を移動
		float angle = m_Obj[0].GetRot().y;

		float xangle = m_Obj[0].GetRot().x;

		if (GetWingState() != 1)
		{
			m_Obj[0].SetRot(Vector3(xangle, angle + 0.03f, 0.5f));
		}
		else
		{
			m_Obj[0].SetRot(Vector3(xangle, angle + 0.01f, 0.5f));
		}
	}
	//dキーを押している間右旋回
	if (kb.D)
	{
		//自機の座標を移動
		float angle = m_Obj[0].GetRot().y;

		float xangle = m_Obj[0].GetRot().x;

		if (GetWingState() != 2)
		{
			m_Obj[0].SetRot(Vector3(xangle, angle - 0.03f, -0.5f));
		}
		else
		{
			m_Obj[0].SetRot(Vector3(xangle, angle - 0.01f, -0.5f));
		}
		//m_Obj[0].SetRot(Vector3(xangle, angle - 0.03f, -0.5f));
	}

	//wキーを押している間前に傾ける
	if (kb.W)
	{
		//自機の座標を移動
		float angle = m_Obj[0].GetRot().x;

		float yangle = m_Obj[0].GetRot().y;

		float zangle = m_Obj[0].GetRot().z;

		if (angle > -1.4f) m_Obj[0].SetRot(Vector3(angle - 0.03f, yangle, zangle));

	}
	//sキーを押している間後ろに傾ける
	if (kb.S)
	{
		//自機の座標を移動
		float angle = m_Obj[0].GetRot().x;

		float yangle = m_Obj[0].GetRot().y;

		float zangle = m_Obj[0].GetRot().z;

		if(angle < 1.4f) m_Obj[0].SetRot(Vector3(angle + 0.03f, yangle, zangle));

	}
}

/// <summary>
/// 弾の発射、前進など
/// </summary>
void Player::PlayerBullet()
{
	//弾丸パーツの発射
	//発射
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::Space) && m_gamesceneResume == false)
	{
		if (!m_FireFlag)
		{
			//死んでいたら抜ける
			if (GetDeathState()) return;
			FireBullet();
		}
	}

	//弾丸パーツの前進処理、消去処理
	if (m_FireFlag)
	{
		//自機のベクトル
		Vector3 pos = m_Obj[PARTS_BULLET].GetTrans();
		m_Obj[PARTS_BULLET].SetTrans(pos + m_BulletVel);

		m_shootcount++;
		if (m_shootcount > m_deletecount)
		{
			m_shootcount = 0;
			ResetBullet();
		}
	}
}
