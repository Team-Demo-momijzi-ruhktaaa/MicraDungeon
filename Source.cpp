#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	
	//----------------------------------------

	bool collisionFlag = false;

	enum GameState { TITLE, PLAY, OVER };
	GameState gameState = TITLE;

	//texture��uv��ݒ肷�邽�߂̂���
	struct SetUvData
	{
		Float2 uvData[6] = {};

		//�O�ʓ���uv�ɐݒ�
		void SetAll(Float2 numUV)
		{
			for (int i = 0; i < 6; i++)
			{
				uvData[i] = numUV;
			}
		}
		//��Ɖ��ȊO��uv�ݒ�
		void SetAround(Float2 numUV)
		{
			for (int i = 0; i < 4; i++)
			{
				uvData[i] = numUV;
			}
		}
		//�O�ʂ�uv�ݒ�
		void SetFront(Float2 numUV)
		{
			uvData[0] = numUV;
		}
		//��ʂ�uv�ݒ�
		void SetBack(Float2 numUV)
		{
			uvData[1] = numUV;
		}
		//���ʂ�uv�ݒ�
		void SetLeft(Float2 numUV)
		{
			uvData[2] = numUV;
		}
		//�O�ʂ�uv�ݒ�
		void SetRight(Float2 numUV)
		{
			uvData[3] = numUV;
		}
		//��ʂ�uv�ݒ�
		void SetUp(Float2 numUV)
		{
			uvData[4] = numUV;
		}
		//���ʂ�uv�ݒ�
		void SetDown(Float2 numUV)
		{
			uvData[5] = numUV;
		}
	};
	SetUvData setUvData;

	//�e�N�X�`���̍쐬-----------------------------------------------------
	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.texUVData.SetDivide(Float2(3.0f, 1.0f));

	//player�̉摜�ݒ�~�X����
	Texture texturePlayer(L"texture/player.jpg");
	texturePlayer.texUVData.SetDivide(Float2(2.0f, 1.0f));

	Texture textureGoal(L"texture/Goal.png");

	Texture textureTitle(L"texture/title.png");
	Texture textureOver(L"texture/clear.png");
	//---------------------------------------------------------------------
	//player�̃f�[�^�쐬
	setUvData.SetAll(Float2(1.0f, 0.0f));
	setUvData.SetBack(Float2(0.0f, 0.0f));
	texturePlayer.texUVData.SetUVNum(setUvData.uvData);
	PlayerManager pMana(PlayerManager::PlayerAngleMode::PLANE, &texturePlayer);

	//Title��UI�쐬
	Mesh TitleUI;
	TitleUI.CreateData(&textureTitle, 0);
	//Over��UI�쐬
	Mesh OverUI;
	OverUI.CreateData(&textureOver, 0);

	//�v���C���[�̑���ƂȂ�u���b�N�̍쐬
	//�M���W����
	const int boxDataSize = 11;
	Mesh box[boxDataSize];
	
	setUvData.SetAll(Float2(0.0f, 0.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);

	box[0].CreateData(&textureBox, 1);

	box[1].CreateData(&textureBox, 1);
	box[1].position = Float3(0.0f, 0.0f, 5.0f);

	box[2].CreateData(&textureBox, 1);
	box[2].position = Float3(0.0f, 2.0f, 10.0f);

	box[3].CreateData(&textureBox, 1);
	box[3].position = Float3(0.0f, -0.5f, 20.0f);
	
	box[4].CreateData(&textureBox, 1);
	box[4].position = Float3(0.0f, 2.0f, 25.0f);

	box[5].CreateData(&textureGoal, 1);
	box[5].position = Float3(0.0f, 5.0f, 30.0f);
	box[5].SetGoalPoint();
	//�����܂łŊȒP�ȓ�
	setUvData.SetAll(Float2(1.0f, 0.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box[6].CreateData(&textureBox, 1);
	box[6].position = Float3(5.0f, 1.0f, 5.0f);

	box[7].CreateData(&textureBox, 1);
	box[7].position = Float3(10.0f, 2.0f, 10.0f);

	box[8].CreateData(&textureBox, 1);
	box[8].position = Float3(10.0f, 4.0f, 15.0f);

	box[9].CreateData(&textureBox, 1);
	box[9].position = Float3(6.0f, 0.0f, 25.0f);
	
	box[10].CreateData(&textureBox, 1);
	box[10].position = Float3(6.0f, 3.0f, 30.0f);

	for (int i = 0; i < boxDataSize; i++)
	{
		box[i].Draw();
	}

	//�����蔻��p�f�[�^�쐬
	OBB obb;

	//�_�C���N�g�T�E���h�̃f�o�C�X�쐬
	DirectSound* pDs = DirectSound::GetInstance();
	pDs->Create(App::GetWindowHandle());

	WaveFile waveFile;			//�����t�@�C���f�[�^
	SoundBuffer soundBuffer;	//�Đ��p�o�b�t�@
	if (waveFile.Load("music/BGM.wav"))
	{
		soundBuffer.Create(waveFile);
	}

	while (App::Refresh())
	{
		switch (gameState)
		{
			case TITLE:
				TitleUI.Draw();
				if (App::GetKeyDown(VK_RETURN))
				{
					gameState = PLAY;
					soundBuffer.Play(true);
				}
				break;
			default:
				if (App::GetKeyDown(VK_ESCAPE))
				{
					//�v���C���ł��C�ӂŃQ�[������߂�悤
					return 0;
				}
				if (gameState == PLAY)
				{
					if (pMana.moveFlag)
					{
						pMana.PlayerAngles();//�v���C���[�̊p�x�ύX
						pMana.PlayerMove();//�v���C���[�̍��W�ړ�
						pMana.JumpPlay();//�W�����v���邩�ǂ���
					}
					pMana.player.SetOBBData();
					for (int i = 0; i < boxDataSize; i++)
					{
						box[i].SetOBBData();
						collisionFlag = obb.OBBCheck(pMana.player.GetOBBData(), box[i].GetOBBData());//�ŏ��ɋ��ŎG�ɔ���i�������Ă�����obb�ł̔���ɓ���j
						if (collisionFlag)
						{//�������Ă���
							if (obb.ColOver(pMana.player.GetOBBData(), box[i].GetOBBData(), pMana.GetPlayerSpeed()))
							{
								if (box[i].GetGoalPoint())
								{
									//�S�[���ɏ�邱�Ƃ��ł����̂ŃN���A
									gameState = OVER;
								}
								//�{�b�N�X�̏�ɏ���Ă��邩�ǂ����̔���
								pMana.GravityReset(&box[i]);
							}
							else
							{
								pMana.PlayerRemove();
							}
							break;
						}
					}
					//���ɂ��������Ă��Ȃ��̂ŗ����Ă���
					if (!collisionFlag)
					{
						pMana.Gravity();
					}
					pMana.Update();//���W�Ȃǂ̃A�b�v�f�[�g
				}
				//�{�b�N�X�̕`��
				for (int i = 0; i < boxDataSize; i++)
				{
					box[i].Draw();
				}
				if (gameState == OVER)
				{
					OverUI.Draw();

					if (App::GetKeyDown(VK_RETURN))
					{
						//���v���C
						pMana.ResetPos();
						gameState = PLAY;
					}
				}
				break;
		}
	}
	return 0;
}

