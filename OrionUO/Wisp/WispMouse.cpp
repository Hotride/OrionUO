// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include <SDL_mouse.h>
namespace WISP_MOUSE
{
#if defined(_MSC_VER)
//forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable : 4800)
#endif

CMouse *g_WispMouse = nullptr;
//----------------------------------------------------------------------------------
CMouse::CMouse()
{
    g_WispMouse = this;
}
//----------------------------------------------------------------------------------
CMouse::~CMouse()
{
    g_WispMouse = nullptr;
}
//----------------------------------------------------------------------------------
void CMouse::Update()
{
    WISPFUN_DEBUG("c8_f1");
#if USE_WISP
    POINT pos;
    GetCursorPos(&pos);

    if (WISP_WINDOW::g_WispWindow != nullptr)
        ScreenToClient(WISP_WINDOW::g_WispWindow->Handle, &pos);

    Position.X = pos.x;
    Position.Y = pos.y;
#else
    SDL_GetMouseState(&Position.X, &Position.Y);
#endif
    Dragging = (LeftButtonPressed || RightButtonPressed || MidButtonPressed);
    RealPosition = Position;
}
//----------------------------------------------------------------------------------
void CMouse::Release() const
{
    WISPFUN_DEBUG("c8_f3");

#if USE_WISP
    if (!(LeftButtonPressed || RightButtonPressed || MidButtonPressed))
        ::ReleaseCapture();
#else
    if (!(LeftButtonPressed || RightButtonPressed || MidButtonPressed))
        SDL_CaptureMouse(SDL_FALSE);
#endif
}
//----------------------------------------------------------------------------------
void CMouse::Capture() const
{
    WISPFUN_DEBUG("");

#if USE_WISP
    if (WISP_WINDOW::g_WispWindow != nullptr)
        ::SetCapture(WISP_WINDOW::g_WispWindow->Handle);
#else
    SDL_CaptureMouse(SDL_TRUE);
#endif
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::LeftDroppedOffset()
{
    WISPFUN_DEBUG("c8_f4");
    WISP_GEOMETRY::CPoint2Di position;

    if (LeftButtonPressed)
    {
        position.X = RealPosition.X - LeftDropPosition.X;
        position.Y = RealPosition.Y - LeftDropPosition.Y;
    }

    return position;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::RightDroppedOffset()
{
    WISPFUN_DEBUG("c8_f5");
    WISP_GEOMETRY::CPoint2Di position;

    if (RightButtonPressed)
    {
        position.X = RealPosition.X - RightDropPosition.X;
        position.Y = RealPosition.Y - RightDropPosition.Y;
    }

    return position;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::MidDroppedOffset()
{
    WISPFUN_DEBUG("c8_f6");
    WISP_GEOMETRY::CPoint2Di position;

    if (MidButtonPressed)
    {
        position.X = RealPosition.X - MidDropPosition.X;
        position.Y = RealPosition.Y - MidDropPosition.Y;
    }

    return position;
}
//----------------------------------------------------------------------------------
}; // namespace WISP_MOUSE
//----------------------------------------------------------------------------------
