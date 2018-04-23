class DirectSound
{
private:

	static DirectSound* pInstance;

	IDirectSound8* pDirectSound8;

	void Release()
	{
		if (pDirectSound8 != NULL)
		{
			pDirectSound8->Release();
			pDirectSound8 = NULL;
		}
	}

	DirectSound()
	{
		pDirectSound8 = NULL;
	}

	~DirectSound()
	{
		Release();
	}

	void operator=(const DirectSound& obj) {}
	DirectSound(const DirectSound& obj) {}
public:

	static DirectSound* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new DirectSound();
		}
		return pInstance;
	}

	bool Create(HWND hWnd)
	{
		if (FAILED(DirectSoundCreate8(NULL, &pDirectSound8, NULL)))
		{
			Release();
			return false;
		}

		if (FAILED(pDirectSound8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		{
			Release();
			return false;
		}
		return true;
	}

	HRESULT CreateSoundBuffer(DSBUFFERDESC* pDesc, IDirectSoundBuffer** ppBuffer)
	{
		if (pDirectSound8 == NULL)
		{
			return E_FAIL;
		}

		HRESULT hr = pDirectSound8->CreateSoundBuffer(pDesc, ppBuffer, NULL);

		return hr;
	}
};