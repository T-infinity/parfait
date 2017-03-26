#pragma once
#include <vector>
#include <map>

class SyncPattern {
public:
    SyncPattern(const std::map<int, std::vector<long>>& send_to, const std::map<int, std::vector<long>>& receive_from);

    std::map<int, std::vector<long>> send_to;
    std::map<int, std::vector<long>> receive_from;
};


SyncPattern buildSyncPattern(const std::vector<long>& have_in, const std::vector<long>& need);