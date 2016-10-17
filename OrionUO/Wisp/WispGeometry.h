//----------------------------------------------------------------------------------
#ifndef WISPGEOMETRY_H
#define WISPGEOMETRY_H
#include "WispGlobal.h"
//----------------------------------------------------------------------------------
namespace WISP_GEOMETRY
{
class CPoint2Df;
class CPoint3Df;
//----------------------------------------------------------------------------------
class CSize
{
	SETGET(int, Width);
	SETGET(int, Height);

public:
	CSize();
	CSize(const int &width, const int &height);
	CSize(const CSize &size);

	virtual ~CSize() {}

	virtual void Reset() { m_Width = 0; m_Height = 0; }
};
//----------------------------------------------------------------------------------
class CPoint2Di
{
	SETGET(int, X);
	SETGET(int, Y);

public:
	CPoint2Di();
	CPoint2Di(const int &x, const int &y);
	CPoint2Di(const float &x, const float &y);
	CPoint2Di(const CPoint2Di &point);
	CPoint2Di(const CPoint2Df &point);

	virtual ~CPoint2Di() {}

	virtual void Reset() { m_X = 0; m_Y = 0; }
};
//----------------------------------------------------------------------------------
class CRect
{
	SETGET(CPoint2Di, Position);
	SETGET(CSize, Size);

public:
	CRect();
	CRect(const int &x, const int &y, const int &width, const int &height);
	CRect(const int &x, const int &y, const CSize &size);
	CRect(const CPoint2Di &position, const int &width, const int &height);
	CRect(const CPoint2Di &position, const CSize &size);

	virtual ~CRect() {}

	virtual void Reset() { m_Position.Reset(); m_Size.Reset(); }
};
//----------------------------------------------------------------------------------
class CPoint3Di : public CPoint2Di
{
	SETGET(int, Z);

public:
	CPoint3Di();
	CPoint3Di(const int &x, const int &y, const int &z);
	CPoint3Di(const float &x, const float &y, const float &z);
	CPoint3Di(const CPoint2Di &point);
	CPoint3Di(const CPoint3Di &point);
	CPoint3Di(const CPoint2Df &point);
	CPoint3Di(const CPoint3Df &point);

	virtual ~CPoint3Di() {}

	virtual void Reset() { CPoint2Di::Reset(); m_Z = 0; }
};
//----------------------------------------------------------------------------------
class CPoint2Df
{
	SETGET(float, X);
	SETGET(float, Y);

public:
	CPoint2Df();
	CPoint2Df(const int &x, const int &y);
	CPoint2Df(const float &x, const float &y);
	CPoint2Df(const CPoint2Di &point);
	CPoint2Df(const CPoint2Df &point);

	virtual ~CPoint2Df() {}

	virtual void Reset() { m_X = 0.0f; m_Y = 0.0f; }
};
//----------------------------------------------------------------------------------
class CPoint3Df : public CPoint2Df
{
	SETGET(float, Z);

public:
	CPoint3Df();
	CPoint3Df(const int &x, const int &y, const int &z);
	CPoint3Df(const float &x, const float &y, const float &z);
	CPoint3Df(const CPoint2Di &point);
	CPoint3Df(const CPoint3Di &point);
	CPoint3Df(const CPoint2Df &point);
	CPoint3Df(const CPoint3Df &point);

	virtual ~CPoint3Df() {}

	virtual void Reset() { CPoint2Df::Reset(); m_Z = 0.0f; }
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

