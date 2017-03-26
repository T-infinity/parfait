#include "SyncPattern.h"
#include <MessagePasser.h>
#include <set>

SyncPattern::SyncPattern(const std::map<int, std::vector<long>>& send_to, const std::map<int, std::vector<long>>& receive_from)
        :send_to(send_to), receive_from(receive_from){}

std::map<int, std::vector<long>> getSendTo(const std::vector<long>& have_in,std::vector<long> need){
    std::set<long> have(have_in.begin(), have_in.end());
    std::map<int, std::set<long>> send_set;
    for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
        if(r == MessagePasser::Rank()) {
            MessagePasser::Broadcast(need, r);
        } else {
            std::vector<long> current_need;
            MessagePasser::Broadcast(current_need, r);
            for(auto thing : current_need){
                if(have.count(thing) != 0){
                    send_set[r].insert(thing);
                }
            }
        }
    }
    std::map<int, std::vector<long>> send_to;
    for(auto& pair : send_set){
        auto& target_rank = pair.first;
        auto& data = pair.second;
        send_to[target_rank] = std::vector<long>(data.begin(), data.end());
    }
    return send_to;
}

SyncPattern buildSyncPattern(const std::vector<long>& have_in, const std::vector<long>& need){
    auto send_to = getSendTo(have_in, need);

    std::vector<long> receive_count(MessagePasser::NumberOfProcesses());
    std::vector<MPI_Request> requests(MessagePasser::NumberOfProcesses());
    for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
        if(r == MessagePasser::Rank()) continue;
        MPI_Irecv(&receive_count[r], 1, MPI_LONG, r, MPI_ANY_TAG, MessagePasser::getCommunicator(), &requests[r]);
    }

    for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
        if(r == MessagePasser::Rank()) continue;
        long count = 0;
        if(send_to.count(r))
            count = send_to[r].size();
        MessagePasser::Send(count, r);
    }

    std::map<int, std::vector<long>> receive_from;
    std::vector<MessagePasser::MessageStatus> statuses;
    for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
        if(r == MessagePasser::Rank()) continue;
        MPI_Status s;
        MPI_Wait(&requests[r], &s);
    }

    for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
        if(r == MessagePasser::Rank()) continue;
        long count = receive_count[r];
        if(count != 0) {
            receive_from[r].resize(receive_count[r]);
            auto status = MessagePasser::NonBlockingRecv(receive_from[r], receive_from[r].size(), r);
            statuses.push_back(status);
        }
    }

    for(int r = 0; r < MessagePasser::NumberOfProcesses(); r++){
        if(r == MessagePasser::Rank()) continue;
        if(send_to.count(r)) {
            MessagePasser::Send(send_to[r], r);
        }
    }

    for(auto& s : statuses){
        MessagePasser::Wait(s);
    }

    return SyncPattern(send_to, receive_from);
}
