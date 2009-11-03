#include <cxxtest/TestSuite.h>

#include <deque>
#include <string>
#include "RefPtr.h"

using namespace std;

template<class T> class Queue
{
private:
    std::deque< RefPtr<T> > _queue;

public:
    void Add(RefPtr<T> item)
    {
        _queue.push_front(item);
    }
    
    //void Remove(T item);
    RefPtr<T> GetNext()
    {
        if (_queue.empty())
        {
            return RefPtr<T>();
        }
        else
        {
            return _queue.back();
        }
    }
    //T GetByString(std::string url);
};

class A_queue : public CxxTest::TestSuite
{
public:
    void Should_by_empty_by_default()
    {
        Queue<string> queue;
        TS_ASSERT(!queue.GetNext().IsAssigned());
    }
    
    void Should_allow_to_add_items()
    {
        Queue<string> queue;
        queue.Add(RefPtr<string>(new string("item")));
        TS_ASSERT(queue.GetNext().IsAssigned());
        TS_ASSERT_EQUALS("item", *queue.GetNext());
    }
};
