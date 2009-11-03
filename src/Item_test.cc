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
#include "Item.h"
#include "RefPtr.h"

namespace
{

class Any_item : public CxxTest::TestSuite
{
private:
    RefPtr<Item> _item;

public:
    void setUp()
    {
        _item = new Item("title", Rfc822DateTime("01 Jan 2002 00:00::00 GMT"), "description");
    }

    void Should_have_a_title()
    {
        TS_ASSERT_EQUALS("title", _item->GetTitle());
    }

    void Should_have_a_date()
    {
        TS_ASSERT_EQUALS("01.01.02", _item->GetDate().ToShortString());
    }

    void Should_have_a_description()
    {
        TS_ASSERT_EQUALS("description", _item->GetDescription());
    }

    void Should_not_have_videocast_streams()
    {
        TS_ASSERT_EQUALS("", _item->GetVideoCastStream(High));
    }

    void Should_not_be_a_videocast_item()
    {
        TS_ASSERT(!_item->IsVideocast());
    }

    void Should_not_be_a_category_item()
    {
        TS_ASSERT(!_item->IsCategory());
    }

    void Should_have_an_empty_default_stream_length()
    {
        TS_ASSERT_EQUALS("", _item->GetStreamLength());
    }
};

class An_item_with_a_sub_feed_url : public CxxTest::TestSuite
{
private:
    RefPtr<Item> _item;

public:
    void setUp()
    {
        _item = new Item("title", Rfc822DateTime("01 Jan 2002 00:00::00 GMT"), "description");
        _item->SetSubFeedUrl("linkUrl");
    }

    void Should_have_a_SubFeedUrl()
    {
        TS_ASSERT_EQUALS("linkUrl", _item->GetSubFeedUrl());
    }

    void Should_be_a_category_item()
    {
        TS_ASSERT(_item->IsCategory());
    }
};

class An_item_with_videocast_streams_in_different_qualities : public CxxTest::TestSuite
{
private:
    RefPtr<Item> _item;

public:
    void setUp()
    {
        _item = new Item("title", Rfc822DateTime("01 Jan 2002 00:00::00 GMT"), "description");
        _item->SetVideoCastStream(High, "hqStreamUrl");
        _item->SetVideoCastStream(Medium, "mqStreamUrl");
        _item->SetVideoCastStream(Low, "lqStreamUrl");
        _item->SetStreamLength("12 min");
    }

    void Should_have_a_high_quality_stream()
    {
        TS_ASSERT_EQUALS("hqStreamUrl", _item->GetVideoCastStream(High));
    }

    void Should_have_a_medium_quality_stream()
    {
        TS_ASSERT_EQUALS("mqStreamUrl", _item->GetVideoCastStream(Medium));
    }

    void Should_have_a_low_quality_stream()
    {
        TS_ASSERT_EQUALS("lqStreamUrl", _item->GetVideoCastStream(Low));
    }

    void Should_be_a_videocast_item()
    {
        TS_ASSERT(_item->IsVideocast());
    }

    void Should_have_a_stream_length()
    {
        TS_ASSERT_EQUALS("12 min", _item->GetStreamLength());
    }
};

class An_item_with_only_a_low_quality_videocast_stream : public CxxTest::TestSuite
{
private:
    RefPtr<Item> _item;

public:
    void setUp()
    {
        _item = new Item("title", Rfc822DateTime("01 Jan 2002 00:00::00 GMT"), "description");
        _item->SetVideoCastStream(Low, "lqStreamUrl");
    }

    void Should_return_the_low_quality_stream_when_requesting_a_high_quality_stream()
    {
        TS_ASSERT_EQUALS("lqStreamUrl", _item->GetVideoCastStream(High));
    }

    void Should_return_the_low_quality_stream_when_requesting_a_medium_quality_stream()
    {
        TS_ASSERT_EQUALS("lqStreamUrl", _item->GetVideoCastStream(Medium));
    }

    void Should_be_a_videocast_item()
    {
        TS_ASSERT(_item->IsVideocast());
    }
};

};
