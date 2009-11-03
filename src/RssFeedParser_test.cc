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

#include <sstream>
#include <cxxtest/TestSuite.h>
#include "RssFeedParser.h"
#include "Feed.h"
#include "DownloadCacheMock.h"

using namespace std;

namespace
{

class FeedBuilder
{
public:

    static string CreateRssFeed(string title, string items)
    {
        string result;

        result = "<?xml version='1.0'?>";
        result += "<rss version='2.0'><channel>";
        result += CreateXmlTag("title", title);
        result += items;
        result += "</channel></rss>";

        return result;
    }

    static string CreateRssItem(string title, string pubDate, string description, string link="", string videoCasts="")
    {
        string result;

        result = "<item>";
        result += CreateXmlTag("title", title);
        result += CreateXmlTag("pubDate", pubDate);
        result += CreateXmlTag("description", description);
        result += link;
        result += videoCasts;
        result += "</item>";

        return result;
    }

    static string CreateSubFeed(string url)
    {
        return CreateXmlTag("videocast:subfeed", "", CreateAttribute("url", url));
    }

    static string CreateVideoCast(string url, string quality)
    {
        return CreateXmlTag("videocast:stream", "", CreateAttribute("quality", quality) + CreateAttribute("url", url));
    }

    static string CreateEnclosure(string url, string type)
    {
        return CreateXmlTag("enclosure", "", CreateAttribute("url", url) + CreateAttribute("type", type));
    }

    static string CreateTimeToLive(string timeToLive)
    {
        return CreateXmlTag("ttl", timeToLive);
    }

    static string CreateVideoCastStreamInfo(string streamLength)
    {
        return CreateXmlTag("videocast:streaminfo", "", CreateAttribute("length", streamLength));
    }

private:
    static string CreateXmlTag(string name, string value, string attributes="")
    {
        return "<" + name + " " + attributes + ">" + value + "</" + name + ">";
    }

    static string CreateAttribute(string name, string value)
    {
        if (value != "")
        {
            return name + "='" + value + "' ";
        }
        else
        {
            return "";
        }
    }
};

class A_RssFeedParser : public CxxTest::TestSuite
{
private:
    RefPtr<Feed> _feed;
    RefPtr<RssFeedParser> _parser;
    RefPtr<DownloadCacheMock> _downloadCache;

public:
    void setUp()
    {
        _feed = new Feed("");
        _downloadCache = new DownloadCacheMock();
        _parser = new RssFeedParser(*_downloadCache);
    }

    void Should_fail_to_parse_a_feed_that_does_not_exist_in_the_download_cache()
    {
        _downloadCache->streamReturnedByCache = NULL;

        TS_ASSERT(!_parser->Parse(*_feed));
    }

    void Should_fail_to_parse_an_invalid_feed()
    {
        _downloadCache->streamReturnedByCache = new stringstream("<invalidXml>");
        TS_ASSERT(!_parser->Parse(*_feed));
    }

    void Should_parse_the_feed_title()
    {
        _downloadCache->streamReturnedByCache = new stringstream(FeedBuilder::CreateRssFeed("FooTitle", ""));
        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(0, (int)_feed->GetItems().size());
        TS_ASSERT_EQUALS("FooTitle", _feed->GetTitle());
    }

    void Should_parse_a_single_item()
    {
        string item = FeedBuilder::CreateRssItem("ItemTitle", "3 Jun 2005 04:00:00 GMT", "ItemDescription", "", "");
        _downloadCache->streamReturnedByCache = new stringstream(FeedBuilder::CreateRssFeed("FooTitle", item));

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("ItemTitle", _feed->GetItems()[0].GetTitle());
        TS_ASSERT_EQUALS("ItemDescription", _feed->GetItems()[0].GetDescription());
        TS_ASSERT_EQUALS("03.06.05", _feed->GetItems()[0].GetDate().ToShortString());
    }

    void Should_parse_two_items()
    {
        string item1 = FeedBuilder::CreateRssItem("ItemTitle1", "", "", "", "");
        string item2 = FeedBuilder::CreateRssItem("ItemTitle2", "", "", "", "");

        _downloadCache->streamReturnedByCache = new stringstream(FeedBuilder::CreateRssFeed("FooTitle", item1 + item2));

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(2, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("ItemTitle1", _feed->GetItems()[0].GetTitle());
        TS_ASSERT_EQUALS("ItemTitle2", _feed->GetItems()[1].GetTitle());
    }
    
    void Should_ignore_tags_after_the_last_item()
    {
        string item = FeedBuilder::CreateRssItem("ItemTitle1", "", "", "", "");

        _downloadCache->streamReturnedByCache = new stringstream(FeedBuilder::CreateRssFeed("FooTitle", item + "<tag/>"));

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());
    }

    void Should_parse_an_item_with_a_sub_feed()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "",
            FeedBuilder::CreateSubFeed("linkUrl")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("linkUrl", _feed->GetItems()[0].GetSubFeedUrl());
    }

    void Should_parse_the_different_streams_of_different_quality()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "", "",
            FeedBuilder::CreateVideoCast("urlHigh", "high") + FeedBuilder::CreateVideoCast("urlMedium", "medium")
                + FeedBuilder::CreateVideoCast("urlLow", "low")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("urlHigh", _feed->GetItems()[0].GetVideoCastStream(High));
        TS_ASSERT_EQUALS("urlMedium", _feed->GetItems()[0].GetVideoCastStream(Medium));
        TS_ASSERT_EQUALS("urlLow", _feed->GetItems()[0].GetVideoCastStream(Low));
    }

    void Should_strip_all_html_from_the_description()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "<![CDATA[<br>test<br>]]>"));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("test", _feed->GetItems()[0].GetDescription());
    }

    void Should_ignore_a_videocast_stream_without_an_url()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "", "",
            FeedBuilder::CreateVideoCast("", "high")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("", _feed->GetItems()[0].GetVideoCastStream(High));
    }

    void Should_ignore_a_videocast_stream_without_a_quality()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "", "",
            FeedBuilder::CreateVideoCast("url", "")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("", _feed->GetItems()[0].GetVideoCastStream(High));
        TS_ASSERT_EQUALS("", _feed->GetItems()[0].GetVideoCastStream(Low));
        TS_ASSERT_EQUALS("", _feed->GetItems()[0].GetVideoCastStream(Medium));
    }

    void Should_parse_the_streaminfo()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "", "",
            FeedBuilder::CreateVideoCast("urlHigh", "high") + FeedBuilder::CreateVideoCastStreamInfo("12 min")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("12 min", _feed->GetItems()[0].GetStreamLength());
    }

    void Should_parse_the_feeds_time_to_live()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateTimeToLive("333"));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(333, _feed->GetTimeToLive());
    }

    void Should_ignore_an_invalid_time_to_live()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateTimeToLive("invalid"));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(720, _feed->GetTimeToLive());
    }

    void Should_add_a_video_encloser_as_low_type_stream()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "", "",
            FeedBuilder::CreateEnclosure("enclosureUrl", "video/dont-care")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("enclosureUrl", _feed->GetItems()[0].GetVideoCastStream(Low));
    }

    void Should_ignore_non_video_enclosures()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "", "",
            FeedBuilder::CreateEnclosure("enclosureUrl", "audio/mp3")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("", _feed->GetItems()[0].GetVideoCastStream(Low));
    }

    void Should_prefer_a_low_quality_videocast_stream_over_a_video_enclosure()
    {
        string feedXml = FeedBuilder::CreateRssFeed("", FeedBuilder::CreateRssItem("", "", "", "",
            FeedBuilder::CreateVideoCast("streamUrl", "low") + FeedBuilder::CreateEnclosure("enclosureUrl",
                "video/dont-care")));

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));
        TS_ASSERT_EQUALS(1, (int)_feed->GetItems().size());

        TS_ASSERT_EQUALS("streamUrl", _feed->GetItems()[0].GetVideoCastStream(Low));
    }
    
    void Should_convert_text_to_current_locale()
    {
        string feedXml = "<?xml version='1.0' encoding='UTF-8'?><rss><channel><title>Ã¤Ã¤Ã¤</title></channel></rss>";

        _downloadCache->streamReturnedByCache = new stringstream(feedXml);

        TS_ASSERT(_parser->Parse(*_feed));

        // TODO
        // TS_ASSERT_EQUALS("äää", _feed->GetTitle());
    }
};

};
