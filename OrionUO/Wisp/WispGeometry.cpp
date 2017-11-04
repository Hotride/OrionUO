// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_GEOMETRY
{
//----------------------------------------------------------------------------------
CSize::CSize()
{
}
//----------------------------------------------------------------------------------
CSize::CSize(const int &width, const int &height)
: m_Width(width), m_Height(height)
{
}
//----------------------------------------------------------------------------------
CSize::CSize(const CSize &size)
: m_Width(size.Width), m_Height(size.Height)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di()
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(const int &x, const int &y)
: m_X(x), m_Y(y)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(const float &x, const float &y)
: m_X((int)x), m_Y((int)y)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(const CPoint2Di &point)
: m_X(point.X), m_Y(point.Y)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(const CPoint2Df &point)
: m_X((int)point.X), m_Y((int)point.Y)
{
}
//----------------------------------------------------------------------------------
CRect::CRect()
{
}
//----------------------------------------------------------------------------------
CRect::CRect(const int &x, const int &y, const int &width, const int &height)
: m_Position(x, y), m_Size(width, height)
{
}
//----------------------------------------------------------------------------------
CRect::CRect(const int &x, const int &y, const CSize &size)
: m_Position(x, y), m_Size(size)
{
}
//----------------------------------------------------------------------------------
CRect::CRect(const CPoint2Di &position, const int &width, const int &height)
: m_Position(position), m_Size(width, height)
{
}
//----------------------------------------------------------------------------------
CRect::CRect(const CPoint2Di &position, const CSize &size)
: m_Position(position), m_Size(size)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di()
: CPoint2Di()
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const int &x, const int &y, const int &z)
: CPoint2Di(x, y), m_Z(z)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const float &x, const float &y, const float &z)
: CPoint2Di(x, y), m_Z((int)z)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint2Di &point)
: CPoint2Di(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint3Di &point)
: CPoint2Di(point), m_Z(point.Z)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint2Df &point)
: CPoint2Di(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint3Df &point)
: CPoint2Di(point), m_Z((int)point.Z)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df()
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(const int &x, const int &y)
: m_X((float)x), m_Y((float)y)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(const float &x, const float &y)
: m_X(x), m_Y(y)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(const CPoint2Di &point)
: m_X((float)point.X), m_Y((float)point.Y)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(const CPoint2Df &point)
: m_X(point.X), m_Y(point.Y)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df()
: CPoint2Df()
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const int &x, const int &y, const int &z)
: CPoint2Df(x, y), m_Z((float)z)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const float &x, const float &y, const float &z)
: CPoint2Df(x, y), m_Z(z)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint2Di &point)
: CPoint2Df(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint3Di &point)
: CPoint2Df(point), m_Z((float)point.Z)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint2Df &point)
: CPoint2Df(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint3Df &point)
: CPoint2Df(point), m_Z(point.Z)
{
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
