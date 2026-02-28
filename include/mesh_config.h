#ifndef MESH_CONFIG_H
#define MESH_CONFIG_H

// =============================================================================
// TURTLE — Mesh Network Configuration
// =============================================================================
// Edit these values to match your setup. If you prefer build flags (-D in
// platformio.ini), those will override these defaults via the #ifndef guards.
// =============================================================================

// --- Mesh Network Credentials ---
#ifndef MESH_SSID
#define MESH_SSID       "TURTLE_Mesh"
#endif

#ifndef MESH_PASSWORD
#define MESH_PASSWORD   "TurtleMesh123!"
#endif

#ifndef MESH_PORT
#define MESH_PORT       5555
#endif

// --- Wi-Fi Channel (1–13) ---
// IMPORTANT: If bridging to a LAN router, this MUST match your router's channel.
// Check your router's channel and set it here.
#ifndef MESH_CHANNEL
#define MESH_CHANNEL    6
#endif

// --- LAN Bridge Settings (uncomment to enable bridge mode on one node) ---
// #define ENABLE_BRIDGE
// #define STATION_SSID      "YourWiFiSSID"
// #define STATION_PASSWORD  "YourWiFiPassword"

// --- Node Role ---
// Set to true on exactly ONE node if bridging to LAN
#ifndef IS_ROOT_NODE
#define IS_ROOT_NODE    false
#endif

// --- Debug ---
// Set to 1 to enable verbose mesh debug output on Serial
#ifndef MESH_DEBUG
#define MESH_DEBUG      1
#endif

#endif // MESH_CONFIG_H
