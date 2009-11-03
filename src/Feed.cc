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
 * $Id: Feed.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "Feed.h"

using namespace std;

Feed::Feed(const string& url) :
    url(url), _timeToLive(720)
{
    title = "unknown";
}

const string Feed::GetTitle() const
{
    return title;
}

void Feed::SetTitle(const string& title)
{
    (*this).title = title;
}

const string Feed::GetUrl() const
{
    return url;
}

const ItemList Feed::GetItems() const
{
    return items;
}

void Feed::SetTimeToLive(const int timeToLive)
{
    _timeToLive = timeToLive;
}

int Feed::GetTimeToLive() const
{
    return _timeToLive;
}

void Feed::AddItem(const Item& item)
{
    items.push_back(item);
}
