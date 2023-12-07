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
	EffectEngine::GetInstance()->ResistEffect(enEffectName_HumanAttack, u"Assets/effect/humanEnemyAttack.efk");
	EffectEngine::GetInstance()->ResistEffect(enEffectName_PlayerPunch, u"Assets/effect/playerPunch.efk");

	return true;
}

void MakeEffect::Update()
{

}

void MakeEffect::Render(RenderContext& rc)
{

}