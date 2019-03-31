#include "PetitPoint.h"
#include "RessourcesRepo.h"
#include "Tiled.h"
#include "LevelState.h"
#include "LMap.h"

namespace {
    const std::string PETITPOINT_TS = "Lore";
}

namespace pp {

    const int PetitPoint::WALK_SPEED = 3;
    const int PetitPoint::IMAGE_SIZE = 64;

    PetitPoint::PetitPoint()
        : Personage(Rectangle(12,40,40,22), 0, 0, ""),
          m_direction(DOWN),
          m_frontIdle(),
          m_frontWalk(),
          m_backIdle(),
          m_backWalk(),
          m_leftIdle(),
          m_leftWalk(),
          m_rightIdle(),
          m_rightWalk()
    {
    }

    PetitPoint::~PetitPoint()
    {
        //dtor
    }

    void PetitPoint::Init(const RessourcesRepo& resources)
    {
        // Init the animations
        m_tileset = &resources.getTileSet(PETITPOINT_TS);
        m_frontIdle = m_tileset->getAnimation(0);
        m_frontWalk = m_tileset->getAnimation(2);
        m_backIdle = m_tileset->getAnimation(4);
        m_backWalk = m_tileset->getAnimation(6);
        m_rightIdle = m_tileset->getAnimation(8);
        m_rightWalk = m_tileset->getAnimation(10);
        m_leftIdle = m_tileset->getAnimation(12);
        m_leftWalk = m_tileset->getAnimation(14);
        m_currentAnimation = &m_frontIdle;

    }
    void PetitPoint::Update(LevelState& p_rLevelState,
                            const Uint8* p_KeyboardState)
    {
		int toMove = WALK_SPEED;

        if (p_KeyboardState[SDL_SCANCODE_UP]) {
            MovePetitPoint(p_rLevelState, 0, 0-toMove);
            m_direction = 0;
            if (m_currentAnimation != &m_backWalk) {
                m_currentAnimation = &m_backWalk;
                m_currentAnimation->Reset();
            }
        }
        else if (p_KeyboardState[SDL_SCANCODE_DOWN]) {
            MovePetitPoint(p_rLevelState, 0, toMove);
            m_direction = 1;
            if (m_currentAnimation != &m_frontWalk) {
                m_currentAnimation = &m_frontWalk;
                m_currentAnimation->Reset();
            }
        }
        else if (p_KeyboardState[SDL_SCANCODE_LEFT]) {
            MovePetitPoint(p_rLevelState, 0-toMove, 0);
            m_direction = LEFT;
            if (m_currentAnimation != &m_leftWalk) {
                m_currentAnimation = &m_leftWalk;
                m_currentAnimation->Reset();
            }
        }
        else if (p_KeyboardState[SDL_SCANCODE_RIGHT]) {
            MovePetitPoint(p_rLevelState, toMove, 0);
            m_direction = RIGHT;
            if (m_currentAnimation != &m_rightWalk) {
                m_currentAnimation = &m_rightWalk;
                m_currentAnimation->Reset();
            }
        }
        else {
            switch(m_direction) {
            case LEFT:
                m_currentAnimation = &m_leftIdle;
                break;
            case RIGHT:
                m_currentAnimation = &m_rightIdle;
                break;
            case UP:
                m_currentAnimation = &m_backIdle;
                break;
            case DOWN:
                m_currentAnimation = &m_frontIdle;
                break;
                }
        }
    }

    void PetitPoint::MovePetitPoint(LevelState& p_rLevelState, int p_dx, int p_dy)
    {
        Rectangle hb = getGroundHb();
        LMap* currentRoom = p_rLevelState.getCurrentRoom();

        std::string warp;
        bool inWarp = currentRoom->inWarp(hb, warp);
        
        while (p_dx != 0 || p_dy != 0) {
			hb = getGroundHb();
			int dx = 0;
			int dy = 0;
			
			if (p_dx < 0) {
				p_dx++;
				dx--;
			}
			if (p_dx > 0) {
				p_dx--;
				dx++;
			}
			if (p_dy < 0) {
				p_dy++;
				dy--;
			}
			if (p_dy > 0) {
				p_dy--;
				dy++;
			}

			Rectangle test(hb.m_x + dx, hb.m_y + dy, hb.m_w, hb.m_h);

            if (currentRoom->isBlocked(test) || p_rLevelState.CheckCollisions(test))
            {
				p_dx = 0; p_dy = 0;
            }
            else if (!inWarp && currentRoom->inWarp(test, warp))
            {
				p_dx = 0; p_dy = 0;
				p_rLevelState.Warp(warp);
            }
            else
            {
				Move(dx, dy);
            }
        }
    }

}

