#ifndef FilenameProvider_h
#define FilenameProvider_h

#include "SearchProvider.h"
#include "Result.h"

namespace Pecera
{
class File;

class FilenameSearchProvider : public SearchProvider
{
public:
    FilenameSearchProvider();
    ~FilenameSearchProvider() {}
    void performSearch(SearchTask* task);
};

class FilenameSearchResult : public Result
{
public:
    FilenameSearchResult(Project*);
    void executeAction();
    void setFile(File*);

private:
    Project* m_project;
};

}

#endif
