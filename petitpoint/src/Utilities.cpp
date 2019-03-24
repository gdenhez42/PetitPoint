#include "Utilities.h"

bool AreColliding(const Rectangle& p_Rec1, const Rectangle& p_Rec2)
{
    return !(p_Rec1.m_x + p_Rec1.m_w < p_Rec2.m_x ||
             p_Rec2.m_x + p_Rec2.m_w < p_Rec1.m_x) &&
           !()
}
