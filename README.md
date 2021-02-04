# spaceConsole

Communicate with space aliens using a weatherized hardware console attached to a 13.41 meter light beacon installed at about 3907' (in altitude) in a high desert. Dessert? whatever.

Requires 4800 ws2815b RGB LED pixels.

## The genetic algorithm

To find the best patterns for communicating with space aliens, we use a genetic algorithm that requires people called _burners_ to vote on how much they like the pattern they are currently looking at.

**pattern** (_n_) A unique combination of motion, colors, and pretty blinky lights displayed on the beacon.

**burner** (_n_) A passerby covered in dust.

Here's how it works.

- A **sequence** (_n_) is a 32 bit unsigned long that strictly defines the exact light show that the beacon will display. Every part of the 32 bit number affects some part of the **pattern** that will be displayed. The same sequence always produces the same pattern.

- Sequences can be displayed on the LED display in the form of 16-letter strings using only the letters A, C, G, and T, such that A = 00, C = 01, G = 10, and T = 11. This helps burners intuit what is going on even without an **artist** (_n_) to explain it to them.

We maintain a **breeding pool** in EEPROM of 120 sequences.

- The number 120 is chosen because a sequence is typically displayed for about 30 seconds, so on average we work through a pool in about an hour.

- Each sequence has an associated **fitness** score from 1-5.

- When the artist resets the EEPROM, the breeding pool is initialized to 120 completely random sequences, each with a fitness score of 1.

- The EEPROM is rewritten only every time 10 changes have been made to the pool, to avoid burning it out.

### Choosing the next sequence

The algorithm chooses a sequence to show to the burners by breeding two existing sequences from the pool as follows:

- Choose two **parent** sequences at random, however

  - Sequences with fitness 5 are 20x as likely to be chosen
  - Sequences with fitness 4 are 10x as likely to be chosen
  - Sequences with fitness 3 are 3x as likely to be chosen
  - Sequences with fitness 2 are 2x as likely to be chosen
  - Sequences with fitness 1 are 1x as likely to be chosen

- Generate a **child** sequence by assigning each bit, in sequence, to the value of either

  - The mommy sequence with probability 0.49
  - The daddy sequence with probability 0.49
  - A random number with probability 0.02 (**mutation**)

- Show the next sequence to the burner

### Responding to voting

The burner will **vote** (_v_) using shiny brightly colored buttons that are fun to push, to indicate their preference for the new sequence. That vote then becomes the fitness of that sequence.

- If there is no vote at all (burner is drunk or high), nothing happens (the new sequence is discarded).

- If a vote is received, the new sequence tries to displace another sequence in the pool that has the lowest fitness score. In other words, we first displace all the 1-fitness sequences from the pool, then all the 2-fitness ones, etc.

- However, we never displace a sequence with a lower fitness than our own fitness. So for example if the pool is already full of 3-, 4-, and 5-fitness sequences, and the new sequence received a vote of 2, it is discarded.

- Thus the sum of all the fitnesses in the pool is monotonically increasing as fast as possible.

- Theoretically, the pool will be filled with only fitness 5 sequences as soon as 120 5-votes have been received.
