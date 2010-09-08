#include "Result.h"
#include "FilenameSearchProvider.h"
#include <cppunit/extensions/HelperMacros.h>

namespace Pecera
{

class ResultTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ResultTest);
    CPPUNIT_TEST(testSetText);
    CPPUNIT_TEST(testAddExtent);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
    }

    void tearDown() 
    {
    }

    void testSetText()
    {
        Result* result = new FilenameSearchResult(0);
        QString text("dummmy text");
        result->setText(text);
        CPPUNIT_ASSERT(result->text() == text);
        delete result;
    }

    void testAddExtent()
    {
        Result* result = new FilenameSearchResult(0);
        CPPUNIT_ASSERT(result->extents().size() == 0);

        Extent first(12, 2); // index 12, length 2
        result->addExtent(first);
        CPPUNIT_ASSERT(result->extents().size() == 1);

        Extent second(3, 3); // index 3, length 3
        result->addExtent(second);
        CPPUNIT_ASSERT(result->extents().size() == 2);

        Extent third(7, 2); // index 7, length 2
        result->addExtent(third);
        CPPUNIT_ASSERT(result->extents().size() == 3);

        CPPUNIT_ASSERT(result->extents()[0] == second);
        CPPUNIT_ASSERT(result->extents()[1] == third);
        CPPUNIT_ASSERT(result->extents()[2] == first);
        delete result;
    }

    void testAddIntersectingExtents()
    {
        Result* result = new FilenameSearchResult(0);
        CPPUNIT_ASSERT(result->extents().size() == 0);

        Extent second(6, 3);
        result->addExtent(second);
        CPPUNIT_ASSERT(result->extents().size() == 1);

        // This should merge with the rear of the previous extent.
        Extent third(7, 2);
        result->addExtent(third);
        CPPUNIT_ASSERT(result->extents().size() == 1);

        Extent merged(6, 4);
        CPPUNIT_ASSERT(result->extents()[0] == merged);

        // This should merge with the front of the previous extent.
        Extent fourth(3, 4);
        result->addExtent(fourth);
        CPPUNIT_ASSERT(result->extents().size() == 1);

        merged.setStart(3);
        merged.setLength(9);
        CPPUNIT_ASSERT(result->extents()[0] == merged);

        Extent noop(3, 2);
        result->addExtent(noop);
        CPPUNIT_ASSERT(result->extents().size() == 1);
        CPPUNIT_ASSERT(result->extents()[0] == merged);

        Extent noop2(4, 2);
        result->addExtent(noop2);
        CPPUNIT_ASSERT(result->extents().size() == 1);
        CPPUNIT_ASSERT(result->extents()[0] == merged);
        delete result;
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(ResultTest);

}
