#pragma once
#include "ObjectBase.h"
class TeapotRender;

class Teapot : public ObjectBase
{
public:

	Teapot();
	~Teapot();

	void Update();				//�X�V����
	void InitCarCollision();	//�Ԃ̃R���W�����̏�����

	void Render(RenderContext& rc)	override;	//�`�揈��
	void InitModel()				override;	//���f���̏�����
	void OnDestroy()				override;	//������Ƃ��ɌĂ΂�鏈��
	void InitCollision()			override;	//�R���W�����̏�����

private:
	TeapotRender*		m_teapotRender = nullptr;
	CollisionObject*	m_carCollision = nullptr;	// �Ԃ̌`�������R���W����
};

