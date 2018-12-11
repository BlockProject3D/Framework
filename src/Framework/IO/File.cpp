// Copyright (c) 2018, BlockProject
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
//     * Neither the name of BlockProject nor the names of its contributors
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

#include <iostream>
#include <climits>
#include "Framework/Framework.hpp"
#include "Framework/IO/File.hpp"

using namespace Framework;

FFile::FFile(const bpf::String &path)
    : Stream(Null), FullPath(path), FileName(path.Sub(path.LastIndexOf('/') + 1)),
    FileExt(path.Sub(path.LastIndexOf('.') + 1))
{
#ifndef WINDOWS
    char buf[PATH_MAX];

    realpath(*path, buf);
    AbsolutePath = buf;
#endif
}

FFile::FFile()
    : Stream(Null), FullPath(""), FileName(FullPath.Sub(FullPath.LastIndexOf('/') + 1)),
    FileExt(FullPath.Sub(FullPath.LastIndexOf('.') + 1))
{
}

FFile::~FFile()
{
    Close();
}

void FFile::Copy(const FFile &dst)
{
    FFile out = dst;
    IFileStream *src = Open(FILEMODE_READ);

    if (dst.IsDirectory())
        out = FFile(bpf::String(dst.GetPath()) + bpf::String("/") + GetFileName());
    IFileStream *dest = out.Open(FILEMODE_WRITE);
    uint8 buf[4096];
    uint32 len;
    while ((len = src->Read(buf, 4096)) > 0)
        dest->Write(buf, len);
    Close();
    out.Close();
}

bool FFile::Exists() const
{
    return (FPlatform::GetFileSystem()->FileExists(FullPath));
}

bool FFile::IsDirectory() const
{
    return (FPlatform::GetFileSystem()->IsDirectory(FullPath));
}

uint64 FFile::GetSizeBytes() const
{
    return (FPlatform::GetFileSystem()->GetFileSize(FullPath));
}

void FFile::Delete()
{
    if (Exists() && IsDirectory())
        FPlatform::GetFileSystem()->DeleteDirectory(FullPath, false);
    else if (Exists())
        FPlatform::GetFileSystem()->DeleteFile(FullPath);
}

void FFile::ListFiles(bpf::List<FFile> &files)
{
    bpf::List<bpf::String> flns;

    FPlatform::GetFileSystem()->ListFiles(FullPath, flns);
    for (auto it = flns.Begin() ; it ; ++it)
        files.Add(FFile(FullPath + '/' + *it));
}

void FFile::CreateDir()
{
    if (Exists())
        return;
    FPlatform::GetFileSystem()->CreateDirectory(FullPath);
}

void FFile::Close()
{
    if (Stream == Null)
        return;
    FPlatform::GetFileSystem()->CloseFile(Stream);
    Stream = Null;
}

IFileStream *FFile::Open(const EFileMode mode)
{
    if (mode == FILEMODE_READ && !Exists())
        return (Null);
    if (Stream != Null)
        return (Stream);
    Stream = FPlatform::GetFileSystem()->OpenFile(FullPath, mode);
    return (Stream);
}

bpf::String	FFile::ToString() const
{
    return (bpf::String("File(") + FullPath + ")");
}
