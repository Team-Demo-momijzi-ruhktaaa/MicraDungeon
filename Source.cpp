#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	//Debug�p�ϐ�-----------------------------
	float num = 0.0f;

	bool playFlag = true;
	//----------------------------------------
	//�g�p�ϐ�
	float moveSpeed = 0.1f;
	float angleSpeed = 0.5f;

	enum GameState{TITLE,PLAY,OVER};
	GameState gameState = TITLE;
	//-------------------------------------

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

	//�}�E�X�̏������W�̐ݒ�
	App::SetMousePosition(App::GetWindowSize().x / 2, App::GetWindowSize().y / 2);
	
	


	//�e�N�X�`���̍쐬-----------------------------------------------------
	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.texUVData.SetDivide(Float2(4.0f, 2.0f));
	//---------------------------------------------------------------------
	//player�̃f�[�^�쐬
	setUvData.SetAll(Float2(3.0f, 1.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	PlayerManager pMana(PlayerManager::PlayerAngleMode::PLANE,&textureBox);

	//�v���C���[�̑���ƂȂ�u���b�N�̍쐬
	Mesh box;
	setUvData.SetAll(Float2(0.0f, 0.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box.CreateData(&textureBox, 1);

	Mesh box2;
	setUvData.SetAll(Float2(2.0f, 1.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box2.CreateData(&textureBox, 1);

	Mesh box3;
	setUvData.SetAll(Float2(2.0f, 0.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box3.CreateData(&textureBox, 1);

	////�_�C���N�g�T�E���h�̃f�o�C�X�쐬
	//DirectSound* pDs = DirectSound::GetInstance();
	//pDs->Create(App::GetWindowHandle());

	//WaveFile waveFile;			//�����t�@�C���f�[�^
	//SoundBuffer soundBuffer;	//�Đ��p�o�b�t�@
	//{
	//	soundBuffer.Create(waveFile);
	//	soundBuffer.Play(true);
	//}

	while (App::Refresh())
	{
		switch (gameState)
		{
			case TITLE:
				if (App::GetKeyDown(VK_RETURN))
				{
					gameState = PLAY;
				}
				break;
			case PLAY:
				if (App::GetKeyDown(VK_ESCAPE))
				{
					//�v���C���ł��C�ӂŃQ�[������߂�悤
					return 0;
				}

				pMana.PlayerBehavior(angleSpeed, moveSpeed);

				box.Draw();
				break;
			case OVER:
				if (App::GetKeyDown(VK_RETURN))
				{
					//���v���C
					//�����Ń��Z�b�g�֐��ق���
					gameState = PLAY;
				}
				break;
			default:
				break;
		}
	}
	return 0;
}

