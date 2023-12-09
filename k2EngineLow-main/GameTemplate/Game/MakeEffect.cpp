#include "stdafx.h"
#include "MakeEffect.h"

MakeEffect::~MakeEffect()
{

}

bool MakeEffect::Start()
{
	//エフェクトの設定
	EffectEngine::GetInstance()->ResistEffect(enEffectName_ObjectPush, u"Assets/effect/objectPush.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_ObjectSmoke, u"Assets/effect/objectSmoke.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_ObjectDrop, u"Assets/effect/objectDrop.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_ObjectBom, u"Assets/effect/objectBom.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_ObjectSand, u"Assets/effect/objectSand.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_HumanAttack, u"Assets/effect/humanEnemyAttack.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_PlayerPunch, u"Assets/effect/playerPunch.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_BossMagic, u"Assets/effect/bossMagic.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_BossPanch, u"Assets/effect/bossPanch.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_BossSwipe, u"Assets/effect/bossSwipe.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_BossJumpAttack, u"Assets/effect/bossJumpAttack.efk");

	return true;
}

void MakeEffect::Update()
{

}

void MakeEffect::Render(RenderContext& rc)
{

}