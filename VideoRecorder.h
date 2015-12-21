/****************************************************************************
**
** VideoRecorder.h
**
** Copyright (C) December 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef VideoRecorderH
#define VideoRecorderH
//--------------------------------------------------------------------------
class TVideoRecorder
{
private:
	/*ICaptureGraphBuilder2 *m_CaptureGraphBuilder;
	IGraphBuilder *m_GraphBuilder;
	IBaseFilter *m_Filter;*/

public:
	TVideoRecorder();
	virtual ~TVideoRecorder();
};
//---------------------------------------------------------------------------
//—сылка на рекордер
extern TVideoRecorder *VideoRecorder;
//---------------------------------------------------------------------------
#endif