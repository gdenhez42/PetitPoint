#include "PetitPoint.h"

PetitPoint::PetitPoint()
: m_x(0),
  m_y(0),
  m_w(30),
  m_h(56),
  m_frame(0)
{
    //ctor
    m_clips[0].x = 16;
    m_clips[0].y = 2;
    m_clips[0].w = m_w;
    m_clips[0].h = m_h;

    m_clips[1].x = 64+16;
    m_clips[1].y = 2;
    m_clips[1].w = m_w;
    m_clips[1].h = m_h;

    m_clips[2].x = 16;
    m_clips[2].y = 64+2;
    m_clips[2].w = m_w;
    m_clips[2].h = m_h;

    m_clips[3].x = 64+16;
    m_clips[3].y = 64+2;
    m_clips[3].w = m_w;
    m_clips[3].h = m_h;
}

PetitPoint::~PetitPoint()
{
    //dtor
}

bool PetitPoint::Load(const LWindow& window)
{
    return m_texture.loadFromFile(window, "Images/Alumette.png");
}
void PetitPoint::Init(int x, int y)
{
    m_x = x; m_y = y;
}
void PetitPoint::Update(bool moved)
{
    if (moved || m_frame != 0)
    {
        m_frame++;
        if (m_frame / 4 >= 4)
        {
            m_frame = 0;
        }
    }
}
void PetitPoint::Render()
{
    m_texture.render(m_x, m_y, &m_clips[m_frame / 4]);
}
