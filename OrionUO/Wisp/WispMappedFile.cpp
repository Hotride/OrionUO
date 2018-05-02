// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "FileSystem.h"

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
string g_WispMappedFileError = "";
//----------------------------------------------------------------------------------
CMappedFile::CMappedFile()
    : WISP_DATASTREAM::CDataReader()
{
}
//----------------------------------------------------------------------------------
CMappedFile::~CMappedFile()
{
    Unload();
}
//----------------------------------------------------------------------------------
#if USE_WISP
bool CMappedFile::Load()
{
    WISPFUN_DEBUG("c7_f1");
    bool result = false;
    Size = GetFileSize(m_File, nullptr);
    if (Size > 0)
    {
        m_Map = CreateFileMapping(m_File, nullptr, 2, 0, 0, nullptr);

        if (m_Map != nullptr)
        {
            Start = (puchar)MapViewOfFile(m_Map, FILE_MAP_READ, 0, 0, Size);

            result = (Start != nullptr);

            if (!result)
            {
                CloseHandle(m_Map);
                CloseHandle(m_File);
                m_Map = nullptr;
                m_File = INVALID_HANDLE_VALUE;
            }
            else
                SetData(Start, Size);
        }
        else
        {
            CloseHandle(m_File);
            m_File = INVALID_HANDLE_VALUE;
        }
    }
    else
    {
        CloseHandle(m_File);
        m_File = INVALID_HANDLE_VALUE;
    }

    return result;
}
#endif
//----------------------------------------------------------------------------------
bool CMappedFile::Load(const os_path &path)
{
    WISPFUN_DEBUG("c7_f2");
    LOG("Mmaping  %s\n", CStringFromPath(path));
    bool result = false;

    if (fs_path_exists(path))
    {
        Unload();

#if USE_WISP
        m_File = CreateFileW(
            path.c_str(), GENERIC_READ, 1, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (m_File != INVALID_HANDLE_VALUE)
            result = Load();
        else
            LOG("INVALID_HANDLE_VALUE for CreateFileW  %s\n", CStringFromPath(path));
#else
        Start = fs_map(path.c_str(), &Size);
        result = Start != nullptr;
        SetData(Start, Size);
#endif
    }
    else
        LOG("File not found %s\n", CStringFromPath(path));

    if (!result)
    {
#if USE_WISP
        auto errorCode = GetLastError();
#else
        auto errorCode = errno;
#endif
        LOG("Failed to memory map, error code: %i\n", errorCode);
        g_WispMappedFileError = CStringFromPath(path);
    }

    return result;
}
//----------------------------------------------------------------------------------
void CMappedFile::Unload()
{
    //WISPFUN_DEBUG("c7_f4");
#if USE_WISP
    if (Start != nullptr)
        UnmapViewOfFile(Start);

    if (m_Map != nullptr)
    {
        CloseHandle(m_Map);
        m_Map = 0;
    }

    if (m_File != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_File);
        m_File = INVALID_HANDLE_VALUE;
    }
#else
    if (Start != nullptr)
        fs_unmap(Start, Size);
#endif

    SetData(nullptr, 0);
}
//----------------------------------------------------------------------------------
}; // namespace WISP_FILE
//----------------------------------------------------------------------------------
