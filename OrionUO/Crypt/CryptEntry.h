#pragma once
#if !USE_ORIONDLL
size_t GetPluginsCount();
void CryptInstallNew(uchar *address, size_t size, uchar *result, size_t &resultSize);
#endif