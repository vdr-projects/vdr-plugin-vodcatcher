/*
 * vdr-vodcatcher - A plugin for the Linux Video Disk Recorder
 * Copyright (c) 2007 - 2009 Tobias Grimm <vdr@e-tobi.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <cxxtest/TestSuite.h>
#include "Rfc822DateTime.h"

namespace
{

class Rfc822DateTimeTest : public CxxTest::TestSuite
{
public:
    void TestInvalidRfc822DateTime()
    {
        Rfc822DateTime dateTime("invalid");
        TS_ASSERT_EQUALS("??.??.??", dateTime.ToShortString());
        TS_ASSERT_EQUALS("invalid", dateTime.ToLongString());
    }

    void TestFullDate()
    {
        Rfc822DateTime dateTime("Tue, 10 Jun 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.06.03", dateTime.ToShortString());
        TS_ASSERT_EQUALS("Tue, 10 Jun 2003 04:00:00 GMT", dateTime.ToLongString());
    }

    void TestNoWeekday()
    {
        Rfc822DateTime dateTime("10 Jun 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.06.03", dateTime.ToShortString());
    }

    void TestSingleDigitDay()
    {
        Rfc822DateTime dateTime("1 Jun 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("01.06.03", dateTime.ToShortString());
    }

    void TestTwoDigitYear()
    {
        Rfc822DateTime dateTime("1 Jun 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("01.06.03", dateTime.ToShortString());
    }

    void TestAllMonths()
    {
        Rfc822DateTime dateTime1("10 Jan 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.01.03", dateTime1.ToShortString());

        Rfc822DateTime dateTime2("10 Feb 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.02.03", dateTime2.ToShortString());

        Rfc822DateTime dateTime3("10 Mar 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.03.03", dateTime3.ToShortString());

        Rfc822DateTime dateTime4("10 Apr 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.04.03", dateTime4.ToShortString());

        Rfc822DateTime dateTime5("10 May 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.05.03", dateTime5.ToShortString());

        Rfc822DateTime dateTime6("10 Jun 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.06.03", dateTime6.ToShortString());

        Rfc822DateTime dateTime7("10 Jul 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.07.03", dateTime7.ToShortString());

        Rfc822DateTime dateTime8("10 Aug 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.08.03", dateTime8.ToShortString());

        Rfc822DateTime dateTime9("10 Sep 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.09.03", dateTime9.ToShortString());

        Rfc822DateTime dateTime10("10 Oct 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.10.03", dateTime10.ToShortString());

        Rfc822DateTime dateTime11("10 Nov 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.11.03", dateTime11.ToShortString());

        Rfc822DateTime dateTime12("10 Dec 03 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.12.03", dateTime12.ToShortString());
    }

    void TestExtraSpacesInDate()
    {
        Rfc822DateTime dateTime("Wed,  21   Dec    2005     11:28     CET");
        TS_ASSERT_EQUALS("21.12.05", dateTime.ToShortString());
    }

    void TestLongMonthNames()
    {
        Rfc822DateTime dateTime1("Tue, 10 January 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.01.03", dateTime1.ToShortString());

        Rfc822DateTime dateTime2("Tue, 10 February 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.02.03", dateTime2.ToShortString());

        Rfc822DateTime dateTime3("Tue, 10 March 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.03.03", dateTime3.ToShortString());

        Rfc822DateTime dateTime4("Tue, 10 April 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.04.03", dateTime4.ToShortString());

        Rfc822DateTime dateTime5("Tue, 10 May 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.05.03", dateTime5.ToShortString());

        Rfc822DateTime dateTime6("Tue, 10 June 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.06.03", dateTime6.ToShortString());

        Rfc822DateTime dateTime7("Tue, 10 July 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.07.03", dateTime7.ToShortString());

        Rfc822DateTime dateTime8("Tue, 10 August 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.08.03", dateTime8.ToShortString());

        Rfc822DateTime dateTime9("Tue, 10 September 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.09.03", dateTime9.ToShortString());

        Rfc822DateTime dateTime10("Tue, 10 October 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.10.03", dateTime10.ToShortString());

        Rfc822DateTime dateTime11("Tue, 10 November 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.11.03", dateTime11.ToShortString());

        Rfc822DateTime dateTime12("Tue, 10 December 2003 04:00:00 GMT");
        TS_ASSERT_EQUALS("10.12.03", dateTime12.ToShortString());
    }
};

};
