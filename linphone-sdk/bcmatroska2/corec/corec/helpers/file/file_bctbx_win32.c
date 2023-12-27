/*****************************************************************************
 *
 * Copyright (c) 2008-2010, CoreCodec, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of CoreCodec, Inc. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CoreCodec, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CoreCodec, Inc. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "file.h"

#define bool_t bctbx_bool_t
#include <bctoolbox/vfs.h>
#undef bool_t

#if defined(TARGET_WIN)

#define FILE_FUNC_ID  FOURCC('F','L','I','D')

#ifndef STRICT
#define STRICT
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <fileapi.h>

#ifndef FO_DELETE
#define FO_DELETE   3
#endif
#ifndef FOF_NO_UI
#define FOF_NO_UI (0x04|0x10|0x400|0x200)
#endif

#if defined(TARGET_WINCE)
static HMODULE CEShellDLL = NULL;
#endif
#ifdef WINDOWS_DESKTOP
static int (WINAPI* FuncSHFileOperation)(SHFILEOPSTRUCT*) = NULL;
#else
static void *FuncSHFileOperation = NULL;
#endif

#ifndef ERROR_INVALID_DRIVE_OBJECT
#define ERROR_INVALID_DRIVE_OBJECT		4321L
#endif

#ifndef ERROR_DEVICE_NOT_AVAILABLE
#define ERROR_DEVICE_NOT_AVAILABLE		4319L
#endif

#ifndef ERROR_DEVICE_REMOVED
#define ERROR_DEVICE_REMOVED            1617L
#endif

#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER        ((DWORD)-1)
#endif

typedef struct filestream
{
	stream Stream;
	tchar_t URL[MAXPATH];
	bctbx_vfs_file_t *fp;
	filepos_t Length;
	filepos_t Pos;
	int Flags;

	void* Find;
	WIN32_FIND_DATA FindData;
    int DriveNo;

} filestream;

static err_t Open(filestream* p, const tchar_t* URL, int Flags)
{
  if (p->fp != -1)
		bctbx_file_close(p->fp);

	p->Length = INVALID_FILEPOS_T;
	p->fp = NULL;

	if (URL && URL[0])
	{
		struct stat file_stats;
		int mode = 0;
#ifdef UNICODE
		char url_buffer[1024];
		memset(url_buffer, 0, sizeof(url_buffer));
#ifdef _WIN32
		if(WideCharToMultiByte(CP_ACP, 0, URL, -1, url_buffer, sizeof(url_buffer), 0, 0) == 0) {
#else
		if (wcstombs_s(&return_length, url_buffer, sizeof(url_buffer), URL, sizeof(url_buffer) - 1) != 0) {
#endif
			NodeReportError(p, NULL, ERR_ID, ERR_INVALID_PARAM, URL);
			return ERR_INVALID_PARAM;
		}
#else
		const char* url_buffer = URL;
#endif

		if (Flags & SFLAG_WRONLY && !(Flags & SFLAG_RDONLY))
			mode = O_WRONLY;
		else if (Flags & SFLAG_RDONLY && !(Flags & SFLAG_WRONLY))
			mode = O_RDONLY;
		else
			mode = O_RDWR;

		if (Flags & SFLAG_CREATE)
			mode |= O_CREAT|O_TRUNC;

		p->fp = bctbx_file_open2(bctbx_vfs_get_default(), url_buffer, mode);
		if (p->fp == NULL)
		{
			if ((Flags & (SFLAG_REOPEN|SFLAG_SILENT))==0)
				NodeReportError(p,NULL,ERR_ID,ERR_FILE_NOT_FOUND, url_buffer);
			return ERR_FILE_NOT_FOUND;
		}

		tcscpy_s(p->URL,TSIZEOF(p->URL),URL);
		if (stat(url_buffer, &file_stats) == 0)
			p->Length = file_stats.st_size;
	}
	return ERR_NONE;
}

static err_t Read(filestream* p,void* Data,size_t Size,size_t* Readed)
{
	err_t Err;
	int n = bctbx_file_read2(p->fp, Data, (unsigned int)Size);
	if (n<0)
	{
		n=0;
		Err = ERR_READ;
	}
	else
		Err = ((size_t)n != Size) ? ERR_END_OF_FILE:ERR_NONE;

	if (Readed)
		*Readed = n;
	return Err;
}

static err_t ReadBlock(filestream* p,block* Block,size_t Ofs,size_t Size,size_t* Readed)
{
	return Read(p,(void*)(Block->Ptr+Ofs),Size,Readed);
}

static err_t Write(filestream* p,const void* Data,size_t Size,size_t* Written)
{
	err_t Err;
	int n = bctbx_file_write2(p->fp, Data, (unsigned int)Size);

	if (n<0)
	{
		n=0;
		Err = ERR_WRITE;
	}
	else
		Err = (n != Size) ? ERR_WRITE:ERR_NONE;

	if (Written)
		*Written = n;
	return Err;
}

static filepos_t Seek(filestream* p,filepos_t Pos,int SeekMode)
{
	off_t NewPos = bctbx_file_seek(p->fp, Pos, SeekMode);
	if (NewPos<0)
		return INVALID_FILEPOS_T;
	return NewPos;
}

static err_t SetLength(filestream* p,dataid Id,const filepos_t* Data,size_t Size)
{
	if (Size != sizeof(filepos_t))
		return ERR_INVALID_DATA;

	if (bctbx_file_truncate(p->fp, *Data)!=0)
		return ERR_BUFFER_FULL;

	return ERR_NONE;
}

static err_t OpenDir(filestream* p,const tchar_t* URL,int UNUSED_PARAM(Flags))
{
#ifndef WINDOWS_DESKTOP
	WIN32_FILE_ATTRIBUTE_DATA attr_data;
#else
	DWORD Attrib;
#endif
	tchar_t Path[MAXPATHFULL];

	if (p->Find != INVALID_HANDLE_VALUE)
	{
		FindClose(p->Find);
		p->Find = INVALID_HANDLE_VALUE;
	}
    p->DriveNo = -1;

#if !defined(TARGET_WINCE)
    if (!URL[0])
    {
        p->DriveNo = 0;
    }
    else
#endif
    {
#ifndef WINDOWS_DESKTOP
		if (GetFileAttributesEx(URL, GetFileExInfoStandard, &attr_data) == 0)
			return ERR_FILE_NOT_FOUND;

		if (!(attr_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return ERR_NOT_DIRECTORY;
#else
		Attrib = GetFileAttributes(URL);
		if (Attrib == (DWORD)-1)
			return ERR_FILE_NOT_FOUND;

		if (!(Attrib & FILE_ATTRIBUTE_DIRECTORY))
			return ERR_NOT_DIRECTORY;
#endif

        tcscpy_s(Path,TSIZEOF(Path),URL);
        AddPathDelimiter(Path,TSIZEOF(Path));
        tcscat_s(Path,TSIZEOF(Path),T("*.*"));
#ifndef WINDOWS_DESKTOP
		WIN32_FIND_DATA FileData;
		p->Find = FindFirstFileEx(Path, FindExInfoStandard, &FileData, FindExSearchNameMatch, NULL, 0);
#else
		p->Find = FindFirstFile(Path, &p->FindData);
#endif
    }

    return ERR_NONE;
}

extern datetime_t FileTimeToRel(FILETIME*);

static err_t EnumDir(filestream* p,const tchar_t* Exts,bool_t ExtFilter,streamdir* Item)
{
	Item->FileName[0] = 0;
	Item->DisplayName[0] = 0;

#if !defined(TARGET_WINCE) && defined(WINDOWS_DESKTOP)
    if (p->DriveNo>=0)
    {
        size_t n = GetLogicalDriveStrings(0,NULL);
        tchar_t* Drives = alloca((n+1)*sizeof(tchar_t));
        if (GetLogicalDriveStrings((DWORD)n,Drives))
        {
            int No = p->DriveNo++;

            while (Drives[0] && --No>=0)
                Drives += tcslen(Drives)+1;

            if (Drives[0])
            {
                size_t n = tcslen(Drives);
                if (Drives[n-1] == '\\')
                    Drives[n-1] = 0;
                tcscpy_s(Item->FileName,TSIZEOF(Item->FileName),Drives);
                Item->ModifiedDate = INVALID_DATETIME_T;
                Item->Size = INVALID_FILEPOS_T;
                Item->Type = FTYPE_DIR;
            }
        }
    }
    else
#endif
    {
	    while (!Item->FileName[0] && p->Find != INVALID_HANDLE_VALUE)
	    {
		    if (p->FindData.cFileName[0]!='.' && // skip unix/mac hidden files and . .. directory entries
                !(p->FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		    {
			    tcscpy_s(Item->FileName,TSIZEOF(Item->FileName),p->FindData.cFileName);

                Item->ModifiedDate = FileTimeToRel(&p->FindData.ftLastWriteTime);

			    if (p->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
				    Item->Size = INVALID_FILEPOS_T;
                    Item->Type = FTYPE_DIR;
                }
			    else
			    {
				    Item->Size = (filepos_t)(((int64_t)p->FindData.nFileSizeHigh << 32) | p->FindData.nFileSizeLow);
				    Item->Type = CheckExts(Item->FileName,Exts);

				    if (!Item->Type && ExtFilter)
					    Item->FileName[0] = 0; // skip
			    }
		    }

		    if (!FindNextFile(p->Find,&p->FindData))
		    {
			    FindClose(p->Find);
			    p->Find = INVALID_HANDLE_VALUE;
		    }
	    }
    }

	if (!Item->FileName[0])
	{
		if (p->Find != INVALID_HANDLE_VALUE)
		{
			FindClose(p->Find);
			p->Find = INVALID_HANDLE_VALUE;
		}
        p->DriveNo = -1;
		return ERR_END_OF_FILE;
	}

	return ERR_NONE;
}

static void Delete(filestream* p)
{
	Open(p,NULL,0);
	if (p->Find != INVALID_HANDLE_VALUE)
		FindClose(p->Find);
}

static err_t CreateFunc(node* UNUSED_PARAM(p))
{
#if defined(TARGET_WINCE)
	CEShellDLL = LoadLibrary(T("ceshell.dll"));
	if (CEShellDLL)
		*(FARPROC*)(void*)&FuncSHFileOperation = GetProcAddress(CEShellDLL,MAKEINTRESOURCE(14));
#elif defined(ENABLE_MICROSOFT_STORE_APP)
    FuncSHFileOperation = NULL;
#elif defined(WINDOWS_DESKTOP)
    FuncSHFileOperation = SHFileOperation;
#endif

    return ERR_NONE;
}

static void DeleteFunc(node* UNUSED_PARAM(p))
{
#if defined(TARGET_WINCE)
	if (CEShellDLL) FreeLibrary(CEShellDLL);
#endif
}

META_START(File_Class,FILE_CLASS)
META_CLASS(SIZE,sizeof(filestream))
META_CLASS(PRIORITY,PRI_MINIMUM)
META_CLASS(DELETE,Delete)
META_VMT(TYPE_FUNC,stream_vmt,Open,Open)
META_VMT(TYPE_FUNC,stream_vmt,Read,Read)
META_VMT(TYPE_FUNC,stream_vmt,ReadBlock,ReadBlock)
META_VMT(TYPE_FUNC,stream_vmt,Write,Write)
META_VMT(TYPE_FUNC,stream_vmt,Seek,Seek)
META_VMT(TYPE_FUNC,stream_vmt,OpenDir,OpenDir)
META_VMT(TYPE_FUNC,stream_vmt,EnumDir,EnumDir)
META_CONST(TYPE_PTR,filestream,Find,INVALID_HANDLE_VALUE)
META_DATA_RDONLY(TYPE_INT,STREAM_FLAGS,filestream,Flags)
META_DATA_RDONLY(TYPE_STRING,STREAM_URL,filestream,URL)
META_PARAM(SET,STREAM_LENGTH,SetLength)
META_DATA(TYPE_FILEPOS,STREAM_LENGTH,filestream,Length)
META_PARAM(STRING,NODE_PROTOCOL,T("file"))
META_END_CONTINUE(STREAM_CLASS)

META_START_CONTINUE(FILE_FUNC_ID)
META_CLASS(FLAGS,CFLAG_SINGLETON)
META_CLASS(CREATE,CreateFunc)
META_CLASS(DELETE,DeleteFunc)
META_END(NODE_CLASS)

bool_t FolderCreate(nodecontext* UNUSED_PARAM(p),const tchar_t* Path)
{
	return CreateDirectory(Path,NULL) != FALSE;
}

bool_t FileExists(nodecontext* UNUSED_PARAM(p),const tchar_t* Path)
{
#ifndef WINDOWS_DESKTOP
	WIN32_FILE_ATTRIBUTE_DATA attr_data;
	return GetFileAttributesEx(Path, GetFileExInfoStandard, &attr_data) != 0;
#else
	return GetFileAttributes(Path) != (DWORD)-1;
#endif
}

#ifdef WINDOWS_DESKTOP
static bool_t FileRecycle(const tchar_t* Path)
{
    tchar_t PathEnded[MAXPATHFULL];
    SHFILEOPSTRUCT DelStruct;
    int Ret;
    size_t l;

    memset(&DelStruct,0,sizeof(DelStruct));
    DelStruct.wFunc = FO_DELETE;
    l = min(tcslen(Path)+1,TSIZEOF(PathEnded)-1);
    tcscpy_s(PathEnded,TSIZEOF(PathEnded),Path);
    PathEnded[l]=0;
    DelStruct.pFrom = PathEnded;
    DelStruct.fFlags = FOF_ALLOWUNDO|FOF_NO_UI;
    Ret = FuncSHFileOperation(&DelStruct);
    return Ret == 0;
}
#endif

bool_t FileErase(nodecontext* UNUSED_PARAM(p),const tchar_t* Path, bool_t Force, bool_t Safe)
{
    if (Force)
    {
#ifndef WINDOWS_DESKTOP
		WIN32_FILE_ATTRIBUTE_DATA attr_data;
		if ((GetFileAttributesEx(Path, GetFileExInfoStandard, &attr_data) != 0) && (attr_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
			attr_data.dwFileAttributes ^= FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(Path, attr_data.dwFileAttributes);
		}
#else
		DWORD attr = GetFileAttributes(Path);
        if ((attr != (DWORD)-1) && (attr & FILE_ATTRIBUTE_READONLY))
        {
            attr ^= FILE_ATTRIBUTE_READONLY;
            SetFileAttributes(Path,attr);
        }
#endif
    }

#ifndef WINDOWS_DESKTOP
	return DeleteFile(Path) != FALSE;
#else
    if (!Safe || !FuncSHFileOperation)
    	return DeleteFile(Path) != FALSE;
    else
        return FileRecycle(Path);
#endif
}

bool_t FolderErase(nodecontext* UNUSED_PARAM(p),const tchar_t* Path, bool_t Force, bool_t Safe)
{
    if (Force)
    {
#ifndef WINDOWS_DESKTOP
		WIN32_FILE_ATTRIBUTE_DATA attr_data;
		if ((GetFileAttributesEx(Path, GetFileExInfoStandard, &attr_data) != 0) && (attr_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)) {
			attr_data.dwFileAttributes ^= FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(Path, attr_data.dwFileAttributes);
		}
#else
        DWORD attr = GetFileAttributes(Path);
        if ((attr != (DWORD)-1) && (attr & FILE_ATTRIBUTE_READONLY))
        {
            attr ^= FILE_ATTRIBUTE_READONLY;
            SetFileAttributes(Path,attr);
        }
#endif
    }

#ifndef WINDOWS_DESKTOP
	return RemoveDirectory(Path) != FALSE;
#else
    if (!Safe || !FuncSHFileOperation)
    	return RemoveDirectory(Path) != FALSE;
    else
        return FileRecycle(Path);
#endif
}

bool_t PathIsFolder(nodecontext* UNUSED_PARAM(p),const tchar_t* Path)
{
#ifndef WINDOWS_DESKTOP
	WIN32_FILE_ATTRIBUTE_DATA attr_data;
	return (GetFileAttributesEx(Path, GetFileExInfoStandard, &attr_data) != 0) && ((attr_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
#else
	DWORD attr = GetFileAttributes(Path);
	return (attr != (DWORD)-1) && (attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
#endif
}

datetime_t FileDateTime(nodecontext* UNUSED_PARAM(p),const tchar_t* Path)
{
	datetime_t Date = INVALID_DATETIME_T;
	HANDLE Find;
	WIN32_FIND_DATA FindData;

#ifndef WINDOWS_DESKTOP
	WIN32_FIND_DATA FileData;
	Find = FindFirstFileEx(Path, FindExInfoStandard, &FileData, FindExSearchNameMatch, NULL, 0);
#else
	Find = FindFirstFile(Path, &FindData);
#endif
	if (Find != INVALID_HANDLE_VALUE)
	{
		Date = FileTimeToRel(&FindData.ftLastWriteTime);
		FindClose(Find);
	}
	return Date;
}

bool_t FileMove(nodecontext* UNUSED_PARAM(p),const tchar_t* In,const tchar_t* Out)
{
#ifndef WINDOWS_DESKTOP
	return MoveFileEx(In, Out, 0);
#else
    return MoveFile(In,Out) != 0;
#endif
}

stream *FileTemp(anynode* UNUSED_PARAM(Any))
{
#ifndef TODO
    assert(NULL); // not supported yet
#endif
    return NULL;
}

bool_t FileTempName(anynode* UNUSED_PARAM(Any),tchar_t* UNUSED_PARAM(Out), size_t UNUSED_PARAM(OutLen))
{
#ifndef TODO
    assert(NULL); // not supported yet
#endif
    return 0;
}

FILE_DLL int64_t GetPathFreeSpace(nodecontext* UNUSED_PARAM(p), const tchar_t* Path)
{
    ULARGE_INTEGER lpFreeBytesAvailable;
    ULARGE_INTEGER lpTotal;

    if (!GetDiskFreeSpaceEx(Path, &lpFreeBytesAvailable, &lpTotal, NULL))
        return -1;
    return (int64_t)lpFreeBytesAvailable.QuadPart;
}
#endif
