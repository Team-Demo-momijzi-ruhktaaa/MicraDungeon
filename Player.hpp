class Player
{
public:
	Player(){}

	Player(Texture* texArm)
	{
		leftArmBase.CreateData(texArm, 1);
		rightArmBase.CreateData(texArm, 1);
		leftArmBase.scale = 0.5f;
		leftArmBase.axis.x = -1.0f;
		leftArmBase.position.y = -1.0f;

		rightArmBase.scale = 0.5f;
		rightArmBase.axis.x = 1.0f;
		rightArmBase.position.y = -1.0f;
	}
	~Player()
	{
	}
	
	void PlayerMove(Float3 position)
	{
		leftArmBase.position += position;
		rightArmBase.position += position;
	}

	void PlayerRatate(Float3 angles)
	{
		leftArmBase.angles += angles;
		rightArmBase.angles += angles;
	}

	void Draw()
	{
		leftArmBase.Draw();
		rightArmBase.Draw();
	}

private:
	Mesh leftArmBase;//ç∂òrÇÃäÓì_Ç…Ç»ÇÈç¿ïW
	Mesh rightArmBase;//âEòr

};