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
private:
    std::map<int, std::vector<T>> itemsToSendToRank;
    std::map<int, std::vector<T>> itemsReceivedFromRank;
};

template <typename T>
void MessageBuilder<T>::sendItems(const std::vector<T> &items, int destination){
    for(auto n : items)
        itemsToSendToRank[destination].push_back(n);
}
template <typename T>
void MessageBuilder<T>::finishSends(){
    for(int target = 0; target < MessagePasser::NumberOfProcesses(); target++){
        if(itemsToSendToRank.count(target) == 0){
            std::vector<T> dummy;
            MessagePasser::Send(dummy, target);
        }
        if(target == MessagePasser::Rank())
            itemsReceivedFromRank[MessagePasser::Rank()] = itemsToSendToRank[MessagePasser::Rank()];
        else
            MessagePasser::Send(itemsToSendToRank[target], target);
    }
    itemsToSendToRank.clear();
}
template <typename T>
std::vector<T> MessageBuilder<T>::recvItemsFrom(int source){
    if(itemsReceivedFromRank.count(source) != 0){
        auto output = itemsReceivedFromRank[source];
        itemsReceivedFromRank.erase(source);
        return output;
    }
    else {
        std::vector<T> items;
        MessagePasser::Recv(items, source);
        return items;
    }
}

#endif
