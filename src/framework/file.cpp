#include <iostream>
#include <climits>
#include "framework/framework.h"
#include "framework/file.h"

using namespace Framework;

FFile::FFile(const FString &path)
    : Stream(Null), FullPath(path), FileName(path.Sub(path.LastIndexOf('/') + 1)),
    FileExt(path.Sub(path.LastIndexOf('.') + 1))
{
    char buf[PATH_MAX];

    realpath(*path, buf);
    AbsolutePath = buf;
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
        out = FFile(FString(dst.GetPath()) + FString("/") + GetFileName());
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

void FFile::ListFiles(FList<FFile> &files)
{
    FList<FString> flns;

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

FString	FFile::ToString() const
{
    return (FString("File(") + FullPath + ")");
}
