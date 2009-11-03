#include <cxxtest/TestSuite.h>

#include "StreamType.h"

class A_StreamType : public CxxTest::TestSuite
{
public:
    void Should_have_a_string_representation()
    {
        TS_ASSERT_EQUALS(std::string("i18n:Low"), StreamTypeToString(Low));
        TS_ASSERT_EQUALS(std::string("i18n:Medium"), StreamTypeToString(Medium));
        TS_ASSERT_EQUALS(std::string("i18n:High"), StreamTypeToString(High));
    }
};

