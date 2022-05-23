#ifndef UTHT_UTILS_INSTRUCTIONS_H_
#define UTHT_UTILS_INSTRUCTIONS_H_

#include <Arduino.h>

#include <utils/base.h>
#include <actuators/actuator.h>

// Instruction label matching `instructions[i]` indicates that the target of actuator `actuators[i]` should be set to the instruction value
typedef struct InstructionActuatorMatrix {
  // Number of actuators
  uint8_t numActuators;

  // Array of pointers to actuators
  Actuator** actuators;

  // Array of instruction label strings
  const char** instructions;
} InstructionActuatorMatrix;

/**
 * Handles incoming instruction set according to an instruction-actuator matrix
 * @param instructionSet JSON-formatted instruction set
 * @param matrix Instruction-actuator matrix
 * @return Error level - `ERR_FATAL`: invalid instruction set formatting; `ERR_WARNING`: one or more instruction labels were unknown
 */
errorlevel_t handleInstructions(String instructionSet, const InstructionActuatorMatrix* matrix);

/**
 * Handles incoming instruction label-target pair.
 * @param instruction Label-target pair (i.e. `"\"actuator_label\":value_float"`)
 * @param matrix Instruction-actuator matrix
 * @return Error level - `ERR_FATAL`: invalid instruction formatting; `ERR_WARNING`: instruction label was unknown
 */
errorlevel_t handleInstruction(String instruction, const InstructionActuatorMatrix* matrix);

#endif
