#ifndef FILECACHEITEM_H
#define FILECACHEITEM_H

#include "includes.h"
// A FileCacheItem that encapsulates all the file aattributes including content.
// One of the most important things is that an object of that type represent the most receint content of the file.

#include "SRI_CLI.h"
/*
    How it works.  Basically, my idea was that the files can be anything, as long as a string uniquely represents it.
    Therefore, each SRI call can be interpreted as a filename.  It also works with the HTTP style, since something like INFERENCE
    requires a known state (the cache) and a way to update its knowledge (updating things in the cache).  We also want to do this in a multithreaded way,
    since new RULEs and FACTs could be added at any time
    Author of all this ADDITIONAL code: Cole Boyer
    I obviously take no credit for Prof Sobh's work.
*/

class FileCacheItem
{
    private:
        string file_name; // The file name
        char * cache_item_stream; // The file content
        long cache_item_size; // The file size
        // A stats object representing the file stats including the last modification date
        struct stat cache_item_stat; 
        char time_string[100]; // A character array to store last update date string
        void load (); // Load file content from disk to cache_item_stream
        pthread_mutex_t mutex;  // A mutex for locking while serving fetch content
        pthread_mutex_t assign_mutex; // A mutex for locking while executing the assignment operator

        SRI_CLI * cli;
    public:
        FileCacheItem(); // Default Constructor
        // Constructor setting and loading file attributes
        FileCacheItem(string p_file_name, SRI_CLI * cli);
        FileCacheItem * fetchContent(); // return a copy of the FileCacheItem object with the latest content
        char * getLastUpdateTime (); // return the last update date string
        char * getStream(); // Return a pointer to the current stream
        long getSize(); // Return the size of the target file.
        void operator=(const FileCacheItem & p_fileCacheItem); // Assignment Operator     
        ~FileCacheItem(); // Destructor
};

#endif // FILECACHEITEM_H