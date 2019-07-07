#include "Utilities.h"
#include "RessourcesRepo.h"
#include "Personage.h"
#include "LAnimation.h"
#include "Tiled.h"
#include "LevelState.h"
#include "LMap.h"

namespace {
    const std::string EMPTY = "";
}

namespace pp {

    Personage::Personage(int x, int y, const std::string& room)
        : m_room(room),
          m_x(x),
          m_y(y),
          m_groundHb(0,0,0,0),
          m_animations(),
          m_currentAnimation(nullptr)
    {
        //ctor
    }

    Personage::~Personage()
    {
        //dtor
    }

    bool Personage::Init(const RessourcesRepo& p_RessourceRepo,
                         const std::string& p_tilesetName)
    {
        bool success = true;
        tiled::TileSet tileset = p_RessourceRepo.getTileSet(p_tilesetName);
        m_width = tileset.m_width;
        m_height = tileset.m_height;


        std::string resourceName = RessourcesRepo::getRessourceName(tileset.m_image);
        const LTexture& image = p_RessourceRepo.getImage(resourceName);

        std::map<int, pp::tiled::Tile>::const_iterator it, end = tileset.m_tiles.end();

        bool foundGroundHb = false;
        for (it = tileset.m_tiles.begin(); it != end; ++it) {
            if (!foundGroundHb &&
                !it->second.m_objects.m_objects.empty()) {
                const pp::tiled::Object& obj = it->second.m_objects.m_objects[0];
                m_groundHb = Rectangle(obj.m_x, obj.m_y, obj.m_w, obj.m_h);
                foundGroundHb = true;
            }
            InitAnimation(tileset, it->second, image);
        }

        if (m_animations.empty()) {
            Log("No animations found for personnage: " + p_tilesetName);
            success = false;
        }

        return success;
    }

    void Personage::InitAnimation(const pp::tiled::TileSet& p_tileSet,
                                  const pp::tiled::Tile& p_tile,
                                  const pp::LTexture& p_image)
    {
        const pp::tiled::Animation tanim = p_tile.m_animation;
        if (p_tile.m_animation.m_frames.empty()) return;

        LAnimation animation;
        std::vector<pp::tiled::Frame>::const_iterator it, end = tanim.m_frames.end();
        for (it = tanim.m_frames.begin(); it != end; ++it) {
            pp::tiled::Frame frame = *it;
            SDL_Rect rect = {(frame.m_tileId % p_tileSet.m_columns)*p_tileSet.m_width,
                             (frame.m_tileId / p_tileSet.m_columns)*p_tileSet.m_height,
                             p_tileSet.m_width,
                             p_tileSet.m_height};
            animation.AddFrame(LAnimation::Frame(&p_image, rect));
        }
        std::string animName = p_tile.m_type;
        if (animName == EMPTY) {
            animName = std::to_string(p_tile.m_id);
        }

        m_animations[animName] = animation;
    }

    void Personage::Render(const LevelState& p_LevelState)
    {
        const LMap* currentRoom = p_LevelState.getCurrentRoom();
        if (m_room == currentRoom->getName()) {
            int x = currentRoom->getX() + m_x;
            int y = currentRoom->getY() + m_y;
            m_currentAnimation->Render(x, y);
        }

    }

	Rectangle Personage::getGroundHb() const {
		return Rectangle(getX() + m_groundHb.m_x, getY() + m_groundHb.m_y, m_groundHb.m_w, m_groundHb.m_h);
	}

}
