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
        /**
         * Cross platform file utilities
         */
        class BPF_API File
        {
        private:
            String FullPath;
            String UserPath;
            String FileName;
            String FileExt;

        public:
            /**
             * Constructs a File from a path
             * @param path the path string (use '/' as path separator on any platform)
             */
            explicit File(const String &path);

            /**
             * Constructs a File
             */
            File();

            ~File();

            /**
             * Checks if the file exists
             * @return true if the file exists, false otherwise
             */
            bool Exists() const;

            /**
             * Checks if the file is a hidden file
             * @return true if the file is hidden, false otherwise
             */
            bool IsHidden() const;

            /**
             * Checks if the file is a directory (or folder on Windows)
             * @return true if the file is a directory (or folder on Windows), false otherwise
             */
            bool IsDirectory() const;

            /**
             * Returns the size in bytes of that file
             * @throw IOException in case of system error
             * @return size in bytes as unsigned
             */
            uint64 GetSizeBytes() const;

            /**
             * Deletes this file from the file system
             */
            void Delete();

            /**
             * Marks this file as hidden to the current platform
             * @param flag true to hide the file, false to make the file visible
             */
            void Hide(const bool flag);

            /**
             * Lists all files in that directory/folder (of course if this file is a directory/folder)
             * @throw IOException in case of system error
             * @return the output list of files (including directories/folders)
             */
            collection::List<File> ListFiles();

            /**
             * Creates the directory/folder
             */
            void CreateDir();

            /**
             * Copies this file to the destination, in case destination is a directory/folder, copy in that directory/folder under the same name
             * @param dst the destination file or directory/folder
             * @param overwrite true to overwrite any existing file, false otherwise
             * @return true if operation succeeded, false otherwise
             */
            bool CopyTo(const File &dst, bool overwrite = false);

            /**
             * Moves this file to the destination, in case destination is a directory/folder, move in that directory/folder under the same name
             * @param dst the destination file or directory/folder
             * @return true if operation succeeded, false otherwise
             */
            bool MoveTo(const File &dst);

            /**
             * Returns the full path to the file (as given in the constructor)
             * @return immutable high-level string
             */
            inline const String &Path() const
            {
                return (UserPath);
            }

            /**
             * Returns the path as expected by platform conventions
             * @return immutable high-level string
             */
            inline const String &PlatformPath() const
            {
                return (FullPath);
            }

            /**
             * Concatenates this file to another
             * @param other the file to append (a path separator is automatically added)
             * @return new File from concatenation
             */
            inline File operator+(const File &other) const
            {
                return (File(FullPath + "/" + other.FullPath));
            }

            /**
             * Concatenates this file to another
             * @param other the file to append (a path separator is automatically added)
             * @return new File from concatenation
             */
            inline File operator+(const String &other) const
            {
                return (File(FullPath + "/" + other));
            }

            /**
             * Get parent directory/folder if any
             * @return empty File if no parent, other parent directory/folder
             */
            File GetParent() const;

            /**
             * Returns the absolute path constructed from this path,
             * That means a path that starts from system's root (usually '/...' on linux and 'C:\' on Windows)
             * @throw IOException in case of system error
             * @return new File
             */
            File GetAbsolutePath() const;

            /**
             * Returns the file's name
             * @return immutable high-level string
             */
            inline const String &Name() const
            {
                return (FileName);
            }

            /**
             * Returns the file's extension
             * @return immutable high-level string
             */
            inline const String &Extension() const
            {
                return (FileExt);
            }
        };
    }
};
