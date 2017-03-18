#include "FileCacheItem.h"
#include "HTTPNotFoundExceptionHandler.h"
#include <sys/stat.h>

// Load file content of the target file
void FileCacheItem::load ()
{
    //"load" the content from the sri_cli
    string response = cli->parse(file_name);
    cache_item_stream = response.c_str();
    if(response == NULL){
        throw(HTTPNotFoundExceptionHandler());
    }
    stat(file_name.c_str(), &cache_item_stat);
    

}
FileCacheItem::FileCacheItem()
{
    file_name = ""; // Set file name
    cache_item_stream = NULL; // Set cache_item_stream to NULL
    pthread_mutex_init (&mutex,NULL); // initialize fetchContent mutex
    pthread_mutex_init (&assign_mutex,NULL); // Initialize assignment operator mutex
}
// Parameterized Constructor
FileCacheItem::FileCacheItem(string p_file_name, SRI_CLI * p_cli)
{
    file_name = p_file_name; // Set file name
    cache_item_stream = NULL; // Set cache_item_stream to NULL
    load(); // Load file contents
    pthread_mutex_init (&mutex,NULL); // initialize fetchContent mutex
    pthread_mutex_init (&assign_mutex,NULL); // Initialize assignment operator mutex
    cli = p_cli
}
// Create another FileCacheItem with the latest content of the target file. This is done to be able to decouple the main object in the cache from instances that is returned to the service engines
FileCacheItem * FileCacheItem::fetchContent()
{
    pthread_mutex_lock(&mutex); // Lock mutex (Critical Section)
    try{ // Try executing the following code block looking from exceptions thrown
        struct stat attrib; // A file attribute structure
        if (stat(file_name.c_str(), &attrib) == -1) // Retrieve file attributes
        { // Could not fetch file attribute
            pthread_mutex_unlock(&mutex); // Unlock
            throw(HTTPNotFoundExceptionHandler()); // Throw exception
        }
        // Check if file modified since the last load
        if ( attrib.st_mtime != cache_item_stat.st_mtime )            load(); // Load if so
        // Create a new cache item
        FileCacheItem * fileCacheItem = new FileCacheItem();
        *fileCacheItem = *this; // Copy the current into it
        pthread_mutex_unlock(&mutex); // unlock mutex
        return fileCacheItem; // return cache item
    }catch (HTTPNotFoundExceptionHandler httpNotFoundExceptionHandler)
    { // Exception caught indicating that the target file could not be found
            pthread_mutex_unlock(&mutex); // Unlock
            throw(httpNotFoundExceptionHandler); // Propagate Exception
            return NULL;
    }
}

// Return a pointer to the time_string representing the last update date/time
char * FileCacheItem::getLastUpdateTime ()
{
    memset (time_string,0,100); // Initialize the time_string
    // Convert stat update time from timespec to tm
    struct tm tm = *gmtime(((time_t *) &cache_item_stat.st_mtime));
    // Format date string
    strftime(time_string, 100, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    return time_string; // return a pointer to the update time string
}
// Return a pointer to the current stream
char * FileCacheItem::getStream()
{
    return cache_item_stream; // return a pointer to the cache buffer
}
// Return the cache buffer size
long FileCacheItem::getSize()
{
    return cache_item_size; // return the cache buffer size
}

// Assignment operator
void FileCacheItem::operator=(const FileCacheItem & p_fileCacheItem)
{
    pthread_mutex_lock(&assign_mutex); // Lock for assignment
    // Dispose the buffer if already allocated
    if ( cache_item_stream != NULL) free (cache_item_stream);
    // Allocate buffer for the file content to copy from the passed parameter
    cache_item_stream = (char *) calloc(p_fileCacheItem.cache_item_size,sizeof(char));
    cache_item_size=p_fileCacheItem.cache_item_size; // Set buffer size
    cache_item_stat = p_fileCacheItem.cache_item_stat; // set states
    // Copy data 
    memcpy (cache_item_stream,p_fileCacheItem.cache_item_stream,cache_item_size);
    pthread_mutex_unlock(&assign_mutex); // Unlock
}

FileCacheItem::~FileCacheItem()
{
    // If cache_item_stream buffer is allocated free it.
    if ( cache_item_stream != NULL ) free (cache_item_stream);
    pthread_mutex_destroy (&mutex);  // Destroy the fetchContent mutex 
    pthread_mutex_destroy (&assign_mutex); // Destroy the assign mutex

}