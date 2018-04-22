class Map
{
public:

	Map()
	{
		mapWidth = 0;
		mapHeight = 0;
		App::Initialize();
	}
	Map(const wchar_t* const filepath)
	{
		mapWidth = 0;
		mapHeight = 0;
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

		mapText >> mapWidth;
		mapText >> mapHeight;

		mapText >> mapNum;
	}

private:
	int startPosx, startPosy, startDirection,
						mapNum,mapWidth, mapHeight;

};