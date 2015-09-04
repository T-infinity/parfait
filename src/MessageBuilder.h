#ifndef PARFAIT_CELLMESSAGER_H
#define PARFAIT_CELLMESSAGER_H

#include <vector>
#include <map>
#include "MessagePasser.h"

template <typename T>
class MessageBuilder {
public:
    MessageBuilder() = default;
    void sendItems(const std::vector<T> &items, int destination);
    void finishSends();
    std::vector<T> recvItemsFrom(int source);
    void padEmptySendsToNonSelf();
private:
    std::map<int, std::vector<T>> itemsToSendToRank;
};

template <typename T>
void MessageBuilder<T>::sendItems(const std::vector<T> &items, int destination){
    for(auto n : items)
        itemsToSendToRank[destination].push_back(n);
}
template <typename T>
void MessageBuilder<T>::finishSends(){
    for(int target = 0; target < MessagePasser::NumberOfProcesses(); target++){
        MessagePasser::Send(itemsToSendToRank[target], target);
    }
    itemsToSendToRank.clear();
}
template <typename T>
std::vector<T> MessageBuilder<T>::recvItemsFrom(int source){
    std::vector<T> items;
    MessagePasser::Recv(items, source);
    return items;
}
template <typename T>
void MessageBuilder<T>::padEmptySendsToNonSelf() {
    for(int target = 0; target < MessagePasser::NumberOfProcesses(); target++){
        if(target == MessagePasser::Rank()) continue;
        if(itemsToSendToRank.count(target) != 0)
            continue;
        itemsToSendToRank[target] = {};
    }
}

#endif

