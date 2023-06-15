// This turing machine simulator uses 2 symbols: 0 and 1; 
// it can handle up to 4 states and a Halting state.
// It uses syntax compatible with https://turingmachinesimulator.com/

// Here is an example of a five tuple:
// 0b11000111       uint8_t
// 0b 11 0 001 1 1  the five components, in binary
//    q3,_  q1,x,>  and now in the turingmachinesimulator.com language

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HALTING_STATE 0b00010000

// Pass a five tuple in binary and return the current state in the turingmachinesimulator.com language.
char *curState(uint8_t five_tuple) {
  switch (five_tuple & 0b11000000) {
  case 0b00000000:
    return "q0";
  case 0b01000000:
    return "q1";
  case 0b10000000:
    return "q2";
  case 0b11000000:
    return "q3";
  default:
    __builtin_unreachable();
  }
}

// Return the reading symbol ( x or _ ) of a five tuple, corresponding to the third bit of a binary number.
char *readSymbol(uint8_t five_tuple) { return (five_tuple & 0b00100000) ? "x" : "_"; }

// Take a five tuple represented in binary and return only the information corresponding to
// the next state in the turingmachinesimulator.com language.
char *nextState(uint8_t five_tuple) {
  switch (five_tuple & 0b00011100) {
  case 0b00000000:
    return "q0";
  case 0b00000100:
    return "q1";
  case 0b00001000:
    return "q2";
  case 0b00001100:
    return "q3";
  case HALTING_STATE:
    return "qHalt";
  default:
    __builtin_unreachable();
  }
}

// Check the bit corresponding to the symbol to write on top of the reading symbol and
// return the result in human language.
char *writeSymbol(uint8_t five_tuple) { return (five_tuple & 0b00000010) ? "x" : "_"; }

// Return < or > (left or right) depending on the bit corresponding
// to the direction to move on the tape.
char *moveDirection(uint8_t five_tuple) { return (five_tuple & 0b00000001) ? "<" : ">"; }

// Take a uint8_t number and return a string containing the binary representation of it.
// Remember to call free() when done with the return value.
char *uint8_t_to_binary(uint8_t input) {
  char *binary_string = (char *)malloc(
      9); 
  memset(binary_string, '0', 8); 
  binary_string[8] = '\0';      

  for (int i = 7; i >= 0; i--) {
    if ((input >> i) & 1) {
      binary_string[7 - i] = '1';
    }
  }

  return binary_string;
}

typedef struct {
  uint8_t *array;
  int length;
} uint8_t_array;

// Remove all the blank characters from a string.
void remove_whitespace(char *str) {
  char *d = str;
  do {
    while (isspace(*str)) {
      str++;
    }
  } while ((*d++ = *str++));
}

// Take a string corresponding to a five tuple in the turingmachinesimulator.com
// language and return the binary version of that five tuple.
uint8_t fiveTupleFromString(char *str) {
  uint8_t result = 0;

  if (strncmp(str, "q0", 2) == 0) {
    result |= 0b00000000;
  } else if (strncmp(str, "q1", 2) == 0) {
    result |= 0b01000000;
  } else if (strncmp(str, "q2", 2) == 0) {
    result |= 0b10000000;
  } else if (strncmp(str, "q3", 2) == 0) {
    result |= 0b11000000;
  }
  str += 3;

  if (*str == 'x') {
    result |= 0b00100000;
  }
  str += 1;
  if (strncmp(str, "q0", 2) == 0) {
    result |= 0b00000000;
  } else if (strncmp(str, "q1", 2) == 0) {
    result |= 0b00000100;
  } else if (strncmp(str, "q2", 2) == 0) {
    result |= 0b00001000;
  } else if (strncmp(str, "q3", 2) == 0) {
    result |= 0b00001100;
  } else if (strncmp(str, "qHalt", 5) == 0) {
    result |= HALTING_STATE;
    str += 3;
  }

  str += 3;

  if (*str == 'x') {
    result |= 0b00000010;
  }
  str += 2;

  if (*str == '>') {
    result |= 0b00000000;
  } else if (*str == '<') {
    result |= 0b00000001;
  }

  return result;
}

// Check if remove_whitespace and fiveTupleFromString work as expected.
void assert_str_to_binary(void) {
  char fiveTupleStr[] = "q0, _\nq1, x, >";
  remove_whitespace(fiveTupleStr);
  uint8_t expected_value = 0b00000110;
  uint8_t actual_value = fiveTupleFromString(fiveTupleStr);
  char fiveTupleStr2[] = "q0, x\nq2, x, <";
  char fiveTupleStr3[] = "q3, _\nqHalt, x, >";
  remove_whitespace(fiveTupleStr2);
  remove_whitespace(fiveTupleStr3);
  assert(expected_value == actual_value);
  assert(fiveTupleFromString(fiveTupleStr2) == 0b00101011);
  assert(fiveTupleFromString(fiveTupleStr3) == 0b11010010);
}

// Return the number of commas in a delta function represented as a string.
int comma_count(char *str) {
  int i, count;
  for (i = 0, count = 0; str[i]; i++) {
    count += (str[i] == ',');
  }

  return count;
}

#define BUFFER_SIZE 14
// Take a delta function (written in the turingmachinesimulator.com language) represented
// as a string and return an array of uint8_t containing the five tuples that
// constitute that delta function.
uint8_t_array translate_from_string(char *delta) {
  char buf[BUFFER_SIZE];
  int length = comma_count(delta) / 3;
  uint8_t *array = malloc(length * sizeof(uint8_t));
  char *pos = delta;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < BUFFER_SIZE - 1; j++, pos++) {
      buf[j] = *pos;
      if (*pos == '<' || *pos == '>') {
        pos++;
        buf[j + 1] = 0;
        break;
      }
    }
    uint8_t five_tuple = fiveTupleFromString(buf);
    array[i] = five_tuple;
  }

  uint8_t_array result = {.array = array, .length = length};
  return result;
}

char bb4_9x_delta[] = "\
  q0, _\
  q1, x, >\
  \
  \
  q1, _\
  q2, _, >\
  \
  q2, _\
  q3, x, >\
  \
  q3, _\
  q0, x, <\
  \
  q0, x\
  q3, x, <\
  \
  q1, x\
  q1, x, >\
  \
  q3, x\
  q2, x, <\
  \
  q2, x\
  qHalt, x, >\
  ";
// Check if translate_from_string works as expected.
void assert_translate_from_string(void) {
  int num_of_commas = comma_count(bb4_9x_delta);
  assert(num_of_commas % 3 == 0);
  remove_whitespace(bb4_9x_delta);
  uint8_t_array bb4_9x = translate_from_string(bb4_9x_delta);
  uint8_t array[8] = {0b00000110, 0b01001000, 0b10001110, 0b11000011,
                      0b00101111, 0b01100110, 0b11101011, 0b10110010};
  uint8_t_array expected = {.array = array, .length = 8};

  for (int i = 0; i < bb4_9x.length; i++) {
    assert(bb4_9x.array[i] == expected.array[i]);
  }
}

// Print the bits of a uint64_t number.
void printBits(uint64_t value) {
  for (int i = 63; i >= 0; i--) {
    uint64_t mask = 1ULL << i;
    uint64_t bit = (value & mask) >> i;
    printf("%llu", bit);
  }
  printf("\n");
}

// Return the number of set bits in a uint64_t number.
int countSetBits(uint64_t value) {
  int count = 0;
  while (value != 0) {
    count += value & 1;
    value >>= 1;
  }
  return count;
}

char non_halting_delta[] = "\
q0, _\
q0, x, >\
";

char bouncing_delta[] = "\
q0, _\
q1, x, >\
\
q1, _\
q0, _, <\
\
q0, x\
q1, x, >\
";

// https://en.wikipedia.org/wiki/Busy_beaver#Examples
char wikipedia_answer[] = "\
  q0, _\
  q1, x, >\
  \
  q1, _\
  q0, x, <\
  \
  q2, _\
  qHalt, x, >\
  \
  q3, _\
  q3, x, >\
  \
  q0, x\
  q1, x, <\
  \
  q1, x\
  q2, _, <\
  \
  q2, x\
  q3, x, <\
  \
  q3, x\
  q0, _, >\
  ";

#define CURRENT_MASK 0b11000000
#define READ_MASK 0b00100000
#define NEXT_STATE_MASK 0b00011100
#define WRITE_MASK 0b00000010
#define MOVEMENT_MASK 0b00000001
#define MAX_ITERATIONS 128
#define COMPUTATION_OUT_OF_BOUNDS -1
uint64_t tape = 0;

// Simulate the behavior of a turing machine. This function takes a delta function that
// contains eight five tuples and returns the number of set bits on the tape
// where the set bits work as x's in the turingmachinesimulator.com language.
int bb4_sim(uint8_t_array delta) {
  tape = 0;
  int pos = 31;
  int iterations;
  uint8_t curState = 0b00000000;
  uint8_t nextState = 0b00000000;
  for(iterations = 0; iterations<MAX_ITERATIONS; iterations++) {
    uint8_t five_tuple;
    uint8_t curSymbol = (uint8_t) ((tape & (1ULL << pos)) >> pos) << 5;
    for (int i = 0; i < delta.length; i++) {
      five_tuple = delta.array[i];
      if (curState == (five_tuple & CURRENT_MASK) && (curSymbol == (five_tuple & READ_MASK))) {
        break;
      }
    }
    uint8_t newSymbol = five_tuple & WRITE_MASK;
    if (newSymbol) {
      tape |= (1ULL << pos);
    } else {
      tape &= ~(1ULL << pos);
    }
    nextState = five_tuple & NEXT_STATE_MASK;
    pos += (five_tuple & MOVEMENT_MASK)? 1 : -1;
    if(nextState == HALTING_STATE || pos < 0 || pos > 63){
      break;
    }
    curState = nextState << 4;
  }
  if(iterations >= MAX_ITERATIONS || pos < 0 || pos > 63){
    return COMPUTATION_OUT_OF_BOUNDS;
  }
  return countSetBits(tape);
}

// Check if bb4_sim works as expected.
void assert_bb4_sim(void){
  remove_whitespace(bb4_9x_delta);
  uint8_t_array bb4_9x = translate_from_string(bb4_9x_delta);

  remove_whitespace(non_halting_delta);
  uint8_t_array non_halting = translate_from_string(non_halting_delta); 

  remove_whitespace(bouncing_delta);
  uint8_t_array bouncing_machine = translate_from_string(bouncing_delta); 

  assert(bb4_sim(bb4_9x) == 9);
  assert(bb4_sim(non_halting) == COMPUTATION_OUT_OF_BOUNDS);
  assert(bb4_sim(bouncing_machine) == COMPUTATION_OUT_OF_BOUNDS);
}

// The following structs were printed out by generate_119_five_tuples.c
static uint8_t_array q0x = {
  .array = (uint8_t[]) {
    0b00100000, // q0,x q0,_,>
    0b00100001, // q0,x q0,_,<
    0b00100010, // q0,x q0,x,>
    0b00100011, // q0,x q0,x,<
    0b00100100, // q0,x q1,_,>
    0b00100101, // q0,x q1,_,<
    0b00100110, // q0,x q1,x,>
    0b00100111, // q0,x q1,x,<
    0b00101000, // q0,x q2,_,>
    0b00101001, // q0,x q2,_,<
    0b00101010, // q0,x q2,x,>
    0b00101011, // q0,x q2,x,<
    0b00101100, // q0,x q3,_,>
    0b00101101, // q0,x q3,_,<
    0b00101110, // q0,x q3,x,>
    0b00101111, // q0,x q3,x,<
    0b00110010, // q0,x qHalt,x,>
  },
  .length = 17
};

static uint8_t_array q1blank = {
  .array = (uint8_t[]) {
    0b01000000, // q1,_ q0,_,>
    0b01000001, // q1,_ q0,_,<
    0b01000010, // q1,_ q0,x,>
    0b01000011, // q1,_ q0,x,<
    0b01000100, // q1,_ q1,_,>
    0b01000101, // q1,_ q1,_,<
    0b01000110, // q1,_ q1,x,>
    0b01000111, // q1,_ q1,x,<
    0b01001000, // q1,_ q2,_,>
    0b01001001, // q1,_ q2,_,<
    0b01001010, // q1,_ q2,x,>
    0b01001011, // q1,_ q2,x,<
    0b01001100, // q1,_ q3,_,>
    0b01001101, // q1,_ q3,_,<
    0b01001110, // q1,_ q3,x,>
    0b01001111, // q1,_ q3,x,<
    0b01010010, // q1,_ qHalt,x,>
  },
  .length = 17
};

static uint8_t_array q1x = {
  .array = (uint8_t[]) {
    0b01100000, // q1,x q0,_,>
    0b01100001, // q1,x q0,_,<
    0b01100010, // q1,x q0,x,>
    0b01100011, // q1,x q0,x,<
    0b01100100, // q1,x q1,_,>
    0b01100101, // q1,x q1,_,<
    0b01100110, // q1,x q1,x,>
    0b01100111, // q1,x q1,x,<
    0b01101000, // q1,x q2,_,>
    0b01101001, // q1,x q2,_,<
    0b01101010, // q1,x q2,x,>
    0b01101011, // q1,x q2,x,<
    0b01101100, // q1,x q3,_,>
    0b01101101, // q1,x q3,_,<
    0b01101110, // q1,x q3,x,>
    0b01101111, // q1,x q3,x,<
    0b01110010, // q1,x qHalt,x,>
  },
  .length = 17
};

static uint8_t_array q2x = {
  .array = (uint8_t[]) {
    0b10100000, // q2,x q0,_,>
    0b10100001, // q2,x q0,_,<
    0b10100010, // q2,x q0,x,>
    0b10100011, // q2,x q0,x,<
    0b10100100, // q2,x q1,_,>
    0b10100101, // q2,x q1,_,<
    0b10100110, // q2,x q1,x,>
    0b10100111, // q2,x q1,x,<
    0b10101000, // q2,x q2,_,>
    0b10101001, // q2,x q2,_,<
    0b10101010, // q2,x q2,x,>
    0b10101011, // q2,x q2,x,<
    0b10101100, // q2,x q3,_,>
    0b10101101, // q2,x q3,_,<
    0b10101110, // q2,x q3,x,>
    0b10101111, // q2,x q3,x,<
    0b10110010, // q2,x qHalt,x,>
  },
  .length = 17
};

static uint8_t_array q2blank = {
  .array = (uint8_t[]) {
    0b10000000, // q2,_ q0,_,>
    0b10000001, // q2,_ q0,_,<
    0b10000010, // q2,_ q0,x,>
    0b10000011, // q2,_ q0,x,<
    0b10000100, // q2,_ q1,_,>
    0b10000101, // q2,_ q1,_,<
    0b10000110, // q2,_ q1,x,>
    0b10000111, // q2,_ q1,x,<
    0b10001000, // q2,_ q2,_,>
    0b10001001, // q2,_ q2,_,<
    0b10001010, // q2,_ q2,x,>
    0b10001011, // q2,_ q2,x,<
    0b10001100, // q2,_ q3,_,>
    0b10001101, // q2,_ q3,_,<
    0b10001110, // q2,_ q3,x,>
    0b10001111, // q2,_ q3,x,<
    0b10010010, // q2,_ qHalt,x,>
  },
  .length = 17
};

static uint8_t_array q3x = {
  .array = (uint8_t[]) {
    0b11100000, // q3,x q0,_,>
    0b11100001, // q3,x q0,_,<
    0b11100010, // q3,x q0,x,>
    0b11100011, // q3,x q0,x,<
    0b11100100, // q3,x q1,_,>
    0b11100101, // q3,x q1,_,<
    0b11100110, // q3,x q1,x,>
    0b11100111, // q3,x q1,x,<
    0b11101000, // q3,x q2,_,>
    0b11101001, // q3,x q2,_,<
    0b11101010, // q3,x q2,x,>
    0b11101011, // q3,x q2,x,<
    0b11101100, // q3,x q3,_,>
    0b11101101, // q3,x q3,_,<
    0b11101110, // q3,x q3,x,>
    0b11101111, // q3,x q3,x,<
    0b11110010, // q3,x qHalt,x,>
  },
  .length = 17
};

static uint8_t_array q3blank = {
  .array = (uint8_t[]) {
    0b11000000, // q3,_ q0,_,>
    0b11000001, // q3,_ q0,_,<
    0b11000010, // q3,_ q0,x,>
    0b11000011, // q3,_ q0,x,<
    0b11000100, // q3,_ q1,_,>
    0b11000101, // q3,_ q1,_,<
    0b11000110, // q3,_ q1,x,>
    0b11000111, // q3,_ q1,x,<
    0b11001000, // q3,_ q2,_,>
    0b11001001, // q3,_ q2,_,<
    0b11001010, // q3,_ q2,x,>
    0b11001011, // q3,_ q2,x,<
    0b11001100, // q3,_ q3,_,>
    0b11001101, // q3,_ q3,_,<
    0b11001110, // q3,_ q3,x,>
    0b11001111, // q3,_ q3,x,<
    0b11010010, // q3,_ qHalt,x,>
  },
  .length = 17
};

#define FIRST_FIVE_TUPLE 0b00000110

/* 
We assume the first move is always: q0,_ q1,x,>
This is based on a comment from the paper 
"The Determination of the Value of Rado's Noncomputable Function Σ(k) for 
Four-State Turing Machine" by Allen H. Brady, 1983: "Furthermore, one can also
impose the requirement (cf. Lin and Rado [6]) that no machine need be considered
which does not have the initial entry (in state 1 scanning 0) to print 1, move
right, and enter state 2 ('1 R 2')."
*/

static uint8_t_array delta = {
  .array = (uint8_t[]) { FIRST_FIVE_TUPLE, 0, 0, 0, 0, 0, 0, 0 },
  .length = 8,
};

// Print out a five tuple, that was originally in binary, in
// the turingmachinesimulator.com language.
void print_five_tuple(uint8_t input){
  printf("%s,%s \n%s,%s,%s", curState(input), readSymbol(input), nextState(input), writeSymbol(input), moveDirection(input)); 
}

// Print a delta function in the turingmachinesimulator.com language
// and some extra information about the machine.
void print_delta(int count){
  printf("A Busy Beaver was found after examining %d 4-state turing machines.\n", count);
  printf("You can run the following code at: https://turingmachinesimulator.com/\n\n");
  printf("name: bb4\n");
  printf("init: q0\n");
  printf("accept: qHalt\n");
  printf("\n");
  for(int i=0; i<delta.length; i++){
    print_five_tuple(delta.array[i]);
    printf("\n");
  }
  printf("\n");
}

// Return the number of five tuples in the delta function that contain a halting state.
int halt_count(){
  int counter = 0;
  for(int i=0; i<delta.length; i++){
    if((delta.array[i] & NEXT_STATE_MASK) == HALTING_STATE){
      counter++;
    }
  }
  return counter;
}

#define EXPECTED_THEORETICAL_MAXIMUM 13

#define DESIRED_BB4_NOT_FOUND -1
// Given the sets of five tuples defined above (q0x, q1blank, q1x, q2blank, q2x, q3blank, q3x),
// create a delta function containing one element of each one of those, then run the bb4_sim 
// with the resulting delta and check if the result has the expected number of x's.
int try_all_delta_functions(void){
  int count = 0;
  for(int i1=0; i1 < q0x.length; i1++){
    delta.array[1] = q0x.array[i1];
    for(int i2=0; i2 < q1blank.length; i2++){
      delta.array[2] = q1blank.array[i2];
      for(int i3=0; i3 < q1x.length; i3++){
        delta.array[3] = q1x.array[i3];
        for(int i4=0; i4<q2blank.length; i4++){
          delta.array[4] = q2blank.array[i4];
          for(int i5=0; i5<q2x.length; i5++){
            delta.array[5] = q2x.array[i5];
            for(int i6=0; i6<q3blank.length; i6++){
              delta.array[6] = q3blank.array[i6];
              for(int i7=0; i7<q3x.length; i7++){
                delta.array[7] = q3x.array[i7];
                if(halt_count() != 1){
                  continue;
                }
                count++;
                if(count % 1000000 == 0){
                  printf("Machines assayed: %d\n", count);
                }
                int x_count = bb4_sim(delta);
                if(x_count == EXPECTED_THEORETICAL_MAXIMUM){
                  print_delta(count);
                  return x_count;
                }
              }
            }
          }
        }
      }
    }
  }
  return DESIRED_BB4_NOT_FOUND;
}

// Run the assertions and try all deltas to find a bb4.
int main() {
  assert_str_to_binary();
  assert_translate_from_string();
  assert_bb4_sim();

  printf("Iterating...\n");

  int x_count = try_all_delta_functions();

  printf("This bb4 prints %d x's.\n", x_count);

  return 0;
}
