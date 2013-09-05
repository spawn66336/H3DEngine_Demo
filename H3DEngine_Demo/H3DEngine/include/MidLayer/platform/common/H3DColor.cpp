#include "H3DColor.h"
#include <string>

H3DColor::H3DColor(float red, float green, float blue, float alpha)
{
    setRGBA(red, green, blue, alpha);
}

H3DColor::~H3DColor()
{
}

H3DColor::H3DColor(const float* color)
{
    memcpy(m_Color, color, COLOR_SIZE * sizeof(float));
}

H3DColor::H3DColor(const H3DColor& color)
{
    memcpy(m_Color, color.m_Color, COLOR_SIZE * sizeof(float));
}

H3DColor& H3DColor::operator=(const float* color)
{
    memcpy(m_Color, color, COLOR_SIZE * sizeof(float));
    return *this;
}

H3DColor& H3DColor::operator=(const H3DColor& color)
{
    memcpy(m_Color, color.m_Color, COLOR_SIZE * sizeof(float));
    return *this;
}

H3DColor H3DColor::operator*(const float a) const
{
    return H3DColor(m_Color[0] * a, m_Color[1] * a, m_Color[2] * a, m_Color[3] * a);
}

H3DColor H3DColor::operator/(const float a) const
{
    return H3DColor(m_Color[0] / a, m_Color[1] / a, m_Color[2] / a, m_Color[3] / a);
}

H3DColor H3DColor::operator+(const H3DColor& color) const
{
    return H3DColor(m_Color[0] + color.m_Color[0], m_Color[1] + color.m_Color[1], 
        m_Color[2] + color.m_Color[2], m_Color[3] + color.m_Color[3]);
}

H3DColor H3DColor::operator-(const H3DColor& color) const
{
    return H3DColor(m_Color[0] - color.m_Color[0], m_Color[1] - color.m_Color[1], 
        m_Color[2] - color.m_Color[2], m_Color[3] - color.m_Color[3]);
}
H3DColor& H3DColor::operator+=(const H3DColor& color)
{
    for (int i = 0; i < COLOR_SIZE; ++i)
    {
        m_Color[i] += color.m_Color[i];
    }
    return *this;
}

H3DColor& H3DColor::operator-=(const H3DColor& color)
{
    for (int i = 0; i < COLOR_SIZE; ++i)
    {
        m_Color[i] -= color.m_Color[i];
    }
    return *this;
}

H3DColor& H3DColor::operator/=(const float a)
{
    for (int i = 0; i < COLOR_SIZE; ++i)
    {
        m_Color[i] /= a;
    }
    return *this;
}
H3DColor& H3DColor::operator*=(const float a)
{

    for (int i = 0; i < COLOR_SIZE; ++i)
    {
        m_Color[i] *= a;
    }
    return *this;
}

void H3DColor::setRGBA(float red, float green, float blue, float alpha)
{
    m_Color[0] = red;
    m_Color[1] = green;
    m_Color[2] = blue;
    m_Color[3] = alpha;
}