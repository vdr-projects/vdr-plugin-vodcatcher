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
 * $Id: RefPtr_test.cc 7652 2008-08-05 21:37:57Z svntobi $
 *
 */

#include <cxxtest/TestSuite.h>
#include "RefPtr.h"
#include "StringMessageMock.h"

using namespace std;

class Foo : public StringMessageMock
{
public:
    Foo()
    {
    }

    Foo(StringMessageMock& parent, string messagePrefix = "") :
        StringMessageMock(parent, messagePrefix)
    {
    }

    ~Foo()
    {
        AddMethod("dtor");
    }

    int Bar(int arg)
    {
        return arg;
    }
};

class RefPtrTest : public CxxTest::TestSuite
{
public:
    void TestEmptyPtr()
    {
        RefPtr<Foo> ptr;
    }

    void TestSingleReference()
    {
        StringMessageMock mock;
        Foo* foo = new Foo(mock);

        mock.ExpectMethod("dtor");

        RefPtr<Foo>* ref = new RefPtr<Foo>(foo);
        delete ref;

        mock.Verify();
    }

    void TestGet()
    {
        Foo* foo = new Foo();
        RefPtr<Foo> ref(foo);
        TS_ASSERT_EQUALS(foo, ref.get());
    }

    void TestPointerOperator()
    {
        Foo* foo = new Foo();
        RefPtr<Foo> ref(foo);
        TS_ASSERT_EQUALS(10, ref->Bar(10));
    }

    void TestAstersikOperator()
    {
        Foo* foo = new Foo();
        RefPtr<Foo> ref(foo);
        TS_ASSERT_EQUALS(10, (*ref).Bar(10));
    }

    void TestMultipleReferences()
    {
        StringMessageMock mock;
        Foo* foo = new Foo(mock);

        RefPtr<Foo>* ref1 = new RefPtr<Foo>(foo);
        RefPtr<Foo>* ref2 = new RefPtr<Foo>(*ref1);
        RefPtr<Foo>* ref3 = new RefPtr<Foo>(*ref2);

        delete ref1;
        mock.Verify();

        delete ref3;
        mock.Verify();

        mock.ExpectMethod("dtor");
        delete ref2;
        mock.Verify();
    }

    void TestAssignmentOperator()
    {
        StringMessageMock mock;
        Foo* foo1 = new Foo(mock, "foo1");
        Foo* foo2 = new Foo(mock, "foo2");

        RefPtr<Foo>* ref1 = new RefPtr<Foo>(foo1);
        RefPtr<Foo>* ref2 = new RefPtr<Foo>(foo2);

        mock.ExpectMethod("foo2:dtor");

        *ref2 = *ref1;

        mock.Verify();
        TS_ASSERT_EQUALS(foo1, ref2->get());

        mock.ResetMock();
        mock.ExpectMethod("foo1:dtor");

        delete ref1;
        delete ref2;

        mock.Verify();
    }

    void TestConstAssignment()
    {
        StringMessageMock mock;
        Foo* foo1 = new Foo(mock, "foo1");
        Foo* foo2 = new Foo(mock, "foo2");

        const RefPtr<Foo>* ref1 = new RefPtr<Foo>(foo1);
        RefPtr<Foo>* ref2 = new RefPtr<Foo>(foo2);

        mock.ExpectMethod("foo2:dtor");

        *ref2 = *ref1;

        mock.Verify();
        TS_ASSERT_EQUALS(foo1, ref2->get());

        mock.ResetMock();
        mock.ExpectMethod("foo1:dtor");

        delete ref1;
        delete ref2;

        mock.Verify();
    }

    void TestSelfAssignment()
    {
        StringMessageMock mock;

        RefPtr<Foo> ref1(new Foo(mock));

        ref1 = ref1;

        mock.Verify();
    }

    void TestPointerAssignment()
    {
        StringMessageMock mock;

        Foo* foo1 = new Foo(mock, "foo1");
        RefPtr<Foo> ref(foo1);

        mock.ExpectMethod("foo1:dtor");

        Foo* foo2 = new Foo(mock, "foo2");
        ref = foo2;

        mock.Verify();
        TS_ASSERT_EQUALS(foo2, ref.get());
    }

    void TestPointerselfAssignment()
    {
        StringMessageMock mock;
        Foo* foo = new Foo(mock);

        RefPtr<Foo> ref(foo);

        ref = foo;

        mock.Verify();
    }

    void TestIncompletType()
    {
        // To some extend, imcomplete types can be handled as Alan Griffiths
        // dscribes in: http://www.octopull.demon.co.uk/arglib/TheGrin.html
        //
        // Unfortunately, I have absolutely no idea, how to write a test
        // case for this. So just watch the compiler warnings about
        // not beeing able to delete an incomplete type!
    }

    void TestConversionToConst()
    {
        RefPtr<int> nonConstContent(new int(99));
        RefPtr<const int> constContent(nonConstContent);
        TS_ASSERT_EQUALS(*nonConstContent, *constContent);
    }

    void TestIsAssigned()
    {
        RefPtr<int> ptr;
        TS_ASSERT(!ptr.IsAssigned());
        ptr = new int(3);
        TS_ASSERT(ptr.IsAssigned());
    }
};
