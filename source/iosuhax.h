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

#ifdef __cplusplus
extern "C" {
#endif

#define IOS_ERROR_UNKNOWN_VALUE     0xFFFFFFD6
#define IOS_ERROR_INVALID_ARG       0xFFFFFFE3
#define IOS_ERROR_INVALID_SIZE      0xFFFFFFE9
#define IOS_ERROR_UNKNOWN           0xFFFFFFF7
#define IOS_ERROR_NOEXISTS          0xFFFFFFFA

typedef struct
{
    u32 flag;
    u32 permission;
    u32 owner_id;
    u32 group_id;
	u32 size; // size in bytes
	u32 physsize; // physical size on disk in bytes
	u32 unk[3];
	u32 id;
	u32 ctime;
	u32 mtime;
	u32 unk2[0x0D];
}fileStat_s;

typedef struct
{
    fileStat_s stat;
	char name[0x100];
}directoryEntry_s;

#define DIR_ENTRY_IS_DIRECTORY      0x80000000

#define FSA_MOUNTFLAGS_BINDMOUNT (1 << 0)
#define FSA_MOUNTFLAGS_GLOBAL (1 << 1)

int IOSUHAX_Open(void);
int IOSUHAX_Close(void);

int IOSUHAX_memwrite(u32 address, const u8 * buffer, u32 size); // IOSU external input
int IOSUHAX_memread(u32 address, u8 * out_buffer, u32 size);    // IOSU external output
int IOSUHAX_memcpy(u32 dst, u32 src, u32 size);                 // IOSU internal memcpy only

int IOSUHAX_SVC(u32 svc_id, u32 * args, u32 arg_cnt);

int IOSUHAX_FSA_Open();
int IOSUHAX_FSA_Close(int fsaFd);

int IOSUHAX_FSA_Mount(int fsaFd, const char* device_path, const char* volume_path, u32 flags, const char* arg_string, int arg_string_len);
int IOSUHAX_FSA_Unmount(int fsaFd, const char* path, u32 flags);

int IOSUHAX_FSA_GetDeviceInfo(int fsaFd, const char* device_path, int type, u32* out_data);

int IOSUHAX_FSA_MakeDir(int fsaFd, const char* path, u32 flags);
int IOSUHAX_FSA_OpenDir(int fsaFd, const char* path, int* outHandle);
int IOSUHAX_FSA_ReadDir(int fsaFd, int handle, directoryEntry_s* out_data);
int IOSUHAX_FSA_RewindDir(int fsaFd, int dirHandle);
int IOSUHAX_FSA_CloseDir(int fsaFd, int handle);
int IOSUHAX_FSA_ChangeDir(int fsaFd, const char *path);

int IOSUHAX_FSA_OpenFile(int fsaFd, const char* path, const char* mode, int* outHandle);
int IOSUHAX_FSA_ReadFile(int fsaFd, void* data, u32 size, u32 cnt, int fileHandle, u32 flags);
int IOSUHAX_FSA_WriteFile(int fsaFd, const void* data, u32 size, u32 cnt, int fileHandle, u32 flags);
int IOSUHAX_FSA_StatFile(int fsaFd, int fileHandle, fileStat_s* out_data);
int IOSUHAX_FSA_CloseFile(int fsaFd, int fileHandle);
int IOSUHAX_FSA_SetFilePos(int fsaFd, int fileHandle, u32 position);
int IOSUHAX_FSA_GetStat(int fsaFd, const char *path, fileStat_s* out_data);
int IOSUHAX_FSA_Remove(int fsaFd, const char *path);

#ifdef __cplusplus
}
#endif

#endif
