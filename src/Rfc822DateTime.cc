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
 * $Id: Rfc822DateTime.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include "Rfc822DateTime.h"
#include <stdlib.h>

using namespace std;

Rfc822DateTime::Rfc822DateTime(string rfc822DateTimeString)
{
    this->rfc822DateTimeString = rfc822DateTimeString;
    ParseRfc822DateTimeString(rfc822DateTimeString);
}

const string Rfc822DateTime::ToShortString() const
{
    if ((year.size() != 4) || (month.size() != 2) || (day.size() != 2))
    {
        return "??.??.??";
    }
    else
    {
        return day + '.' + month + '.' + year.substr(2, 2);
    }
}

const string Rfc822DateTime::ToLongString() const
{
    return rfc822DateTimeString;
}

void Rfc822DateTime::ParseRfc822DateTimeString(string rfc822DateTimeString)
{
    string zone = ExtractLastWord(rfc822DateTimeString);
    string time = ExtractLastWord(rfc822DateTimeString);

    year = ExtractLastWord(rfc822DateTimeString);
    month = ExtractLastWord(rfc822DateTimeString);
    day = ExtractLastWord(rfc822DateTimeString);

    month = MonthNameToNumber(month);

    if (year.length() == 2)
    {
        year = "20" + year;
    }

    if (day.length() == 1)
    {
        day = "0" + day;
    }
}

string Rfc822DateTime::ExtractLastWord(string& line)
{
    string lastWord;

    if (line != "")
    {
        unsigned int delimiterPos = line.rfind(" ");
        unsigned int wordPos;

        if (delimiterPos == string::npos)
        {
            delimiterPos = 0;
            wordPos = 0;
        }
        else
        {
            wordPos = delimiterPos + 1;
        }

        lastWord = line.substr(wordPos);
        line = line.substr(0, delimiterPos);
        line.erase(line.find_last_not_of(" ") + 1);
    }

    return lastWord;
}

string Rfc822DateTime::MonthNameToNumber(string monthName)
{
    static const string months("JanFebMarAprMayJunJulAugSepOctNovDec");

    monthName = monthName.substr(0, 3);

    unsigned int monthIndex = months.find(monthName);

    if (monthIndex == string::npos)
    {
        return "";
    }
    else
    {
        char* formattedMonth;
        asprintf(&formattedMonth, "%.2d", 1 + monthIndex / 3);
        string monthNumber = formattedMonth;
        free(formattedMonth);

        return monthNumber;
    }
}
