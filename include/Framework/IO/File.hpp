#ifndef FILE_H_
# define FILE_H_

namespace Framework
{
    class ENGINE_API FFile
    {
    private:
        IFileStream *Stream;
        FString FullPath;
        FString FileName;
        FString FileExt;
        FString AbsolutePath;
    public:
        FFile(const FString &path);
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
        void ListFiles(FList<FFile> &files);

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
        inline const FString &GetPath() const
        {
            return (FullPath);
        }
      
        /**
         * Returns the absolute path constructed from this path,
         * That means a path that starts from system's root folder
         */
        inline const FString &GetAbsolutePath() const
        {
            return (AbsolutePath);
        }
      
        inline FFile operator+(const FFile &other) const
        {
            return (FFile(FullPath + "/" + other.FullPath));
        }

        inline FFile operator+(const FString &other) const
        {
            return (FFile(FullPath + "/" + other));
        }

        /**
         * Returns the file's name
         */
        inline const FString &GetFileName() const
        {
            return (FileName);
        }

        /**
         * Returns the file's extension
         */
        inline const FString &GetFileExt() const
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

        FString ToString() const;
    };
};

#endif /* !FILE_H_ */
