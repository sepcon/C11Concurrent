#include "headers/Libs/Utils/IDManager.h"


namespace thaf {
namespace util {


IDManager::IDType IDManager::allocateNewID()
{ 
    IDType id = INVALID_ID;
    std::lock_guard<std::mutex> lock(_mt);
    if(_fragmentedIDSet.empty())
    {
        if(_inUsedIDSet.size() < INVALID_ID)
        {
            id = static_cast<IDType>(_inUsedIDSet.size());
            _inUsedIDSet.insert(static_cast<IDType>(_inUsedIDSet.size()));
        }
    }
    else
    {
        id = *_fragmentedIDSet.begin();
        _fragmentedIDSet.erase(_fragmentedIDSet.begin());
        _inUsedIDSet.insert(id);
    }
    return id;
}

void IDManager::reclaimUsedID(IDManager::IDType id)
{
    std::lock_guard<std::mutex> lock(_mt);
    _inUsedIDSet.erase(id);
    _fragmentedIDSet.insert(std::move(id));
}

}
}