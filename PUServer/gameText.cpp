#include "gametext.h"
#include "filesystem.h"
#include "configmanager.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern ConfigManager g_config;

bool GameText::loadText( std::string xmlfile )
{
	std::string dataFile = g_config.pString("dataFolder") + "/" + xmlfile;

	if(!FileSystem::FileExists(dataFile)) {	
		return false;
	}

	xmlDocPtr doc = xmlParseFile(dataFile.c_str());
	if(doc)
	{
		xmlNodePtr root, search;
		root = xmlDocGetRootElement(doc);
		std::string rootstr = (char*)root->name; 

		if(rootstr == "gametext")
		{
			search = root->children;
			while(search = search->next)
			{
				std::string searchStr = (char*)search->name;
				if(searchStr == "sentence")
				{
					int sentence_id = atoi((const char*)xmlGetProp(search, (const xmlChar *) "id"));
					TextPieceList sentencePieces;

					xmlNodePtr pieces = search->children;
					while(pieces = pieces->next)
					{
						std::string pieceStr = (char*)pieces->name;

						if(pieceStr == "part")
						{
							int color, var;
							std::string text;

							color = atoi((const char*)xmlGetProp(pieces, (const xmlChar *) "color"));
							var = atoi((const char*)xmlGetProp(pieces, (const xmlChar *) "var"));

							if(var == 0) {
								text = (char*)xmlGetProp(pieces, (const xmlChar *)"text");
							}

							sentencePieces.push_back(TextPiece((Colors)color, var, text));
						}
					}

					addText(sentence_id, sentencePieces);
				}
			}
		}
	}

	return true;
}

TextPieceList GameText::getText( int textId )
{
	TextList::iterator find = textArray.find(textId);
	if(find != textArray.end())
	{
		return find->second;
	}

	std::vector<TextPiece> tmp;
	return tmp;
}

void GameText::addText( int textId, TextPieceList pieces )
{
	TextList::iterator find = textArray.find(textId);
	if(find != textArray.end()) {	
		find->second = pieces;
	}
	else {
		textArray.insert(std::make_pair(textId, pieces));
	}
}