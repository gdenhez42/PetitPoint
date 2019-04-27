#ifndef UTILITIES_H
#define UTILITIES_H

namespace pp {

    enum Dir { UP, DOWN, LEFT, RIGHT };

    struct Rectangle {
        int m_x;
		int m_y;
		int m_w;
		int m_h;
		Rectangle(int x, int y, int w, int h) : m_x(x), m_y(y), m_w(w), m_h(h) {}
	};

    bool AreColliding(const Rectangle&, const Rectangle&);

}

#endif // UTILITIES_H
