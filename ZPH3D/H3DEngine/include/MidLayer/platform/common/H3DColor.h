/**
* @file H3DColor.h 
* 
* 定义了H3DColor类
* 
* @author 叶向宇
* 
* Copyright (c) 2009 by H3D
*/

#ifndef __H3D_COLOR_H
#define __H3D_COLOR_H

/**
* \brief
* H3DCamera
* 
* H3DColor表示颜色, 包括rgba四个值, 各个值范围在0-1
*/
class H3DColor
{
public:
    H3DColor(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
    H3DColor(const float* color);
    H3DColor(const H3DColor& color);
    ~H3DColor();
    H3DColor& operator=(const float* color);
    H3DColor& operator=(const H3DColor& color);
    H3DColor operator*(const float a) const;
    H3DColor operator/(const float a) const;
    H3DColor& operator/=(const float a);
    H3DColor& operator*=(const float a);
    H3DColor operator+(const H3DColor& color) const;
    H3DColor operator-(const H3DColor& color) const;
    H3DColor& operator+=(const H3DColor& color);
    H3DColor& operator-=(const H3DColor& color);


    const float* getColor() const { return m_Color; }

    float getRed() const { return m_Color[0]; }
    void setRed(float red) { m_Color[0] = red; }

    float getGreen() const { return m_Color[1]; }
    void setGreen(float green) { m_Color[1] = green; }

    float getBlue() const { return m_Color[2]; }
    void setBlue(float blue) { m_Color[2] = blue; }

    float getAlpha() const { return m_Color[3]; }
    void setAlpha(float alpha) { m_Color[3] = alpha; }

    void setRGBA(float red, float green, float blue, float alpha = 1.0f);
private:
    enum { COLOR_SIZE = 4 };
    float m_Color[COLOR_SIZE];
};

#endif