/*
...
*/
#pragma once

#include "json/json.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include "glm/glm.hpp"
#include "util/tileID.h"
#include "platform.h"

//Todo: Impelement TileData, a generic datastore for all tile formats,
//Have an instance of this in DataSource
//Every implementation of a DataSource will fill this TileData instance.
//Example MapzenVectorTile will read the json and fill this TileData
class TileData {
};


// TODO: divide DataSource into network and non-network dataSources.
//       Same has been done on the webgl tangram. Follow the same pattern.
class DataSource {
protected:
    // map of tileIDs to json data for that tile
    std::map< TileID, std::shared_ptr<Json::Value> > m_JsonRoots;

    /* m_urlTemplate needs to be defined for every network dataSource */
    std::string m_urlTemplate;

public:
    /*
     * Does all the curl network calls to load the tile data and fills the data associated with a tileID
     */
    virtual bool LoadTile(const std::vector<TileID>& _tileCoords) = 0;

    /* Returns the data corresponding to a tileID */
    virtual std::shared_ptr<Json::Value> GetData(const TileID& _tileID) = 0;

    /* Checks if data exists for a specific tileID */
    virtual bool CheckDataExists(const TileID& _tileID) = 0;

    /* 
     * constructs the URL for a tile based on tile coordinates/IDs.
     * Used by LoadTile to construct URL
     */
    virtual std::unique_ptr<std::string> constructURL(const TileID& _tileCoord) = 0;

    /* 
     * extracts tileIDs from a url
     * Used by LoadTile to extract tileIDs from curl url.
     */
    virtual TileID extractIDFromUrl(const std::string& _url) = 0;
    
    /* 
     * clears all data associated with this dataSource
     */
    void ClearGeoRoots();

    /*
     * returns the number of tiles having data wrt this datasource
     */
    size_t JsonRootSize();

    DataSource() {}
    virtual ~DataSource() {
        m_JsonRoots.clear();
    }
};

//Extends DataSource class to read MapzenVectorTileJsons.
class MapzenVectorTileJson: public DataSource {

public:
    MapzenVectorTileJson();
    virtual std::unique_ptr<std::string> constructURL(const TileID& _tileCoord) override;
    virtual TileID extractIDFromUrl(const std::string& _url) override;
    virtual bool LoadTile(const std::vector<TileID>& _tileCoords) override;
    virtual std::shared_ptr<Json::Value> GetData(const TileID& _tileID) override;
    virtual bool CheckDataExists(const TileID& _tileID) override;
    virtual ~MapzenVectorTileJson() {}
};
