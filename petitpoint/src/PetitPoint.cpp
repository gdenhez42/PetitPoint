#include "PetitPoint.h"
#include "RessourcesRepo.h"
#include "Tiled.h"
#include "LevelState.h"
#include "LMap.h"

namespace {
    const std::string PETITPOINT_TS = "Lore";
}

namespace pp {

    const int PetitPoint::WALK_SPEED = 2;
    const int PetitPoint::IMAGE_SIZE = 64;

    PetitPoint::PetitPoint()
        : Personage(HitBox(12,40,40,22), 0, 0, ""),
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
        if (p_KeyboardState[SDL_SCANCODE_UP]) {
            MovePetitPoint(p_rLevelState, UP);
            m_direction = 0;
            if (m_currentAnimation != &m_backWalk) {
                m_currentAnimation = &m_backWalk;
                m_currentAnimation->Reset();
            }
        }
        else if (p_KeyboardState[SDL_SCANCODE_DOWN]) {
            MovePetitPoint(p_rLevelState, DOWN);
            m_direction = 1;
            if (m_currentAnimation != &m_frontWalk) {
                m_currentAnimation = &m_frontWalk;
                m_currentAnimation->Reset();
            }
        }
        else if (p_KeyboardState[SDL_SCANCODE_LEFT]) {
            MovePetitPoint(p_rLevelState, LEFT);
            m_direction = LEFT;
            if (m_currentAnimation != &m_leftWalk) {
                m_currentAnimation = &m_leftWalk;
                m_currentAnimation->Reset();
            }
        }
        else if (p_KeyboardState[SDL_SCANCODE_RIGHT]) {
            MovePetitPoint(p_rLevelState, RIGHT);
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

    /***********************************************************
     There must be a better way to do the walking part :-(
    ***********************************************************/
    void PetitPoint::MovePetitPoint(LevelState& p_rLevelState, Dir d)
    {
        const Personage::HitBox& hb = getGroundHb();
        LMap* currentRoom = p_rLevelState.getCurrentRoom();

        std::string warp;
        bool inWarp = currentRoom->inWarp(getX()+hb.m_x, getY()+hb.m_y, hb.m_w, hb.m_h, warp);
        int dx = 0;
        int dy = 0;
        int toMove = WALK_SPEED;
        bool warped = false;

        while (toMove > 0) {
            int x1, y1, x2, y2, ddx, ddy;

            switch (d) {
            case LEFT:
                x1 = getX()+hb.m_x+dx-1;
                y1 = getY()+hb.m_y;
                x2 = getX()+hb.m_x+dx-1;
                y2 = getY()+hb.m_y+hb.m_h;
                ddx = -1;
                ddy = 0;
                break;
            case RIGHT:
                x1 = getX()+hb.m_x+hb.m_w+dx+1;
                y1 = getY()+hb.m_y;
                x2 = getX()+hb.m_x+hb.m_w+dx+1;
                y2 = getY()+hb.m_y+hb.m_h;
                ddx = 1;
                ddy = 0;
                break;
            case UP:
                x1 = getX()+hb.m_x;
                y1 = getY()+hb.m_y+dy-1;
                x2 = getX()+hb.m_x+hb.m_w;
                y2 = getY()+hb.m_y+dy-1;
                ddx = 0;
                ddy = -1;
                break;
            case DOWN:
                x1 = getX()+hb.m_x;
                y1 = getY()+hb.m_y+hb.m_h+dy+1;
                x2 = getX()+hb.m_x+hb.m_w;
                y2 = getY()+hb.m_y+hb.m_h+dy+1;
                ddx = 0;
                ddy = 1;
                break;
            }

            if (currentRoom->isBlocked(x1, y1) || currentRoom->isBlocked(x2, y2) || p_rLevelState.CheckCollisions(getX() + hb.m_x + dx + ddx, getY() + hb.m_y + dy + ddy, hb.m_w, hb.m_h))
                {
                    toMove = 0;
                }
            else if (!inWarp && (currentRoom->inWarp(x1, y1, hb.m_w, hb.m_h, warp) || currentRoom->inWarp(x2, y2, hb.m_w, hb.m_h, warp)))
                {
                    toMove = 0;
                    warped = true;
                }
            else
                {
                    dx += ddx;
                    dy += ddy;
                    toMove--;
                }
        }

        if (warped) {
            p_rLevelState.Warp(warp);
        } else {
            Move(dx, dy);
        }

    }

}

