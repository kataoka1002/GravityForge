#include "stdafx.h"
#include "EnemyUI.h"
#include "Player.h"
#include "Game.h"

namespace
{
	//スプライトのサイズ(左：画像のサイズ、右：大きさの倍率)
	const Vector2 HP_BLACK_SIZE = { 657.0f * 0.3f,29.0f * 0.5f };
	const Vector2 HP_ORANGE_SIZE = { 657.0f * 0.3f,29.0f * 0.5f };
	const Vector2 HP_RED_SIZE = { 657.0f * 0.3f,29.0f * 0.5f };
	const Vector2 HP_FRAME_SIZE = { 681.0f * 0.3f,53.0f * 0.5f };

	//HPバーのローカルポジション
	const float HP_BER_POS_Y = 220.0f;
	const float HP_BER_POS_Y_CRAWL = 120.0f;

	//オレンジが動く速さ
	const float ORANGE_MOVE_SPEED = 2.0f;
}

EnemyUI::EnemyUI()
{
	//HPバーの初期化
	m_HPBlackSprite.Init("Assets/spriteData/UI/enemy/EnemyHPBlack.dds", HP_BLACK_SIZE.x, HP_BLACK_SIZE.y);
	m_HPBlackSprite.SetScale(m_redScale);
	m_HPOrangeSprite.Init("Assets/spriteData/UI/enemy/EnemyHPOrange.dds", HP_ORANGE_SIZE.x, HP_ORANGE_SIZE.y);
	m_HPOrangeSprite.SetScale(m_orangeScale);
	m_HPRedSprite.Init("Assets/spriteData/UI/enemy/EnemyHPRed.dds", HP_RED_SIZE.x, HP_RED_SIZE.y);
	m_HPRedSprite.SetScale(m_redScale);

	//HPフレームの初期化
	m_HPFrameSprite.Init("Assets/spriteData/UI/enemy/enemyHPFrame.dds", HP_FRAME_SIZE.x, HP_FRAME_SIZE.y);
	m_HPFrameSprite.SetScale(m_redScale);
}

EnemyUI::~EnemyUI()
{

}

bool EnemyUI::Start()
{
	//クラスを見つける
	m_game = FindGO<Game>("game");
	m_player = FindGO<nsPlayer::Player>("player");

	return true;
}

void EnemyUI::Update()
{
	//フレームの処理
	CalcFrame();

	//赤の処理
	CalcRed();

	//オレンジの処理
	CalcOrange();
}

void EnemyUI::CalcScale(Vector3& scale)
{
	//親の体力の割合からUIのサイズを計算している。
	scale.x = m_enemy->GetHP() * (1.0f / m_enemy->GetMaxHP());

	//サイズは0より小さくならない
	if (scale.x <= 0.0f)
	{
		scale.x = 0.0f;
	}
}

void EnemyUI::CalcPosition(Vector2& position, Vector2 spriteSize, bool useLerp, bool isFrame)
{
	//ポジションの取得
	Vector3 pos = m_enemy->GetPosition();

	//エネミーの上の方に画像を表示したいので,y座標を少し大きくする。
	//クロール中なら
	if (m_enemy->IsCrawling())
	{
		pos.y += HP_BER_POS_Y_CRAWL;
	}
	//二足歩行中なら
	else
	{
		pos.y += HP_BER_POS_Y;
	}

	//ワールド座標からスクリーン座標を計算(計算結果がpositionに代入される)
	g_camera3D->CalcScreenPositionFromWorldPosition(position, pos);

	//画像がフレームなら返す
	if (isFrame == true)
	{
		return;
	}

	//画像を左に寄せる量
	Vector3 BerSizeSubtraction = SendHPBer(spriteSize, m_redScale);

	if (useLerp == true)
	{
		//求めた移動量まで補間で近づける
		m_orangeOffset = Math::Lerp(g_gameTime->GetFrameDeltaTime() * ORANGE_MOVE_SPEED, m_orangeOffset, BerSizeSubtraction);
		position.x -= m_orangeOffset.x;
	}
	else
	{
		//移動量の分寄せる
		position.x -= BerSizeSubtraction.x;
	}
}

void EnemyUI::CalcFrame()
{
	//ポジションの計算
	CalcPosition(m_framePosition, Vector2::Zero, false, true);

	//フレームと背景をセットする
	m_HPFrameSprite.SetPosition(Vector3(m_framePosition.x, m_framePosition.y, 0.0f));
	m_HPFrameSprite.Update();
	m_HPBlackSprite.SetPosition(Vector3(m_framePosition.x, m_framePosition.y, 0.0f));
	m_HPBlackSprite.Update();
}

void EnemyUI::CalcRed()
{
	//スケールの処理
	CalcScale(m_redScale);

	//セットする
	m_HPRedSprite.SetScale(m_redScale);
	m_HPRedSprite.Update();


	//ポジションの処理
	CalcPosition(m_redPosition, HP_RED_SIZE);

	//HPバーをセットする
	m_HPRedSprite.SetPosition(Vector3(m_redPosition.x, m_redPosition.y, 0.0f));
	m_HPRedSprite.Update();
}

void EnemyUI::CalcOrange()
{
	//スケールの処理
	CalcScale(m_orangeScale);

	//オレンジのバーも赤の後を追いかける
	m_orangeScaleNew = Math::Lerp(g_gameTime->GetFrameDeltaTime() * ORANGE_MOVE_SPEED, m_orangeScaleNew, m_orangeScale);

	//セットする
	m_HPOrangeSprite.SetScale(m_orangeScaleNew);
	m_HPOrangeSprite.Update();


	//ポジションの処理
	CalcPosition(m_orangePosition, HP_ORANGE_SIZE, true);

	//HPバーをセットする
	m_HPOrangeSprite.SetPosition({ m_orangePosition.x,m_orangePosition.y ,0.0f });
	m_HPOrangeSprite.Update();
}

Vector3 EnemyUI::SendHPBer(Vector2 size, Vector3 scale)
{
	Vector3 BerSize = { size.x,size.y ,1.0f };				//画像の元の大きさ
	Vector3 changedBerSize = Vector3::Zero;					//画像をスケール変換したあとの大きさ
	Vector3 BerSizeSubtraction = Vector3::Zero;				//元画像と変換後画像のスケール差

	changedBerSize.x = BerSize.x * scale.x;
	BerSizeSubtraction.x = BerSize.x - changedBerSize.x;
	BerSizeSubtraction.x *= 0.5f;	//サイズ減少量を2で割ることで,移動させる距離を求める

	return BerSizeSubtraction;
}

bool EnemyUI::CalcAngle(EnemyBase* enemy)
{
	//カメラからエネミーの位置へのベクトルを求める
	Vector3 toEnemy = enemy->GetPosition() - g_camera3D->GetPosition();
	toEnemy.Normalize();

	//カメラの前向きとカメラからエネミーへのベクトルの内積を求める
	float angle = g_camera3D->GetForward().Dot(toEnemy);

	//内積の結果から角度を求める
	angle = acos(angle);

	//カメラから見てエネミーが一定角度以内のとき
	if (fabsf(angle) <= Math::DegToRad(50.0f))
	{
		//プレイヤーとエネミーの距離を求める
		Vector3 diff = enemy->GetPosition() - m_player->GetPosition();

		//一定距離以内なら
		if (diff.Length() <= 1000.0f)
		{
			//描画する
			return true;
		}
	}

	//描画しない
	return false;
}

void EnemyUI::Render(RenderContext& rc)
{
	//描画するかしないかの処理
	if (CalcAngle(m_enemy) == false)
	{
		return;
	}

	//描画
	m_HPBlackSprite.Draw(rc);
	m_HPOrangeSprite.Draw(rc);
	m_HPRedSprite.Draw(rc);
	m_HPFrameSprite.Draw(rc);
}