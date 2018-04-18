#pragma once

#include <string>
#include <stdio.h>

#if defined(ORION_WINDOWS)
#define os_path wstring
#define ToPath(x) ToWString(x)
#define StringFromPath(x) ToString(x)
#define CStringFromPath(x) ToString(x).c_str()
#define PATH_SEP L"\\"
#else
#define os_path string
#define ToPath(x) x
#define StringFromPath(x) x
#define CStringFromPath(x) x.c_str()
#define PATH_SEP string("/")
#endif

enum fs_mode
{
  FS_READ = 0x01,
  FS_WRITE = 0x02,
};

FILE *fs_open(const os_path &path_str, fs_mode mode);
void fs_close(FILE *fp);
size_t fs_size(FILE *fp);

bool fs_path_exists(const os_path &path_str);
bool fs_path_create(const os_path &path_str);
os_path fs_path_current();

unsigned char *fs_map(const os_path &path, size_t *length);
void fs_unmap(unsigned char *ptr, size_t length);
