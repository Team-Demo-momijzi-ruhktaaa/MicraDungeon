#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	
	//----------------------------------------

	bool collisionFlag = false;

	enum GameState { TITLE, PLAY, OVER };
	GameState gameState = TITLE;

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

	//テクスチャの作成-----------------------------------------------------
	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.texUVData.SetDivide(Float2(3.0f, 1.0f));

	//playerの画像設定ミスあり
	Texture texturePlayer(L"texture/player.jpg");
	texturePlayer.texUVData.SetDivide(Float2(2.0f, 1.0f));

	Texture textureGoal(L"texture/Goal.png");

	Texture textureTitle(L"texture/title.png");
	Texture textureOver(L"texture/clear.png");
	//---------------------------------------------------------------------
	//playerのデータ作成
	setUvData.SetAll(Float2(1.0f, 0.0f));
	setUvData.SetBack(Float2(0.0f, 0.0f));
	texturePlayer.texUVData.SetUVNum(setUvData.uvData);
	PlayerManager pMana(PlayerManager::PlayerAngleMode::PLANE, &texturePlayer);

	//TitleのUI作成
	Mesh TitleUI;
	TitleUI.CreateData(&textureTitle, 0);
	//OverのUI作成
	Mesh OverUI;
	OverUI.CreateData(&textureOver, 0);

	//プレイヤーの足場となるブロックの作成
	//ギリジャン
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
	//ここまでで簡単な道
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

	//あたり判定用データ作成
	OBB obb;

	//ダイレクトサウンドのデバイス作成
	DirectSound* pDs = DirectSound::GetInstance();
	pDs->Create(App::GetWindowHandle());

	WaveFile waveFile;			//音声ファイルデータ
	SoundBuffer soundBuffer;	//再生用バッファ
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
					//プレイ中でも任意でゲームをやめるよう
					return 0;
				}
				if (gameState == PLAY)
				{
					if (pMana.moveFlag)
					{
						pMana.PlayerAngles();//プレイヤーの角度変更
						pMana.PlayerMove();//プレイヤーの座標移動
						pMana.JumpPlay();//ジャンプするかどうか
					}
					pMana.player.SetOBBData();
					for (int i = 0; i < boxDataSize; i++)
					{
						box[i].SetOBBData();
						collisionFlag = obb.OBBCheck(pMana.player.GetOBBData(), box[i].GetOBBData());//最初に球で雑に判定（当たっていた時obbでの判定に入る）
						if (collisionFlag)
						{//当たっている
							if (obb.ColOver(pMana.player.GetOBBData(), box[i].GetOBBData(), pMana.GetPlayerSpeed()))
							{
								if (box[i].GetGoalPoint())
								{
									//ゴールに乗ることができたのでクリア
									gameState = OVER;
								}
								//ボックスの上に乗っているかどうかの判定
								pMana.GravityReset(&box[i]);
							}
							else
							{
								pMana.PlayerRemove();
							}
							break;
						}
					}
					//何にも当たっていないので落ちていく
					if (!collisionFlag)
					{
						pMana.Gravity();
					}
					pMana.Update();//座標などのアップデート
				}
				//ボックスの描画
				for (int i = 0; i < boxDataSize; i++)
				{
					box[i].Draw();
				}
				if (gameState == OVER)
				{
					OverUI.Draw();

					if (App::GetKeyDown(VK_RETURN))
					{
						//リプレイ
						pMana.ResetPos();
						gameState = PLAY;
					}
				}
				break;
		}
	}
	return 0;
}

