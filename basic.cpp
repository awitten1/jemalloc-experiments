#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jemalloc/jemalloc.h>
#include <cstdlib>
#include <tuple>

// Copied from 
// https://github.com/jemalloc/jemalloc/wiki/Use-Case%3A-Introspection-Via-mallctl%2A%28%29

//  BUILD_TYPE=Debug ./install-jemalloc-and-build.sh
// build in debug mode, or everything is optimized out.

// ./install/bin/jeprof --text build/basic my_prefix.63665.0.i0.heap 

// Dump memory profile after every 2^22=4MB of allocation activity.
//  MALLOC_CONF="prof:true,lg_prof_interval:22" ./build/basic 


void
do_something(size_t i)
{
        // Compare these two.  They will have different totals.

        // Leak some memory.
        // std::ignore = malloc(i * 1000);

        void* ptr = malloc(i * 1000);
        free(ptr);
}

int
main(int argc, char **argv)
{
        size_t i, sz;

        // This can be an absolute path though.
        const char* my_prefix = "my_prefix";
        mallctl("prof.prefix", NULL, NULL, (void*)(&my_prefix), sizeof(const char*));

        for (i = 0; i < 100; i++) {
                do_something(i);

                // Update the statistics cached by mallctl.
                uint64_t epoch = 1;
                sz = sizeof(epoch);
                mallctl("epoch", &epoch, &sz, &epoch, sz);

                // Get basic allocation statistics.  Take care to check for
                // errors, since --enable-stats must have been specified at
                // build time for these statistics to be available.
                size_t sz, allocated, active, metadata, resident, mapped;
                sz = sizeof(size_t);
                if (mallctl("stats.allocated", &allocated, &sz, NULL, 0) == 0
                    && mallctl("stats.active", &active, &sz, NULL, 0) == 0
                    && mallctl("stats.metadata", &metadata, &sz, NULL, 0) == 0
                    && mallctl("stats.resident", &resident, &sz, NULL, 0) == 0
                    && mallctl("stats.mapped", &mapped, &sz, NULL, 0) == 0) {
                        fprintf(stderr,
                            "Current allocated/active/metadata/resident/mapped: %zu/%zu/%zu/%zu/%zu\n",
                            allocated, active, metadata, resident, mapped);
                }
        }

        return (0);
}