#include "PetitPoint.h"

PetitPoint::PetitPoint()
: m_x(0),
  m_y(0),
  m_w(30),
  m_h(56)
{
    //ctor
}

PetitPoint::~PetitPoint()
{
    //dtor
}

bool PetitPoint::Load(SDL_Renderer* p_pRenderer)
{
    return m_texture.loadFromFile(p_pRenderer, "Images/Alumette.png");
}
void PetitPoint::Init(int x, int y)
{
    Update(x, y);
}
void PetitPoint::Update(int x, int y)
{
    m_x = x; m_y = y;
}
void PetitPoint::Render()
{
    SDL_Rect clip = {16, 2, m_w, m_h};
    m_texture.render(m_x, m_y, &clip);
}
