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

#include "HtmlToText.h"

using namespace std;

string HtmlToText::Convert(std::string html)
{
    string result = "";
    bool isTag = false;

    string::const_iterator htmlIterator = html.begin();

    while (htmlIterator != html.end())
    {
        switch (*htmlIterator)
        {
            case '<':
            {
                isTag = true;
                break;
            }

            case '>':
            {
                isTag = false;
                break;
            }

            default:
            {
                if (!isTag)
                {
                     result = result + *htmlIterator;
                }
            }

        }
        htmlIterator++;
    }

    return result;
}
