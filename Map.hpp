class Map
{
public:


	int TestMap[10][10] =
	{
		{ 2,2,2,2,2,2,0,0,0,0 },
		{ 2,2,0,0,2,2,0,0,0,0 },
		{ 2,2,0,0,2,2,0,0,0,0 },
		{ 2,0,0,0,0,2,0,0,0,0 },
		{ 2,0,0,0,0,2,0,0,0,0 },
		{ 2,0,0,0,0,2,0,0,0,0 },
		{ 2,2,2,2,2,2,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0 },
	};

	Map()
	{
		App::Initialize();
	}
	Map(const wchar_t* const filepath)
	{
		App::Initialize();
		CreateMap(filepath);
	}

	~Map()
	{

	}

	void CreateMap(const wchar_t* const filepath)
	{
		std::ifstream mapText;
		mapText.open("mapText/mapLayer.txt");

	}

private:
	


	

};