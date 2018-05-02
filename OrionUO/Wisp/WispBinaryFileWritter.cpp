// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_FILE
{
//----------------------------------------------------------------------------------
CBinaryFileWritter::CBinaryFileWritter()
    : WISP_DATASTREAM::CDataWritter()
{
}
//----------------------------------------------------------------------------------
CBinaryFileWritter::~CBinaryFileWritter()
{
    Close();
}
//----------------------------------------------------------------------------------
bool CBinaryFileWritter::Open(const os_path &path)
{
    WISPFUN_DEBUG("c2_f1");
    bool result = false;

    //if (fs_path_exists(path))
    {
        m_File = fs_open(path, FS_WRITE);
        result = (m_File != nullptr);
    }

    return result;
}
//----------------------------------------------------------------------------------
void CBinaryFileWritter::Close()
{
    WISPFUN_DEBUG("c2_f3");
    WriteBuffer();

    if (m_File != nullptr)
    {
        fs_close(m_File);
        m_File = nullptr;
    }
}
//----------------------------------------------------------------------------------
void CBinaryFileWritter::WriteBuffer()
{
    WISPFUN_DEBUG("c2_f4");
    if (m_File != nullptr && m_Data.size() > 0)
    {
        fwrite(&m_Data[0], m_Data.size(), 1, m_File);
        m_Data.clear();
    }
}
//----------------------------------------------------------------------------------
}; // namespace WISP_FILE
//----------------------------------------------------------------------------------
