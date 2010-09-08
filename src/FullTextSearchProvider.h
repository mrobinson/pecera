#ifndef FullTextSearchProvider_h
#define FullTextSearchProvider_h

#include "Result.h"
#include <QMessageBox>

namespace Pecera
{

class FullTextSearchResult : public Result
{
public:
    FullTextSearchResult(const QString& searchString)
    {
        QString resultText("Full text search for ");
        resultText.append(searchString);
        setText(resultText);
    }

    ~FullTextSearchResult() {}

    void executeAction()
    {
        QMessageBox::information(0, "Sorry", "There is no full text search yet, "
        "so just imagine that it's going very slowly.");
    }
};

}

#endif
