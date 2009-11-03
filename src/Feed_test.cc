/*
 * vdr-vodcatcher - A plugin for the Linux Video Disk Recorder
 * Copyright (c) 2007 - 2008 Tobias Grimm <vdr@e-tobi.net>
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
 * $Id: Feed_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include "Feed.h"

class A_Feed : public CxxTest::TestSuite
{
public:
    void Should_have_an_url()
    {
        Feed feed("url");
        TS_ASSERT_EQUALS("url", feed.GetUrl());
    }

    void Should_have_unknown_as_default_title()
    {
        Feed feed("foo");
        TS_ASSERT_EQUALS("unknown", feed.GetTitle());
    }

    void Should_have_an_assignable_title()
    {
        Feed feed("");
        feed.SetTitle("bar");
        TS_ASSERT_EQUALS("bar", feed.GetTitle());
    }

    void Should_allow_to_add_items()
    {
        Feed feed("");
        Item item1 = Item("item1", Rfc822DateTime(""), "");
        Item item2 = Item("item2", Rfc822DateTime(""), "");
        feed.AddItem(item1);
        feed.AddItem(item2);
        TS_ASSERT_EQUALS(2, (int) feed.GetItems().size());
        TS_ASSERT_EQUALS("item1", feed.GetItems()[0].GetTitle());
        TS_ASSERT_EQUALS("item2", feed.GetItems()[1].GetTitle());
    }

    void Should_have_a_default_time_to_live_of_720_minutes()
    {
        Feed feed("");
        TS_ASSERT_EQUALS(720, feed.GetTimeToLive());
    }
};
