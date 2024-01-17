#include "stdafx.h"
#include "system/system.h"
#include "Title.h"
#include "MakeSound.h"
#include "Game.h"

// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;
namespace nsK2EngineLow {
	extern int g_hoge;
}
/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Gravity Forge"));
	
	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition(0, 100.0f, 350.0f);
	g_camera3D->SetTarget(0, 100.0f, 0);

	auto makeSound = NewGO<MakeSound>(0, "makesound");
	auto title = NewGO<Title>(0, "title");
	
	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		nsK2EngineLow::g_hoge = 0;
		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_k2EngineLow->ExecuteUpdate();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_k2EngineLow->ExecuteRender();

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
		K2_LOG("�`��I�u�W�F�N�g�� = %d\n", nsK2EngineLow::g_hoge);

	}

	delete g_k2EngineLow;

	return 0;
}

