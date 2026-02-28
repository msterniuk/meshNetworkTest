# TURTLE — painlessMesh Network

ESP32 mesh network project using [painlessMesh](https://gitlab.com/painlessMesh/painlessMesh).

## Quick Start

1. Open this folder in VSCode with PlatformIO installed.
2. PlatformIO will auto-detect `platformio.ini` and install all dependencies.
3. Connect an ESP32 via USB.
4. Click **Build** (✓) then **Upload** (→) in the PlatformIO toolbar.
5. Open **Serial Monitor** at 115200 baud — you should see `TURTLE mesh node ready`.
6. Flash a second ESP32 — they will auto-discover each other.

## Project Structure

```
TURTLE/
├── platformio.ini          # Build config, deps, board settings
├── include/
│   └── mesh_config.h       # Mesh SSID, password, channel, bridge settings
├── src/
│   └── main.cpp            # Application entry point
├── lib/                    # Project-specific libraries (your own)
├── data/                   # SPIFFS files (web UI, config JSON, etc.)
└── test/                   # Unit tests
```

## Connecting to Your LAN (Bridge Mode)

To bridge the mesh to your local network (for MQTT, HTTP, etc.):

1. **Find your router's Wi-Fi channel** (important — mesh and router must share a channel).
2. Edit `include/mesh_config.h`:
   - Uncomment `#define ENABLE_BRIDGE`
   - Set `STATION_SSID` and `STATION_PASSWORD`
   - Set `MESH_CHANNEL` to match your router
   - Set `IS_ROOT_NODE` to `true`
3. Flash **one** ESP32 as the bridge node. All other nodes stay as-is.

## Key Things to Know

- **Channel must match**: The mesh and your LAN router must be on the same Wi-Fi channel.
- **One root node**: Only the bridge node should be `setRoot(true)`.
- **JSON messaging**: painlessMesh uses JSON internally — integrates well with web UIs.
- **Don't spam broadcasts**: Keep messages conservative to avoid destabilizing the mesh.
