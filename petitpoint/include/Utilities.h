#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace pp {

    enum Dir { UP, DOWN, LEFT, RIGHT };

    struct Rectangle {
        int m_x;
		int m_y;
		int m_w;
		int m_h;
		Rectangle(int x, int y, int w, int h) : m_x(x), m_y(y), m_w(w), m_h(h) {}
	};

    struct LoadZone {
        Rectangle m_rect;
        std::string m_room;
        LoadZone() : m_rect(0,0,0,0), m_room() {}
    };

    struct WarpZone {
        Rectangle m_rect;
        LoadZone m_load;
        WarpZone() : m_rect(0,0,0,0), m_load() {}
    };

    bool AreColliding(const Rectangle&, const Rectangle&);

    void Log(const std::string& p_string);
}

#endif // UTILITIES_H
