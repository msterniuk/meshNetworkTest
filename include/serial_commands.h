#ifndef SERIAL_COMMANDS_H
#define SERIAL_COMMANDS_H

#include <Arduino.h>
#include <painlessMesh.h>

// Initialize the command handler (call once in setup)
void serialCommandsInit(painlessMesh *meshPtr);

// Check for and process serial input (call every loop)
void serialCommandsUpdate();

#endif // SERIAL_COMMANDS_H