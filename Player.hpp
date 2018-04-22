class Player
{
public:
	Mesh head;//��
	Mesh body;//��
	Mesh leftArm;//���r
	Mesh rightArm;//�E�r
	Mesh leftLeg;//���r
	Mesh rightLeg;//�E�r

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