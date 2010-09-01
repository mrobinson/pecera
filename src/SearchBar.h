#ifndef SearchBar_h
#define SearchBar_h

#include <cstdio>

#include <QObject>
#include <QGroupBox>
#include <QLineEdit>
#include <QTabWidget>

#define SUGGESTION_LINE_PADDING 5

namespace Pecera
{

class SuggestionBox;
class Project;
class SearchBar : public QLineEdit
{
    Q_OBJECT

public:
    SearchBar(QWidget* parent=0);
    ~SearchBar();
    SuggestionBox* suggestionBox() { return m_suggestionBox; }
private:
    SuggestionBox* m_suggestionBox;
};

}

#endif
