#pragma once

namespace WISP_FILE
{
class CMappedFile : public WISP_DATASTREAM::CDataReader
{
#if USE_WISP
private:
    HANDLE m_File{ nullptr };
    void *m_Map{ nullptr };

    bool Load();
#endif

public:
    CMappedFile();

    virtual ~CMappedFile();

    bool Load(const os_path &path);

    void Unload();
};

}; // namespace
