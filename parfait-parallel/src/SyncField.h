#pragma once
#include <vector>
#include <map>
#include <MessagePasser.h>

template <typename T>
class SyncWrapper {
public:
    SyncWrapper(std::vector<T>& Q, const std::map<long, int>& global_to_local) :
            Q(Q), global_to_local(global_to_local){}

    inline T& getEntry(long global_id) {
        auto local = global_to_local.at(global_id);
        return Q[local];
    }

    inline void setEntry(long global_id, const T& q) {
        auto local = global_to_local.at(global_id);
        Q[local] = q;
    }

private:
    std::vector<T>& Q;
    const std::map<long, int>& global_to_local;
};

template <typename T, typename Field>
class Syncer {
public:
    Syncer(Field& f, const SyncPattern& syncPattern):f(f), syncPattern(syncPattern){}

    void start() {
        for(auto& pair : syncPattern.receive_from){
            auto& source = pair.first;
            auto& recv_list = pair.second;
            recv_buffer[source].resize(recv_list.size());
            auto s = MessagePasser::NonBlockingRecv(recv_buffer[source], recv_list.size(), source);
            recv_statuses.push_back(s);
        }
        for(auto& pair : syncPattern.send_to){
            auto& destination = pair.first;
            auto& send_list = pair.second;
            for(auto t : send_list){
                send_buffer[destination].push_back(f.getEntry(t));
            }
            auto s = MessagePasser::NonBlockingSend(send_buffer[destination], destination);
            send_statuses.push_back(s);
        }
    }
    void finish() {
        for(auto& s : recv_statuses){
            MessagePasser::Wait(s);
        }

        for(auto& pair : syncPattern.receive_from) {
            auto& source = pair.first;
            auto& recv_list = pair.second;
            for(int i = 0; i < recv_list.size(); i++){
                auto global_id = recv_list[i];
                f.setEntry(global_id, recv_buffer[source][i]);
            }
        }

        for(auto& s : send_statuses){
            MessagePasser::Wait(s);
        }
    }
private:

    Field& f;
    const SyncPattern& syncPattern;
    std::map<int, std::vector<T>> recv_buffer;
    std::map<int, std::vector<T>> send_buffer;
    std::vector<MessagePasser::MessageStatus> send_statuses;
    std::vector<MessagePasser::MessageStatus> recv_statuses;

};

template <typename T, typename Field>
void syncField(Field& f, const SyncPattern& syncPattern){
    Syncer<T, Field> syncer(f, syncPattern);
    syncer.start();
    syncer.finish();
}

template <typename T>
void syncVector(std::vector<T>& vec, std::map<long, int>& global_to_local, SyncPattern& sync_pattern){
    SyncWrapper<T> syncer(vec, global_to_local);
    syncField<T>(syncer, sync_pattern);
}
