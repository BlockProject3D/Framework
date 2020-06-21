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
#include "Framework/Collection/HashMap.hpp"
#include "Framework/String.hpp"
#include "Framework/System/Paths.hpp"

namespace bpf
{
    namespace system
    {
        /**
         * Class to represent the running application
         */
        class BPF_API Application
        {
        public:
            /**
             * The environment of the application
             */
            const collection::HashMap<String, String> &Environment;

            /**
             * The executable file name
             */
            const String &FileName;

            /**
             * Application's paths properties
             */
            const Paths &Props;

            /**
             * Constructs an Application container
             * @param env the environment map reference
             * @param fileName the application's file name reference
             * @param props the application's paths properties reference
             */
            inline Application(collection::HashMap<String, String> &env, String &fileName, Paths &props)
                : Environment(env)
                , FileName(fileName)
                , Props(props)
            {
            }

            /**
             * Virtual destructor
             */
            virtual ~Application() {}

            /**
             * Allocate a console for this application if it does not already have a console
             * @param rows number of rows in the console window
             * @param columns number of columns in the console window
             */
            virtual void CreateConsole(const fint rows = 32, const fint columns = 80) = 0; //Only usefull when the system do not create a console

            /**
             * Gets the current working directory
             * @return new io::File containing current working directory as given by system
             */
            virtual io::File GetWorkingDirectory() const = 0;

            /**
             * Sets the current working directory
             * @param file the new current working directory (relative paths are supported)
             * @return true if operation succeeded, false otherwise
             */
            virtual bool SetWorkingDirectory(const io::File &file) const = 0;
        };
    }
};