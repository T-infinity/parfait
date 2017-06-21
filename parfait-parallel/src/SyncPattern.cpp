#include "SyncPattern.h"
#include <MessagePasser.h>
#include <set>

SyncPattern::SyncPattern(const std::map<int, std::vector<long>>& send_to,
                         const std::map<int, std::vector<long>>& receive_from)
        : send_to(send_to), receive_from(receive_from) {}

std::map<int, std::vector<long>> getSendTo(
        std::shared_ptr<MessagePasser> mp,
        const std::vector<long>& have_in, std::vector<long> need) {

    std::set<long> have(have_in.begin(), have_in.end());
    std::map<int, std::set<long>> send_set;
    for (int r = 0; r < mp->NumberOfProcesses(); r++) {
        if (r == mp->Rank()) {
            mp->Broadcast(need, r);
        } else {
            std::vector<long> current_need;
            mp->Broadcast(current_need, r);
            for (auto thing : current_need) {
                if (have.count(thing) != 0) {
                    send_set[r].insert(thing);
                };
            }
        }
    }
    std::map<int, std::vector<long>> send_to;
    for (auto& pair : send_set) {
        auto& target_rank = pair.first;
        auto& data = pair.second;
        send_to[target_rank] = std::vector<long>(data.begin(), data.end());
    }
    return send_to;
}

SyncPattern
buildSyncPattern(std::shared_ptr<MessagePasser> mp, const std::vector<long>& have_in, const std::vector<long>& need) {

    auto send_to = getSendTo(mp, have_in, need);

    std::vector<long> receive_count(mp->NumberOfProcesses());
    std::vector<MPI_Request> requests(mp->NumberOfProcesses());
    for (int r = 0; r < mp->NumberOfProcesses(); r++) {
        if (r == mp->Rank()) continue;
        MPI_Irecv(&receive_count[r], sizeof(long), MPI_CHAR, r, 0, mp->getCommunicator(), &requests[r]);
    }

    for (int r = 0; r < mp->NumberOfProcesses(); r++) {
        if (r == mp->Rank()) continue;
        long count = 0;
        if (send_to.count(r))
            count = send_to[r].size();
        mp->Send(count, r);
    }

    std::map<int, std::vector<long>> receive_from;
    std::vector<MessagePasser::MessageStatus> statuses;
    for (int r = 0; r < mp->NumberOfProcesses(); r++) {
        if (r == mp->Rank()) continue;
        MPI_Status s;
        MPI_Wait(&requests[r], &s);
    }

    for (int r = 0; r < mp->NumberOfProcesses(); r++) {
        if (r == mp->Rank()) continue;
        long count = receive_count[r];
        if (count != 0) {
            receive_from[r].resize(receive_count[r]);
            auto status = mp->NonBlockingRecv(receive_from[r], receive_from[r].size(), r);
            statuses.push_back(status);
        }
    }

    for (int r = 0; r < mp->NumberOfProcesses(); r++) {
        if (r == mp->Rank()) continue;
        if (send_to.count(r)) {
            mp->Send(send_to[r], r);
        }
    }

    for (auto& s : statuses) {
        mp->Wait(s);
    }

    return SyncPattern(send_to, receive_from);
}
