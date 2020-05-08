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
#include "Framework/System/ModuleException.hpp"

namespace bpf
{
    namespace system
    {
        class BPF_API Module
        {
        private:
            bpf::String _path;
            void *_handle;

#ifdef WINDOWS
            String ObtainErrorString();
#endif
        public:
            /**
             * Opens a module binary at a specified path, file extension omitted.
             * @param path the path to the binary module without file extension
             * @throws ModuleException
             */
            explicit Module(const bpf::String &path);
            inline Module() noexcept
                : _path(String::Empty)
                , _handle(Null)
            {
            }
            ~Module();

            inline Module(Module &&other)
                : _path(std::move(other._path))
                , _handle(other._handle)
            {
                other._handle = Null;
            }

            Module(const Module &other) = delete;

            Module &operator=(Module &&other);

            Module &operator=(const Module &other) = delete;

            /**
             * Loads a symbol from the module
             * @param name the symbol name
             * @throws ModuleException
             */
            void *LoadSymbol(const bpf::String &name);

            /**
             * Returns the path to this module
             */
            inline const bpf::String &Path() const noexcept
            {
                return (_path);
            }
        };
    }
}
