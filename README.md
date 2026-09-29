# Bat-Sinal — Node B (farol)

Este repositório é só o **Node B**: o ESP32-S3 que acende o LED. Os dois nós falam pelo Mosquitto do laboratório (`mqtt://10.1.133.82:1883`). O roteiro está em `docs/`.

O **Node A** (botão, quem publica o alerta) é o projeto do Caio:

https://github.com/caiodantass/IOT-Controle-de-iluminacao

Firmware do botão na branch [`bat-button`](https://github.com/caiodantass/IOT-Controle-de-iluminacao/tree/bat-button).

| Nó | Placa | Função |
|----|--------|--------|
| A — [IOT-Controle-de-iluminacao](https://github.com/caiodantass/IOT-Controle-de-iluminacao/tree/bat-button) | ESP32-S3 + botão no GPIO 4 (outro lado em GND) | Publica `BAT_SIGNAL_ON` / `BAT_SIGNAL_OFF` |
| B — este repositório | ESP32-S3 + LED | Inscreve no tópico e acende ou apaga o LED |

## Contrato MQTT

Tópico de comando: `gotham/dpgc/berg_caio/batsignal`

- `BAT_SIGNAL_ON` → LED em nível alto. Log: `[ALERTA] Bat-Sinal Ativado! O Cavaleiro das Trevas foi convocado.`
- `BAT_SIGNAL_OFF` → LED em nível baixo. Log: `[INFO] Bat-Sinal Desativado.`

Heartbeat a cada 30 s em `gotham/dpgc/status`:

```text
{"device": "bat_sinal", "status": "ONLINE", "uptime_s": 120}
```

O Node A envia o mesmo formato com `"device": "bat_button"`.

## Layout

```text
main/app_main.c              # boot: NVS, LED, Wi-Fi, MQTT, heartbeat
components/gotham_protocol/  # tópicos e payloads
components/wifi_sta/         # Wi-Fi station
components/mqtt_app/         # cliente MQTT (subscribe + publish)
components/bat_led/          # GPIO do LED
components/heartbeat/        # status a cada 30 s
docs/                        # roteiro
```

## Clonar no laboratório

Clonar num caminho **sem acento** e abrir com o ambiente do ESP-IDF. Um `cmd` comum não acha o `idf.py`.

```text
cd C:\esp
git clone <URL-deste-repo> Bat-Sinal
```

1. **Cursor / VS Code.** File → Open Folder → `C:\esp\Bat-Sinal`. `Ctrl+Shift+P` → **ESP-IDF: Select Current ESP-IDF Version** (o IDF daquele PC). Se ainda não estiver instalado: **ESP-IDF: Open ESP-IDF Installation Manager**. Depois **ESP-IDF: SDK Configuration Editor (Menuconfig)**. No Windows essa janela é o menuconfig.
2. **ESP-IDF PowerShell** (atalho do instalador da Espressif):

```text
cd C:\esp\Bat-Sinal
idf.py set-target esp32s3
idf.py menuconfig
```

Em **Bat-Sinal Configuration**, preencher SSID e senha. Isso grava o `sdkconfig` local (não vai no git). Sem isso o build sobe com SSID vazio e a placa não conecta.

```text
idf.py build
idf.py -p PORT flash monitor
```

| Configuração | Valor |
|--------------|--------|
| Alvo | ESP32-S3 |
| LED | GPIO **10**, ativo em alto |
| Broker | `mqtt://10.1.133.82:1883` |
| Flash | 8 MB, partição large app |

Hardware: **GPIO 10 → 220 Ω → anodo do LED**, catodo no GND.

## Teste sem o botão

```text
mosquitto_pub -h 10.1.133.82 -t gotham/dpgc/berg_caio/batsignal -m "BAT_SIGNAL_ON"
mosquitto_pub -h 10.1.133.82 -t gotham/dpgc/berg_caio/batsignal -m "BAT_SIGNAL_OFF"
mosquitto_sub -h 10.1.133.82 -t "gotham/dpgc/#" -v
```

## Se compilou e a placa não responde

1. Alvo errado: `idf.py set-target esp32s3`.
2. Wi-Fi vazio: o monitor mostra `Wi-Fi SSID vazio!`. Rodar o menuconfig.
3. LED noutro pino: o padrão é o GPIO 10. Trocar em **Bat-Sinal Configuration**.
4. Fora do Wi-Fi do laboratório, ou broker inacessível: o MQTT não conecta.
5. Caminho com acento (por exemplo `Área de Trabalho`): copiar o projeto para `C:\esp\Bat-Sinal` antes de compilar.
