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
 * $Id: Feed.h 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#ifndef ___FEED_H
#define ___FEED_H

#include <string>
#include <vector>
#include "Item.h"

class Feed
{
private:
    std::string url;
    std::string title;
    ItemList items;
    int _timeToLive;

public:
    Feed(const std::string& url);
    const std::string GetTitle() const;
    void SetTitle(const std::string& title);
    const std::string GetUrl() const;
    void AddItem(const Item& item);
    const ItemList GetItems() const;
    void SetTimeToLive(const int _timeToLive);
    int GetTimeToLive() const;
};

typedef std::vector<Feed> FeedList;

#endif // ___FEED_H
