// Copyright (c) 2020, BlockProject 3D
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimer in the documentation
//       and/or other materials provided with the distribution.
//     * Neither the name of BlockProject 3D nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <cstring>
#include <iostream>
#ifdef WINDOWS
    #include <Windows.h>
    #define PATH_MAX MAX_PATH
#else
    #include <dirent.h>
    #include <cstdlib>
    #include <sys/stat.h>
    #include <unistd.h>
#endif
#include "./OSPrivate.hpp"
#include "Framework/IO/File.hpp"
#include "Framework/IO/IOException.hpp"

using namespace bpf::io;
using namespace bpf::collection;
using namespace bpf;

File::File(const bpf::String &path)
    : FullPath(path)
    , UserPath(path)
    , FileName("")
    , FileExt("")
{
    if (FullPath.Size() == 0)
        return;
#ifdef WINDOWS
    FullPath = FullPath.Replace('/', '\\');
    String result = "";
    char old = '\0';
    for (fisize i = 0; i < FullPath.Size(); ++i)
    {
        if (FullPath.ByteAt(i) == '\\' && old != '\\')
            result += '\\';
        else if (FullPath.ByteAt(i) != '\\')
            result += FullPath.ByteAt(i);
        old = FullPath.ByteAt(i);
    }
    if (result.ByteAt(result.Size() - 1) == '\\')
        result = result.Sub(0, result.Len() - 1);
    FullPath = std::move(result);
    UserPath = FullPath.Replace('\\', '/');
    FileName = FullPath.Sub(FullPath.LastIndexOf('\\') + 1);
    FileExt = FullPath.Sub(FullPath.LastIndexOf('.') + 1);
#else
    String result = "";
    char old = '\0';
    for (fisize i = 0; i < FullPath.Size(); ++i)
    {
        if (FullPath.ByteAt(i) == '/' && old != '/')
            result += '/';
        else if (FullPath.ByteAt(i) != '/')
            result += FullPath.ByteAt(i);
        old = FullPath.ByteAt(i);
    }
    if (result.ByteAt(result.Size() - 1) == '/')
        result = result.Sub(0, result.Len() - 1);
    FullPath = std::move(result);
    UserPath = FullPath;
    FileName = path.Sub(path.LastIndexOf('/') + 1);
    FileExt = path.Sub(path.LastIndexOf('.') + 1);
#endif
}

File::File()
    : FullPath("")
    , UserPath("")
    , FileName(FullPath.Sub(FullPath.LastIndexOf('/') + 1))
    , FileExt(FullPath.Sub(FullPath.LastIndexOf('.') + 1))
{
}

bool File::HasAccess(const int type) const
{
#ifdef WINDOWS
    int acs = 0;
    if (type & FILE_MODE_READ && type & FILE_MODE_WRITE)
        acs = 06;
    else if (type & FILE_MODE_READ)
        acs = 04;
    else if (type & FILE_MODE_WRITE)
        acs = 02;
    if (_waccess(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), acs) != 0)
        return (false);
    return (true);
#else
    int md = F_OK;
    if (type & FILE_MODE_READ)
        md |= R_OK;
    if (type & FILE_MODE_WRITE)
        md |= W_OK;
    return (access(*FullPath, md) == 0);
#endif
}

File File::GetAbsolutePath() const
{
    String str;

#ifdef WINDOWS
    WCHAR buf[PATH_MAX];
    std::memset(buf, 0, PATH_MAX);
    if (GetFullPathNameW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), PATH_MAX, buf, nullptr) == 0)
        throw IOException(String("Could not read absolute path: ") + OSPrivate::ObtainLastErrorString());
    str = String::FromUTF16(reinterpret_cast<const bpf::fchar16 *>(buf));
#else
    char buf[PATH_MAX];
    std::memset(buf, 0, PATH_MAX);
    if (realpath(*FullPath, buf) == nullptr)
        throw IOException(String("Could not read absolute path: ") + OSPrivate::ObtainLastErrorString());
    str = String(buf);
#endif
    return (File(str));
}

File File::GetParent() const
{
    if (FullPath.LastIndexOf('/') == -1)
        return (File());
    return (File(FullPath.Sub(0, FullPath.LastIndexOf('/'))));
}

bool File::CopyTo(const File &dst, bool overwrite) //NOLINT (False-positive / not yet implemented)
{
#ifdef WINDOWS
    BOOL val = CopyFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), reinterpret_cast<LPCWSTR>(*dst.FullPath.ToUTF16()), !overwrite);
    return (val == TRUE ? true : false);
#else
    return (false);
#endif
    //TODO : Implement
    //File out = dst;
    /*IFileStream *src = Open(FILEMODE_READ);

    if (dst.IsDirectory())
        out = FFile(bpf::String(dst.GetPath()) + bpf::String("/") + GetFileName());
    IFileStream *dest = out.Open(FILEMODE_WRITE);
    uint8 buf[4096];
    uint32 len;
    while ((len = src->Read(buf, 4096)) > 0)
        dest->Write(buf, len);
    Close();
    out.Close();*/
}

bool File::MoveTo(const File &dst)
{
#ifdef WINDOWS
    BOOL val = MoveFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), reinterpret_cast<LPCWSTR>(*dst.FullPath.ToUTF16()));
    return (val == TRUE);
#else
    int val = rename(*FullPath, *dst.FullPath);
    return (val == 0);
#endif
}

bool File::Exists() const
{
#ifdef WINDOWS
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
    return (attr != INVALID_FILE_ATTRIBUTES);
#else
    return (access(*FullPath, F_OK) != -1);
#endif
}

bool File::IsHidden() const
{
#ifdef WINDOWS
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
    if (attr == INVALID_FILE_ATTRIBUTES)
        return (false);
    return (attr & FILE_ATTRIBUTE_HIDDEN);
#else
    return (FileName[0] == '.');
#endif
}

bool File::Hide(const bool flag)
{
#ifdef WINDOWS
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
    if (flag)
        attr |= FILE_ATTRIBUTE_HIDDEN;
    else
        attr &= ~FILE_ATTRIBUTE_HIDDEN;
    return (SetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), attr) == TRUE);
#else
    if (flag && !IsHidden())
    {
        File f = File(GetParent().Path() + "/" + "." + Name());
        if (rename(*FullPath, *f.Path()) != 0)
            return (false);
        FileName = f.Name();
        FullPath = f.PlatformPath();
        UserPath = f.Path();
        FileExt = f.Extension();
    }
    else if (IsHidden())
    {
        File f = File(GetParent().Path() + "/" + Name().Sub(1));
        if (rename(*FullPath, *f.Path()) != 0)
            return (false);
        FileName = f.Name();
        FullPath = f.PlatformPath();
        UserPath = f.Path();
        FileExt = f.Extension();
    }
    return (true);
#endif
}

bool File::IsDirectory() const
{
    if (!Exists())
        return (false);
#ifdef WINDOWS
    DWORD attr = GetFileAttributesW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()));
    if (attr == INVALID_FILE_ATTRIBUTES)
        return (false);
    return (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st;
    if (stat(*FullPath, &st) == -1)
        return (false);
    return (S_ISDIR(st.st_mode));
#endif
}

uint64 File::GetSizeBytes() const
{
    if (!Exists())
        throw IOException(String("Could not find file: ") + FullPath);
#ifdef WINDOWS
    WIN32_FIND_DATAW data;
    HANDLE hdl = FindFirstFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), &data);
    if (hdl == INVALID_HANDLE_VALUE)
        throw IOException(String("FindFirstFileW failed: ") + OSPrivate::ObtainLastErrorString());
    FindClose(hdl);
    return (data.nFileSizeLow | (uint64)data.nFileSizeHigh << 32);
#else
    struct stat st;
    if (stat(*FullPath, &st))
        throw IOException(String("stat failed: ") + OSPrivate::ObtainLastErrorString());
    return (st.st_size);
#endif
}

bool File::Delete()
{
    if (!Exists())
        return (false);
#ifdef WINDOWS
    if (IsDirectory())
        return (RemoveDirectoryW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16())) == TRUE);
    else
        return (DeleteFileW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16())) == TRUE);
#else
    if (IsDirectory())
        return (rmdir(*FullPath) == 0);
    else
        return (unlink(*FullPath) == 0);
#endif
}

List<File> File::ListFiles() const
{
    List<File> flns;

    if (!Exists() || !IsDirectory())
        throw IOException(String("File does not exist or is not a directory: ") + FullPath);
#ifdef WINDOWS
    WIN32_FIND_DATAW data;
    File dir = (*this + "/*");
    HANDLE hdl = FindFirstFileW(reinterpret_cast<LPCWSTR>(*dir.FullPath.ToUTF16()), &data);
    if (hdl != INVALID_HANDLE_VALUE)
    {
        auto name = String::FromUTF16(reinterpret_cast<const fchar16 *>(data.cFileName));
        if (name != '.' && name != "..")
            flns.Add(File(FullPath + '/' + name));
        while (FindNextFileW(hdl, &data))
        {
            auto sfk = String::FromUTF16(reinterpret_cast<const fchar16 *>(data.cFileName));
            if (sfk != '.' && sfk != "..")
                flns.Add(File(FullPath + '/' + sfk));
        }
        FindClose(hdl);
    }
#else
    DIR *d = opendir(*FullPath);

    if (d)
    {
        struct dirent *dir;
        while ((dir = readdir(d)) != nullptr)
        {
            auto name = String(dir->d_name);
            if (name != '.' && name != "..")
                flns.Add(File(FullPath + '/' + name));
        }
    }
    closedir(d);
#endif
    return (flns);
}

bool File::CreateDir()
{
    if (Exists())
        return (false);
#ifdef WINDOWS
    return (CreateDirectoryW(reinterpret_cast<LPCWSTR>(*FullPath.ToUTF16()), nullptr) == TRUE);
#else
    return (mkdir(*FullPath, 0755) == 0);
#endif
}
