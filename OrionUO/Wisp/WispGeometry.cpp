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
CSize::CSize(int width, int height)
: Width(width), Height(height)
{
}
//----------------------------------------------------------------------------------
CSize::CSize(const CSize &size)
: Width(size.Width), Height(size.Height)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di()
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(int x, int y)
: X(x), Y(y)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(float x, float y)
: X((int)x), Y((int)y)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(const CPoint2Di &point)
: X(point.X), Y(point.Y)
{
}
//----------------------------------------------------------------------------------
CPoint2Di::CPoint2Di(const CPoint2Df &point)
: X((int)point.X), Y((int)point.Y)
{
}
//----------------------------------------------------------------------------------
CRect::CRect()
{
}
//----------------------------------------------------------------------------------
CRect::CRect(int x, int y, int width, int height)
: Position(x, y), Size(width, height)
{
}
//----------------------------------------------------------------------------------
CRect::CRect(int x, int y, const CSize &size)
: Position(x, y), Size(size)
{
}
//----------------------------------------------------------------------------------
CRect::CRect(const CPoint2Di &position, int width, int height)
: Position(position), Size(width, height)
{
}
//----------------------------------------------------------------------------------
CRect::CRect(const CPoint2Di &position, const CSize &size)
: Position(position), Size(size)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di()
: CPoint2Di()
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(int x, int y, int z)
: CPoint2Di(x, y), Z(z)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(float x, float y, float z)
: CPoint2Di(x, y), Z((int)z)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint2Di &point)
: CPoint2Di(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint3Di &point)
: CPoint2Di(point), Z(point.Z)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint2Df &point)
: CPoint2Di(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Di::CPoint3Di(const CPoint3Df &point)
: CPoint2Di(point), Z((int)point.Z)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df()
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(int x, int y)
: X((float)x), Y((float)y)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(float x, float y)
: X(x), Y(y)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(const CPoint2Di &point)
: X((float)point.X), Y((float)point.Y)
{
}
//----------------------------------------------------------------------------------
CPoint2Df::CPoint2Df(const CPoint2Df &point)
: X(point.X), Y(point.Y)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df()
: CPoint2Df()
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(int x, int y, int z)
: CPoint2Df(x, y), Z((float)z)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(float x, float y, float z)
: CPoint2Df(x, y), Z(z)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint2Di &point)
: CPoint2Df(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint3Di &point)
: CPoint2Df(point), Z((float)point.Z)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint2Df &point)
: CPoint2Df(point)
{
}
//----------------------------------------------------------------------------------
CPoint3Df::CPoint3Df(const CPoint3Df &point)
: CPoint2Df(point), Z(point.Z)
{
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
