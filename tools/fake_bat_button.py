#!/usr/bin/env python3
"""Simula o Node A (Bat-Button) publicando no Mosquitto do lab."""

import argparse
import socket
import sys
import time

try:
    import paho.mqtt.publish as publish
    import paho.mqtt.client as mqtt
except ImportError:
    print("Instale a dependencia: pip install paho-mqtt")
    sys.exit(1)

BROKER = "192.168.1.107"
PORT = 1883
TOPIC = "gotham/dpgc/batsignal"
TOPIC_STATUS = "gotham/dpgc/status"
CONNECT_TIMEOUT = 5


def local_ip() -> str:
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except OSError:
        return "?"


def check_broker(broker: str, port: int) -> bool:
    print(f"Seu IP nesta rede: {local_ip()}")
    print(f"Testando {broker}:{port} ...")
    try:
        with socket.create_connection((broker, port), timeout=CONNECT_TIMEOUT):
            print("OK: broker acessivel na porta 1883")
            return True
    except TimeoutError:
        print(f"FALHA: timeout ao conectar em {broker}:{port}")
    except OSError as exc:
        print(f"FALHA: {exc}")

    print()
    print("Possiveis causas:")
    print("  1. PC do professor (192.168.1.107) desligado ou Mosquitto parado")
    print("  2. IP do broker mudou — confirme com o professor")
    print("  3. Firewall do Windows bloqueando porta 1883 no PC do broker")
    print("  4. Isolamento de clientes (AP isolation) no roteador MERCUSYS")
    print()
    print("Pergunte ao professor: Mosquitto esta rodando em 192.168.1.107?")
    return False


def send(message: str, broker: str, port: int) -> None:
    if not check_broker(broker, port):
        sys.exit(1)

    try:
        publish.single(
            TOPIC,
            message,
            hostname=broker,
            port=port,
            qos=0,
        )
    except (TimeoutError, OSError) as exc:
        print(f"Erro ao publicar: {exc}")
        sys.exit(1)

    print(f"OK -> {TOPIC} = {message}")


def listen(broker: str, port: int, seconds: int) -> None:
    if not check_broker(broker, port):
        sys.exit(1)

    print(f"Ouvindo gotham/dpgc/# por {seconds}s...")

    def on_message(_client, _userdata, msg):
        print(f"  [{msg.topic}] {msg.payload.decode(errors='replace')}")

    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.on_message = on_message
    client.connect(broker, port, keepalive=60)
    client.subscribe("gotham/dpgc/#")
    client.loop_start()
    time.sleep(seconds)
    client.loop_stop()
    client.disconnect()


def main() -> None:
    parser = argparse.ArgumentParser(description="Fake Bat-Button (Node A)")
    parser.add_argument(
        "action",
        nargs="?",
        choices=["on", "off", "toggle", "listen", "check"],
        default="on",
        help="on/off/toggle/listen/check (padrao: on)",
    )
    parser.add_argument("--broker", default=BROKER)
    parser.add_argument("--port", type=int, default=PORT)
    parser.add_argument("--listen", type=int, default=15, help="segundos ao usar listen")
    args = parser.parse_args()

    if args.action == "check":
        sys.exit(0 if check_broker(args.broker, args.port) else 1)

    if args.action == "listen":
        listen(args.broker, args.port, args.listen)
        return

    if args.action == "on":
        send("BAT_SIGNAL_ON", args.broker, args.port)
    elif args.action == "off":
        send("BAT_SIGNAL_OFF", args.broker, args.port)
    elif args.action == "toggle":
        send("BAT_SIGNAL_ON", args.broker, args.port)
        time.sleep(2)
        send("BAT_SIGNAL_OFF", args.broker, args.port)


if __name__ == "__main__":
    main()
