#include "LevelXML.h"

/*!
	Used for loading XML file containing info about level
*/

LevelXML::LevelXML(int x) {
	XMLDocument doc;
	XMLElement* brickTypeElement;
	string pathXML = "XMLfiles/Level" + to_string(x) + ".xml";

	doc.LoadFile(pathXML.c_str());
	if (doc.Error()) cout << "Error: " << doc.ErrorStr() << endl;

	else {
		brickTypeElement = doc.FirstChildElement("Level")->FirstChildElement("BrickTypes")->FirstChildElement("BrickType");

		while (brickTypeElement != NULL) {
			if (string(brickTypeElement->Attribute("Id")) != "I") {
				brickInfo[make_pair(brickTypeElement->Attribute("Id"), "HitPoints")] = brickTypeElement->Attribute("HitPoints");
				brickInfo[make_pair(brickTypeElement->Attribute("Id"), "BreakSound")] = brickTypeElement->Attribute("BreakSound");
				brickInfo[make_pair(brickTypeElement->Attribute("Id"), "BreakScore")] = brickTypeElement->Attribute("BreakScore");
			}
			brickTexturePath[brickTypeElement->Attribute("Id")] = brickTypeElement->Attribute("Texture");
			brickInfo[make_pair(brickTypeElement->Attribute("Id"), "HitSound")] = brickTypeElement->Attribute("HitSound");
	
			brickTypeElement = brickTypeElement->NextSiblingElement("BrickType");
		
		}
		rowSpacing = stoi(doc.FirstChildElement("Level")->Attribute("RowSpacing"));
		columnSpacing = stoi(doc.FirstChildElement("Level")->Attribute("ColumnSpacing"));
		backgroundTexturePath = doc.FirstChildElement("Level")->Attribute("BackgroundTexture");
		bricksLayout = doc.FirstChildElement("Level")->FirstChildElement("BrickTypes")->NextSiblingElement("Bricks")->GetText();
	}

}
