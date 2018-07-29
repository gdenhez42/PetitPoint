#include "PetitPoint.h"
#include "RessourcesRepo.h"

namespace {

    const int IMAGE_SIZE = 64;
    enum Directions {
        DOWN,UP,LEFT,RIGHT
    };

}

const int PetitPoint::WALK_SPEED = 2;

PetitPoint::PetitPoint()
: m_x(0),
  m_y(0),
  m_offsetX(12),
  m_offsetY(5),
  m_w(40),
  m_h(57),
  m_frame(0),
  m_direction(DOWN),
  m_moved(false)
{
    m_frontIdle =    {0,            0,            IMAGE_SIZE, IMAGE_SIZE};
    m_frontWalk[0] = {IMAGE_SIZE,   0,            IMAGE_SIZE, IMAGE_SIZE};
    m_frontWalk[1] = {IMAGE_SIZE*2, 0,            IMAGE_SIZE, IMAGE_SIZE};
    m_backIdle =     {0,            IMAGE_SIZE,   IMAGE_SIZE, IMAGE_SIZE};
    m_backWalk[0] =  {IMAGE_SIZE,   IMAGE_SIZE,   IMAGE_SIZE, IMAGE_SIZE};
    m_backWalk[1] =  {IMAGE_SIZE*2, IMAGE_SIZE,   IMAGE_SIZE, IMAGE_SIZE};
    m_rightIdle =    {0,            IMAGE_SIZE*2, IMAGE_SIZE, IMAGE_SIZE};
    m_rightWalk[0] = {IMAGE_SIZE,   IMAGE_SIZE*2, IMAGE_SIZE, IMAGE_SIZE};
    m_rightWalk[1] = {IMAGE_SIZE*2, IMAGE_SIZE*2, IMAGE_SIZE, IMAGE_SIZE};
    m_leftIdle =     {0,            IMAGE_SIZE*3, IMAGE_SIZE, IMAGE_SIZE};
    m_leftWalk[0] =  {IMAGE_SIZE,   IMAGE_SIZE*3, IMAGE_SIZE, IMAGE_SIZE};
    m_leftWalk[1] =  {IMAGE_SIZE*2, IMAGE_SIZE*3, IMAGE_SIZE, IMAGE_SIZE};
}

PetitPoint::~PetitPoint()
{
    //dtor
}

void PetitPoint::Init(const RessourcesRepo& resources, int x, int y)
{
    m_ptexture = resources.getPetitPoint();
    m_x = x; m_y = y;
}
void PetitPoint::Update(Command::Command command)
{
    switch (command)
    {
        case Command::NONE:
            if (m_frame == 0) m_moved = false;
            else
            {
                m_frame++;
                if (m_frame >= 8) m_frame = 0;
            }
            break;
        case Command::MOVE_LEFT:
            m_direction = LEFT;
            if (!m_moved)
            {
                m_moved = true;
                m_frame = 0;
            }
            m_frame++;
            if (m_frame >= 8) m_frame = 0;
            break;
        case Command::MOVE_RIGHT:
            m_direction = RIGHT;
            if (!m_moved)
            {
                m_moved = true;
                m_frame = 0;
            }
            m_frame++;
            if (m_frame >= 8) m_frame = 0;
            break;
        case Command::MOVE_UP:
            m_direction = UP;
            if (!m_moved)
            {
                m_moved = true;
                m_frame = 0;
            }
            m_frame++;
            if (m_frame >= 8) m_frame = 0;
            break;
        case Command::MOVE_DOWN:
            m_direction = DOWN;
            if (!m_moved)
            {
                m_moved = true;
                m_frame = 0;
            }
            m_frame++;
            if (m_frame >= 8) m_frame = 0;
            break;
    }
}
void PetitPoint::Render()
{

    switch (m_direction)
    {
    case DOWN:
        if (m_moved)
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_frontWalk[m_frame/4]);
        }
        else
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_frontIdle);
        }
        break;
    case UP:
        if (m_moved)
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_backWalk[m_frame/4]);
        }
        else
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_backIdle);
        }
        break;
    case LEFT:
        if (m_moved)
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_leftWalk[m_frame/4]);
        }
        else
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_leftIdle);
        }
        break;
    case RIGHT:
        if (m_moved)
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_rightWalk[m_frame/4]);
        }
        else
        {
            m_ptexture->render(m_x-m_offsetX, m_y-m_offsetY, &m_rightIdle);
        }
        break;
    }
}

