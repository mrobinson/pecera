#include "SearchBar.h"
#include "SuggestionBox.h"

namespace Pecera
{

SearchBar::SearchBar(QWidget* parent)
    : QLineEdit(parent)
    , m_suggestionBox(new SuggestionBox(
        this, Qt::Tool | Qt::Window | Qt::FramelessWindowHint))
{
    connect(this, SIGNAL(textChanged(const QString&)),
        m_suggestionBox, SLOT(lineEditChanged(const QString&)));
    QLineEdit::resize(600, sizeHint().height());
}

SearchBar::~SearchBar()
{
    delete m_suggestionBox;
}

}
