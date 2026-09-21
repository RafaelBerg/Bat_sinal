# Bat-Sinal — Node B (Rooftop Actuator)

ESP-IDF firmware for the Bat-Sinal LED on **ESP32-S3**. Connects to lab Wi-Fi, subscribes to Mosquitto, and drives a GPIO LED.

## Layout

```text
main/app_main.c                 # boot: NVS, LED, Wi-Fi, MQTT, heartbeat
components/gotham_protocol/     # MQTT topics and payloads
components/wifi_sta/              # Wi-Fi station
components/mqtt_app/            # esp-mqtt client (subscribe + publish)
components/bat_led/             # GPIO LED actuator
components/heartbeat/           # 30s status on gotham/dpgc/status
docs/                           # assignment
```

## Clone on another PC (lab)

Yes — clone. Do **not** use a random CMD. Clone into a path **without accents** and open it with the **ESP-IDF environment**.

```text
cd C:\esp
git clone <URL-do-repo> Bat-Sinal
```

Then **one** of these:

1. **Cursor / VS Code (recommended)**  
   File → Open Folder → `C:\esp\Bat-Sinal`  
   `Ctrl+Shift+P` → **ESP-IDF: Select Current ESP-IDF Version** (pick the IDF **on that PC**)  
   If the IDF is not installed there yet: **ESP-IDF: Open ESP-IDF Installation Manager**  
   `Ctrl+Shift+P` → **ESP-IDF: SDK Configuration Editor (Menuconfig)**  
   That GUI **is** menuconfig on Windows. The old black Linux TUI often does not open in `cmd.exe`.

2. **ESP-IDF PowerShell / ESP-IDF CMD** (shortcut created by the Espressif installer)  
   ```text
   cd C:\esp\Bat-Sinal
   idf.py set-target esp32s3
   idf.py menuconfig
   ```

In **Bat-Sinal Configuration** set WiFi SSID and password. Save. That writes local `sdkconfig` (gitignored).

```text
idf.py build
idf.py -p PORT flash monitor
```

Do **not** skip menuconfig: Wi-Fi is not in git. Build alone leaves SSID empty and the board will not connect.

### Defaults already in the repo

| Setting | Value |
|---------|--------|
| Target | ESP32-S3 |
| LED GPIO | **10** |
| MQTT broker | `mqtt://10.1.133.82:1883` |
| Flash | 8 MB, large app partition |

Hardware: **GPIO 10 → 220 Ω → LED anode**, cathode to GND. Active high.

## MQTT contract

- Command topic: `gotham/dpgc/batsignal`
  - `BAT_SIGNAL_ON` → LED high
  - `BAT_SIGNAL_OFF` → LED low
- Heartbeat every 30s on `gotham/dpgc/status`:
  `{"device": "bat_sinal", "status": "ONLINE", "uptime_s": 120}`

## Troubleshooting

**Builds but nothing works on the board**

1. Wrong chip — must be `idf.py set-target esp32s3` (not esp32c6).
2. Wi-Fi empty — check monitor for `Wi-Fi SSID vazio!` and run `menuconfig`.
3. LED on wrong pin — default is GPIO **10**; change in menuconfig if wired elsewhere.
4. Not on lab Wi-Fi / broker unreachable — MQTT never connects.

**Windows path with accents** (e.g. `Área de Trabalho`)

Clone or copy the project to a plain path such as `C:\esp\Bat-Sinal` before building.

## Test without the button node

```text
mosquitto_pub -h 10.1.133.82 -t gotham/dpgc/batsignal -m "BAT_SIGNAL_ON"
mosquitto_pub -h 10.1.133.82 -t gotham/dpgc/batsignal -m "BAT_SIGNAL_OFF"
mosquitto_sub -h 10.1.133.82 -t "gotham/dpgc/#" -v
```
