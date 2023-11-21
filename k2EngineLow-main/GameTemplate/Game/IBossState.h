#pragma once
#include "Boss.h"
#include "BossConstants.h"

/// <summary>
/// ボスの名前空間
/// </summary>
namespace nsBoss
{
	//前方宣言
	class Boss;

	/// <summary>
	/// ボスステート基底クラス
	/// </summary>
	class IBossState
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="enemy">ボス</param>
		IBossState(Boss* boss) : m_boss(boss) {}

		/// <summary>
		/// デスストラクタ
		/// </summary>
		virtual ~IBossState() {}

		/// <summary>
		/// ステート開始時の処理。
		/// </summary>
		virtual void Enter() = 0;

		/// <summary>
		/// ステートの遷移処理。
		/// </summary>
		/// <returns>遷移するステート</returns>
		virtual IBossState* StateChange() = 0;

		/// <summary>
		/// 各ステートにおける更新処理。
		/// </summary>
		virtual void Update() = 0;

	protected:
		Boss* m_boss = nullptr;		// ボス
	};

}