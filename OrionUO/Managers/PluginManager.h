/***********************************************************************************
**
** PluginManager.h
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
//----------------------------------------------------------------------------------
class CPlugin : public CBaseQueueItem
{
private:
    uint m_Flags{ 0 };

public:
    CPlugin(uint flags);
    virtual ~CPlugin();

    bool CanParseRecv() { return (m_Flags & PLUGIN_FLAGS_PARSE_RECV); }
    bool CanParseSend() { return (m_Flags & PLUGIN_FLAGS_PARSE_SEND); }
    bool CanRecv() { return (m_Flags & PLUGIN_FLAGS_RECV); }
    bool CanSend() { return (m_Flags & PLUGIN_FLAGS_SEND); }
    bool CanWindowProc() { return (m_Flags & PLUGIN_FLAGS_WINDOW_PROC); }
    bool CanClientAccess() { return (m_Flags & PLUGIN_FLAGS_CLIENT_ACCESS); }
    bool CanEnterWorldRender() { return (m_Flags & PLUGIN_FLAGS_GAME_WORLD_DRAW); }
    bool CanEnterSceneRender() { return (m_Flags & PLUGIN_FLAGS_SCENE_DRAW); }
    bool CanEnterWorldMapRender() { return (m_Flags & PLUGIN_FLAGS_WORLD_MAP_DRAW); }

    PPLUGIN_INTERFACE m_PPS;
};
//----------------------------------------------------------------------------------
class CPluginManager : public CBaseQueue
{
public:
    CPluginManager();
    virtual ~CPluginManager() {}

    LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
    bool PacketRecv(puchar buf, int size);
    bool PacketSend(puchar buf, int size);
    void Disconnect();
    void WorldDraw();
    void SceneDraw();
    void WorldMapDraw();
};
//----------------------------------------------------------------------------------
extern CPluginManager g_PluginManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
