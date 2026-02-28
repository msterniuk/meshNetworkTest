// =============================================================================
// TURTLE — painlessMesh Starter
// =============================================================================
// This is a minimal "hello mesh" skeleton. Flash it to verify your toolchain
// and that nodes discover each other. Replace with your application logic.
// =============================================================================

#include <Arduino.h>
#include <painlessMesh.h>
#include "mesh_config.h"
#include "serial_commands.h"

painlessMesh mesh;

// --- Scheduler (painlessMesh uses this internally; you can add your own tasks) ---
Scheduler userScheduler;

// --- Forward declarations ---
void receivedCallback(uint32_t from, String &msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);


// --- Callbacks ---
void receivedCallback(uint32_t from, String &msg) {
    Serial.printf("RX from %u: %s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("New connection: nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Connections changed. Total nodes: %d\n",
                  mesh.getNodeList().size() + 1);
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Time adjusted. Offset = %d\n", offset);
}

// =============================================================================
void setup() {
    Serial.begin(115200);

    // Debug verbosity (set before init)
#if MESH_DEBUG
    mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
#endif

    // Initialize mesh
    mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, MESH_CHANNEL);

    // Register callbacks
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    // --- LAN Bridge (uncomment in mesh_config.h to enable) ---
#ifdef ENABLE_BRIDGE
    mesh.stationManual(STATION_SSID, STATION_PASSWORD);
    mesh.setRoot(true);
    mesh.setHostname("turtle-bridge");
#endif

    // If any node in the mesh has a root, all nodes should know
#if IS_ROOT_NODE
    mesh.setRoot(true);
#endif
    mesh.setContainsRoot(IS_ROOT_NODE);

    // Start periodic send

    Serial.println("\n=== TURTLE mesh node ready ===");
    Serial.printf("Node ID: %u\n", mesh.getNodeId());

    serialCommandsInit(&mesh); 
    
}

void loop() {
    mesh.update();

    //going to add a call here to check for commands on every iteration
    //keeps code clean and modular, see serial_commands.cpp

    serialCommandsUpdate(); 
}
