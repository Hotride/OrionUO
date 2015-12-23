#pragma once
//----------------------------------------------------------------------------
#ifndef interface
#define interface struct
#endif
//----------------------------------------------------------------------------
class TTextureObject;
class TVector;
class TMappedHeader;
class TGump;
//----------------------------------------------------------------------------
interface IGLEngine
{
	virtual void ViewPort(int X, int Y, int SizeX, int SizeY) = 0;
	virtual void RestorePort() = 0;

	virtual void Enable(GLenum val) = 0;
	virtual void Disable(GLenum val) = 0;

	virtual void DrawLine(DWORD Color, float X, float Y, float TargetX, float TargetY) = 0;
	virtual void DrawPolygone(DWORD Color, float X, float Y, float Width, float Height) = 0;
	virtual void DrawTriangle(DWORD Color, float X, float Y, float radius) = 0;

	virtual void DrawLandTexture(GLuint &Texture, float X, float Y, float Width, float Height, RECT &rc, TVector *normals) = 0;
	virtual void Draw(GLuint &Texture, float X, float Y, float Width, float Height) = 0;
	virtual void Draw(GLuint &Texture, float X, float Y, float Width, float Height, bool Mirror) = 0;
	virtual void Draw(GLuint &Texture, float X, float Y, float Width, float Height, float DrawWidth, float DrawHeight) = 0;
	virtual void DrawResizepic(TTextureObject **th, float X, float Y, float W, float H) = 0;
};
//----------------------------------------------------------------------------
interface IUltimaOnline
{
	virtual bool ExecuteGumpPart(WORD id, int count) = 0;

	virtual void DrawSphereGump(int value, float maxValue, int x, int y, int width) = 0;

	virtual void DrawGump(WORD id, WORD color, int x, int y, bool partialHue) = 0;
	virtual void DrawGump(WORD id, WORD color, int x, int y, int width, int height, bool partialHue) = 0;
	virtual void DrawResizepicGump(WORD id, int x, int y, int width, int height) = 0;
	virtual void DrawLandTexture(WORD id, WORD color, int x, int y, RECT rc, TVector *normals) = 0;
	virtual void DrawLandArt(WORD id, WORD color, int x, int y, int z) = 0;
	virtual void DrawStaticArt(WORD id, WORD color, int x, int y, int z, bool selection) = 0;
	virtual void DrawStaticArtAnimated(WORD id, WORD color, int x, int y, int z, bool selection) = 0;
	virtual void DrawStaticArtInContainer(WORD id, WORD color, int x, int y, bool selection, bool onMouse) = 0;
	virtual void DrawLight(BYTE id, WORD color, int x, int y) = 0;

	virtual bool PolygonePixelsInXY(int x, int y, int width, int height) = 0;
	virtual bool GumpPixelsInXY(WORD id, int x, int y, bool noSubMouse) = 0;
	virtual bool GumpPixelsInXY(WORD id, int x, int y, int width, int height, bool noSubMouse) = 0;
	virtual bool ResizepicPixelsInXY(WORD id, int x, int y, int width, int height) = 0;
	virtual bool StaticPixelsInXY(WORD id, int x, int y, int z) = 0;
	virtual bool StaticPixelsInXYAnimated(WORD id, int x, int y, int z) = 0;
	virtual bool StaticPixelsInXYInContainer(WORD id, int x, int y) = 0;
	virtual bool LandPixelsInXY(WORD id, int x, int y, int z) = 0;

	virtual DWORD GetLandFlags(WORD id) = 0;
	virtual DWORD GetStaticFlags(WORD id) = 0;

	virtual void GetArtDimension(WORD id, POINT &p) = 0;
	virtual void GetGumpDimension(WORD id, POINT &p) = 0;
};
//----------------------------------------------------------------------------
interface IGump
{
	virtual void GetBaseProperties(DWORD &serial, WORD &graphic, WORD &color, WORD &x, WORD &y) = 0;

	virtual GUMP_TYPE GetGumpType() const = 0;
	virtual DWORD GetID() const = 0;
	virtual int GetMinimizedX() const = 0;
	virtual int GetMinimizedY() const = 0;
	virtual bool GetNoMove() const = 0;
	virtual bool GetNoClose() const = 0;
	virtual bool GetMinimized() const = 0;
	virtual bool GetFrameCreated() const = 0;
	virtual bool GetFrameRedraw() const = 0;
	virtual bool GetBlocked() const = 0;
	virtual bool GetLockMoving() const = 0;

	virtual void UpdateFrame() = 0;
	virtual void GenerateFrame(int posX, int posY) = 0;
	virtual bool CanBeDisplayed() = 0;
	virtual bool CanBeMoved() = 0;
	virtual int Draw(bool &mode) = 0;
	virtual void OnLeftMouseDown() = 0;
	virtual void OnLeftMouseUp() = 0;
	virtual void OnRightMouseDown() = 0;
	virtual void OnRightMouseUp() = 0;
	virtual bool OnLeftMouseDoubleClick() = 0;
	virtual bool OnRightMouseDoubleClick() = 0;
	virtual void OnMouseWheel(MOUSE_WHEEL_STATE &state) = 0;
	virtual void OnCharPress(WPARAM &wparam, LPARAM &lparam) = 0;
	virtual void OnKeyPress(WPARAM &wparam, LPARAM &lparam) = 0;
	virtual bool EntryPointerHere() = 0;
};
//----------------------------------------------------------------------------
interface IGumpManager
{
	virtual void AddGump(TGump *obj) = 0;
	virtual IGump *GetGump(DWORD serial, DWORD ID, GUMP_TYPE Type) = 0;
	virtual IGump *GumpExists(DWORD gumpID) = 0;
	virtual void CloseGump(DWORD serial, DWORD ID, GUMP_TYPE Type) = 0;
	virtual void RemoveGump(TGump *obj) = 0;
};
//----------------------------------------------------------------------------
interface IMappedHeader
{
	//virtual void Move(int offset) = 0;

	virtual BYTE ReadByte() = 0;
	virtual WORD ReadWord() = 0;
	virtual DWORD ReadDWord() = 0;
	virtual char ReadChar() = 0;
	virtual short ReadShort() = 0;
	virtual int ReadInt() = 0;
	virtual string ReadString(int size) = 0;
};
//----------------------------------------------------------------------------
interface IFileManager
{
	virtual bool GetUseVerdata() const = 0;

	virtual bool LoadFileToMemory(TMappedHeader &object, const char *fName) = 0;
	virtual void UnloadFileFromMemory(TMappedHeader &object) = 0;
	virtual IMappedHeader* GetMappedFile(int index) = 0;
};
//----------------------------------------------------------------------------
interface ICliloc
{
	virtual string GetLanguage() const = 0;
	virtual bool GetLoaded() const = 0;

	virtual string GetA(DWORD id, string result) = 0;
	virtual wstring GetW(DWORD id, string result) = 0;
};
//----------------------------------------------------------------------------
interface IClilocManager
{
	virtual ICliloc *Cliloc(const char *lang) = 0;
};
//----------------------------------------------------------------------------
interface IColorManager
{
	virtual int GetHuesCount() const = 0;

	virtual WORD Color32To16(DWORD &C) = 0;
	virtual DWORD Color16To32(WORD &C) = 0;
	virtual WORD ConvertToGray(WORD &C) = 0;

	virtual DWORD GetPolygoneColor(WORD C, WORD Color) = 0;
	virtual DWORD GetColor(WORD &C, WORD Color) = 0;
	virtual DWORD GetPartialHueColor(WORD &C, WORD Color) = 0;
};
//----------------------------------------------------------------------------
interface IPathFinder
{
	virtual bool CanWalk(BYTE &direction, int &x, int &y, char &z) = 0;
	virtual bool Walk(bool run, BYTE direction) = 0;
};
//---------------------------------------------------------------------------
typedef struct PLUGIN_CLIENT_INTERFACE
{
	IGLEngine *_GL;
	IUltimaOnline *_UO;
	IGumpManager *_GumpManager;
	IFileManager *_FileManager;
	IClilocManager *_ClilocManager;
	IColorManager *_ColorManager;
	IPathFinder *_PathFinder;
} *PPLUGIN_CLIENT_INTERFACE;
//----------------------------------------------------------------------------
extern PLUGIN_CLIENT_INTERFACE PluginClientInterface;
//----------------------------------------------------------------------------