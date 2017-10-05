#pragma once
#include <vector>
#include <set>
#include <map>
#include <MessagePasser/MessagePasser.h>
#include <parfait/SyncPattern.h>
#include <parfait/SyncField.h>

namespace Parfait {
class GraphOrderings {
public:

    inline static std::map<long, long> buildDistributedToContiguous(MPI_Comm comm,
                                                                    const std::vector<long> &local_mesh_to_global_mesh,
                                                                    const std::vector<bool> &do_own_local_node) {

        auto mp = std::make_shared<MessagePasser>(comm);
        std::vector<long> have, need;
        std::tie(have, need) = buildHaveNeed(local_mesh_to_global_mesh, do_own_local_node);
        Parfait::SyncPattern sync_pattern = Parfait::SyncPattern::build(mp, have, need);

        int num_owned_nodes = countOwnedNodes(do_own_local_node);
        long contiguous_row_start = getRankRowStart(mp, num_owned_nodes);
        auto local_to_owned_compact = buildLocalToCompactOwned(do_own_local_node);

        auto local_to_petsc_global =
            assignOwnedContiguousGlobal(do_own_local_node, contiguous_row_start, local_to_owned_compact);

        auto global_mesh_to_local_mesh = buildGlobalToLocal(local_mesh_to_global_mesh);

        Parfait::syncVector(mp, local_to_petsc_global, global_mesh_to_local_mesh, sync_pattern);

        return buildGlobalToGlobalMap(local_mesh_to_global_mesh, local_to_petsc_global);
    }

    inline static int countOwnedNodes(const std::vector<bool> &do_own) {
        int count = 0;
        for (bool own : do_own)
            if (own) count++;
        return count;
    }

    inline static long getRankRowStart(const std::shared_ptr<MessagePasser> &mp, int num_owned_nodes) {
        std::vector<long> rows_per_rank(mp->NumberOfProcesses());
        mp->AllGather(long(num_owned_nodes), rows_per_rank);
        rows_per_rank.push_back(0);
        long start = 0;
        for (int r = 0; r < mp->Rank(); r++) {
            start += rows_per_rank[r];
        }
        return start;
    }

    inline static std::vector<int> buildLocalToCompactOwned(const std::vector<bool> &do_own_local_node) {
        std::vector<int> local_to_compact(do_own_local_node.size(), -1);
        int next_compact = 0;
        for (int local = 0; local < do_own_local_node.size(); local++) {
            if (do_own_local_node[local])
                local_to_compact[local] = next_compact++;
        }
        return local_to_compact;
    }

    inline static std::vector<long> assignOwnedContiguousGlobal(const std::vector<bool> &do_own_local_node,
                                                                long petsc_row_start,
                                                                const std::vector<int> &local_to_owned_compact) {
        std::vector<long> local_to_petsc_global(do_own_local_node.size(), -1);
        for (int local = 0; local < local_to_petsc_global.size(); local++) {
            if (do_own_local_node[local]) {
                local_to_petsc_global[local] = local_to_owned_compact[local] + petsc_row_start;
            }
        }
        return local_to_petsc_global;
    }

    inline static std::tuple<std::vector<long>, std::vector<long>> buildHaveNeed(
        const std::vector<long> &local_to_global,
        const std::vector<bool> &do_own_local) {
        std::vector<long> have, need;
        for (int local = 0; local < local_to_global.size(); local++) {
            if (do_own_local[local]) have.push_back(local_to_global[local]);
            else need.push_back(local_to_global[local]);
        }
        return {have, need};
    };

    inline static std::map<long, int> buildGlobalToLocal(const std::vector<long> &local_to_global) {
        std::map<long, int> global_to_local;
        for (int local = 0; local < local_to_global.size(); local++) {
            global_to_local[local_to_global[local]] = local;
        }
        return global_to_local;
    }

    inline static std::map<long, long> buildGlobalToGlobalMap(
        const std::vector<long> &local_to_global_distributed,
        const std::vector<long> &local_to_global_contiguous) {
        std::map<long, long> distributed_to_contiguous;
        for (int local = 0; local < local_to_global_contiguous.size(); local++) {
            distributed_to_contiguous[local_to_global_distributed[local]] = local_to_global_contiguous[local];
        }
        return distributed_to_contiguous;
    }
};
}
