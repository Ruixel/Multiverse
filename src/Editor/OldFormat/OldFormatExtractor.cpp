#include "OldFormatExtractor.h"
#include "OldFormatUtil.h"

#include <regex>
#include <iostream>

namespace RegexExtractor
{
    Property::Colour stringToColour(const std::string& colourString)
    {
        size_t valueEnd;
        int pointer = 7;
        auto getComponent = [&](sf::Uint8& comp)
        {
            valueEnd = colourString.find(",", pointer);
            comp = (sf::Uint8)std::stoi(colourString.substr(pointer, valueEnd - pointer));
            pointer = valueEnd + 2;
        };

        Property::Colour colour;
        getComponent(colour.r);
        getComponent(colour.g);
        getComponent(colour.b);
        return colour;
    }

    Property::Position extractVector(const std::smatch& match, int firstIndex)
    {
        Property::Position vector;
        //Extract x
        vector.x = (u16)std::stoi(match[firstIndex]);

        //Extract y
        vector.y = (u16)std::stoi(match[firstIndex + 1]);

        return vector;
    }

    Property::Material extractMaterial(const std::smatch& match, int firstIndex, TextureType type)
    {
        Property::Material material;

        if (match[firstIndex] != "") { //Colour
			material.textureId = TextureID::Color;
            material.colour = stringToColour(match[firstIndex].str());
        }
        else {  //Texture
			TextureID tex_id;
			if (type == TextureType::WallTex)
				tex_id = WorldTextures::getWallTexture(std::stoi(match[firstIndex + 1].str()));
			else 
				tex_id = WorldTextures::getPlatTexture(std::stoi(match[firstIndex + 1].str()));

            material.textureId = tex_id;
            material.colour = RegexExtractor::WHITE;
        }
        return material;
    }

	u8 extractIntAsByte(const std::smatch& match, int firstIndex)
	{
		return (u8)std::stoi(match[firstIndex].str());
	}
}

namespace OldFormat {
    LevelHeader extractHeader(std::string &levelData) {
        LevelHeader header;
        std::smatch matchGroups;
        std::regex headerRegex("#name: \"([^\"]+)\", #levels: (\\d+), #version: ([^,]+), #creator: \"([^\"]+)\"");

        if (std::regex_search(levelData, matchGroups, headerRegex)) {
            header.gameName = matchGroups[1].str();
            header.floorCount = (u8) std::stoi(matchGroups[2].str());
            header.gameAuthor = matchGroups[4].str();
        } else {
            std::cout << "Cannot match header\n";
            return {};
        }
        return header;
    }

    std::shared_ptr<CYPlat> createPlatformFromRegex(std::smatch & matchGroups) {
        Property::Position position;
        Property::Material material;

        u8 floor;
        u8 height;
        u8 size;

        position = RegexExtractor::extractVector(matchGroups, 1);
        size = (u8) std::stoi(matchGroups[3].str());

        // If group 4 and 5 are empty then it's
        // [X, Y, [Size], Level] (Very old format)
        if (matchGroups[4] == "" && matchGroups[5] == "") {
            // Default
            material.textureId = TextureID::Wood;
            material.colour = sf::Color(255, 255, 255);
        } else {
            material = RegexExtractor::extractMaterial(matchGroups, 4, RegexExtractor::TextureType::PlatTex);

            // Check if there's a height element
            if (matchGroups[6] != "") {
                height = (u8) std::stoi(matchGroups[6].str());
            }
        }

        // Level
        floor = (u8) std::stoi(matchGroups[7].str());

        return std::make_shared<CYPlat>(position, material, floor, height, size);
    }

    std::shared_ptr<CYWall> createWallFromRegex(std::smatch & matchGroups) {
        Property::Position startPosition;
        Property::Position endPosition;
        Property::Material frontMaterial;
        Property::Material backMaterial;
        u8 floor;
        u8 height;

        auto displacement = RegexExtractor::extractVector(matchGroups, 1);
        startPosition = RegexExtractor::extractVector(matchGroups, 3);
        endPosition = {displacement.x + startPosition.x,
                       displacement.y + startPosition.y};

        //Texture
        frontMaterial = RegexExtractor::extractMaterial(matchGroups, 5, RegexExtractor::TextureType::WallTex);
        backMaterial = RegexExtractor::extractMaterial(matchGroups, 7, RegexExtractor::TextureType::WallTex);

        //Height and floor
        if (matchGroups[10] != "") {
            height = (u8) std::stoi(matchGroups[9].str());
            floor = (u8) std::stoi(matchGroups[10].str());
        } else {//Old versions have no wall height
            height = 1;
            floor = (u8) std::stoi(matchGroups[9].str());
        }

        return std::make_shared<CYWall>(startPosition, endPosition, frontMaterial,
                backMaterial, floor, height);
    }

    std::vector<std::shared_ptr<CYGeneric>> extractGeometry(std::unordered_map<std::string, std::string> &table) {
        std::vector<std::shared_ptr<CYGeneric>> objs;
        std::smatch matchGroups;

        // Walls (convert to lambda)
        std::string wallData = table["walls"];
        std::regex wallRegex(
                "\\[(-?\\d+), (-?\\d+), (-?\\d+), (-?\\d+), \\[(c[^\\)]*\\))?(\\d+)?, (c[^\\)]*\\))?(\\d+)?\\]?, (\\d+)\\],? ?(\\d+)?");
        while (std::regex_search(wallData, matchGroups, wallRegex)) {
            std::shared_ptr<CYWall> wall = OldFormat::createWallFromRegex(matchGroups);
            objs.emplace_back(std::move(wall));

            wallData = matchGroups.suffix();
        }

        // Plats (convert to lambda)
        std::string platData = table["Plat"];
        std::regex platRegex(
                "\\[\\[([\\d\\.]+), ([\\d\\.]+)\\], \\[(\\d+),? ?(c[^\\)]*\\))?(\\d+)?,? ?(\\d+)?\\], (\\d+)");
        while (std::regex_search(platData, matchGroups, platRegex)) {
            std::shared_ptr<CYPlat> plat = OldFormat::createPlatformFromRegex(matchGroups);
            objs.emplace_back(std::move(plat));

            platData = matchGroups.suffix();
        }

        return objs;
    }
}
