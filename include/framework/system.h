#ifndef SYSTEM_H_
# define SYSTEM_H_

namespace Framework
{
    typedef void(*ThreadFunc)(void *);

    class ENGINE_API FTimeDate
    {
    public:
        /**
         * Hour, Minute and Second of day
         */
        uint8 Hours;
        uint8 Minutes;
        uint8 Seconds;

        /**
         * Day relative to the Month
         */
        uint8 Day;
        uint8 Month;

        /**
         * Year
         */
        uint32 Year;

        /**
         * Formats this date in a string
         * %h = Hours, %n = Minutes, %s = Seconds, %m = Month, %d = Day, %y = Year
         */
        FString Format(const FString &format);
    };

    /**
     * The process is still running
     */
    constexpr int PROCESS_STILL_RUNNING = -1;
  
    /**
     * The process execution failed
     */
    constexpr int PROCESS_EXEC_FAILURE = -2;
  
    /**
     * The process specified binary path is not executable
     */
    constexpr int PROCESS_BIN_ERROR = -3;

    /**
     * Represent a user created process (Child process under Unix based systems)
     */
    class ENGINE_API IProcess
    {
    public:
        virtual ~IProcess() {}
        
        /**
         * Kills this process returns true if succeeded, false otherwise
         * IsValid will return false if this succeeded
         * If force is true, then the lowest level allowed signal will be used (SIGKILL under UNIX)
         */
        virtual bool Kill(const bool force) = 0;

        /**
         * Returns the process ID
         */
        virtual int64 GetID() const = 0;

        /**
         * Returns the process binary name
         */
        virtual const FString &GetBinaryName() const = 0;

        /**
         * Returns the exit code
         */
        virtual int GetExitCode() const = 0;

        /**
         * Returns true if this process is still running false otherwise
         */
        virtual bool IsRunning() const = 0;
      
        /**
         * Updates process status (non blocking)
         */
        virtual void Update() = 0;
      
        /**
         * Joins the process with the main process : blocks until complete exit of the process
         */
        virtual void Join() = 0;

        /**
         * Reads one line of the standard output of a redirected process
         * Returns true if the line could be read, false otherwise
         * Only works if the process has been redirected (redirectoutput set to true)
         */
        virtual bool ReadData(FString &data) = 0;
    };

    struct ENGINE_API FMemoryStats
    {
        /**
         * Maximum physical memory of the system (in bytes)
         */
        uint64 MaxPhysical;

        /**
         * Available physical memory of the system (in bytes)
         */
        uint64 FreePhysical;

        /**
         * Used physical memory of the system (in bytes)
         */
        uint64 UsedPhysical;
    };

    struct ENGINE_API FCPUStats
    {
        /**
         * The CPU clock speed in Mhz
         */
        uint32 ClockSpeed;

        /**
         * The current CPU clock speed in Mhz
         */
        uint32 CurClock;

        /**
         * The number of cores of this CPU
         */
        uint8 CoreCount;
    };

    class ENGINE_API IThread
    {
    public:
        virtual ~IThread() {}

        /**
         * Returns this thread's name
         */
        virtual const FString &GetName() const = 0;

        /**
         * Kills this thread
         */
        virtual void Kill() = 0;

        /**
         * Joins this thread
         */
        virtual void Join() = 0;

        /**
         * Returns true if this thread is still running
         */
        virtual bool IsRunning() const = 0;

        /**
         * Returns the ID of this thread
         */
        virtual int64 GetID() const = 0;
      
        /**
         * Returns the creation ID of this thread
         */
        virtual uint8 GetCreationID() const = 0;
    };

    class ENGINE_API IMutex
    {
    public:
        virtual ~IMutex() {}

        /**
         * Locks this mutex
         */
        virtual void Lock() = 0;

        /**
         * Unlocks this mutex
         */
        virtual void Unlock() = 0;
    };

    class ENGINE_API ISystemManager
    {
    public:
        virtual ~ISystemManager() {}

        /**
         * Creates a new thread
         * @param name the name of the thread to create
         * @param threadfunc the function to execute asynchronously
         */
        virtual IThread *CreateThread(const FString &name, const std::function<void()> &threadfunc) = 0;

        /**
         * Destroys a thread
         * @param thread thread to destroy
         */
        virtual void DestroyThread(IThread *thread) = 0;

        /**
         * Creates a mutex
         */
        virtual IMutex *CreateMutex() = 0;

        /**
         * Destroys a mutex
         * @param mutex mutex to destroy
         */
        virtual void DestroyMutex(IMutex *mutex) = 0;

        /**
         * Creates a child process
         * @param binname binary name of the image to execute in the new process
         * @param args the arguments
         * @param redirectoutput set to true if you require redirecting the output of the process to the main process
         */
        virtual IProcess *CreateProcess(const FString &binname, const FArray<FString> &args, const bool redirectoutput = false) = 0;

        /**
         * Destroys a process
         * @param process process to destroy
         */
        virtual void DestroyProcess(IProcess *process) = 0;

        /**
         * Returns the number of threads currently running
         */
        virtual uint16 GetThreadCount() const = 0;

        /**
         * Returns a given thread
         * @param id thread's creation index
         */
        virtual IThread *GetThread(const uint8 id) const = 0;

        /**
         * Returns the system's time and date in a structure
         */
        virtual FTimeDate GetSystemTime() const = 0;

        /**
         * Returns the system's time in milliseconds
         */
        virtual long GetSystemTimeMillis() const = 0;

        /**
         * Returns CPU statistics (recalculates)
         */
        virtual FCPUStats GetCPUStats() const = 0;

        /**
         * Returns memory statistics (recalculates)
         */
        virtual FMemoryStats GetMemoryStats() const = 0;
    };
    
    class ENGINE_API FMutexPtr
    {
    private:
        IMutex *Mutex;
    public:
        inline FMutexPtr()
        {
            if (FPlatform::GetBaseSystem() == Null)
                throw FRuntimeException("Mutex", "No system module");
            if ((Mutex = FPlatform::GetBaseSystem()->CreateMutex()) == Null)
                throw FRuntimeException("Mutex", "Creation failure");
        }
        inline ~FMutexPtr()
        {
            if (FPlatform::GetBaseSystem() != Null && Mutex != Null)
                FPlatform::GetBaseSystem()->DestroyMutex(Mutex);
        }
        inline IMutex *operator->() const noexcept
        {
            return (Mutex);
        }
    };

    class ENGINE_API FThreadPtr
    {
    private:
        IThread *Thread;
    public:
        inline FThreadPtr(const FString &name, const std::function<void()> &threadfunc)
        {
            if (FPlatform::GetBaseSystem() == Null)
                throw FRuntimeException("Thread", "No system module");
            if ((Thread = FPlatform::GetBaseSystem()->CreateThread(name, threadfunc)) == Null)
                throw FRuntimeException("Thread", "Creation failure");
        }
        inline ~FThreadPtr()
        {
            if (FPlatform::GetBaseSystem() != Null && Thread != Null)
                FPlatform::GetBaseSystem()->DestroyThread(Thread);
        }
        inline IThread *operator->() const noexcept
        {
            return (Thread);
        }
    };
};

#endif /* !SYSTEM_H_ */
