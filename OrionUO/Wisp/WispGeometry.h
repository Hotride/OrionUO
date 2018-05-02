//----------------------------------------------------------------------------------
#ifndef WISPGEOMETRY_H
#define WISPGEOMETRY_H
//----------------------------------------------------------------------------------
namespace WISP_GEOMETRY
{
class CPoint2Df;
class CPoint3Df;
//----------------------------------------------------------------------------------
class CSize
{
public:
    int Width = 0;
    int Height = 0;

    CSize();
    CSize(int width, int height);
    CSize(const CSize &size);

    virtual ~CSize() {}

    virtual void Reset()
    {
        Width = 0;
        Height = 0;
    }
};
//----------------------------------------------------------------------------------
class CPoint2Di
{
public:
    int X = 0;
    int Y = 0;

    CPoint2Di();
    CPoint2Di(int x, int y);
    CPoint2Di(float x, float y);
    CPoint2Di(const CPoint2Di &point);
    CPoint2Di(const CPoint2Df &point);

    virtual ~CPoint2Di() {}

    virtual void Reset()
    {
        X = 0;
        Y = 0;
    }
};
//----------------------------------------------------------------------------------
class CRect
{
public:
    CPoint2Di Position = CPoint2Di();
    CSize Size = CSize();

    CRect();
    CRect(int x, int y, int width, int height);
    CRect(int x, int y, const CSize &size);
    CRect(const CPoint2Di &position, int width, int height);
    CRect(const CPoint2Di &position, const CSize &size);

    virtual ~CRect() {}

    virtual void Reset()
    {
        Position.Reset();
        Size.Reset();
    }
};
//----------------------------------------------------------------------------------
class CPoint3Di : public CPoint2Di
{
public:
    int Z = 0;

    CPoint3Di();
    CPoint3Di(int x, int y, int z);
    CPoint3Di(float x, float y, float z);
    CPoint3Di(const CPoint2Di &point);
    CPoint3Di(const CPoint3Di &point);
    CPoint3Di(const CPoint2Df &point);
    CPoint3Di(const CPoint3Df &point);

    virtual ~CPoint3Di() {}

    virtual void Reset()
    {
        CPoint2Di::Reset();
        Z = 0;
    }
};
//----------------------------------------------------------------------------------
class CPoint2Df
{
public:
    float X = 0.0f;
    float Y = 0.0f;

    CPoint2Df();
    CPoint2Df(int x, int y);
    CPoint2Df(float x, float y);
    CPoint2Df(const CPoint2Di &point);
    CPoint2Df(const CPoint2Df &point);

    virtual ~CPoint2Df() {}

    virtual void Reset()
    {
        X = 0.0f;
        Y = 0.0f;
    }
};
//----------------------------------------------------------------------------------
class CPoint3Df : public CPoint2Df
{
public:
    float Z = 0.0f;

    CPoint3Df();
    CPoint3Df(int x, int y, int z);
    CPoint3Df(float x, float y, float z);
    CPoint3Df(const CPoint2Di &point);
    CPoint3Df(const CPoint3Di &point);
    CPoint3Df(const CPoint2Df &point);
    CPoint3Df(const CPoint3Df &point);

    virtual ~CPoint3Df() {}

    virtual void Reset()
    {
        CPoint2Df::Reset();
        Z = 0.0f;
    }
};
//----------------------------------------------------------------------------------
}; // namespace WISP_GEOMETRY
//----------------------------------------------------------------------------------
#endif
