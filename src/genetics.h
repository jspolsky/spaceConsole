#pragma once

// see README.md for terminology and algorithms 

typedef uint32_t sequence_t;    
typedef uint8_t fitness_t;

typedef struct poolentry_t {
    sequence_t sequence;
    fitness_t fitness;
    uint16_t p_cum;             // cumulative probability, in array
} poolentry_t;


namespace Genetics
{
    // How many sequences are in the gene pool?
    const size_t POOL_SIZE = 120;
    
    // Given a fitness level, what is the relative probability that
    // this sequence will be chosen to breed?
    const uint16_t PROB_FROM_FITNESS[6] = {0, 1, 2, 3, 10, 20};

    void setup();
    void init_random_pool();
    void add_child_to_pool(sequence_t sequence, fitness_t fitness);
    void calc_p_cum();
    void dump_pool_to_console();
    sequence_t get_random_parent();
    sequence_t get_child();
    void acgt_from_sequence(char* rgch, sequence_t sequence);

    void start_cycle();
    void loop();
    void record_vote(fitness_t fitness);
};
