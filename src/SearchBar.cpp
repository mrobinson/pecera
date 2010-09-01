#include "Project.h"
#include "SearchBar.h"
#include "SuggestionBox.h"
#include <QFile>
#include <QProcess>
#include <QTabWidget>
#include <QStringList>
#include <QX11EmbedContainer>
#include <sys/stat.h>

namespace Pecera
{

SearchBar::SearchBar(QWidget* parent)
    : QLineEdit(parent)
    , m_suggestionBox(new SuggestionBox(this))
{
    connect(this, SIGNAL(textChanged(const QString&)),
        m_suggestionBox, SLOT(searchBarChanged(const QString&)));
    connect(this, SIGNAL(returnPressed()),
        m_suggestionBox, SLOT(returned()));
    QLineEdit::resize(600, sizeHint().height());
}

SearchBar::~SearchBar()
{
    delete m_suggestionBox;
}

}
