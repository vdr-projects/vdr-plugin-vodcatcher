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
 * $Id: Item.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___ITEM_H
#define ___ITEM_H

#include <string>
#include <vector>
#include "Rfc822DateTime.h"
#include "StreamType.h"

class Item
{
private:
    std::string _title;
    Rfc822DateTime _date;
    std::string _description;
    std::string _subFeedUrl;
    std::string _streams[3];
    std::string _streamLength;

public:
    Item(const std::string& title, const Rfc822DateTime& date, const std::string& description);
    const std::string GetTitle() const;
    const Rfc822DateTime GetDate() const;
    const std::string GetDescription() const;

    void SetSubFeedUrl(const std::string& url);
    const std::string GetSubFeedUrl() const;

    void SetVideoCastStream(const StreamType quality, const std::string& url);
    const std::string GetVideoCastStream(const StreamType quality) const;

    void SetStreamLength(const std::string& streamLength);
    const std::string GetStreamLength() const;

    bool IsVideocast() const;
    bool IsCategory() const;
};

typedef std::vector<Item> ItemList;

#endif // ___ITEM_H
