#ifndef FILESYSTEM_H_
# define FILESYSTEM_H_

#include "Framework/IO/Stream.hpp"

/**
 * This is low-level file system abstraction layer
 * Direct usage of IFileSystem is no longer necessary, prefer to use high level API FFile to represent a file.
 * Read/Write operations are done through IFileStream
 * Closing of files is not needed when using High-Level FFile (at least when correctly used)
 */
namespace Framework
{
    enum EFileMode
    {
        /**
         * Opens the file in read mode for either read binary or text data
         * Allows to freely seek to random positions in the file
         */
        FILEMODE_READ,

        /**
         * Opens the file in write mode for either writing binary or text data
         * Allows to freely seek to random positions in the file
         * Does not cause truncation and will create a file if does not exist
         */
        FILEMODE_WRITE,

        /**
         * The standard write mode, working same as FILEMODE_WRITE except that it performs file truncation
         */
        FILEMODE_OVERWRITE,

        /**
         * Open a file in both read and write modes,
         * Writing does not truncate and work like FILEMODE_WRITE
         */
        FILEMODE_READWRITE,

        /**
         * Open a file for appending to the end only (create a file if does not exist)
         * Writes either binary or text data
         * Does not support seeking
         */
        FILEMODE_APPEND
    };

    enum ENGINE_API ESystemPath
    {
        /**
         * The %appdata% folder under windows (will return the data path under android due to weird path policy)
         */
        SYSPATH_APPDATA,

        /**
         * The temporary files path (will return /data/local/tmp under android)
         */
        SYSPATH_TEMP,

        /**
         * The user home directory (usually /home/myusername under Unix - will return the data path under android due to weird path policy)
         */
        SYSPATH_USERHOME,

        /**
         * The path to the user's desktop folder (will return the data path under android due to weird path policy)
         */
        SYSPATH_USERDESKTOP,

        /**
         * The path to user's documents (will return the data path under android due to weird path policy)
         */
        SYSPATH_USERDOCS
    };

    /**
     * Interface to represent a stream to a file
     * ReadShort/Float/Int/Unsigned and WriteShort/Float/Int/Unsigned are enforced in little endian
     */
    class ENGINE_API IFileStream : public IInputStream, public IOutputStream
    {
    public:
        virtual ~IFileStream() {}
        virtual void Seek(const uint64 pos) = 0;
        virtual bool ReadLine(bpf::String &line) = 0;
        virtual uint8 ReadByte() = 0;
        virtual int16 ReadShort() = 0;
        virtual uint16 ReadUShort() = 0;
        virtual float ReadFloat() = 0;
        virtual int ReadInt() = 0;
        virtual uint32 ReadUInt() = 0;
        virtual uint32 Read(uint8 *buffer, const uint32 sizebytes) = 0;
        virtual void WriteString(const bpf::String &str) = 0;
        virtual void NewLine() = 0;
        virtual void WriteByte(const uint8 b) = 0;
        virtual void WriteShort(const int16 s) = 0;
        virtual void WriteUShort(const uint16 s) = 0;
        virtual void WriteFloat(const float f) = 0;
        virtual void WriteInt(const int i) = 0;
        virtual void WriteUInt(const uint32 u) = 0;
        virtual void Write(const uint8 *buffer, const uint32 sizebytes) = 0;
    };

    /**
     * Interface to integrate with the operating system's file system
     * Prefer to use FFile rather than this class directly
     */
    class ENGINE_API IFileSystem
    {
    public:
        virtual ~IFileSystem() {}
        virtual IFileStream *OpenFile(const bpf::String &file, const EFileMode mode) = 0;
        virtual void CloseFile(IFileStream *f) = 0;
        virtual bool FileExists(const bpf::String &file) const = 0;
        virtual bool IsDirectory(const bpf::String &file) const = 0;
        virtual void DeleteFile(const bpf::String &file) = 0;
        virtual void DeleteDirectory(const bpf::String &file, bool recursive=true) = 0;
        virtual void CreateDirectory(const bpf::String &file) = 0;
        virtual void ListFiles(const bpf::String &dir, bpf::List<bpf::String> &l) = 0;
        virtual uint64 GetFileSize(const bpf::String &file) const = 0;
        virtual FString GetSystemPath(const ESystemPath path) const = 0;
    };
};

#endif /* !FILESYSTEM_H_ */
