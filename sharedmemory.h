#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <atomic>
#include <string>

const int DONT_UPDATE_MEM = -1;

struct sharedMemory {
    std::atomic<bool>    shared_string_filled     = ATOMIC_VAR_INIT(false);
    std::string          shared_string            = "";

    std::atomic<bool>    kill_simulator_thread    = ATOMIC_VAR_INIT(false);
    std::atomic<bool>    input_available          = ATOMIC_VAR_INIT(false);
    std::atomic<bool>    input_needed             = ATOMIC_VAR_INIT(false);
    std::atomic<bool>    simulator_running        = ATOMIC_VAR_INIT(false);
    std::atomic<int>     mem_index_to_update      = ATOMIC_VAR_INIT(false);
    std::atomic<bool>    reset_again              = ATOMIC_VAR_INIT(false);
};



#endif // SHAREDMEMORY_H
