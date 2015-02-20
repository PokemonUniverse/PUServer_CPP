#ifndef INC_SENDTILESMESSAGE_H_
#define INC_SENDTILESMESSAGE_H_

#include "definitions.h"
#include "netmessage.h"

#include "tile.h"
#include "game.h"
#include "towns.h"

extern Game g_game;

class SendTilesMessage : public NetMessage
{
	tileList_t m_tiles;

public:
	SendTilesMessage() : NetMessage(HEADER_SENDTILES) {};
	~SendTilesMessage() {};

	void addTile(int32_t x, int32_t y, int32_t z) {
		Tile* tile = g_game.getTile(x, y, z);
		if(tile != NULL) {
			m_tiles.push_back(tile);
		}
	}
	void addTiles(tileList_t tiles) { m_tiles = tiles; };

	virtual void completePacket(Packet& packet)
	{
		packet.AddInt(m_tiles.size());
		if(m_tiles.size() <= 0) return;

		BOOST_FOREACH(Tile* tile, m_tiles)
		{
			if(!tile) continue;

			Position position = tile->getPosition();
			packet.AddInt(position.x);
			packet.AddInt(position.y);
			//packet.AddInt(tile->getBlockingVolume());

			LayerMap tile_layers = tile->getLayers();
			packet.AddInt(tile_layers.size());

			TileLayer* layer = NULL;
			BOOST_FOREACH(LayerMap::value_type layerIt, tile_layers)
			{
				if((layer = layerIt.second) == NULL) continue;

				packet.AddInt(layerIt.first);
				packet.AddLong(layer->getTileID());
				packet.AddInt(tile->getBlockingVolume());
			}

			/*
			Town* town = tile->getTown();
						if(town != NULL) {
							packet.AddInt(town->getID());
							packet.AddString(town->getName());
						} else {*/
			
				packet.AddInt(0);
				packet.AddString("Unknown");
			//}
		}

		m_tiles.clear();
	};
};

#endif