#include "Window.h"
#include "SearchBar.h"

namespace Pecera
{

Window::Window(QWidget* parent)
    : QGroupBox(parent)
    , m_searchBar(new SearchBar())
{
    m_tabs.setTabsClosable(true);

    m_layout.addWidget(m_searchBar);
    m_layout.addWidget(&m_tabs);
    setLayout(&m_layout);
}

Window::~Window()
{
    delete m_searchBar;
}

void Window::closeEvent(QCloseEvent* event)
{
    m_searchBar->closeSuggestionBox();
    QGroupBox::closeEvent(event);
}

void Window::focusSearchBar()
{
    std::cout << "focusSearchBar" << std::endl;
    m_searchBar->selectAll();
    m_searchBar->setFocus();
}


}
