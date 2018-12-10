#ifndef LFONT_H
#define LFONT_H

#include <SDL2/SDL_ttf.h>
#include <string>

/*********************************************
Wrapper autour du TTF_Font de SDL pour
etre exception safe.
*********************************************/
class LFont
{
    public:
        LFont();
        virtual ~LFont();

        void Free();
        bool Load(const std::string& p_path, int p_size);
        TTF_Font* Font() const
        { return m_pFont; }

    private:
        TTF_Font *m_pFont;
};

#endif // LFONT_H
