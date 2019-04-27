#include "Utilities.h"

namespace pp {

	bool AreColliding(const Rectangle& p_Rec1, const Rectangle& p_Rec2)
	{
		return !(p_Rec1.m_x + p_Rec1.m_w < p_Rec2.m_x ||
			p_Rec2.m_x + p_Rec2.m_w < p_Rec1.m_x) &&
			!(p_Rec1.m_y + p_Rec1.m_h < p_Rec2.m_y ||
				p_Rec2.m_y + p_Rec2.m_h < p_Rec1.m_y);
	}

}