public class Bb4Finder {
  static long tape = 0;

  static final byte HALTING_STATE = (byte) 0b00010000;
  static final byte CURRENT_MASK = (byte) 0b11000000;
  static final byte READ_MASK = (byte) 0b00100000;
  static final byte NEXT_STATE_MASK = (byte) 0b00011100;
  static final byte WRITE_MASK = (byte) 0b00000010;
  static final byte MOVEMENT_MASK = (byte) 0b00000001;
  static final byte FIRST_FIVE_TUPLE = (byte) 0b00000110;

  static final int MAX_ITERATIONS = 128;
  static final int DELTA_LENGTH = 8;
  static final int EXPECTED_THEORETICAL_MAXIMUM = 13;
  static final int INSTRUCTION_ARRAYS_LENGTH = 17;

  static final int COMPUTATION_OUT_OF_BOUNDS = -1;
  static final int DESIRED_BB4_NOT_FOUND = -1;

  // Instruction arrays:

  private static final byte[] q0x = {
      (byte) 0b00100000, // q0,x q0,_,>
      (byte) 0b00100001, // q0,x q0,_,<
      (byte) 0b00100010, // q0,x q0,x,>
      (byte) 0b00100011, // q0,x q0,x,<
      (byte) 0b00100100, // q0,x q1,_,>
      (byte) 0b00100101, // q0,x q1,_,<
      (byte) 0b00100110, // q0,x q1,x,>
      (byte) 0b00100111, // q0,x q1,x,<
      (byte) 0b00101000, // q0,x q2,_,>
      (byte) 0b00101001, // q0,x q2,_,<
      (byte) 0b00101010, // q0,x q2,x,>
      (byte) 0b00101011, // q0,x q2,x,<
      (byte) 0b00101100, // q0,x q3,_,>
      (byte) 0b00101101, // q0,x q3,_,<
      (byte) 0b00101110, // q0,x q3,x,>
      (byte) 0b00101111, // q0,x q3,x,<
      (byte) 0b00110010, // q0,x qHalt,x,>
  };

  private static final byte[] q1blank = {
      (byte) 0b01000000, // q1,_ q0,_,>
      (byte) 0b01000001, // q1,_ q0,_,<
      (byte) 0b01000010, // q1,_ q0,x,>
      (byte) 0b01000011, // q1,_ q0,x,<
      (byte) 0b01000100, // q1,_ q1,_,>
      (byte) 0b01000101, // q1,_ q1,_,<
      (byte) 0b01000110, // q1,_ q1,x,>
      (byte) 0b01000111, // q1,_ q1,x,<
      (byte) 0b01001000, // q1,_ q2,_,>
      (byte) 0b01001001, // q1,_ q2,_,<
      (byte) 0b01001010, // q1,_ q2,x,>
      (byte) 0b01001011, // q1,_ q2,x,<
      (byte) 0b01001100, // q1,_ q3,_,>
      (byte) 0b01001101, // q1,_ q3,_,<
      (byte) 0b01001110, // q1,_ q3,x,>
      (byte) 0b01001111, // q1,_ q3,x,<
      (byte) 0b01010010, // q1,_ qHalt,x,>
  };

  private static final byte[] q1x = {
      (byte) 0b01100000, // q1,x q0,_,>
      (byte) 0b01100001, // q1,x q0,_,<
      (byte) 0b01100010, // q1,x q0,x,>
      (byte) 0b01100011, // q1,x q0,x,<
      (byte) 0b01100100, // q1,x q1,_,>
      (byte) 0b01100101, // q1,x q1,_,<
      (byte) 0b01100110, // q1,x q1,x,>
      (byte) 0b01100111, // q1,x q1,x,<
      (byte) 0b01101000, // q1,x q2,_,>
      (byte) 0b01101001, // q1,x q2,_,<
      (byte) 0b01101010, // q1,x q2,x,>
      (byte) 0b01101011, // q1,x q2,x,<
      (byte) 0b01101100, // q1,x q3,_,>
      (byte) 0b01101101, // q1,x q3,_,<
      (byte) 0b01101110, // q1,x q3,x,>
      (byte) 0b01101111, // q1,x q3,x,<
      (byte) 0b01110010, // q1,x qHalt,x,>
  };

  private static final byte[] q2x = {
      (byte) 0b10100000, // q2,x q0,_,>
      (byte) 0b10100001, // q2,x q0,_,<
      (byte) 0b10100010, // q2,x q0,x,>
      (byte) 0b10100011, // q2,x q0,x,<
      (byte) 0b10100100, // q2,x q1,_,>
      (byte) 0b10100101, // q2,x q1,_,<
      (byte) 0b10100110, // q2,x q1,x,>
      (byte) 0b10100111, // q2,x q1,x,<
      (byte) 0b10101000, // q2,x q2,_,>
      (byte) 0b10101001, // q2,x q2,_,<
      (byte) 0b10101010, // q2,x q2,x,>
      (byte) 0b10101011, // q2,x q2,x,<
      (byte) 0b10101100, // q2,x q3,_,>
      (byte) 0b10101101, // q2,x q3,_,<
      (byte) 0b10101110, // q2,x q3,x,>
      (byte) 0b10101111, // q2,x q3,x,<
      (byte) 0b10110010, // q2,x qHalt,x,>
  };

  private static final byte[] q2blank = {
      (byte) 0b10000000, // q2,_ q0,_,>
      (byte) 0b10000001, // q2,_ q0,_,<
      (byte) 0b10000010, // q2,_ q0,x,>
      (byte) 0b10000011, // q2,_ q0,x,<
      (byte) 0b10000100, // q2,_ q1,_,>
      (byte) 0b10000101, // q2,_ q1,_,<
      (byte) 0b10000110, // q2,_ q1,x,>
      (byte) 0b10000111, // q2,_ q1,x,<
      (byte) 0b10001000, // q2,_ q2,_,>
      (byte) 0b10001001, // q2,_ q2,_,<
      (byte) 0b10001010, // q2,_ q2,x,>
      (byte) 0b10001011, // q2,_ q2,x,<
      (byte) 0b10001100, // q2,_ q3,_,>
      (byte) 0b10001101, // q2,_ q3,_,<
      (byte) 0b10001110, // q2,_ q3,x,>
      (byte) 0b10001111, // q2,_ q3,x,<
      (byte) 0b10010010, // q2,_ qHalt,x,>
  };

  private static final byte[] q3x = {
      (byte) 0b11100000, // q3,x q0,_,>
      (byte) 0b11100001, // q3,x q0,_,<
      (byte) 0b11100010, // q3,x q0,x,>
      (byte) 0b11100011, // q3,x q0,x,<
      (byte) 0b11100100, // q3,x q1,_,>
      (byte) 0b11100101, // q3,x q1,_,<
      (byte) 0b11100110, // q3,x q1,x,>
      (byte) 0b11100111, // q3,x q1,x,<
      (byte) 0b11101000, // q3,x q2,_,>
      (byte) 0b11101001, // q3,x q2,_,<
      (byte) 0b11101010, // q3,x q2,x,>
      (byte) 0b11101011, // q3,x q2,x,<
      (byte) 0b11101100, // q3,x q3,_,>
      (byte) 0b11101101, // q3,x q3,_,<
      (byte) 0b11101110, // q3,x q3,x,>
      (byte) 0b11101111, // q3,x q3,x,<
      (byte) 0b11110010, // q3,x qHalt,x,>
  };

  private static final byte[] q3blank = {
      (byte) 0b11000000, // q3,_ q0,_,>
      (byte) 0b11000001, // q3,_ q0,_,<
      (byte) 0b11000010, // q3,_ q0,x,>
      (byte) 0b11000011, // q3,_ q0,x,<
      (byte) 0b11000100, // q3,_ q1,_,>
      (byte) 0b11000101, // q3,_ q1,_,<
      (byte) 0b11000110, // q3,_ q1,x,>
      (byte) 0b11000111, // q3,_ q1,x,<
      (byte) 0b11001000, // q3,_ q2,_,>
      (byte) 0b11001001, // q3,_ q2,_,<
      (byte) 0b11001010, // q3,_ q2,x,>
      (byte) 0b11001011, // q3,_ q2,x,<
      (byte) 0b11001100, // q3,_ q3,_,>
      (byte) 0b11001101, // q3,_ q3,_,<
      (byte) 0b11001110, // q3,_ q3,x,>
      (byte) 0b11001111, // q3,_ q3,x,<
      (byte) 0b11010010, // q3,_ qHalt,x,>
  };

  private static String curState(byte fiveTuple) {
    return switch (fiveTuple & (byte) 0b11000000) {
      case (byte) 0b00000000 -> "q0";
      case (byte) 0b01000000 -> "q1";
      case (byte) 0b10000000 -> "q2";
      case (byte) 0b11000000 -> "q3";
      default -> throw new IllegalStateException();
    };
  }

  private static String readSymbol(byte five_tuple) {
    return ((five_tuple & (byte) 0b00100000) != 0) ? "x" : "_";
  }

  private static String nextState(byte five_tuple) {
    return switch (five_tuple & (byte) 0b00011100) {
      case (byte) 0b00000000 -> "q0";
      case (byte) 0b00000100 -> "q1";
      case (byte) 0b00001000 -> "q2";
      case (byte) 0b00001100 -> "q3";
      case HALTING_STATE -> "qHalt";
      default -> throw new IllegalStateException();
    };
  }

  private static String writeSymbol(byte five_tuple) {
    return ((five_tuple & (byte) 0b00000010) != 0) ? "x" : "_";
  }

  private static String moveDirection(byte five_tuple) {
    return ((five_tuple & (byte) 0b00000001) != 0) ? "<" : ">";
  }

  private static int countSetBits() {
    int count = 0;
    while (tape != 0) {
      count += tape & 1;
      tape >>>= 1;
    }
    return count;
  }

  private static int bb4_sim(byte[] delta, int count) {
    tape = 0;
    int pos = 31;
    int iterations;
    byte cur_state = (byte) 0b00000000;
    byte next_state;
    for (iterations = 0; iterations < MAX_ITERATIONS; iterations++) {
      byte five_tuple = 0;
      byte curSymbol = (byte) ((tape & (1L << pos)) >>> pos << 5);
      for (byte b : delta) {
        five_tuple = b;
        if (cur_state == (byte) (five_tuple & CURRENT_MASK) && curSymbol == (byte) (five_tuple & READ_MASK)) {
          break;
        }
      }
      byte newSymbol = (byte) (five_tuple & WRITE_MASK);
      if (newSymbol != 0) {
        tape |= (1L << pos);
      } else {
        tape &= ~(1L << pos);
      }
      next_state = (byte) (five_tuple & NEXT_STATE_MASK);
      pos += (five_tuple & MOVEMENT_MASK) != 0 ? 1 : -1;
      if (next_state == HALTING_STATE || pos < 0 || pos > 63) {
        break;
      }
      cur_state = (byte) (next_state << 4);
    }
    if (iterations >= MAX_ITERATIONS || pos < 0 || pos > 63) {
      return COMPUTATION_OUT_OF_BOUNDS;
    }
    return countSetBits();
  }

  static byte[] delta = { FIRST_FIVE_TUPLE, 0, 0, 0, 0, 0, 0, 0 };

  private static void print_five_tuple(byte input) {
    System.out.printf("%s,%s \n%s,%s,%s",
        curState(input),
        readSymbol(input),
        nextState(input),
        writeSymbol(input),
        moveDirection(input));
  }

  private static void print_delta(int count) {
    System.out.printf("A Busy Beaver was found after examining %d 4-state turing machines.\n", count);
    System.out.println("You can run the following code at: https://turingmachinesimulator.com/\n");
    System.out.println("name: bb4");
    System.out.println("init: q0");
    System.out.print("accept: qHalt");
    System.out.println();
    for (int i = 0; i < DELTA_LENGTH; i++) {
      print_five_tuple(delta[i]);
      System.out.println();
    }
    System.out.println();
  }

  private static int halt_count() {
    int counter = 0;
    for (int i = 0; i < DELTA_LENGTH; i++) {
      if ((delta[i] & NEXT_STATE_MASK) == HALTING_STATE) {
        counter++;
      }
    }
    return counter;
  }

  private static int try_all_delta_functions() {
    int count = 0;
    for (int i1 = 0; i1 < INSTRUCTION_ARRAYS_LENGTH; i1++) {
      delta[1] = q0x[i1];
      for (int i2 = 0; i2 < INSTRUCTION_ARRAYS_LENGTH; i2++) {
        delta[2] = q1blank[i2];
        for (int i3 = 0; i3 < INSTRUCTION_ARRAYS_LENGTH; i3++) {
          delta[3] = q1x[i3];
          for (int i4 = 0; i4 < INSTRUCTION_ARRAYS_LENGTH; i4++) {
            delta[4] = q2blank[i4];
            for (int i5 = 0; i5 < INSTRUCTION_ARRAYS_LENGTH; i5++) {
              delta[5] = q2x[i5];
              for (int i6 = 0; i6 < INSTRUCTION_ARRAYS_LENGTH; i6++) {
                delta[6] = q3blank[i6];
                for (int i7 = 0; i7 < INSTRUCTION_ARRAYS_LENGTH; i7++) {
                  delta[7] = q3x[i7];
                  if (halt_count() != 1) {
                    continue;
                  }
                  count++;
                  if (count % 1000000 == 0) {
                    System.out.printf("Machines assayed: %d\n", count);
                  }
                  int x_count = bb4_sim(delta, count);
                  if (x_count == EXPECTED_THEORETICAL_MAXIMUM) {
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

  public static void main(String[] args) {
    System.out.println("Iterating...");

    int x_count = try_all_delta_functions();

    System.out.printf("This bb4 prints %d x's.\n", x_count);
  }
}
