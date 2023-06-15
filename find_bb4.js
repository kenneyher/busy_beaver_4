const first_five_tuple = {
  curState: "q0", readSymbol: "_", nextState: "q1", writeSymbol: "x", moveDirection: ">"
}

const delta = [first_five_tuple, {}, {}, {}, {}, {}, {}, {}]; 

const max_iterations = 128;
const computation_out_of_bounds = -1;
function bb_sim() {
  const tape = new Array(150).fill("_");
  let iterations = 0;
  let pos = tape.length / 2;
  let curState = "q0";
  let five_tuple;
  for(iterations=0; iterations<max_iterations; iterations++){
    for(let i=0; i<delta.length; i++){
      if(delta[i].curState == curState && delta[i].readSymbol == tape[pos]){
        five_tuple = delta[i];
        break;
      }
    }
    if(five_tuple){
      tape[pos] = five_tuple.writeSymbol;
      pos += five_tuple.moveDirection == ">" ? 1 : -1;
      curState = five_tuple.nextState;
      if(curState == "qHalt" || pos < 0 || pos > tape.length-1){
        break;
      }
    }
  }
  if(iterations >= max_iterations || pos < 0 || pos > tape.length-1){
    return computation_out_of_bounds;
  }
  let numOfX = 0;
  for(let i=0; i<tape.length; i++){
    let cell = tape[i];
    if(cell == "x"){
      numOfX++;
    }
  }
  return numOfX;
}

function translate(delta_str) {
  let lines = delta_str.split("\n");
  let result = [];
  let counter = 0;
  for (let line of lines) {
    if (line.trim().length == 0) {
      continue;
    }
    let tokens = line.split(/,\s*/);
    if (tokens.length == 2) {
      let obj = {};
      obj.curState = tokens[0];
      obj.readSymbol = tokens[1];
      result.push(obj);
    } else {
      let obj = result[result.length - 1];
      obj.nextState = tokens[0];
      obj.writeSymbol = tokens[1];
      obj.moveDirection = tokens[2];
    }
  }
  return result;
}

function print_delta(count){
  console.log(`A Busy Beaver was found after examining ${count} 4-state turing machines.`);
  console.log("You can run the following code at: https://turingmachinesimulator.com/\n");
  console.log("name: bb4");
  console.log("init: q0");
  console.log("accept: qHalt\n");
  for(let i=0; i<delta.length; i++){
    console.log(delta[i].curState, ",", delta[i].readSymbol);
    console.log(delta[i].nextState, ",", delta[i].writeSymbol, ",", delta[i].moveDirection == ">" ? ">" : "<");
    console.log("");
  }
}

const q0x = [
  { curState: "q0", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: ">" },
  { curState: "q0", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: "<" },
  { curState: "q0", readSymbol: "x", nextState: "qHalt", writeSymbol: "x", moveDirection: ">" },
]
const q1blank = [
  { curState: "q1", readSymbol: "_", nextState: "q0", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q0", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "q0", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q0", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "q1", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q1", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "q1", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q1", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "q2", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q2", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "q2", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q2", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "q3", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q3", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "q3", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "_", nextState: "q3", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "_", nextState: "qHalt", writeSymbol: "x", moveDirection: ">" },
]
const q1x = [
  { curState: "q1", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: ">" },
  { curState: "q1", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: "<" },
  { curState: "q1", readSymbol: "x", nextState: "qHalt", writeSymbol: "x", moveDirection: ">" },
]
const q2x = [
  { curState: "q2", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "x", nextState: "qHalt", writeSymbol: "x", moveDirection: ">" },
]
const q2blank = [
  { curState: "q2", readSymbol: "_", nextState: "q0", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q0", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "q0", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q0", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "q1", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q1", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "q1", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q1", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "q2", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q2", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "q2", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q2", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "q3", writeSymbol: "_", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q3", writeSymbol: "_", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "q3", writeSymbol: "x", moveDirection: ">" },
  { curState: "q2", readSymbol: "_", nextState: "q3", writeSymbol: "x", moveDirection: "<" },
  { curState: "q2", readSymbol: "_", nextState: "qHalt", writeSymbol: "x", moveDirection: ">" },
]
const q3x = [
  { curState: "q3", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q0", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q0", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q1", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q1", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q2", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q2", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q3", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "x", nextState: "q3", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "x", nextState: "qHalt", writeSymbol: "x", moveDirection: ">" },
]
const q3blank = [
  { curState: "q3", readSymbol: "_", nextState: "q0", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q0", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "q0", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q0", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "q1", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q1", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "q1", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q1", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "q2", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q2", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "q2", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q2", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "q3", writeSymbol: "_", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q3", writeSymbol: "_", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "q3", writeSymbol: "x", moveDirection: ">" },
  { curState: "q3", readSymbol: "_", nextState: "q3", writeSymbol: "x", moveDirection: "<" },
  { curState: "q3", readSymbol: "_", nextState: "qHalt", writeSymbol: "x", moveDirection: ">" },
]

function haltCount(){
  let numHaltingStates = 0;
  for(let i=0; i<delta.length; i++){
    if(delta[i].nextState == "qHalt"){
      numHaltingStates++;
    }
  }
  return numHaltingStates;
}

const desired_bb4_not_found = -1;
const expected_theoretical_maximum = 13;
function try_all_deltas() {
  let count = 0;
  for(let i1=0; i1 < q0x.length; i1++){
    delta[1] = q0x[i1];
    for(let i2=0; i2 < q1blank.length; i2++){
      delta[2] = q1blank[i2];
      for(let i3=0; i3 < q1x.length; i3++){
        delta[3] = q1x[i3];
        for(let i4=0; i4<q2blank.length; i4++){
          delta[4] = q2blank[i4];
          for(let i5=0; i5<q2x.length; i5++){
            delta[5] = q2x[i5];
            for(let i6=0; i6<q3blank.length; i6++){
              delta[6] = q3blank[i6];
              for(let i7=0; i7<q3x.length; i7++){
                delta[7] = q3x[i7];
                if(haltCount() != 1){
                  continue;
                }
                count++;
                if(count % 1000000 == 0){
                  console.log("Machines assayed:", count);
                }
                let x_count = bb_sim();
                if(x_count == expected_theoretical_maximum){
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
  return desired_bb4_not_found;
}

let x_count = try_all_deltas();
console.log(`\nThe above code will print out ${x_count} x's.`);
