#include <Arduino.h>
#include <Entropy.h>

#include "Util.h"
#include "genetics.h"

namespace Genetics
{
    poolentry_t pool[POOL_SIZE];

    void init_random_pool()
    {
        for (uint32_t i = 0; i < POOL_SIZE; i++) {
            pool[i].sequence = Entropy.random(0, 0x10000);
            pool[i].fitness = 1;
        }
        calc_p_cum();
    }

    void calc_p_cum()
    {
        uint16_t p_cum = 0;
        for (uint32_t i = 0; i < POOL_SIZE; i++) {
            p_cum += PROB_FROM_FITNESS[pool[i].fitness];
            pool[i].p_cum = p_cum;
        }
    }

    void add_child_to_pool(sequence_t sequence, fitness_t fitness)
    {
        // The pool is always sorted by fitness, so find the highest
        // spot this kid can be inserted while preserving that sort

        for (uint32_t i = POOL_SIZE - 1; i; i--) {
            if (pool[i].fitness <= fitness) {
                for (uint32_t j = 0; j<i; j++) {
                    pool[j] = pool[j + 1];
                }
                pool[i].sequence = sequence;
                pool[i].fitness = fitness;
                break;
            }
        }

        calc_p_cum();
        
    }

    void dump_pool_to_console() 
    {
        for (uint32_t i = 0; i < POOL_SIZE; i++) {
            dbgprintf("[%d] - '%x' %d %d\n", i, 
                pool[i].sequence, 
                pool[i].fitness,
                pool[i].p_cum);
        }
    }

    sequence_t get_random_parent()
    {
        uint16_t p_cum_target = random( 1, pool[POOL_SIZE-1].p_cum + 1 );
        for (uint32_t i = 0; i < POOL_SIZE; i++)
        {
            if (p_cum_target <= pool[i].p_cum)
                return pool[i].sequence;
        }

        dbgprintf("impossible!");
        return Entropy.random(0, 0x10000);
    }

    // This is the heart of the algorithm - generates a new
    // descendent of the pool by mating two parents
    sequence_t get_child() 
    {
        sequence_t parents[2], child;
        parents[0] = get_random_parent();
        parents[1] = get_random_parent();
        child = 0x0000;

        for (uint32_t mask = 0x8000; mask; mask >>= 1) {

            int newbit;

            uint8_t r = random(0,100);
            if (r < 2) {
                // mutation
                newbit = random(0, 1);
            }
            else if (r < 51) {
                newbit = (parents[0] & mask);
            }
            else {
                newbit = (parents[1] & mask);
            }

            if (newbit) 
                child |= mask; 

        }

        return child;
    }

    void setup()
    {
        Entropy.Initialize();           // hardware random numbers! wow!
        randomSeed(Entropy.random(0, 0x10000));

        init_random_pool(); 

        // for (int i=0; i<200; i++)
        //     add_child_to_pool(get_child(), random(1,6));

        // dump_pool_to_console();         // testing
        
        // // now insert a 1
        // add_child_to_pool(get_child(), 1);        
        
        // dump_pool_to_console();

        // // now insert a 5
        // add_child_to_pool(get_child(), 5);
        // dump_pool_to_console();

    }
} // namespace Genetics