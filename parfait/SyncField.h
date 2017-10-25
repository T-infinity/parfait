#pragma once
#include <vector>
#include <map>
#include "../MessagePasser/MessagePasser.h"

namespace Parfait {
    template<typename T>
    class SyncWrapper {
    public:
        SyncWrapper(std::vector<T> &Q, const std::map<long, int> &global_to_local) :
                Q(Q), global_to_local(global_to_local) {
        }

        inline T &getEntry(long global_id) {
            auto local = global_to_local.at(global_id);
            return Q[local];
        }

        inline void setEntry(long global_id, const T &q) {
            auto local = global_to_local.at(global_id);
            Q[local] = q;
        }

    private:
        std::vector<T> &Q;
        const std::map<long, int> &global_to_local;
    };

    template<typename T, typename Field>
    class Syncer {
    public:
        Syncer(std::shared_ptr<MessagePasser> mp,
               Field &f,
               const Parfait::SyncPattern &syncPattern) : mp(mp), f(f), syncPattern(syncPattern) {
        }

        void start() {
            for (auto &pair : syncPattern.receive_from) {
                auto &source = pair.first;
                auto &recv_list = pair.second;
                recv_buffer[source].resize(recv_list.size());
                auto s = mp->NonBlockingRecv(recv_buffer[source], recv_list.size(), source);
                recv_statuses.push_back(s);
            }
            for (auto &pair : syncPattern.send_to) {
                auto &destination = pair.first;
                auto &send_list = pair.second;
                for (auto t : send_list) {
                    send_buffer[destination].push_back(f.getEntry(t));
                }
                auto s = mp->NonBlockingSend(send_buffer[destination], destination);
                send_statuses.push_back(s);
            }
        }

        void finish() {
            mp->WaitAll(recv_statuses);

            for (auto &pair : syncPattern.receive_from) {
                auto &source = pair.first;
                auto &recv_list = pair.second;
                for (int i = 0; i < recv_list.size(); i++) {
                    auto global_id = recv_list[i];
                    f.setEntry(global_id, recv_buffer[source][i]);
                }
            }

            mp->WaitAll(send_statuses);
        }

    private:
        std::shared_ptr<MessagePasser> mp;
        Field &f;
        const Parfait::SyncPattern &syncPattern;
        std::map<int, std::vector<T>> recv_buffer;
        std::map<int, std::vector<T>> send_buffer;
        std::vector<MessagePasser::MessageStatus> send_statuses;
        std::vector<MessagePasser::MessageStatus> recv_statuses;
    };

    template<typename T, typename Field>
    void syncField(std::shared_ptr<MessagePasser> mp, Field &f, const Parfait::SyncPattern &syncPattern) {
        Syncer<T, Field> syncer(mp, f, syncPattern);
        syncer.start();
        syncer.finish();
    }

    template<typename T>
    void syncVector(std::shared_ptr<MessagePasser> mp, std::vector<T> &vec, const std::map<long, int> &global_to_local,
                    const Parfait::SyncPattern &sync_pattern) {
        SyncWrapper<T> syncer(vec, global_to_local);
        syncField<T>(mp, syncer, sync_pattern);
    }
}