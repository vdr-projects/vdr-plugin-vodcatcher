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

#ifndef ___STRINGMESSAGEMOCK_H
#define ___STRINGMESSAGEMOCK_H

#include <string>
#include <vector>
#include <sstream>

class StringMessageMock
{
private:
    std::vector<std::string> expectations;
    std::vector<std::string> current;
    StringMessageMock* parent;
    std::string messagePrefix;

private:
    std::string MakeMethodMessage(std::string methodName, std::string argument1, std::string argument2);

protected:
    void AddMessage(std::string message);
    void AddMethod(std::string methodName, std::string argument1 = "", std::string argument2 = "");

public:
    StringMessageMock();
    StringMessageMock(StringMessageMock& parent, std::string messagePrefix = "");

    template<class T> std::string MakeString(const T& subject)
    {
        std::stringstream sstream;
        sstream << subject;
        return sstream.str();
    }

    void Expect(std::string expectation);
    void ExpectMethod(std::string methodName, std::string argument1 = "", std::string argument2 = "");
    void Verify();
    void Verify(const char* file, unsigned int line);
    void VerifyExpectations(const char* file, unsigned int line, bool ordered=true);
    void ResetMock();
};

#define VERIFY_EXPECTATIONS(mock) (mock).VerifyExpectations(__FILE__, __LINE__)
#define VERIFY_EXPECTATIONS_UNORDERED(mock) (mock).VerifyExpectations(__FILE__, __LINE__, false)
#define VERIFY(mock) (mock).Verify(__FILE__, __LINE__)

#endif // ___STRINGMESSAGEMOCK_H
