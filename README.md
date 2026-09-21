# Bat-Sinal — Node B (Rooftop Actuator)

ESP-IDF firmware for the Bat-Sinal LED. Connects to lab Wi-Fi as a station, subscribes to the Mosquitto broker, and drives a GPIO LED.

## Layout

```text
main/app_main.c                 # boot: NVS, LED, Wi-Fi, MQTT, heartbeat
components/gotham_protocol/     # MQTT topics and payloads
components/wifi_sta/            # Wi-Fi station
components/mqtt_app/            # esp-mqtt client (subscribe + publish)
components/bat_led/             # GPIO LED actuator
components/heartbeat/           # 30s status on gotham/dpgc/status
docs/                           # assignment
```

## MQTT contract

- Broker: `mqtt://10.1.133.82:1883`
- Command topic: `gotham/dpgc/batsignal`
  - `BAT_SIGNAL_ON` → LED GPIO high
  - `BAT_SIGNAL_OFF` → LED GPIO low
- Heartbeat every 30s on `gotham/dpgc/status`:
  `{"device": "bat_sinal", "status": "ONLINE", "uptime_s": 120}`

## Configure

```text
idf.py set-target esp32c6
idf.py menuconfig
```

In **Bat-Sinal Configuration** set the lab SSID and password. Broker URI and LED GPIO (default 8) are already in `sdkconfig.defaults`. Do not commit a `sdkconfig` that contains the Wi-Fi password.

Hardware: `GPIO → 220 Ω → LED anode`, cathode to GND. Active high.

## Build and flash

```text
idf.py -p PORT flash monitor
```

If `idf.py build` fails on `picolibc.specs` with `\x0area de Trabalho`, copy the project to a path without accents (for example `C:\esp\Bat-Sinal`) and build from there. The `Á` in *Área de Trabalho* breaks the RISC-V GCC spec file path on Windows.

## Test without the button node

```text
mosquitto_pub -h 10.1.133.82 -t gotham/dpgc/batsignal -m "BAT_SIGNAL_ON"
mosquitto_pub -h 10.1.133.82 -t gotham/dpgc/batsignal -m "BAT_SIGNAL_OFF"
mosquitto_sub -h 10.1.133.82 -t "gotham/dpgc/#" -v
```
