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
#include "Framework/IO/IInputStream.hpp"
#include "Framework/IO/IOutputStream.hpp"
#include "Framework/String.hpp"
#include "Framework/System/Application.hpp"

namespace bpf
{
    namespace system
    {
#ifdef WINDOWS
        constexpr const char *PROCESS_DEFAULT_PATH = "%SystemRoot%/system32;%SystemRoot%;%SystemRoot%/System32/Wbem";
#else
        constexpr const char *PROCESS_DEFAULT_PATH = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin";
#endif
        class BPF_API Process
        {
        public:
            class BPF_API Builder
            {
            private:
                bool _redirectStdIn;
                bool _redirectStdOut;
                bool _redirectStdErr;
                io::File _workDir;
                collection::HashMap<String, String> _envp;
                String _appExe;
                String _appName;
                collection::Array<String> _argv;

#ifndef WINDOWS
                void ProcessWorker(int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2]);
                Process ProcessMaster(int pid, int fdStdOut[2], int fdStdErr[2], int fdStdIn[2], int commonfd[2]);
#endif

            public:
                /**
                 * Constructs a Process::Builder
                 */
                Builder();

                /**
                 * Constructs a Process::Builder
                 * @param app instance of calling Application to copy defaults from
                 */
                Builder(const Application &app);

                /**
                 * Sets the application to run as a separate process
                 * @param appName the application path or name string (this overload does automatically check the PATH
                 * using this Builder's environment)
                 * @throw io::IOException when the application could not be found
                 * @return Builder reference
                 */
                Builder &SetApplication(const String &name);

                /**
                 * Sets the process working directory
                 * @param dir the new directory
                 * @throw io::IOException when dir does not exist or is not a directory
                 * @return Builder reference
                 */
                Builder &SetWorkingDirectory(const io::File &dir);

                /**
                 * Sets the arguments
                 * @param args arguments to pass to the new process
                 * @return Builder reference
                 */
                inline Builder &SetArguments(const collection::Array<String> &args)
                {
                    _argv = args;
                    return (*this);
                }

                /**
                 * Sets the arguments
                 * @param args arguments to pass to the new process
                 * @return Builder reference
                 */
                inline Builder &SetArguments(collection::Array<String> &&args)
                {
                    _argv = std::move(args);
                    return (*this);
                }

                /**
                 * Sets the environment
                 * @param env the new environment
                 * @return Builder reference
                 */
                inline Builder &SetEnvironment(const collection::HashMap<String, String> &env)
                {
                    _envp = env;
                    return (*this);
                }

                /**
                 * Sets the environment
                 * @param env the new environment
                 * @return Builder reference
                 */
                inline Builder &SetEnvironment(collection::HashMap<String, String> &&env)
                {
                    _envp = std::move(env);
                    return (*this);
                }

                /**
                 * Enables standard input redirect
                 * @return Builder reference
                 */
                inline Builder &RedirectInput()
                {
                    _redirectStdIn = true;
                    return (*this);
                }

                /**
                 * Enables standard output redirect
                 * @return Builder reference
                 */
                inline Builder &RedirectOutput()
                {
                    _redirectStdOut = true;
                    return (*this);
                }

                /**
                 * Enables standard error redirect
                 * @return Builder reference
                 */
                inline Builder &RedirectError()
                {
                    _redirectStdErr = true;
                    return (*this);
                }

                /**
                 * Create a process from the information defined in this builder
                 * @throw OSException in case the process could not be created
                 * @return instance to represent the new process
                 */
                Process Build();
            };

        private:
            class BPF_API PStream final : public io::IInputStream, public io::IOutputStream
            {
            private:
#ifdef WINDOWS
#else
                int _pipfd[2];
                PStream(int pipefd[2]);
                ~PStream();
#endif

            public:
                fsize Read(void *buf, fsize bufsize);
                fsize Write(const void *buf, fsize bufsize);

                friend class Process;
            };

            fint _lastExitCode;
            bool _redirectStdIn;
            bool _redirectStdOut;
            bool _redirectStdErr;
            PStream _stdIn;
            PStream _stdOut;
            PStream _stdErr;

#ifdef WINDOWS
#else
            int _pid;
            Process(int pid, int fdStdIn[2], int fdStdOut[2], int fdStdErr[2]);
#endif

        public:
            /**
             * Destructor, the process gets automatically killed if this is reached
             */
            ~Process();

            /**
             * Wait for the process to terminate
             * @throw OSException in case the system can't wait for the process
             */
            void Wait();

            /**
             * Kill the target process
             * @param force true to force kill, false otherwise
             * @throw OSException in case the system could not send signal
             */
            void Kill(bool force);

            /**
             * Returns the exit code of the process
             * @throw OSException in case the system could not poll the target process
             * @return exit code as an integer, -1 if the process is still running
             */
            fint GetExitCode(); // WAITPID WNOHANG

            /**
             * Provides access to process standard input
             * @throw OSException if the target process does not allow standard input redirection
             * @return reference to IOutputStream
             */
            io::IOutputStream &GetStandardInput();

            /**
             * Provides access to process standard output
             * @throw OSException if the target process does not allow standard output redirection
             * @return reference to IInputStream
             */
            io::IInputStream &GetStandardOutput();

            /**
             * Provides access to process standard error
             * @throw OSException if the target process does not allow standard error redirection
             * @return reference to IInputStream
             */
            io::IInputStream &GetStandardError();

            friend class Process::Builder;
        };
    }
}
