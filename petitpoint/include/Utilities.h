#ifndef UTILITIES_H
#define UTILITIES_H

namespace pp {

    enum Dir { UP, DOWN, LEFT, RIGHT };

    struct Rectangle {
        m_x; m_y; m_w; m_h;
    }

    bool AreColliding(const Rectangle&, const Rectangle&);

}



#endif // UTILITIES_H
