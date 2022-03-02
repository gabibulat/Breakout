#include <iostream>
#include <map>
#include <string>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class LevelXML {

public:
	LevelXML(int x);

	//const references
	const string& getBricksLayout() { return bricksLayout; };
	const string& getBackgroundTexturePath() { return backgroundTexturePath; };
	const int& getRowSpacing() { return rowSpacing; };
	const int& getColumnSpacing() { return columnSpacing; };
	const map<pair<string, string>, string>& getBrickInfo() { return brickInfo; }
	const map<string, string>& getBrickTexturePath() { return brickTexturePath; };
	
private:
	string bricksLayout, backgroundTexturePath;
	int rowSpacing=0, columnSpacing=0;
	map<pair<string, string>, string> brickInfo;
	map<string, string> brickTexturePath;


};


