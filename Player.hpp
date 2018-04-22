class Player
{
public:
	Mesh head;//ì™
	Mesh body;//ëÃ
	Mesh leftArm;//ç∂òr
	Mesh rightArm;//âEòr
	Mesh leftLeg;//ç∂ãr
	Mesh rightLeg;//âEãr

	Player(){}

	Player(Texture texHead,Texture texBody,Texture texArm,Texture texLeg)
	{

	}
	~Player()
	{

	}
	void Release()
	{

	}

	void SetPlayerUv()
	{

	}
	
	void PlayerMove(Float3 position)
	{
		head.position += position;
		body.position += position;
		leftArm.position += position;
		rightArm.position += position;
		leftLeg.position += position;
		rightLeg.position += position;
	}


	void Update()
	{
		head.Draw();
		body.Draw();
		leftArm.Draw();
		rightArm.Draw();
		leftLeg.Draw();
		rightLeg.Draw();
	}

private:
	
};