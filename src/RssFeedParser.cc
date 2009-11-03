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
 * $Id: RssFeedParser.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "RssFeedParser.h"
#include "HtmlToText.h"
#include "Feed.h"
#include "IDownloadCache.h"
#include "tinyxml.h"
#include "Item.h"

using namespace std;

RssFeedParser::RssFeedParser(const IDownloadCache& downloadCache) :
    _downloadCache(downloadCache)
{
}

bool RssFeedParser::Parse(Feed& feed) const
{
    RefPtr<istream> source(_downloadCache.CreateStreamByUrl(feed.GetUrl()));

    if (source.IsAssigned())
    {
        RefPtr<TiXmlDocument> rssDoc = CreateRssDocument(*source);

        TiXmlHandle docHandle(rssDoc.get());
        const TiXmlNode* channelNode = docHandle.FirstChild("rss").FirstChild("channel").Node();
        if (channelNode)
        {
            feed.SetTitle(GetChildValue(*channelNode, "title"));

            feed.SetTimeToLive(StringToIntDefault(GetChildValue(*channelNode, "ttl"), feed.GetTimeToLive()));

            for (const TiXmlNode* itemNode = channelNode->FirstChild("item"); itemNode; itemNode
                = itemNode->NextSibling("item"))
            {
                Item item = ParseItem(*itemNode);
                feed.AddItem(item);
            }

            return true;
        }
    }
    return false;
}

RefPtr<TiXmlDocument> RssFeedParser::CreateRssDocument(istream& rssStream) const
{
    RefPtr<TiXmlDocument> rssDoc(new TiXmlDocument());
    rssStream >> *rssDoc;
    if (!rssDoc->Error())
    {
        return rssDoc;
    }

    return RefPtr<TiXmlDocument>();
}

string RssFeedParser::GetValue(const TiXmlNode& node) const
{
    const TiXmlNode* textNode = node.FirstChild();
    if (textNode)
    {
        return textNode->Value();
    }
    return "";
}

const string RssFeedParser::GetAttribute(const TiXmlElement& element, string name) const
{
    const string* value = element.Attribute(name);
    if (value != NULL)
    {
        return *value;
    }
    else
    {
        return "";
    }
}

string RssFeedParser::GetChildValue(const TiXmlNode& root, const string& childName) const
{
    const TiXmlNode* childNode = root.FirstChild(childName);
    if (childNode)
    {
        return GetValue(*childNode);
    }

    return "";
}

Item RssFeedParser::ParseItem(const TiXmlNode& itemNode) const
{
    string title = GetChildValue(itemNode, "title");
    string description = GetChildValue(itemNode, "description");
    string date = GetChildValue(itemNode, "pubDate");

    Item item(title, Rfc822DateTime(date), HtmlToText::Convert(description));

    const TiXmlElement* subFeedElement = itemNode.FirstChildElement("videocast:subfeed");
    if (subFeedElement)
    {
        item.SetSubFeedUrl(GetAttribute(*subFeedElement, "url"));
    }

    const TiXmlElement* enclosureElement = itemNode.FirstChildElement("enclosure");
    if (enclosureElement)
    {
        string type = GetAttribute(*enclosureElement, "type");
        if (type.find("video/") == 0)
        {
            item.SetVideoCastStream(Low, GetAttribute(*enclosureElement, "url"));
        }
    }

    const TiXmlElement* streamElement = itemNode.FirstChildElement("videocast:stream");
    while (streamElement)
    {
        ParseStreamElement(*streamElement, item);
        streamElement = streamElement->NextSiblingElement("videocast:stream");
    }

    const TiXmlElement* streamInfoElement = itemNode.FirstChildElement("videocast:streaminfo");
    if (streamInfoElement)
    {
        item.SetStreamLength(GetAttribute(*streamInfoElement, "length"));
    }

    return item;
}

void RssFeedParser::ParseStreamElement(const TiXmlElement& streamElement, Item& item) const
{
    string quality = GetAttribute(streamElement, "quality");
    string url = GetAttribute(streamElement, "url");

    if (quality == "high")
    {
        item.SetVideoCastStream(High, url);
    }
    else if (quality == "medium")
    {
        item.SetVideoCastStream(Medium, url);
    }
    else if (quality == "low")
    {
        item.SetVideoCastStream(Low, url);
    }
}

int RssFeedParser::StringToIntDefault(const string& str, const int defaultValue) const
{
    istringstream tempStream(str);
    int value;
    if (!(tempStream >> value))
    {
        value = defaultValue;
    }
    return value;
}
