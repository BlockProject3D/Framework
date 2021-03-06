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
#include "Framework/IO/File.hpp"
#include "Framework/System/Plugin.hpp"

namespace bpf
{
    namespace system
    {
        /**
         * Utility class to allow easy loading of plugins
         */
        class BP_TPL_API PluginLoader
        {
        private:
            io::File _modulePath;

            template <class BaseClass>
            using ModuleLinkFunc = BaseClass *(*)();
            using ModuleDescribeFunc = fint(*)();

        public:
            /**
             * Constructs a PluginLoader
             * @param modulePath the path to the folder in which all plugins are located
             */
            explicit inline PluginLoader(const io::File &modulePath)
                : _modulePath(modulePath)
            {
            }

            /**
             * Load a plugin with a file name; virtual name is deduced from file name
             * @tparam BaseClass the interface class type
             * @param fileName the file name without extension, prefixed with modulePath
             * @throw ModuleException in case of system error or plugin incompatibility
             * @return new plugin
             */
            template <class BaseClass>
            Plugin<BaseClass> Load(const String &fileName);

            /**
             * Load a plugin with a file name
             * @tparam BaseClass the interface class type
             * @param fileName the file name without extension, prefixed with modulePath
             * @param virtualName the module name
             * @throw ModuleException in case of system error or plugin incompatibility
             * @return new plugin
             */
            template <class BaseClass>
            Plugin<BaseClass> Load(const String &virtualName, const String &fileName);
        };
    }
}

#include "Framework/System/PluginLoader.impl.hpp"