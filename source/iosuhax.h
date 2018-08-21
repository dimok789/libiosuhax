/***************************************************************************
 * Copyright (C) 2016
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#ifndef _LIB_IOSUHAX_H_
#define _LIB_IOSUHAX_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IOS_ERROR_UNKNOWN_VALUE     0xFFFFFFD6
#define IOS_ERROR_INVALID_ARG       0xFFFFFFE3
#define IOS_ERROR_INVALID_SIZE      0xFFFFFFE9
#define IOS_ERROR_UNKNOWN           0xFFFFFFF7
#define IOS_ERROR_NOEXISTS          0xFFFFFFFA

#define FLAG_IS_LINK                0x00010000
#define FLAG_IS_UNENCRYPTED         0x00800000
#define FLAG_IS_FILE                0x01000000
#define FLAG_IS_QUOTA               0x60000000
#define FLAG_IS_DIRECTORY           0x80000000

typedef struct
{
    uint32_t flag;
    uint32_t permission;
    uint32_t owner_id;
    uint32_t group_id;
    uint32_t size; // size in bytes
    uint32_t physsize; // physical size on disk in bytes
    uint64_t quota_size;
    uint32_t id;
    uint64_t ctime;
    uint64_t mtime;
    uint8_t attributes[48];
} FSStat;

typedef struct
{
    FSStat stat;
    char name[0x100];
}directoryEntry_s;

typedef struct
{
    uint8_t unknown[0x1E];
} FileSystemInfo;

typedef struct
{
    uint8_t unknown[0x28];
} DeviceInfo;

typedef struct
{
    uint64_t blocks_count;
    uint64_t some_count;
    uint32_t block_size;
} BlockInfo;

#define FSA_MOUNTFLAGS_BINDMOUNT (1 << 0)
#define FSA_MOUNTFLAGS_GLOBAL (1 << 1)

#define FSA_OPENFLAGS_OPEN_UNENCRYPTED (1 << 0)
#define FSA_OPENFLAGS_PREALLOC_SPACE (1 << 1)

int IOSUHAX_Open(const char *dev);  // if dev == NULL the default path /dev/iosuhax will be used
int IOSUHAX_Close(void);

int IOSUHAX_memwrite(uint32_t address, const uint8_t * buffer, uint32_t size); // IOSU external input
int IOSUHAX_memread(uint32_t address, uint8_t * out_buffer, uint32_t size);    // IOSU external output
int IOSUHAX_memcpy(uint32_t dst, uint32_t src, uint32_t size);                 // IOSU internal memcpy only

int IOSUHAX_SVC(uint32_t svc_id, uint32_t * args, uint32_t arg_cnt);

int IOSUHAX_FSA_Open();
int IOSUHAX_FSA_Close(int fsaFd);

int IOSUHAX_FSA_Mount(int fsaFd, const char* device_path, const char* volume_path, uint32_t flags, const char* arg_string, int arg_string_len);
int IOSUHAX_FSA_Unmount(int fsaFd, const char* path, uint32_t flags);
int IOSUHAX_FSA_FlushVolume(int fsaFd, const char* volume_path);
int IOSUHAX_FSA_RollbackVolume(int fsaFd, const char* volume_path);

int IOSUHAX_FSA_GetFreeSpaceSize(int fsaFd, const char *path, uint64_t* out_data);
int IOSUHAX_FSA_GetDirSize(int fsaFd, const char *path, uint64_t* out_data);
int IOSUHAX_FSA_GetEntryNum(int fsaFd, const char *path, uint32_t* out_data);
int IOSUHAX_FSA_GetFileSystemInfo(int fsaFd, const char *path, FileSystemInfo* out_data);
int IOSUHAX_FSA_GetDeviceInfo(int fsaFd, const char* device_path, DeviceInfo* out_data);
int IOSUHAX_FSA_GetStat(int fsaFd, const char *path, FSStat* out_data);
int IOSUHAX_FSA_GetBadBlockInfo(int fsaFd, const char *path, BlockInfo* out_data);
int IOSUHAX_FSA_GetJournalFreeSpaceSize(int fsaFd, const char *path, uint64_t* out_data);
int IOSUHAX_FSA_GetFragmentBlockInfo(int fsaFd, const char *path, BlockInfo* out_data);

int IOSUHAX_FSA_MakeDir(int fsaFd, const char* path, uint32_t flags);
int IOSUHAX_FSA_OpenDir(int fsaFd, const char* path, int* outHandle);
int IOSUHAX_FSA_ReadDir(int fsaFd, int handle, directoryEntry_s* out_data);
int IOSUHAX_FSA_RewindDir(int fsaFd, int dirHandle);
int IOSUHAX_FSA_CloseDir(int fsaFd, int handle);
int IOSUHAX_FSA_ChangeDir(int fsaFd, const char *path);
int IOSUHAX_FSA_GetCwd(int fsaFd, char *out_path, int out_size);

int IOSUHAX_FSA_MakeQuota(int fsaFd, const char* quota_path, uint32_t flags, uint64_t size);
int IOSUHAX_FSA_FlushQuota(int fsaFd, const char* quota_path);
int IOSUHAX_FSA_RollbackQuota(int fsaFd, const char* quota_path);
int IOSUHAX_FSA_RollbackQuotaForce(int fsaFd, const char* quota_path);

int IOSUHAX_FSA_OpenFile(int fsaFd, const char* path, const char* mode, int* outHandle);
int IOSUHAX_FSA_OpenFileEx(int fsaFd, const char* path, const char* mode, int* outHandle, uint32_t flags, int create_mode, uint32_t create_alloc_size);
int IOSUHAX_FSA_ReadFile(int fsaFd, void* data, uint32_t size, uint32_t cnt, int fileHandle, uint32_t flags);
int IOSUHAX_FSA_WriteFile(int fsaFd, const void* data, uint32_t size, uint32_t cnt, int fileHandle, uint32_t flags);
int IOSUHAX_FSA_ReadFileWithPos(int fsaFd, void* data, uint32_t size, uint32_t cnt, uint32_t position, int fileHandle, uint32_t flags);
int IOSUHAX_FSA_WriteFileWithPos(int fsaFd, const void* data, uint32_t size, uint32_t cnt, uint32_t position, int fileHandle, uint32_t flags);
int IOSUHAX_FSA_GetStatFile(int fsaFd, int fileHandle, FSStat* out_data);
int IOSUHAX_FSA_CloseFile(int fsaFd, int fileHandle);
int IOSUHAX_FSA_FlushFile(int fsaFd, int fileHandle);
int IOSUHAX_FSA_TruncateFile(int fsaFd, int fileHandle);
int IOSUHAX_FSA_GetFilePos(int fsaFd, int fileHandle, uint32_t* position);
int IOSUHAX_FSA_SetFilePos(int fsaFd, int fileHandle, uint32_t position);
int IOSUHAX_FSA_IsEof(int fsaFd, int fileHandle);
int IOSUHAX_FSA_Remove(int fsaFd, const char *path);
int IOSUHAX_FSA_Rename(int fsaFd, const char *old_path, const char *new_path);
int IOSUHAX_FSA_ChangeMode(int fsaFd, const char* path, int mode);
int IOSUHAX_FSA_ChangeModeEx(int fsaFd, const char* path, int mode, int mask);
int IOSUHAX_FSA_ChangeOwner(int fsaFd, const char* path, uint32_t owner, uint32_t group);

int IOSUHAX_FSA_RawOpen(int fsaFd, const char* device_path, int* outHandle);
int IOSUHAX_FSA_RawRead(int fsaFd, void* data, uint32_t block_size, uint32_t block_cnt, uint64_t sector_offset, int device_handle);
int IOSUHAX_FSA_RawWrite(int fsaFd, const void* data, uint32_t block_size, uint32_t block_cnt, uint64_t sector_offset, int device_handle);
int IOSUHAX_FSA_RawClose(int fsaFd, int device_handle);

#ifdef __cplusplus
}
#endif

#endif
