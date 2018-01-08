#pragma once
#include <vector>
#include <map>
#include <set>
#include <MessagePasser/MessagePasser.h>

namespace Parfait {

    class SyncPattern {
    public:
        inline SyncPattern(const std::map<int, std::vector<long>> &send_to,
                                 const std::map<int, std::vector<long>> &receive_from)
                : send_to(send_to), receive_from(receive_from) {}

        static inline SyncPattern build(MessagePasser mp, const std::vector<long> &have_in, const std::vector<long> &need) {

            auto send_to = getSendTo(mp, have_in, need);
            auto receive_from = getReceiveFrom(mp, send_to);

            return SyncPattern(send_to, receive_from);
        }

        static inline SyncPattern build(MessagePasser mp,
                                        const std::vector<long> &have_in,
                                        const std::vector<long> &need,
                                        const std::vector<int>& need_from) {

            auto send_to = getSendTo(mp, have_in, need, need_from);
            auto receive_from = getReceiveFrom(mp, send_to);

            return SyncPattern(send_to, receive_from);
        }

        static inline std::map<int, std::vector<long>> getSendTo(
            MessagePasser mp,
            const std::vector<long> &have_in,
            std::vector<long> need, const std::vector<int>& need_from) {

            if(need.size() != need_from.size())
                throw std::logic_error("Attempted to build sync pattern with mismatched need, need_from");

            std::vector<std::vector<long>> i_need_from_rank(mp.NumberOfProcesses());
            for(int index = 0; index < need.size(); index++){
                long global = need[index];
                int from = need_from[index];
                i_need_from_rank.at(from).push_back(global);
            }

            std::vector<int> send_length_for_each_rank = determineHowManyISendToEachRank(mp, need_from);
            std::map<int, std::vector<long>> rank_needs_from_me;
            for(int r = 0; r < mp.NumberOfProcesses(); r++){
                if(send_length_for_each_rank[r] != 0)
                    rank_needs_from_me[r] = std::vector<long>(send_length_for_each_rank[r]);
            }

            std::vector<MessagePasser::MessageStatus> recv_statuses;
            for(auto& pair : rank_needs_from_me){
                int source_rank = pair.first;
                auto& vec = pair.second;
                recv_statuses.emplace_back(mp.NonBlockingRecv(vec, vec.size(), source_rank));
            }

            std::vector<MessagePasser::MessageStatus> send_statuses;
            for(int r = 0; r < i_need_from_rank.size(); r++){
                if(i_need_from_rank[r].size() == 0) continue;
                send_statuses.emplace_back(mp.NonBlockingSend(i_need_from_rank[r], i_need_from_rank[r].size(), r));
            }

            mp.WaitAll(recv_statuses);
            mp.WaitAll(send_statuses);

            return rank_needs_from_me;
        }

        static std::vector<int> determineHowManyISendToEachRank(MessagePasser mp,
                                                                const std::vector<int> &need_from) {
            std::vector<int> i_need_this_many_from_you(mp.NumberOfProcesses(), 0);
            for(int from : need_from){
                i_need_this_many_from_you[from]++;
            }

            std::vector<int> i_will_send_you_this_many(mp.NumberOfProcesses(), 0);
            std::vector<MessagePasser::MessageStatus> statuses;
            for(int r = 0; r < mp.NumberOfProcesses(); r++){
                statuses.emplace_back(mp.NonBlockingSend(i_need_this_many_from_you[r], r));
                statuses.emplace_back(mp.NonBlockingRecv(i_will_send_you_this_many[r], r));
            }

            mp.WaitAll(statuses);
            return i_will_send_you_this_many;
        }

        static inline std::map<int, std::vector<long>> getSendTo(
                MessagePasser mp,
                const std::vector<long> &have_in, std::vector<long> need) {

            std::set<long> have(have_in.begin(), have_in.end());
            std::map<int, std::set<long>> send_set;
            for (int r = 0; r < mp.NumberOfProcesses(); r++) {
                if (r == mp.Rank()) {
                    mp.Broadcast(need, r);
                } else {
                    std::vector<long> current_need;
                    mp.Broadcast(current_need, r);
                    for (auto thing : current_need) {
                        if (have.count(thing) != 0) {
                            send_set[r].insert(thing);
                        };
                    }
                }
            }
            std::map<int, std::vector<long>> send_to;
            for (auto &pair : send_set) {
                auto &target_rank = pair.first;
                auto &data = pair.second;
                send_to[target_rank] = std::vector<long>(data.begin(), data.end());
            }
            return send_to;
        }

        static inline std::map<int, std::vector<long>> getReceiveFrom(MessagePasser mp, const std::map<int, std::vector<long>>& send_to){
            std::vector<long> receive_count(mp.NumberOfProcesses());
            std::vector<MPI_Request> requests(mp.NumberOfProcesses());
            for (int r = 0; r < mp.NumberOfProcesses(); r++) {
                if (r == mp.Rank()) continue;
                MPI_Irecv(&receive_count[r], sizeof(long), MPI_CHAR, r, 0, mp.getCommunicator(), &requests[r]);
            }

            for (int r = 0; r < mp.NumberOfProcesses(); r++) {
                if (r == mp.Rank()) continue;
                long count = 0;
                if (send_to.count(r))
                    count = send_to.at(r).size();
                mp.Send(count, r);
            }

            std::map<int, std::vector<long>> receive_from;
            std::vector<MessagePasser::MessageStatus> statuses;
            for (int r = 0; r < mp.NumberOfProcesses(); r++) {
                if (r == mp.Rank()) continue;
                MPI_Status s;
                MPI_Wait(&requests[r], &s);
            }

            for (int r = 0; r < mp.NumberOfProcesses(); r++) {
                if (r == mp.Rank()) continue;
                long count = receive_count[r];
                if (count != 0) {
                    receive_from[r].resize(receive_count[r]);
                    auto status = mp.NonBlockingRecv(receive_from[r], receive_from[r].size(), r);
                    statuses.push_back(status);
                }
            }

            for (int r = 0; r < mp.NumberOfProcesses(); r++) {
                if (r == mp.Rank()) continue;
                if (send_to.count(r)) {
                    mp.Send(send_to.at(r), r);
                }
            }

            mp.WaitAll(statuses);

            return receive_from;
        };


        std::map<int, std::vector<long>> send_to;
        std::map<int, std::vector<long>> receive_from;
    };

}