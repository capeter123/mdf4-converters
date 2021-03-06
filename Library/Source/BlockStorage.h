#ifndef MDFSIMPLECONVERTERS_BLOCKSTORAGE_H
#define MDFSIMPLECONVERTERS_BLOCKSTORAGE_H

#include <map>
#include <unordered_map>
#include <set>

#include <boost/bimap.hpp>

#include "IBlockStorage.h"

#include "mio.h"

namespace mdf {

    // Sort the order of the set, such that it follows:
    // HD -> TX ->
    // Favor original file position if tied.
    struct MdfBlockSortingOrder {
        bool operator()(std::shared_ptr<MdfBlock> const& lhs, std::shared_ptr<MdfBlock> const& rhs) const;
    };

    struct BlockStorage : IBlockStorage {
        explicit BlockStorage(mio::shared_mmap_source mmap);

        [[nodiscard]] boost::bimap<uint64_t, std::shared_ptr<MdfBlock>> const& getBlockMap() const;
        std::map<uint64_t, std::shared_ptr<MdfBlock>> getPackedBlockMap();
        std::shared_ptr<MdfBlock> getBlockAt(uint64_t address) override;
    private:
        mio::shared_mmap_source mmap;
        boost::bimap<uint64_t, std::shared_ptr<MdfBlock>> blockMap;

        std::set<std::shared_ptr<MdfBlock>, MdfBlockSortingOrder> getLinkedBlocks(std::shared_ptr<MdfBlock> startBlock);
    };

}


#endif //MDFSIMPLECONVERTERS_BLOCKSTORAGE_H
