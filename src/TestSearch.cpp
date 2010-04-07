#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QtAlgorithms>
#include <cstdio>

#include "Result.h"
#include "SearchProvider.h"

bool sortResultExtents(QPair<int, int> extentOne, QPair<int, int> extentTwo)
{
    return extentOne.first < extentTwo.first;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage %s wordfile query\n", argv[0]);
        return 1;
    }

    //QString wordFile(argv[1]);
    //QString query(argv[2]);
    //Pecera::NaiveSearchProvider provider(query, wordFile);

    //QList<Pecera::Result*> matches;
    //provider.getMatches(matches);

    //printf("Found %i matches\n", matches.size());
    //for (int i = 0; i < matches.size(); ++i) {
    //    Pecera::Result* result = matches[i];
    //    const QString& text = result->text();

    //    int currentIndex = 0;
    //    const QList<Pecera::Extent>& extents = result->extents();
    //    for (int j = 0; j < extents.size(); ++j) {
    //        const Pecera::Extent& extent = extents[j];

    //        // Print section before extent.
    //        printf("%s", text.mid(currentIndex, extent.start() - currentIndex).toUtf8().data());

    //        // Print extent with an underline.
    //        printf("%c[4m", (char) 27);
    //        printf("%s", text.mid(extent.start(), extent.length()).toUtf8().data());
    //        printf("%c[0m", (char) 27);

    //        currentIndex = extent.end();
    //    }

    //    if (currentIndex < text.size())
    //        printf("%s", text.right(text.size() - currentIndex).toUtf8().data());
    //    printf("\n");
    //}

    return 0;
}
