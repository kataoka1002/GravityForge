#pragma once
#include "HumanEnemy.h"
#include "HumanEnemyConstants.h"

/// <summary>
/// ヒューマンエネミーの名前空間
/// </summary>
namespace nsHumanEnemy
{
	//前方宣言
	class HumanEnemy;

	/// <summary>
	/// プレイヤーステート基底クラス
	/// </summary>
	class IHumanEnemyState
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="enemy">エネミー</param>
		IHumanEnemyState(HumanEnemy* enemy) : m_enemy(enemy) {}

		/// <summary>
		/// デスストラクタ
		/// </summary>
		virtual ~IHumanEnemyState() {}

		/// <summary>
		/// ステート開始時の処理。
		/// </summary>
		virtual void Enter() = 0;

		/// <summary>
		/// ステートの遷移処理。
		/// </summary>
		/// <returns>遷移するステート</returns>
		virtual IHumanEnemyState* StateChange() = 0;

		/// <summary>
		/// 各ステートにおける更新処理。
		/// </summary>
		virtual void Update() = 0;

	protected:
		HumanEnemy* m_enemy = nullptr;		// エネミー
	};

}