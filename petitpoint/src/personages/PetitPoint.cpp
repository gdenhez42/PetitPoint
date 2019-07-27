#include "PetitPoint.h"
#include "RessourcesRepo.h"
#include "Tiled.h"
#include "LevelState.h"
#include "LMap.h"

namespace {
    const std::string PETITPOINT_TS = "PetitLore";

    // Animations
    const std::string FRONT_IDLE = "front_idle";
    const std::string FRONT_WALK = "front_walk";
    const std::string BACK_IDLE = "back_idle";
    const std::string BACK_WALK = "back_walk";
    const std::string RIGHT_IDLE = "right_idle";
    const std::string RIGHT_WALK = "right_walk";
    const std::string LEFT_IDLE = "left_idle";
    const std::string LEFT_WALK = "left_walk";
}

namespace pp {

    const int PetitPoint::WALK_SPEED = 3;

    PetitPoint::PetitPoint()
        : Personage(0, 0, ""),
          m_direction(DOWN)
    {
    }

    PetitPoint::~PetitPoint()
    {
        //dtor
    }

    bool PetitPoint::Init(const RessourcesRepo& resources)
    {
        bool success = Personage::Init(resources, PETITPOINT_TS);

        m_currentAnimation = &m_animations[FRONT_IDLE];
        return success;
    }
    void PetitPoint::Update(LevelState& p_rLevelState,
                            const Commands& p_command)
    {
		int toMove = WALK_SPEED;

        const Uint8* keyboardState = p_command.GetKeyboardState();

        if (keyboardState[Commands::WALK_UP]) {
            MovePetitPoint(p_rLevelState, 0, 0-toMove);
            m_direction = 0;
            if (m_currentAnimation != &m_animations[BACK_WALK]) {
                m_currentAnimation = &m_animations[BACK_WALK];
                m_currentAnimation->Reset();
            }
        }
        else if (keyboardState[Commands::WALK_DOWN]) {
            MovePetitPoint(p_rLevelState, 0, toMove);
            m_direction = 1;
            if (m_currentAnimation != &m_animations[FRONT_WALK]) {
                m_currentAnimation = &m_animations[FRONT_WALK];
                m_currentAnimation->Reset();
            }
        }
        else if (keyboardState[Commands::WALK_LEFT]) {
            MovePetitPoint(p_rLevelState, 0-toMove, 0);
            m_direction = LEFT;
            if (m_currentAnimation != &m_animations[LEFT_WALK]) {
                m_currentAnimation = &m_animations[LEFT_WALK];
                m_currentAnimation->Reset();
            }
        }
        else if (keyboardState[Commands::WALK_RIGHT]) {
            MovePetitPoint(p_rLevelState, toMove, 0);
            m_direction = RIGHT;
            if (m_currentAnimation != &m_animations[RIGHT_WALK]) {
                m_currentAnimation = &m_animations[RIGHT_WALK];
                m_currentAnimation->Reset();
            }
        }
        else {
            switch(m_direction) {
            case LEFT:
                m_currentAnimation = &m_animations[LEFT_IDLE];
                break;
            case RIGHT:
                m_currentAnimation = &m_animations[RIGHT_IDLE];
                break;
            case UP:
                m_currentAnimation = &m_animations[BACK_IDLE];
                break;
            case DOWN:
                m_currentAnimation = &m_animations[FRONT_IDLE];
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

            if (currentRoom->isBlocked(test))
            {
				if (dy != 0) {
					int align = currentRoom->AlignH(test);
					if (align != 0) {
						Move(align, dy);
					}
				}
				else {
					p_dx = 0; p_dy = 0;
				}

            }
			else if (p_rLevelState.CheckCollisions(test)) {
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

