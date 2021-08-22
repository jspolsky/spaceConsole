#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Entropy.h>

#include "Alnum.h"
#include "Util.h"
#include "genetics.h"
#include "led.h"
#include "oled.h"

namespace Genetics
{
    poolentry_t pool[POOL_SIZE];
    sequence_t sequence_current;
    unsigned long sequence_start_time;
    fitness_t sequence_fitness; // 0 - no vote. Otherwise 1-5
    char sequence_as_string[17];

    void init_random_pool()
    {
        for (uint32_t i = 0; i < POOL_SIZE; i++)
        {
            pool[i].sequence = Entropy.random(0, 0xFFFFFFFF);
            pool[i].fitness = 1;
        }
        calc_p_cum();
    }

    void calc_p_cum()
    {
        uint16_t p_cum = 0;
        uint16_t sum = 0;
        for (uint32_t i = 0; i < POOL_SIZE; i++)
        {
            p_cum += PROB_FROM_FITNESS[pool[i].fitness];
            sum += pool[i].fitness;
            pool[i].p_cum = p_cum;
        }

        dbgprintf("Average fitness is now %f\n", (double)sum / (double)POOL_SIZE);
    }

    void add_child_to_pool(sequence_t sequence, fitness_t fitness)
    {
        // The pool is always sorted by fitness, so find the highest
        // spot this kid can be inserted while preserving that sort

        for (uint32_t i = POOL_SIZE - 1; i; i--)
        {
            if (pool[i].fitness <= fitness)
            {
                for (uint32_t j = 0; j < i; j++)
                {
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
        for (uint32_t i = 0; i < POOL_SIZE; i++)
        {
            dbgprintf("[%d] - '%x' %d %d\n", i,
                      pool[i].sequence,
                      pool[i].fitness,
                      pool[i].p_cum);
        }
    }

    sequence_t get_random_parent()
    {
        uint16_t p_cum_target = random(1, pool[POOL_SIZE - 1].p_cum + 1);
        for (uint32_t i = 0; i < POOL_SIZE; i++)
        {
            if (p_cum_target <= pool[i].p_cum)
                return pool[i].sequence;
        }

        dbgprintf("impossible!");
        return Entropy.random(0, 0xFFFFFFFF);
    }

    // This is the heart of the algorithm - generates a new
    // descendent of the pool by mating two parents
    sequence_t get_child()
    {
        sequence_t parents[2], child;
        parents[0] = get_random_parent();
        parents[1] = get_random_parent();
        child = 0x00000000;

        for (uint32_t mask = 0x80000000; mask; mask >>= 1)
        {

            int newbit;

            uint8_t r = random(0, 100);
            if (r < 2)
            {
                // mutation
                newbit = random(0, 1);
            }
            else if (r < 51)
            {
                newbit = (parents[0] & mask);
            }
            else
            {
                newbit = (parents[1] & mask);
            }

            if (newbit)
                child |= mask;
        }

        return child;
    }

    void setup()
    {
        Entropy.Initialize(); // hardware random numbers! wow!
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

        start_cycle();
    }

    void acgt_from_sequence(char *rgch, sequence_t sequence)
    {
        for (signed int shift = 30; shift >= 0; shift -= 2)
        {
            sequence_t mask = 3 << shift;
            sequence_t masked = sequence & mask;
            sequence_t unshifted = masked >> shift;
            *rgch++ = "ACGT"[unshifted];
        }
        *rgch = '\0';
    }

    void start_cycle()
    {
        sequence_current = get_child();
        sequence_start_time = millis();
        sequence_fitness = 0; // no vote yet
        Led::setButtonStatus(sequence_fitness);
        OLED::status(0, "Please vote");
        dbgprintf("new cycle, trying %b at time %d\n", sequence_current, sequence_start_time);
        acgt_from_sequence(sequence_as_string, sequence_current);
        Alnum::setMainMessage(sequence_as_string);
        Alnum::setSecondaryMessage("PLEASE RATE 1-5");
    }

    void loop()
    {
        unsigned long elapsed_time = millis() - sequence_start_time;
        unsigned long elapsed_seconds = elapsed_time / 1000;

        int remaining_seconds = 30 - elapsed_seconds;

        if (sequence_fitness > 3)
            remaining_seconds = 60 - elapsed_seconds;
        if (sequence_fitness == 1 || sequence_fitness == 2)
            remaining_seconds = 3 - elapsed_seconds;

        if (remaining_seconds < 0)
        {
            remaining_seconds = 0;
        }

        // EVERY_N_MILLISECONDS(250) {
        //     char rgch[16];
        //     sprintf(rgch, "Remaining: %ds", remaining_seconds);
        //     OLED::status(1, rgch);
        // }

        if (remaining_seconds <= 0)
        {
            if (sequence_fitness)
            {
                dbgprintf("vote recorded %d\n", sequence_fitness);
                add_child_to_pool(sequence_current, sequence_fitness);
            }
            start_cycle();
        }
    }

    void record_vote(fitness_t fitness)
    {
        sequence_fitness = fitness;
        Led::setButtonStatus(sequence_fitness);
        char s[17];
        sprintf(s, "RATED %d STARS", fitness);
        Alnum::setSecondaryMessage(s);
        Alnum::showOneTime(s);

        OLED::status(0, s);
    }

} // namespace Genetics
