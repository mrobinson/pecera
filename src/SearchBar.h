#ifndef SearchBar_h
#define SearchBar_h

#include <QObject>
#include <QLineEdit>
#include <cstdio>

#define SUGGESTION_LINE_PADDING 5

namespace Pecera
{

class SuggestionBox;
class SearchBar : public QLineEdit
{
    Q_OBJECT

public:
    SearchBar(QWidget* parent = 0);
    ~SearchBar();

private:
    SuggestionBox* m_suggestionBox;
};

}

#endif
