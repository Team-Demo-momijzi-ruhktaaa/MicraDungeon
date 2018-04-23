#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	//Debug用変数-----------------------------
	float num = 0.0f;

	bool playFlag = true;
	//----------------------------------------
	//使用変数
	float moveSpeed = 0.1f;
	float angleSpeed = 0.5f;

	enum GameState{TITLE,PLAY,OVER};
	GameState gameState = TITLE;
	//-------------------------------------

	//textureのuvを設定するためのもの
	struct SetUvData
	{
		Float2 uvData[6] = {};

		//前面同じuvに設定
		void SetAll(Float2 numUV)
		{
			for (int i = 0; i < 6; i++)
			{
				uvData[i] = numUV;
			}
		}
		//上と下以外のuv設定
		void SetAround(Float2 numUV)
		{
			for (int i = 0; i < 4; i++)
			{
				uvData[i] = numUV;
			}
		}
		//前面のuv設定
		void SetFront(Float2 numUV)
		{
			uvData[0] = numUV;
		}
		//後面のuv設定
		void SetBack(Float2 numUV)
		{
			uvData[1] = numUV;
		}
		//左面のuv設定
		void SetLeft(Float2 numUV)
		{
			uvData[2] = numUV;
		}
		//前面のuv設定
		void SetRight(Float2 numUV)
		{
			uvData[3] = numUV;
		}
		//上面のuv設定
		void SetUp(Float2 numUV)
		{
			uvData[4] = numUV;
		}
		//下面のuv設定
		void SetDown(Float2 numUV)
		{
			uvData[5] = numUV;
		}
	};
	SetUvData setUvData;

	//マウスの初期座標の設定
	App::SetMousePosition(App::GetWindowSize().x / 2, App::GetWindowSize().y / 2);
	
	


	//テクスチャの作成-----------------------------------------------------
	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.texUVData.SetDivide(Float2(4.0f, 2.0f));
	//---------------------------------------------------------------------
	//playerのデータ作成
	setUvData.SetAll(Float2(3.0f, 1.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	PlayerManager pMana(PlayerManager::PlayerAngleMode::PLANE,&textureBox);

	//プレイヤーの足場となるブロックの作成
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

	////ダイレクトサウンドのデバイス作成
	//DirectSound* pDs = DirectSound::GetInstance();
	//pDs->Create(App::GetWindowHandle());

	//WaveFile waveFile;			//音声ファイルデータ
	//SoundBuffer soundBuffer;	//再生用バッファ
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
					//プレイ中でも任意でゲームをやめるよう
					return 0;
				}

				pMana.PlayerBehavior(angleSpeed, moveSpeed);

				box.Draw();
				break;
			case OVER:
				if (App::GetKeyDown(VK_RETURN))
				{
					//リプレイ
					//ここでリセット関数ほしい
					gameState = PLAY;
				}
				break;
			default:
				break;
		}
	}
	return 0;
}

