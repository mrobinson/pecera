project(pecera)
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)
find_package(PkgConfig)
find_package(X11)
pkg_check_modules(CPPUNIT cppunit)
pkg_check_modules(SQLITE sqlite3)
pkg_check_modules(QTSQL QtSql)
pkg_check_modules(GLIBC glib-2.0)

include_directories(${CPPUNIT_INCLUDES} ${GLIBC_INCLUDES} ${QTSQL_INCLUDES} ${SQLITE_INCLUDES})
include(${QT_USE_FILE})

# Pecera
SET(CMAKE_BUILD_TYPE Debug)
set(pecera_sources
    src/EditorTab.cpp
    src/EditorTabs.cpp
    src/FilenameSearchProvider.cpp
    src/Project.cpp
    src/SearchBar.cpp
    src/SuggestionBox.cpp
    src/SearchProvider.cpp
    src/TabbedProcess.cpp
    src/Pecera.cpp
    src/PeceraApplication.cpp
    src/Window.cpp
)

set(pecera_moc_classes
     src/EditorTab.h
     src/EditorTabs.h
     src/SearchBar.h
     src/SuggestionBox.h
     src/TabbedProcess.h
     src/PeceraApplication.h
     src/Window.h
)
QT_WRAP_CPP(pecera pecera_moc_files ${pecera_moc_classes})

add_executable(pecera ${pecera_sources} ${pecera_moc_files})
target_link_libraries(pecera ${QT_LIBRARIES} ${SQLITE_LIBRARIES} ${QTSQL_LIBRARIES} ${X11_LIBRARIES} ${GLIBC_LIBRARIES})

# Unit tests
set(unitTestsSources
    src/EditorTab.cpp
    src/EditorTabs.cpp
    src/FilenameSearchProvider.cpp
    src/Project.cpp
    src/SearchBar.cpp
    src/SuggestionBox.cpp
    src/SearchProvider.cpp
    src/TabbedProcess.cpp
    src/PeceraApplication.cpp
    src/UnitTest.cpp
    src/Window.cpp
)

QT_WRAP_CPP(unittests pecera_moc_files ${pecera_moc_classes})
add_executable(unittests ${unitTestsSources} ${pecera_moc_files})
target_link_libraries(unittests ${QT_LIBRARIES} ${SQLITE_LIBRARIES} ${QTSQL_LIBRARIES} ${X11_LIBRARIES} ${CPPUNIT_LDFLAGS})

