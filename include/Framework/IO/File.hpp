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

#ifndef FILE_H_
# define FILE_H_

#include "Framework/IO/FileSystem.hpp"

namespace Framework
{
    class ENGINE_API FFile
    {
    private:
        IFileStream *Stream;
        bpf::String FullPath;
        bpf::String FileName;
        bpf::String FileExt;
        bpf::String AbsolutePath;
    public:
        FFile(const bpf::String &path);
        FFile();
        ~FFile();

        /**
         * Returns true if the file exists
         */
        bool Exists() const;

        /**
         * Returns true if this file is a directory
         */
        bool IsDirectory() const;

        /**
         * Returns the size in bytes of that file
         */
        uint64 GetSizeBytes() const;

        /**
         * Deletes this file from the file system
         */
        void Delete();

        /**
         * Lists all files in that directory (of course if this is a directory)
         * @param files the output list of files to fill
         */
        void ListFiles(bpf::List<FFile> &files);

        /**
         * Creates the directory if this is a directory
         */
        void CreateDir();

        /**
         * Copies this file to the destination, in case destination is a directory, copy in that directory under the same name
         * @param dst the destination file or directory
         */
        void Copy(const FFile &dst);

        /**
         * Returns the full path to the file (as given in the constructor)
         */
        inline const bpf::String &GetPath() const
        {
            return (FullPath);
        }
      
        /**
         * Returns the absolute path constructed from this path,
         * That means a path that starts from system's root folder
         */
        inline const bpf::String &GetAbsolutePath() const
        {
            return (AbsolutePath);
        }
      
        inline FFile operator+(const FFile &other) const
        {
            return (FFile(FullPath + "/" + other.FullPath));
        }

        inline FFile operator+(const bpf::String &other) const
        {
            return (FFile(FullPath + "/" + other));
        }

        /**
         * Returns the file's name
         */
        inline const bpf::String &GetFileName() const
        {
            return (FileName);
        }

        /**
         * Returns the file's extension
         */
        inline const bpf::String &GetFileExt() const
        {
            return (FileExt);
        }

        /**
         * Closes the stream to that file (if opened)
         */
        void Close();

        /**
         * Opens a stream to this file
         * @param mode the open mode for that file
         */
        IFileStream *Open(const EFileMode mode);

        bpf::String ToString() const;
    };
};

#endif /* !FILE_H_ */
