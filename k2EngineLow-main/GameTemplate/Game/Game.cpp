#include "stdafx.h"
#include "Game.h"
#include "GameClear.h"
#include "GameOver.h"
#include "Fade.h"

Game::Game()
{
	
}

Game::~Game()
{
	//�G�l�~�[�����ׂč폜
	for (auto enemy : m_enemyList)
	{
		//�폜
		DeleteGO(enemy);
	}
	//���X�g����ɂ���
	m_enemyList.clear();

	//�I�u�W�F�N�g��S�č폜
	for (auto object : m_objectList)
	{
		//�폜
		DeleteGO(object);
	}
	//���X�g����ɂ���
	m_objectList.clear();


	//�폜
	//DeleteGO(m_skyCube);	//ResultUI�Ńf���[�g���Ă���
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_camera);
	DeleteGO(m_light);
	DeleteGO(m_boss);
	DeleteGO(m_reticle);
	DeleteGO(m_playerUI);
	DeleteGO(m_wall);
	//DeleteGO(m_gameInfo); //ResultUI�Ńf���[�g���Ă���
	DeleteGO(m_blackFade);
	DeleteGO(m_missionUI);
	DeleteGO(m_makeEfe);

	DeleteGO(m_airRender);
	DeleteGO(m_barrierFenceRender);
	DeleteGO(m_benchBigRender);
	DeleteGO(m_benchSmallRender);
	DeleteGO(m_billboardSmallRender);
	DeleteGO(m_bushBigRender);
	DeleteGO(m_coneRender);
	DeleteGO(m_dustbinRender);
	DeleteGO(m_fenceRender);
	DeleteGO(m_hydrantRender);
	DeleteGO(m_plantLongRender);
	DeleteGO(m_plantLowRender);
	DeleteGO(m_rockBigRender);
	DeleteGO(m_solarPanelRender);
	DeleteGO(m_teapotRender);
}

bool Game::Start()
{
	Fade* m_fade = FindGO<Fade>("fade");
	m_fade->SetAlphaDown();

	//�G�t�F�N�g�̏�����
	m_makeEfe = NewGO<MakeEffect>(0, "makeeffect");

	//�Q�[�����N���X�̍쐬
	m_gameInfo = NewGO<GameInformation>(0, "gameinformation");

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	
	//�C���X�^���X��
	int AirNum = 0, barrierFenceNum = 0, benchBigNum = 0, benchSmallNum = 0,
		billboardSmallNum = 0, bushBigNum = 0, coneNum = 0, dustbinNum = 0, fenceNum = 0,
		hydrantNum = 0, plantLongNum = 0, plantLowNum = 0, rockBigNum = 0, solarPanelNum = 0,
		teapotNum = 0, humanEnemyNum = 0;

	m_levelRender.Init("Assets/modelData/level/newLevel.tkl",
		[&](LevelObjectData& objData)
		{
			//���O��human��������B
			if (objData.EqualObjectName(L"player") == true)
			{
				//�v���C���[�̃I�u�W�F�N�g���쐬����B
				m_player = NewGO<nsPlayer::Player>(0, "player");
				//���W��ݒ肷��B
				m_player->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_player->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_player->SetRotation(objData.rotation);
				//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�܂��B
				return true;
			}
			//���O��background��������B
			else if (objData.EqualObjectName(L"background") == true)
			{
				//�w�i�I�u�W�F�N�g���쐬����B
				m_bg = NewGO<BackGround>(0, "background");
				//���W��ݒ肷��B
				m_bg->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_bg->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_bg->SetRotation(objData.rotation);
				return true;
			}
			//���O��boss��������B
			else if (objData.EqualObjectName(L"boss") == true)
			{
				//�{�X�I�u�W�F�N�g���쐬����B
				m_boss = NewGO<nsBoss::Boss>(0, "boss");
				//���W��ݒ肷��B
				m_boss->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_boss->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_boss->SetRotation(objData.rotation);
				return true;
			}
			else if (objData.EqualObjectName(L"air") == true)
			{
				//���C��I�u�W�F�N�g���쐬����B
				m_air = NewGO<Air>(0, "air");

				SetLevel(m_air, objData);
				m_air->SetInstanceNo(AirNum);
				AirNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_air);

				return true;
			}
			else if (objData.EqualObjectName(L"barrierFence") == true)
			{
				//�H���̍�I�u�W�F�N�g���쐬����B
				m_barrierFence = NewGO<BarrierFence>(0, "barrierfence");

				SetLevel(m_barrierFence, objData);
				m_barrierFence->SetInstanceNo(barrierFenceNum);
				barrierFenceNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_barrierFence);
				return true;
			}
			else if (objData.EqualObjectName(L"bench_big") == true)
			{
				//�ł����x���`�I�u�W�F�N�g���쐬����B
				m_benchBig = NewGO<BenchBig>(0, "benchbig");

				SetLevel(m_benchBig, objData);
				m_benchBig->SetInstanceNo(benchBigNum);
				benchBigNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_benchBig);
				return true;
			}
			else if (objData.EqualObjectName(L"bench_small") == true)
			{
				//�������x���`�I�u�W�F�N�g���쐬����B
				m_benchSmall = NewGO<BenchSmall>(0, "benchsmall");

				SetLevel(m_benchSmall, objData);
				m_benchSmall->SetInstanceNo(benchSmallNum);
				benchSmallNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_benchSmall);
				return true;
			}
			else if (objData.EqualObjectName(L"billboard_small") == true)
			{
				//�ŔI�u�W�F�N�g���쐬����B
				m_billboardSmall = NewGO<BillboardSmall>(0, "billboardsmall");

				SetLevel(m_billboardSmall, objData);
				m_billboardSmall->SetInstanceNo(billboardSmallNum);
				billboardSmallNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_billboardSmall);
				return true;
			}
			else if (objData.EqualObjectName(L"bush_big") == true)
			{
				//���I�u�W�F�N�g���쐬����B
				m_bushBig = NewGO<BushBig>(0, "bushbig");

				SetLevel(m_bushBig, objData);
				m_bushBig->SetInstanceNo(bushBigNum);
				bushBigNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_bushBig);
				return true;
			}
			else if (objData.EqualObjectName(L"cone") == true)
			{
				//�O�p�R�[���I�u�W�F�N�g���쐬����B
				m_cone = NewGO<Cone>(0, "cone");

				SetLevel(m_cone, objData);
				m_cone->SetInstanceNo(coneNum);
				coneNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_cone);
				return true;
			}
			else if (objData.EqualObjectName(L"dustbin") == true)
			{
				//�S�~���I�u�W�F�N�g���쐬����B
				m_dustbin = NewGO<Dustbin>(0, "dustbin");

				SetLevel(m_dustbin, objData);
				m_dustbin->SetInstanceNo(dustbinNum);
				dustbinNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_dustbin);
				return true;
			}
			else if (objData.EqualObjectName(L"fence") == true)
			{
				//�t�F���X�I�u�W�F�N�g���쐬����B
				m_fence = NewGO<Fence>(0, "fence");

				SetLevel(m_fence, objData);
				m_fence->SetInstanceNo(fenceNum);
				fenceNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_fence);
				return true;
			}
			else if (objData.EqualObjectName(L"hydrant") == true)
			{
				//���ΐ��I�u�W�F�N�g���쐬����B
				m_hydrant = NewGO<Hydrant>(0, "hydrant");

				SetLevel(m_hydrant, objData);
				m_hydrant->SetInstanceNo(hydrantNum);
				hydrantNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_hydrant);
				return true;
			}
			else if (objData.EqualObjectName(L"plant_long") == true)
			{
				//��ԕr�I�u�W�F�N�g���쐬����B
				m_plantLong = NewGO<PlantLong>(0, "plantlong");

				SetLevel(m_plantLong, objData);
				m_plantLong->SetInstanceNo(plantLongNum);
				plantLongNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_plantLong);
				return true;
			}
			else if (objData.EqualObjectName(L"plant_low") == true)
			{
				//���ԕr�I�u�W�F�N�g���쐬����B
				m_plantLow = NewGO<PlantLow>(0, "plantlow");

				SetLevel(m_plantLow, objData);
				m_plantLow->SetInstanceNo(plantLowNum);
				plantLowNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_plantLow);
				return true;
			}
			else if (objData.EqualObjectName(L"rock_big") == true)
			{
				//��I�u�W�F�N�g���쐬����B
				m_rockBig = NewGO<RockBig>(0, "rockbig");

				SetLevel(m_rockBig, objData);
				m_rockBig->SetInstanceNo(rockBigNum);
				rockBigNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_rockBig);
				return true;
			}
			else if (objData.EqualObjectName(L"solarPanel") == true)
			{
				//�\�[���[�p�l���I�u�W�F�N�g���쐬����B
				m_solarPanel = NewGO<SolarPanel>(0, "solarpanel");

				SetLevel(m_solarPanel, objData);
				m_solarPanel->SetInstanceNo(solarPanelNum);
				solarPanelNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_solarPanel);
				return true;
			}
			//���O��teapot��������B
			else if (objData.EqualObjectName(L"teapot") == true)
			{
				//�e�B�[�|�b�g�I�u�W�F�N�g���쐬����B
				m_teapot = NewGO<Teapot>(0, "teapot");

				SetLevel(m_teapot, objData);
				m_teapot->SetInstanceNo(teapotNum);
				teapotNum++;

				//���X�g�ɒǉ�
				m_objectList.emplace_back(m_teapot);
				return true;
			}
			//���O��humanEnemy��������B
			else if (objData.EqualObjectName(L"humanEnemy") == true)
			{
				//�G�l�~�[�I�u�W�F�N�g���쐬����B
				m_humanEnemy = NewGO<nsHumanEnemy::HumanEnemy>(0, "humanenemy");
				//���W��ݒ肷��B
				m_humanEnemy->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_humanEnemy->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_humanEnemy->SetRotation(objData.rotation);

				m_humanEnemy->SetInstanceNo(humanEnemyNum);
				humanEnemyNum++;

				//���X�g�ɒǉ�
				m_enemyList.emplace_back(m_humanEnemy);
				return true;
			}
			//���O��wall��������B
			else if (objData.EqualObjectName(L"wall") == true)
			{
				//�����蔻��ǃI�u�W�F�N�g���쐬����B
				m_wall = NewGO<Wall>(0, "wall");
				//���W��ݒ肷��B
				m_wall->SetPosition(objData.position);
				//�傫����ݒ肷��B
				m_wall->SetScale(objData.scale);
				//��]��ݒ肷��B
				m_wall->SetRotation(objData.rotation);
				return true;
			}

			return true;
		});

	//�����_���[���쐬
	m_airRender = NewGO<AirRender>(0, "airrender");
	m_airRender->SetMaxAir(AirNum);
	m_barrierFenceRender = NewGO<BarrierFenceRender>(0, "barrierfencerender");
	m_barrierFenceRender->SetMaxModel(barrierFenceNum);
	m_benchBigRender = NewGO<BenchiBigRender>(0, "benchibigrender");
	m_benchBigRender->SetMaxModel(benchBigNum);
	m_benchSmallRender = NewGO<BenchiSmallRender>(0, "benchismallrender");
	m_benchSmallRender->SetMaxModel(benchSmallNum);
	m_billboardSmallRender = NewGO<BillboardSmallRender>(0, "billboardsmallrender");
	m_billboardSmallRender->SetMaxModel(billboardSmallNum);
	m_bushBigRender = NewGO<BushBigRender>(0, "bushbigrender");
	m_bushBigRender->SetMaxModel(bushBigNum);
	m_coneRender = NewGO<ConeRender>(0, "conerender");
	m_coneRender->SetMaxModel(coneNum);
	m_dustbinRender = NewGO<DustbinRender>(0, "dustbinrender");
	m_dustbinRender->SetMaxModel(dustbinNum);
	m_fenceRender = NewGO<FenceRender>(0, "fencerender");
	m_fenceRender->SetMaxModel(fenceNum);
	m_hydrantRender = NewGO<HydrantRender>(0, "hydrantrender");
	m_hydrantRender->SetMaxModel(hydrantNum);
	m_plantLongRender = NewGO<PlantLongRender>(0, "plantlongrender");
	m_plantLongRender->SetMaxModel(plantLongNum);
	m_plantLowRender = NewGO<PlantLowRender>(0, "plantlowrender");
	m_plantLowRender->SetMaxModel(plantLowNum);
	m_rockBigRender = NewGO<RockBigRender>(0, "rockbigrender");
	m_rockBigRender->SetMaxModel(rockBigNum);
	m_solarPanelRender = NewGO<SolarPanelRender>(0, "solarpanelrender");
	m_solarPanelRender->SetMaxModel(solarPanelNum);
	m_teapotRender = NewGO<TeapotRender>(0, "teapotrender");
	m_teapotRender->SetMaxModel(teapotNum);
	//m_humanEnemyRender = NewGO<nsHumanEnemy::HumanEnemyRender>(0, "humanenemyrender");
	//m_humanEnemyRender->SetMaxHumanEnemy(humanEnemyNum);
	
	//�X�J�C�L���[�u�̍쐬
	SetSkyCube();

	//���e�B�N���̍쐬
	m_reticle = NewGO<nsPlayer::Reticle>(0, "reticle");

	//UI�̍쐬
	m_playerUI = NewGO<nsPlayer::PlayerUI>(0, "playerui");

	//�Q�[���J�����̍쐬
	m_camera = NewGO<GameCamera>(0, "gamecamera");

	//���C�g�̍쐬
	m_light = NewGO<Lighting>(0, "lighting");

	//���t�F�[�h�̍쐬
	m_blackFade = NewGO<BlackFade>(1, "blackfade");

	//�~�b�V����UI�̍쐬
	m_missionUI = NewGO<MissionUI>(0, "missionui");

	return true;
}

template <typename T>
void Game::SetLevel(T* objct, LevelObjectData& objData)
{
	//���W��ݒ肷��B
	objct->SetPosition(objData.position);
	//�傫����ݒ肷��B
	objct->SetScale(objData.scale);
	//��]��ݒ肷��B
	objct->SetRotation(objData.rotation);
}

void Game::SetGameClear()
{
	if (m_gameClear == false && m_gameOver == false)
	{
		//�t�F�[�h�C���̊J�n
		m_blackFade->SetAlphaUp(true);

		m_gameClear = true;

		//BGM�̒�~
		m_gameInfo->StopBGM();
	}
}

void Game::SetGameOver()
{
	if (m_gameClear == false && m_gameOver == false)
	{
		//�t�F�[�h�C���̊J�n
		m_blackFade->SetAlphaUp(true);

		m_gameOver = true;

		//BGM�̒�~
		m_gameInfo->StopBGM();
	}
}

void Game::Update()
{
	if (m_gameClear == true)
	{
		//�t�F�[�h�C�����I�������
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//�Q�[���N���A�𐶐�
			NewGO<GameClear>(0, "gameclear");

			//�������g�̍폜
			DeleteGO(this);
		}
	}
	else if (m_gameOver == true)
	{
		//�t�F�[�h�C�����I�������
		if (m_blackFade->GetBlackAlpha() >= 1.0f)
		{
			//�Q�[���I�[�o�[�𐶐�
			NewGO<GameOver>(0, "gameover");

			//�������g�̍폜
			DeleteGO(this);
		}
	}
}

void Game::SetSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(2000.0f);
	m_skyCube->SetPosition({-5000.0f,500.0f,6000.0f});
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	// �����̌v�Z�̂��߂�IBL�e�N�X�`�����Z�b�g����B
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}

void Game::Render(RenderContext& rc)
{

}