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

#ifndef ___REFPTR__H
#define ___REFPTR__H

#include <stddef.h>

//
// A small note: RefPtr can cope with incomplete types as described in
// http://www.octopull.demon.co.uk/arglib/TheGrin.html by Alan Griffiths
//

template<class X> class RefPtr
{
    typedef void (*DeleteFunction)(X* pointer);

private:
    X* managedPointer;
    int* refCounter;
    DeleteFunction deleteFunction;

public:
    explicit RefPtr(X* ptr = 0) :
        deleteFunction(DeleteManagedPointer)
    {
        ManagePointer(ptr);
    }

    RefPtr(const RefPtr& refPtr) :
        deleteFunction(DeleteManagedPointer)
    {
        AttachToForeignRefPtr(refPtr.get(), refPtr.refCounter);
    }

    ~RefPtr()
    {
        ReleasePointer();
    }

    X* get() const
    {
        return managedPointer;
    }

    X* operator->() const
    {
        return managedPointer;
    }

    X& operator*() const
    {
        return *managedPointer;
    }

    RefPtr& operator=(const RefPtr& refPtr)
    {
        if (this != &refPtr)
        {
            ReleasePointer();

            AttachToForeignRefPtr(refPtr.get(), refPtr.refCounter);
        }

        return *this;
    }

    RefPtr operator=(X* ptr)
    {
        if (this->managedPointer != ptr)
        {
            ReleasePointer();
            ManagePointer(ptr);
        }

        return *this;
    }

    template<class Y> RefPtr(const RefPtr<Y> &refPtr) :
        deleteFunction(DeleteManagedPointer)
    {
        AttachToForeignRefPtr(refPtr.get(), refPtr.refCounter);
    }

    bool IsAssigned() const
    {
        return (managedPointer != NULL);
    }

private:
    template<class Y> friend class RefPtr;

    void ReleasePointer()
    {
        if (*refCounter > 0)
        {
            (*refCounter)--;
        }
        else
        {
            deleteFunction(managedPointer);
            delete refCounter;
        }
    }

    void AttachToForeignRefPtr(X* foreignPtr, int* foreignRefCounter)
    {
        refCounter = foreignRefCounter;
        (*refCounter)++;
        managedPointer = foreignPtr;
    }

    void ManagePointer(X* ptr)
    {
        this->managedPointer = ptr;
        refCounter = new int(0);
    }

    static void DeleteManagedPointer(X* pointer)
    {
        delete pointer;
    }
};

#endif
