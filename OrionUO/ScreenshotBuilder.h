/***********************************************************************************
**
** ScreenshotBuilder.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SCREENSHOTBUILDER_H
#define SCREENSHOTBUILDER_H
//----------------------------------------------------------------------------------
class CScreenshotBuilder
{
public:
    CScreenshotBuilder();
    virtual ~CScreenshotBuilder();

    void SaveScreen();

    void SaveScreen(int x, int y, int width, int height);

    UINT_LIST GetScenePixels(int x, int y, int width, int height);
};
//----------------------------------------------------------------------------------
extern CScreenshotBuilder g_ScreenshotBuilder;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
