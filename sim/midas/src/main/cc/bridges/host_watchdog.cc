#include "host_watchdog.h"

host_watchdog_t::host_watchdog_t(simif_t* sim, std::vector<std::string> &args):
        bridge_driver_t(sim),
        sim(sim) {
    log.open("heartbeat.csv", std::ios_base::out);
    if (!log.is_open()) {
        fprintf(stderr, "Could not heartbeat output file.\n");
        abort();
    }
    log << "Target Cycle (fastest), Seconds Since Start" << std::endl;
    time(&start_time);
}

void host_watchdog_t::tick(){
    if (trip_count == polling_interval)  {
        trip_count = 0;
        uint64_t current_cycle = sim->actual_tcycle();
        has_timed_out |= current_cycle == last_cycle;

        time_t current_time;
        time(&current_time);
        struct tm * tm_local = localtime(&current_time);
        log << current_cycle << ", " << current_time - start_time << std::endl;
        last_cycle = current_cycle;
    } else {
        trip_count++;
    }
}