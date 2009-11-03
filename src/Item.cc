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

#include "Item.h"

using namespace std;

Item::Item(const string& title, const Rfc822DateTime& date, const string& description)
:_title(title), _date(date), _description(description)
{
}

const string Item::GetTitle() const
{
    return _title;
}

const Rfc822DateTime Item::GetDate() const
{
    return _date;
}

const string Item::GetDescription() const
{
    return _description;
}

void Item::SetSubFeedUrl(const string& url)
{
	_subFeedUrl = url;
}

const string Item::GetSubFeedUrl() const
{
    return _subFeedUrl;
}

void Item::SetVideoCastStream(const StreamType quality, const std::string& url)
{
	_streams[quality] = url;
}

const string Item::GetVideoCastStream(const StreamType quality) const
{
	if ((_streams[quality] == "") && (quality > Low))
	{
		return GetVideoCastStream(StreamType(quality - 1));
	}
	else
	{
		return _streams[quality];
	}
}

void Item::SetStreamLength(const string& streamLength)
{
    _streamLength = streamLength;
}

const string Item::GetStreamLength() const
{
    return _streamLength;
}

bool Item::IsVideocast() const
{
    return (Item::GetVideoCastStream(High) != "");
}

bool Item::IsCategory() const
{
    return (_subFeedUrl != "");
}
