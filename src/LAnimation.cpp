#include "LAnimation.h"

namespace pp {

LAnimation::LAnimation()
: m_frames(),
  m_currentFrame(0),
  m_totalFrames(0)
{
}

void LAnimation::AddFrame(const Frame& frame) {
    m_frames.push_back(frame);
    m_totalFrames += 4;
}

void LAnimation::Render(int x, int y) const
{
    const Frame& frame = m_frames[m_currentFrame/4];
    frame.m_texture->render(x, y, &frame.m_rect);
    m_currentFrame = (m_currentFrame + 1) % m_totalFrames;
}

}
