#pragma once 
#include <memory>
#include <vector>
#include <atomic>
#include "../mutex.h"
namespace zws {
   
class Memory {
public:
typedef std::shared_ptr<Memory> ptr;

    Memory(int block_size, int block_count);

    // void free();

    ~Memory();

    int getRefCount();

    char* getStart();

    char* getEnd();

    char* getBlock();

    void backBlock(char* s);

    bool hasBlock(char* s);

private:
    int m_block_size {0};
    int m_block_count {0};

    int m_size {0};
    char* m_start {NULL};
    char* m_end {NULL};

    std::atomic<int> m_ref_counts {0};
    std::vector<bool> m_blocks;
    Mutex m_mutex;

}; 
}