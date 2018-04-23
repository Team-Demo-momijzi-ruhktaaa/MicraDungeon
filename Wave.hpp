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
		//.wav �`���̃f�[�^�̕ۑ��̂����
		//3�̃`�����N�ɕ�����Ă���
		//�`�����N �f�[�^����؂�����

		//RIFF�w�b�_�[�`�����N	�t�@�C���̎�ނ�\���Ă���
		//�t�H�[�}�b�g�`�����N	�����f�[�^�̋L�^�̂����
		//�f�[�^�`�����N		�����i�g�`�j�f�[�^

		FILE* fp = nullptr;

		//�t�@�C���̃I�[�v��
		//�o�C�i���`���̓ǂݍ��݃��[�h�ŊJ��
		fp = _tfopen(filepath, _T("rb"));// r read
										 // b binary

		if (fp == nullptr)
		{
			//�t�@�C���I�[�v���Ɏ��s
			return false;
		}

		//�t�@�C���̃I�[�v���ɐ���

		//�`�����N�Ɋւ����������ϐ�
		char chunkID[5]{};
		char type[5]{};
		unsigned int chunkSize = 0;

		//RIFF�w�b�_�[�`�����N��ǂݍ���

		//RIFF�w�b�_�[�̍\��-----------------------------------
		//riff 4byte "RIFF"��4����
		//size 4byte ���t�@�C���T�C�Y-8(byte �P��)
		//			 �����ꂽ8��riff��size�̗e�ʕ�
		//			�i�܂� size �ȍ~�̎c��̃f�[�^�ʁj
		//tipe 4byte "Wave"��4����(�����f�[�^�ł��邱�Ƃ�����)
		//-----------------------------------------------------

		//riff == "RIFF" type == "Wave"
		//�̏ꍇ�̂݁A���̌�̓ǂݍ��݂��s��

		//fread�֐�
		//chunkID�̃A�h���X��fp����ǂݏo�����f�[�^���R�s�[����
		//�f�[�^��char4��
		//1 �i�����j//�z��ϐ��ɂ܂Ƃ߂ĕ������̃f�[�^��ǂݍ��݂����Ƃ��ɗ��p
		fread(chunkID, sizeof(char) * 4, 1, fp);

		//fread�s���� �ǂݏo������ fp�̎���̓ǂݏo���J�n�ʒu��
		//�ǂݏo�����f�[�^�ʕ��������ւ����

		//�t�@�C���f�[�^�ʂ̓ǂݍ���
		//�t�@�C���̑��e�ʎ��͈̂ȍ~���p���Ȃ���
		//fp�̓ǂݍ��݊J�n�n�_�����炷���߂Ɏ��s
		fread(&chunkSize, sizeof(unsigned int), 1, fp);
		//type�̓ǂݏo��
		fread(type, sizeof(char) * 4, 1, fp);

		//������̓�����r�����₷���悤��
		//string�^�ɕ�������R�s�[����
		std::string chunkID_st = chunkID;
		std::string type_st = type;

		//������WAVE�t�@�C�����ǂ����𒲂ׂ�
		if (!(chunkID_st == "RIFF" && type_st == "WAVE"))
		{
			return false;//��������O��Ă���̂Ŏ��s
		}

		//���ꂼ��̃`�����N��ǂݏo������
		bool fmtChunk = false;
		bool dataChunk = false;

		//�`�����N���Ƃ̓ǂݏo��
		//�t�H�[�}�b�g�`�����N�ƃf�[�^�`�����N��
		//�����ǂݏo�����烋�[�v���甲����
		while (!(fmtChunk && dataChunk))
		{
			//�ǂ̃`�����N��chunkID��chunk�̗v�̂���
			//�f�[�^���X�^�[�g����
			fread(chunkID, sizeof(char) * 4, 1, fp);
			fread(&chunkSize, sizeof(unsigned int), 1, fp);
			//�`�����N�T�C�Y�̓`�����N�S�̂���-8
			//(id��size�̗e�ʕ�)���ꂽ����

			chunkID_st = chunkID;

			if (chunkID_st == "fmt ")//fmt�̌��ɃX�y�[�X
			{
				//�ǂݍ������Ƃ��Ă���̂��t�H�[�}�b�g�`�����N

				//�t�H�[�}�b�g�`�����N---------------------------------------
				//id				4byte	�`�����N���ʎq
				//size				4byte	id��size�������`�����N�T�C�Y
				//format			2byte	�g�`�f�[�^�̃t�H�[�}�b�g
				//channels			2byte	�`�����l�����i���m����or�X�e���I�j
				//samplerate		4byte	�T���v�����O���g��
				//bytepresec		4byte	���σf�[�^�̊���
				//blockalign		2byte	�f�[�^�̃u���b�N�T�C�Y
				//bitswidth			2byte	1�T���v��������̃r�b�g��
				//extended_size		2byte	�g���f�[�^�̃T�C�Y
				//extended			Nbyte	�g���f�[�^
				//-----------------------------------------------------------
				//�����̃f�[�^���܂Ƃ߂�WAVEFORMATEX�̕ϐ��ɓǂݍ���

				if (chunkSize >= sizeof(WAVEFORMATEX))
				{
					//�`���b�N�T�C�Y��WAVEFORMATEX���傫���ꍇ
					//�����炭�g���f�[�^������ꍇ

					//���̏ꍇ�͊g�����𖳎�

					fread(&waveFormat, sizeof(WAVEFORMATEX), 1, fp);

					//�`�����N�̎c��̗̈�̑傫�����v�Z
					int diff = chunkSize - sizeof(WAVEFORMATEX);
					//diff difference ��

					//���̃`�����N�̐擪�܂ŃV�[�N
					//(�ǂݔ�΂�)
					fseek(fp, diff, SEEK_CUR);//CUR�͌��݂̈ʒu����

				}
				else
				{
					//�`�����N�T�C�Y��WAVEFORMATEX�̑傫���ɖ����Ȃ��ꍇ
					//�`�����N�̏I���܂œǂ�
					//��肠�������镪����
					fread(&waveFormat, chunkSize, 1, fp);

				}

				//�t�H�[�}�b�g�`�����N�ǂݍ��ݍς݃t���O
				fmtChunk = true;
			}
			else if (chunkID_st == "data")//�f�[�^�`�����N
			{
				//�f�[�^�`�����N-------------------------------------------
				//id				4byte	�`�����N���ʎq
				//size				4byte	id��size�������`�����N�̃T�C�Y
				//							�i���̂܂ܔg�`�f�[�^�̃f�[�^�ʁj
				//waveforamtdata	Nbyte	�g�`�f�[�^
				//---------------------------------------------------------

				//�f�[�^�̈�̊m��
				dataSize = chunkSize;
				pWaveData = new byte[dataSize];//���I�Ƀf�[�^�m��

											   //�f�[�^�̓ǂݍ���
											   //fread�̖߂�l�͐������ǂ߂�����
											   //dataSize�����ǂ�
				unsigned long readCount
					= fread(pWaveData, sizeof(byte), dataSize, fp);

				if (readCount != dataSize)
				{
					//�S�ēǂ߂Ȃ�����
					fclose(fp);//�t�@�C������

					Release();//�f�[�^���Z�b�g����

					return false;//���s
				}

				dataChunk = true;
			}
			else
			{
				//fmt�ł�data�ł��Ȃ��`�����N�͓ǂݔ�΂�
				//(JUNK�Ƃ�)

				fseek(fp, chunkSize, SEEK_CUR);
			}
		}
		fclose(fp);
		return true;//����
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