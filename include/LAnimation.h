#ifndef LANIMATION_H
#define LANIMATION_H

#include "LTexture.h"
#include <vector>

namespace pp {

class LAnimation
{
    public:
        struct Frame {
            const LTexture* m_texture;
            SDL_Rect m_rect;
            Frame(const LTexture* txt, const SDL_Rect& rect) : m_texture(txt), m_rect(rect) {}
        };

        LAnimation();
        void AddFrame(const Frame&);
        void Render(int x, int y) const;
        void Reset() const { m_currentFrame = 0; }

    private:
        std::vector<Frame> m_frames;
        mutable int m_currentFrame;
        int m_totalFrames;
};

}

#endif // LANIMATION_H
