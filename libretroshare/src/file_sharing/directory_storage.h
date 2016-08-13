#pragma once

#include <string>
#include <stdint.h>
#include <list>

#include "retroshare/rsids.h"
#include "retroshare/rsfiles.h"

#define NOT_IMPLEMENTED() { std::cerr << __PRETTY_FUNCTION__ << ": not yet implemented." << std::endl; }

class InternalFileHierarchyStorage ;

class DirectoryStorage
{
	public:
        DirectoryStorage(const std::string& local_file_name,const RsPeerId& pid) ;
        virtual ~DirectoryStorage() {}

        typedef int32_t EntryIndex ;
        static const EntryIndex NO_INDEX = 0xffffffff;

		void save() const ;

        virtual int searchTerms(const std::list<std::string>& terms, std::list<EntryIndex> &results) const { NOT_IMPLEMENTED() ; return 0;}
        virtual int searchHash(const RsFileHash& hash, std::list<EntryIndex> &results) const { NOT_IMPLEMENTED() ; return 0; }
        virtual int searchBoolExp(Expression * exp, std::list<EntryIndex> &results) const { NOT_IMPLEMENTED() ; return 0; }

		void getFileDetails(EntryIndex i) ;
        uint32_t getEntryType(const EntryIndex& indx) ;	                     // returns DIR_TYPE_*, not the internal directory storage stuff.
        virtual bool extractData(const EntryIndex& indx,DirDetails& d) =0;

		// This class allows to abstractly browse the stored directory hierarchy in a depth-first manner.
		// It gives access to sub-files and sub-directories below.
		//
		class DirIterator
		{
			public:
                DirIterator(const DirIterator& d) ;
                DirIterator(DirectoryStorage *d,EntryIndex i) ;

				DirIterator& operator++() ;
                EntryIndex operator*() const ;

                operator bool() const ;			// used in for loops. Returns true when the iterator is valid.

                // info about the directory that is pointed by the iterator

                std::string name() const ;
            private:
                EntryIndex mParentIndex ;		// index of the parent dir.
                uint32_t mDirTabIndex ;				// index in the vector of subdirs.
                InternalFileHierarchyStorage *mStorage ;

                friend class DirectoryStorage ;
        };
		class FileIterator
		{
			public:
                FileIterator(DirIterator& d);	// crawls all files in specified directory
                FileIterator(DirectoryStorage *d,EntryIndex e);		// crawls all files in specified directory

				FileIterator& operator++() ;
                EntryIndex operator*() const ;	// current file entry

                operator bool() const ;			// used in for loops. Returns true when the iterator is valid.

                // info about the file that is pointed by the iterator

                std::string name() const ;
                uint64_t size() const ;
                RsFileHash hash() const ;
                time_t modtime() const ;

            private:
                EntryIndex mParentIndex ;		// index of the parent dir.
                uint32_t   mFileTabIndex ;		// index in the vector of subdirs.
                InternalFileHierarchyStorage *mStorage ;
        };

        struct FileTS
        {
            uint64_t size ;
            time_t modtime;
        };

        EntryIndex root() const ;					// returns the index of the root directory entry.
        const RsPeerId& peerId() const { return mPeerId ; }
        int parentRow(EntryIndex e) const ;

        bool updateSubDirectoryList(const EntryIndex& indx,const std::set<std::string>& subdirs) ;
        bool updateSubFilesList(const EntryIndex& indx, const std::map<std::string, FileTS> &subfiles, std::map<std::string, FileTS> &new_files) ;
        bool removeDirectory(const EntryIndex& indx) ;

        bool updateFile(const EntryIndex& index,const RsFileHash& hash, const std::string& fname,  uint64_t size, time_t modf_time) ;
        bool updateHash(const EntryIndex& index,const RsFileHash& hash);

        void print();
        void cleanup();

    private:
		void load(const std::string& local_file_name) ;
		void save(const std::string& local_file_name) ;

        void loadNextTag(const unsigned char *data, uint32_t& offset, uint8_t& entry_tag, uint32_t& entry_size) ;
        void saveNextTag(unsigned char *data,uint32_t& offset,uint8_t entry_tag,uint32_t entry_size) ;

        // storage of internal structure. Totally hidden from the outside. EntryIndex is simply the index of the entry in the vector.

        std::string mFileName;
        RsPeerId mPeerId;

    protected:
        mutable RsMutex mDirStorageMtx ;

        InternalFileHierarchyStorage *mFileHierarchy ;
};

class RemoteDirectoryStorage: public DirectoryStorage
{
public:
    RemoteDirectoryStorage(const RsPeerId& pid,const std::string& fname) : DirectoryStorage(fname,pid) {}
    virtual ~RemoteDirectoryStorage() {}

    virtual bool extractData(const EntryIndex& indx,DirDetails& d)
    {
        NOT_IMPLEMENTED();
        return false;
    }
};

class LocalDirectoryStorage: public DirectoryStorage
{
public:
    LocalDirectoryStorage(const std::string& fname,const RsPeerId& own_id) : DirectoryStorage(fname,own_id) {}
    virtual ~LocalDirectoryStorage() {}

    void setSharedDirectoryList(const std::list<SharedDirInfo>& lst) ;
    void getSharedDirectoryList(std::list<SharedDirInfo>& lst) ;

    void updateShareFlags(const SharedDirInfo& info) ;
    bool convertSharedFilePath(const std::string& path_with_virtual_name,std::string& fullpath) ;

    /*!
     * \brief getFileInfo Converts an index info a full file info structure.
     * \param i index in the directory structure
     * \param info structure to be filled in
     * \return false if the file does not exist, or is a directory,...
     */
    bool getFileInfo(DirectoryStorage::EntryIndex i,FileInfo& info) ;

    virtual bool extractData(const EntryIndex& indx,DirDetails& d) ;

private:
    std::string locked_findRealRootFromVirtualFilename(const std::string& virtual_rootdir) const;

    std::map<std::string,SharedDirInfo> mLocalDirs ;	// map is better for search. it->first=it->second.filename
};











