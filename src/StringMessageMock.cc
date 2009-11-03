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
#include "StringMessageMock.h"
#include <algorithm>

using namespace std;

StringMessageMock::StringMessageMock() :
    parent(NULL)
{
}

StringMessageMock::StringMessageMock(StringMessageMock& parent, string messagePrefix) :
    parent(&parent), messagePrefix(messagePrefix)
{
}

void StringMessageMock::AddMessage(string message)
{
    if (parent)
    {
        if (messagePrefix != "")
        {
            message = messagePrefix + ":" + message;
        }
        parent->AddMessage(message);
    }
    current.push_back(message);
}

void StringMessageMock::AddMethod(string methodName, string argument1, string argument2)
{
    AddMessage(MakeMethodMessage(methodName, argument1, argument2));
}

void StringMessageMock::Expect(string expectation)
{
    if (parent)
    {
        parent->Expect(expectation);
    }
    expectations.push_back(expectation);
}

void StringMessageMock::ExpectMethod(string methodName, string argument1, string argument2)
{
    Expect(MakeMethodMessage(methodName, argument1, argument2));
}

void StringMessageMock::VerifyExpectations(const char* file, unsigned int line, bool ordered)
{
    vector<string>::iterator expectation = expectations.begin();
    vector<string>::iterator found = current.begin();

    while (expectation != expectations.end())
    {
        found = find(ordered ? found : current.begin(), current.end(), *expectation);
        if (found == current.end())
        {
            _TS_FAIL(file, line, *expectation + " expected");
        }
        expectation++;
    }
}

void StringMessageMock::Verify(const char* file, unsigned int line)
{
    vector<string>::iterator i = expectations.begin();
    vector<string>::iterator j = current.begin();

    while (i != expectations.end() && j != current.end())
    {
        _TS_ASSERT_EQUALS(file, line, *i++, *j++)
    }

    if (i != expectations.end())
    {
        _TS_FAIL(file, line, (*i + " expected").c_str());
    }

    if (j != current.end())
    {
        _TS_FAIL(file, line, (*j + " was not expected").c_str());
    }
}

void StringMessageMock::Verify()
{
    vector<string>::iterator i = expectations.begin();
    vector<string>::iterator j = current.begin();

    while (i != expectations.end() && j != current.end())
    {
        TS_ASSERT_EQUALS(*i++, *j++)
    }

    if (i != expectations.end())
    {
        TS_FAIL((*i + " expected").c_str());
    }

    if (j != current.end())
    {
        TS_FAIL((*j + " was not expected").c_str());
    }
}

void StringMessageMock::ResetMock()
{
    expectations.clear();
    current.clear();
}

string StringMessageMock::MakeMethodMessage(string methodName, string argument1, string argument2)
{
    string message;

    message = methodName + "(";

    if (argument1 != "")
    {
        message = message + argument1;
    }

    if (argument2 != "")
    {
        message = message + ", " + argument2;
    }

    message = message + ")";

    return message;
}
