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
};

template <typename T>
void MessageBuilder<T>::sendItems(const std::vector<T> &items, int destination){
    for(auto n : items)
        itemsToSendToRank[destination].push_back(n);
}
template <typename T>
void MessageBuilder<T>::finishSends(){
    for(auto &i : itemsToSendToRank)
        MessagePasser::Send(i.second, i.first);
}
template <typename T>
std::vector<T> MessageBuilder<T>::recvItemsFrom(int source){
    std::vector<T> items;
    MessagePasser::Recv(items, source);
    return items;
}

#endif
