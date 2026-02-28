#include "serial_commands.h"

// --- State ---
static painlessMesh *_mesh = nullptr;
static String inputBuffer = "";

// --- Forward declarations ---
static void processCommand(String cmd);
static void cmdHelp();
static void cmdStatus();
static void cmdId();
static void cmdSend(String args);
static void cmdScan();
static void cmdRestart();
static void cmdNodes();

// =============================================================================
// Public API
// =============================================================================

void serialCommandsInit(painlessMesh *meshPtr) {
    _mesh = meshPtr;
    Serial.println("\nType 'help' for available commands.\n");
    Serial.print("> ");
}

void serialCommandsUpdate() {
    while (Serial.available()) {
        char c = Serial.read();

        // Handle backspace
        if (c == '\b' || c == 127) {
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1);
                Serial.print("\b \b");  // erase character on terminal
            }
            continue;
        }

        // Handle enter (newline or carriage return)
        if (c == '\n' || c == '\r') {
            Serial.println();  // echo newline
            inputBuffer.trim();
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
            }
            inputBuffer = "";
            Serial.print("> ");
            continue;
        }

        // Echo and accumulate printable characters
        if (isPrintable(c)) {
            inputBuffer += c;
            Serial.print(c);
        }
    }
}

// =============================================================================
// Command Router
// =============================================================================

static void processCommand(String cmd) {
    // Extract command name and arguments
    String name = cmd;
    String args = "";
    int spaceIdx = cmd.indexOf(' ');
    if (spaceIdx > 0) {
        name = cmd.substring(0, spaceIdx);
        args = cmd.substring(spaceIdx + 1);
        args.trim();
    }
    name.toLowerCase();

    if      (name == "help")    cmdHelp();
    else if (name == "status")  cmdStatus();
    else if (name == "id")      cmdId();
    else if (name == "send")    cmdSend(args);
    else if (name == "scan")    cmdScan();
    else if (name == "nodes")   cmdNodes();
    else if (name == "restart") cmdRestart();
    else {
        Serial.printf("  Unknown command: '%s'. Type 'help' for options.\n", name.c_str());
    }
}

// =============================================================================
// Command Implementations
// =============================================================================

static void cmdHelp() {
    Serial.println("  ---- TURTLE Commands ----");
    Serial.println("  help      Show this message");
    Serial.println("  status    Node info, uptime, memory, mesh stats");
    Serial.println("  id        Print this node's ID");
    Serial.println("  nodes     List all connected node IDs");
    Serial.println("  send <msg>  Broadcast a message to the mesh");
    Serial.println("  scan      Scan nearby Wi-Fi networks");
    Serial.println("  restart   Reboot this node");
}

static void cmdStatus() {
    if (!_mesh) { Serial.println("  ERROR: mesh not initialized"); return; }

    auto nodeList = _mesh->getNodeList();
    uint32_t uptime = millis() / 1000;
    uint32_t mins = uptime / 60;
    uint32_t secs = uptime % 60;

    Serial.println("  ---- Node Status ----");
    Serial.printf("  Node ID:     %u\n", _mesh->getNodeId());
    Serial.printf("  Uptime:      %um %us\n", mins, secs);
    Serial.printf("  Free heap:   %u bytes\n", ESP.getFreeHeap());
    Serial.printf("  Mesh nodes:  %d (including self)\n", nodeList.size() + 1);
}

static void cmdId() {
    if (!_mesh) { Serial.println("  ERROR: mesh not initialized"); return; }
    Serial.printf("  %u\n", _mesh->getNodeId());
}

static void cmdNodes() {
    if (!_mesh) { Serial.println("  ERROR: mesh not initialized"); return; }

    auto nodeList = _mesh->getNodeList();
    Serial.printf("  Total nodes in mesh: %d\n", nodeList.size() + 1);
    Serial.printf("  Self: %u\n", _mesh->getNodeId());
    for (auto &id : nodeList) {
        Serial.printf("  Node: %u\n", id);
    }
}

static void cmdSend(String args) {
    if (!_mesh) { Serial.println("  ERROR: mesh not initialized"); return; }

    if (args.length() == 0) {
        Serial.println("  Usage: send <message>");
        return;
    }

    bool sent = _mesh->sendBroadcast(args);
    if (sent) {
        Serial.printf("  Broadcast sent: %s\n", args.c_str());
    } else {
        Serial.println("  Failed to send (no connected nodes?)");
    }
}

static void cmdScan() {
    Serial.println("  Scanning Wi-Fi networks...");

    int n = WiFi.scanNetworks(false, true);  // sync scan, show hidden
    if (n == 0) {
        Serial.println("  No networks found.");
    } else {
        Serial.printf("  Found %d networks:\n", n);
        for (int i = 0; i < n; i++) {
            Serial.printf("    %2d) Ch:%2d  %4ddBm  %s %s\n",
                          i + 1,
                          WiFi.channel(i),
                          WiFi.RSSI(i),
                          WiFi.SSID(i).c_str(),
                          (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "[open]" : "");
        }
    }
    WiFi.scanDelete();
    Serial.println("  Note: Your mesh channel must match your router's channel for bridging.");
}

static void cmdRestart() {
    Serial.println("  Restarting in 1 second...");
    delay(1000);
    ESP.restart();
}