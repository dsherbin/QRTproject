#ifndef FILECACHE_H
#define FILECACHE_H
#include "FileCacheItem.h"
#include "SRI_CLI.h"

// A class that handles a cache to manage files.
class FileCache
{
    private:
        // A map that maps filenames to their corresponding FileCacheItem objects
        std::map <string,FileCacheItem *> cache;
        string root;

        SR_CLI * cli;
    public:
        FileCache(string p_root); // Default Constructor sets HTML root directory
        //Returns a pointer to the FileCacheItem object correposnindg to p_file_name
        FileCacheItem * getFile(string p_file_name);
        ~FileCache (); // Destructor
};

#endif // FILECACHE_H
