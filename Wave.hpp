class WaveFile
{
	friend class SoundBuffer;

private:

	WAVEFORMATEX waveFormat;

	int dataSize;
	byte* pWaveData;

public:

	WaveFile()
	{
		memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

		pWaveData = nullptr;
		dataSize = 0;
	}

	~WaveFile()
	{
		Release();
	}

	void Release()
	{
		if (pWaveData)
		{
			delete[] pWaveData;
			pWaveData = nullptr;
		}
		memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
		dataSize = 0;
	}

	bool Load(TCHAR *filepath)
	{
		//.wav 形式のデータの保存のされ方
		//3つのチャンクに分かれている
		//チャンク データを区切った塊

		//RIFFヘッダーチャンク	ファイルの種類を表している
		//フォーマットチャンク	音声データの記録のされ方
		//データチャンク		音声（波形）データ

		FILE* fp = nullptr;

		//ファイルのオープン
		//バイナリ形式の読み込みモードで開く
		fp = _tfopen(filepath, _T("rb"));// r read
										 // b binary

		if (fp == nullptr)
		{
			//ファイルオープンに失敗
			return false;
		}

		//ファイルのオープンに成功

		//チャンクに関する情報を入れる変数
		char chunkID[5]{};
		char type[5]{};
		unsigned int chunkSize = 0;

		//RIFFヘッダーチャンクを読み込み

		//RIFFヘッダーの構成-----------------------------------
		//riff 4byte "RIFF"の4文字
		//size 4byte 総ファイルサイズ-8(byte 単位)
		//			 引かれた8はriffとsizeの容量分
		//			（つまり size 以降の残りのデータ量）
		//tipe 4byte "Wave"の4文字(音声データであることを示す)
		//-----------------------------------------------------

		//riff == "RIFF" type == "Wave"
		//の場合のみ、その後の読み込みを行う

		//fread関数
		//chunkIDのアドレスにfpから読み出したデータをコピーする
		//データ量char4個分
		//1 （件数）//配列変数にまとめて複数件のデータを読み込みたいときに利用
		fread(chunkID, sizeof(char) * 4, 1, fp);

		//fread行うと 読み出した後 fpの次回の読み出し開始位置が
		//読み出したデータ量分だけ後ろへずれる

		//ファイルデータ量の読み込み
		//ファイルの総容量自体は以降利用しないが
		//fpの読み込み開始地点をずらすために実行
		fread(&chunkSize, sizeof(unsigned int), 1, fp);
		//typeの読み出し
		fread(type, sizeof(char) * 4, 1, fp);

		//文字列の等価比較がやりやすいように
		//string型に文字列をコピーする
		std::string chunkID_st = chunkID;
		std::string type_st = type;

		//正しいWAVEファイルかどうかを調べる
		if (!(chunkID_st == "RIFF" && type_st == "WAVE"))
		{
			return false;//条件から外れているので失敗
		}

		//それぞれのチャンクを読み出したか
		bool fmtChunk = false;
		bool dataChunk = false;

		//チャンクごとの読み出し
		//フォーマットチャンクとデータチャンクが
		//両方読み出せたらループから抜ける
		while (!(fmtChunk && dataChunk))
		{
			//どのチャンクもchunkIDとchunkの要領から
			//データがスタートする
			fread(chunkID, sizeof(char) * 4, 1, fp);
			fread(&chunkSize, sizeof(unsigned int), 1, fp);
			//チャンクサイズはチャンク全体から-8
			//(idとsizeの容量分)されたもの

			chunkID_st = chunkID;

			if (chunkID_st == "fmt ")//fmtの後ろにスペース
			{
				//読み込もうとしているのがフォーマットチャンク

				//フォーマットチャンク---------------------------------------
				//id				4byte	チャンク識別子
				//size				4byte	idとsizeを除くチャンクサイズ
				//format			2byte	波形データのフォーマット
				//channels			2byte	チャンネル数（モノラルorステレオ）
				//samplerate		4byte	サンプリング周波数
				//bytepresec		4byte	平均データの割合
				//blockalign		2byte	データのブロックサイズ
				//bitswidth			2byte	1サンプル当たりのビット数
				//extended_size		2byte	拡張データのサイズ
				//extended			Nbyte	拡張データ
				//-----------------------------------------------------------
				//これらのデータをまとめてWAVEFORMATEXの変数に読み込む

				if (chunkSize >= sizeof(WAVEFORMATEX))
				{
					//チャックサイズがWAVEFORMATEXより大きい場合
					//おそらく拡張データがある場合

					//その場合は拡張分を無視

					fread(&waveFormat, sizeof(WAVEFORMATEX), 1, fp);

					//チャンクの残りの領域の大きさを計算
					int diff = chunkSize - sizeof(WAVEFORMATEX);
					//diff difference 差

					//次のチャンクの先頭までシーク
					//(読み飛ばす)
					fseek(fp, diff, SEEK_CUR);//CURは現在の位置から

				}
				else
				{
					//チャンクサイズがWAVEFORMATEXの大きさに満たない場合
					//チャンクの終わりまで読む
					//取りあえずある分だけ
					fread(&waveFormat, chunkSize, 1, fp);

				}

				//フォーマットチャンク読み込み済みフラグ
				fmtChunk = true;
			}
			else if (chunkID_st == "data")//データチャンク
			{
				//データチャンク-------------------------------------------
				//id				4byte	チャンク識別子
				//size				4byte	idとsizeを除くチャンクのサイズ
				//							（そのまま波形データのデータ量）
				//waveforamtdata	Nbyte	波形データ
				//---------------------------------------------------------

				//データ領域の確保
				dataSize = chunkSize;
				pWaveData = new byte[dataSize];//動的にデータ確保

											   //データの読み込み
											   //freadの戻り値は正しく読めた件数
											   //dataSize件分読む
				unsigned long readCount
					= fread(pWaveData, sizeof(byte), dataSize, fp);

				if (readCount != dataSize)
				{
					//全て読めなかった
					fclose(fp);//ファイル閉じる

					Release();//データリセットする

					return false;//失敗
				}

				dataChunk = true;
			}
			else
			{
				//fmtでもdataでもないチャンクは読み飛ばし
				//(JUNKとか)

				fseek(fp, chunkSize, SEEK_CUR);
			}
		}
		fclose(fp);
		return true;//成功
	}

	bool Load(std::string filepath)
	{
		TCHAR* tp;
		CString cs(filepath.c_str());
		int num = filepath.size();
		tp = new TCHAR[num];
		_tcscpy(tp, cs);
		return Load(tp);
	}
};