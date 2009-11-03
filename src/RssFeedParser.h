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
 * $Id: RssFeedParser.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___RSSFEEDPARSER_H
#define ___RSSFEEDPARSER_H

#include <string>
#include "IFeedParser.h"
#include "Item.h"
#include "RefPtr.h"

class IDownloadCache;
class TiXmlDocument;
class TiXmlNode;
class TiXmlElement;

class RssFeedParser : public IFeedParser
{
private:
    const IDownloadCache& _downloadCache;

private:
    RefPtr<TiXmlDocument> CreateRssDocument() const;
    RefPtr<TiXmlDocument> CreateRssDocument(std::istream& rssStream) const;
    std::string GetChildValue(const TiXmlNode& root, const std::string& childName) const;
    std::string GetValue(const TiXmlNode& node) const;
    const std::string GetAttribute(const TiXmlElement& element, const std::string name) const;
    Item ParseItem(const TiXmlNode& itemNode) const;
    void ParseStreamElement(const TiXmlElement& streamElement, Item& item) const;
    int StringToIntDefault(const std::string& str, const int defaultValue) const;

public:
    RssFeedParser(const IDownloadCache& downloadCache);
    // <IFeedParser>
    bool Parse(Feed& feed) const;
    // </IFeedParser>
};

#endif //___RSSFEEDPARSER_H
