project(fishbowl)
find_package(KDE4 REQUIRED)
find_package(PkgConfig)
pkg_check_modules(CPPUNIT cppunit)

include_directories(${KDE4_INCLUDES} ${CPPUNIT_INCLUDES})

set(testui_sources
    src/TestUI.cpp
    src/SearchBar.cpp
    src/SearchProvider.cpp
    src/SuggestionBox.cpp
)
set(testui_moc_classes
    src/SearchBar.h
    src/SuggestionBox.h
)

QT_WRAP_CPP(testui_moc_files ${testui_moc_classes})
kde4_add_executable(testui ${testui_sources} ${testui_moc_files})
target_link_libraries(testui ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS})

set(testSearchSources
    src/SearchProvider.cpp
    src/TestSearch.cpp
)
kde4_add_executable( testsearch ${testSearchSources} )
target_link_libraries( testsearch ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} )

set(unitTestsSources
    src/UnitTest.cpp
    src/ResultTest.cpp
)
kde4_add_executable(unittests ${unitTestsSources})
target_link_libraries(unittests ${CPPUNIT_LDFLAGS} ${KDE4_KDEUI_LIBS})
