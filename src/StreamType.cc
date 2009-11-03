#include "StreamType.h"

using namespace std;

string StreamTypeToString(StreamType type)
{
    switch (type)
    {
        case Low:
            return tr("Low");
        case Medium:
            return tr("Medium");
        case High:
            return tr("High");
    }
    return "";
}
