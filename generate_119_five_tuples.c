#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CONDITION_MASK   0b11100000
#define CURRENT_MASK     0b11000000
#define READ_MASK        0b00100000
#define NEXT_MASK        0b00011100
#define WRITE_MASK       0b00000010
#define MOVEMENT_MASK    0b00000001
#define FIRST_FIVE_TUPLE 0b00000110

typedef struct {
  uint8_t *array;
  int length;
} uint8_t_array;

char *curState(uint8_t n) {
  switch (n & CURRENT_MASK) {
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

char *readSymbol(uint8_t n) { return (n & READ_MASK) ? "x" : "_"; }

char *nextState(uint8_t n) {
  switch (n & NEXT_MASK) {
  case 0b00000000:
    return "q0";
  case 0b00000100:
    return "q1";
  case 0b00001000:
    return "q2";
  case 0b00001100:
    return "q3";
  case 0b00010000:
    return "qHalt";
  default:
    __builtin_unreachable();
  }
}

char *writeSymbol(uint8_t n) { return (n & WRITE_MASK) ? "x" : "_"; }

char *moveDirection(uint8_t n) { return (n & MOVEMENT_MASK) ? "<" : ">"; }

char *uint8_to_binary(uint8_t input) {
  char *binary_string = (char *)malloc(9);
  memset(binary_string, '0', 8);
  binary_string[8] = '\0';       

  for (int i = 7; i >= 0; i--) {
    if ((input >> i) & 1) {
      binary_string[7 - i] = '1';
    }
  }

  return binary_string;
}

uint8_t_array five_tuples() {
  uint8_t *array = malloc(256 * sizeof(uint8_t));
  int count = 0;
  for (int i = 0; i < 256; i++) {
    uint8_t five_tuple = (uint8_t)i;
    if ((five_tuple & CONDITION_MASK) == 0b00000000) {
      continue;
    }
    if ((five_tuple & NEXT_MASK) > 0b00010000) {
      continue;
    }
    if ((five_tuple & NEXT_MASK) == 0b00010000 &&
        (((five_tuple & WRITE_MASK) == 0b00000000) ||
         ((five_tuple & MOVEMENT_MASK) == 0b00000001))) {
      continue;
    }
    array[count] = five_tuple;
    count++;
  }
  uint8_t_array result = {.array = array, .length = count};
  return result;
}

void print_five_tuple(uint8_t input){
  printf("%s,%s %s,%s,%s", curState(input), readSymbol(input), nextState(input), writeSymbol(input), moveDirection(input)); 
}

void print_set_of_tuples(uint8_t condition, uint8_t_array tuples){
  printf("static uint8_t_array %s%s = {\n", curState(condition), strcmp(readSymbol(condition), "x") == 0 ? "x" : "blank");  
  printf("  .array = (uint8_t[]) {\n");
  int count = 0;
  for(int i = 0; i<tuples.length; i++){
    uint8_t five_tuple = tuples.array[i];
    if((five_tuple & CONDITION_MASK) != (condition & CONDITION_MASK)){
      continue;
    }
    char *five_tuple_str = uint8_to_binary(five_tuple);
    printf("    0b%s, // ", five_tuple_str);
    print_five_tuple(five_tuple);
    printf("\n");
    count++;
    free(five_tuple_str);
  }
  printf("  },\n");
  printf("  .length = %d\n};\n\n", count);
}

int main(void){
  uint8_t_array relevant_tuples = five_tuples();
  
  print_set_of_tuples(0b00100000, relevant_tuples);
  print_set_of_tuples(0b01000000, relevant_tuples);
  print_set_of_tuples(0b01100000, relevant_tuples);
  print_set_of_tuples(0b10100000, relevant_tuples);
  print_set_of_tuples(0b10000000, relevant_tuples);
  print_set_of_tuples(0b11100000, relevant_tuples);
  print_set_of_tuples(0b11000000, relevant_tuples);

  free(relevant_tuples.array);

  return 0;
}