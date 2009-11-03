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
 * $Id: LocalFileCache_test.cc 7656 2008-08-09 21:07:45Z svntobi $
 *
 */

#include <time.h>
#include <sstream>
#include <cxxtest/TestSuite.h>
#include "LocalFileCache.h"
#include "SdbmHashCalculator.h"
#include "values.h"
#include <stdlib.h>

using namespace std;

namespace
{

const string CacheDir("test.cache");

class An_empty_LocalFileCache: public CxxTest::TestSuite
{
private:
    RefPtr<LocalFileCache> _localFileCache;

private:
    void CreateCachedFile(string fileName, long age)
    {
        string cachedFileName = SdbmHashCalculator::Calculate(fileName);

        time_t fileTime = time(NULL) - age * 60;
        struct tm *timeComponents = localtime(&fileTime);

        char formattedTime[11];
        strftime(formattedTime, 11, "%y%m%d%H%M", timeComponents);

        string touchCommand = string("touch -t ")  + formattedTime + " " +
          CacheDir + "/" + cachedFileName;

        system(touchCommand.c_str());
    }

public:
    void setUp()
    {
        system(("rm -rf " + CacheDir).c_str());
        system(("mkdir " + CacheDir).c_str());
        _localFileCache = new LocalFileCache(CacheDir);
    }

    void tearDown()
    {
        system(("rm -rf " + CacheDir).c_str());
    }

    void Should_not_return_any_data()
    {
        TS_ASSERT(NULL == _localFileCache->CreateStreamByUrl("foo://bar").get());
    }

    void Should_return_the_data_put_into_the_cache()
    {
        stringstream input("foo");
        _localFileCache->Put(input, "http://foo");

        RefPtr<istream> output = _localFileCache->CreateStreamByUrl("http://foo");
        TS_ASSERT(NULL != output.get());

        stringstream outputStr;
        outputStr << output->rdbuf();
        TS_ASSERT_EQUALS(input.str(), outputStr.str());
    }

    void Should_return_different_data_by_different_urls()
    {
        stringstream input1("foo");
        stringstream input2("bar");

        _localFileCache->Put(input1, "http://foo/baz");
        _localFileCache->Put(input2, "http://bar/baz");

        stringstream output1;
        stringstream output2;

        output1 << _localFileCache->CreateStreamByUrl("http://foo/baz")->rdbuf();
        output2 << _localFileCache->CreateStreamByUrl("http://bar/baz")->rdbuf();

        TS_ASSERT_EQUALS(input1.str(), output1.str());
        TS_ASSERT_EQUALS(input2.str(), output2.str());
    }

    void Should_return_the_age_of_an_existing_entry_in_minutes()
    {
        CreateCachedFile("testFile", 5);
        TS_ASSERT_EQUALS(5, _localFileCache->GetAgeInMinutes("testFile"));
    }

    void Should_return_the_maximum_age_for_a_non_existing_entry_in_minutes()
    {
        TS_ASSERT_EQUALS(MAXLONG, _localFileCache->GetAgeInMinutes("testFile"));
    }

    void Should_tell_if_a_file_is_cached()
    {
        TS_ASSERT(!_localFileCache->IsCached("testFile"));

        CreateCachedFile("testFile", 0);
        TS_ASSERT(_localFileCache->IsCached("testFile"));
    }

    void Should_not_clean_up_younger_files()
    {
        CreateCachedFile("testFile1", 5); // 5 minutes
        CreateCachedFile("testFile2", 5 * 60 * 24); // 5 days
        TS_ASSERT(_localFileCache->IsCached("testFile1"));
        TS_ASSERT(_localFileCache->IsCached("testFile2"));

        _localFileCache->CleanUp(10);

        TS_ASSERT(_localFileCache->IsCached("testFile1"));
        TS_ASSERT(_localFileCache->IsCached("testFile2"));
    }

    void Should_clean_up_older_files()
    {
        CreateCachedFile("testFile1", 5 * 60 * 24); // 5 days
        CreateCachedFile("testFile2", 10 * 60 * 24); // 10 days
        TS_ASSERT(_localFileCache->IsCached("testFile1"));
        TS_ASSERT(_localFileCache->IsCached("testFile2"));

        _localFileCache->CleanUp(10);

        TS_ASSERT(_localFileCache->IsCached("testFile1"));
        TS_ASSERT(!_localFileCache->IsCached("testFile2"));
    }

    void Should_delete_a_file_if_it_can_not_be_written_successfully()
    {
        // TODO: Any ideas how to test this?
    }
};

};
