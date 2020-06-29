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

#pragma once
#include "Framework/Collection/List.hpp"
#include "Framework/String.hpp"

namespace bpf
{
    namespace io
    {
        constexpr int FILE_ACCESS_READ = 0x1;
        constexpr int FILE_ACCESS_WRITE = 0x2;

        class BPF_API File
        {
        private:
            String FullPath;
            String UserPath;
            String FileName;
            String FileExt;

        public:
            explicit File(const String &path);
            File();
            ~File();

            /**
             * Checks wether the current application is allowed to access a given file or folder
             * @param type the access type to check (see FILE_ACCESS_* constants)
             * @return true if the access is granted, false otherwise
             */
            bool HasAccess(const int type) const;

            /**
             * Returns true if the file exists
             */
            bool Exists() const;

            /**
             * Returns true if the file is hidden
             */
            bool IsHidden() const;

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
             * @return true if the operation succeeded, false otherwise
             */
            bool Delete();

            /**
             * Hides or shows the file in the file explorer.
             * @return true if the operation succeeded, false otherwise
             */
            bool Hide(const bool flag);

            /**
             * Lists all files in that directory (of course if this is a directory)
             * @return the output list of files
             */
            collection::List<File> ListFiles();

            /**
             * Creates the directory if this is a directory
             * @return true if the operation succeeded, false otherwise
             */
            bool CreateDir();

            /**
             * Copies this file to the destination, in case destination is a directory, copy in that directory under the same name
             * @param dst the destination file or directory
             */
            bool CopyTo(const File &dst, bool overwrite = false);

            bool MoveTo(const File &dst);

            /**
             * Returns the full path to the file (as given in the constructor)
             */
            inline const String &Path() const
            {
                return (UserPath);
            }

            inline const String &PlatformPath() const
            {
                return (FullPath);
            }

            inline File operator+(const File &other) const
            {
                return (File(FullPath + "/" + other.FullPath));
            }

            inline File operator+(const String &other) const
            {
                return (File(FullPath + "/" + other));
            }

            File GetParent() const;

            /**
             * Returns the absolute path constructed from this path,
             * That means a path that starts from system's root folder
             */
            File GetAbsolutePath() const;

            /**
             * Returns the file's name
             */
            inline const String &Name() const
            {
                return (FileName);
            }

            /**
             * Returns the file's extension
             */
            inline const String &Extension() const
            {
                return (FileExt);
            }
        };
    }
};
