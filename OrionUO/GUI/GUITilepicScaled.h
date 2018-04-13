/***********************************************************************************
**
** GUITilepicScaled.h
**
** Компонента для отображения картинки статики (масштабированная
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUITILEPICSCALED_H
#define GUITILEPICSCALED_H
//----------------------------------------------------------------------------------
class CGUITilepicScaled : public CGUITilepic
{
	int Width = 0;
	int Height = 0;

public:
	CGUITilepicScaled(const ushort &graphic, const ushort &color, const int &x, const int &y, const int &width, const int &height);
	virtual ~CGUITilepicScaled();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select() { return false; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
