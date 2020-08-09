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
#include "Framework/Graphics/EPixelFormat.hpp"
#include "Framework/Graphics/PixelArray.hpp"
#include "Framework/IO/ByteBuf.hpp"
#include "Framework/IO/File.hpp"
#include "Framework/Memory/UniquePtr.hpp"
#include "Framework/System/PluginLoader.hpp"
#include "Framework/Types.hpp"

namespace bpf
{
    namespace graphics
    {
        class BPF_GRAPHICS_API IImageLoader;

        class BPF_GRAPHICS_API Image
        {
        private:
            static memory::UniquePtr<IImageLoader> LoadInternal(const String &format);

        public:
            const fsize Width;
            const fsize Height;
            const uint16 BPP;
            const EPixelFormat PixelFormat;

            inline Image(const fsize w, const fsize h, const uint16 bpp, const EPixelFormat pformat)
                : Width(w)
                , Height(h)
                , BPP(bpp)
                , PixelFormat(pformat)
            {
            }

            virtual ~Image()
            {
            }

            virtual PixelArray &GetPixelArray() = 0;
            virtual void SetPixelArray(PixelArray &&array) = 0;
            virtual void SetPixelArray(const PixelArray &array) = 0;
            virtual void Save(io::IOutputStream &stream) = 0;

            /**
             * Loads an image file
             * @param file the file to load
             * @throw io::IOException if the file does not exist, cannot be loaded or if the format is unsupported
             * @return loaded image
             */
            static memory::UniquePtr<Image> Load(const io::File &file);

            /**
             * Loads an image from a stream
             * @param stream source to decode image
             * @param format the image format
             * @throw io::IOException if the file does not exist, cannot be loaded or if the format is unsupported
             * @return loaded image
             */
            static memory::UniquePtr<Image> Load(io::IInputStream &stream, const String &format);

            /**
             * Loads an image from a stream
             * @param ploader an instance of a PluginLoader in order to load required plugin
             * @param stream source to decode image
             * @param format the image format
             * @throw io::IOException if the file does not exist, cannot be loaded or if the format is unsupported
             * @throw system::ModuleException if a required plugin could not be loaded
             * @return loaded image
             */
            static memory::UniquePtr<Image> Load(system::PluginLoader &ploader, io::IInputStream &stream,
                                                 const String &format);

            /**
             * Loads an image file
             * @param ploader an instance of a PluginLoader in order to load required plugin
             * @param file the file to load
             * @throw io::IOException if the file does not exist, cannot be loaded or if the format is unsupported
             * @throw system::ModuleException if a required plugin could not be loaded
             * @return loaded image
             */
            static memory::UniquePtr<Image> Load(system::PluginLoader &ploader, const io::File &file);

            /**
             * Creates an empty image
             * @param format the image format
             * @param w the image width
             * @param h the image height
             * @param pformat the pixel format
             * @throw io::IOException if the format is unsupported
             * @return new empty image
             */
            static memory::UniquePtr<Image> CreateEmpty(const String &format, fsize w, fsize h, EPixelFormat pformat);

            /**
             * Creates an empty image
             * @param ploader an instance of a PluginLoader in order to load required plugin
             * @param format the image format
             * @param w the image width
             * @param h the image height
             * @param pformat the pixel format
             * @throw io::IOException if the format is unsupported
             * @throw system::ModuleException if a required plugin could not be loaded
             * @return new empty image
             */
            static memory::UniquePtr<Image> CreateEmpty(system::PluginLoader &ploader, const String &format, fsize w,
                                                        fsize h, EPixelFormat pformat);
        };
    }
}
