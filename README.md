# ESP32 Relay-Clap

If you've ever wished you could just clap to turn on the lights, this is that project

This project pairs an ESP32 DevKit (mic + clap detection) with an Athom
Smart Plug V3 (running ESPHome on an ESP32-C3) that actually drives the
relay. The two talk to each other over MQTT.

## How it works (in short)

One ESP32 listens for claps through a mic, the other one controls the
smart plug the lamp is plugged into. When it hears a clap, it tells the
plug to flip the relay.

I'm building this step by step: get the plug responding to basic commands
first, then add remote control via Telegram, then move everything to MQTT,
and only at the end add the actual clap detection. One thing at a time.
Right now: v0.1, just validating HTTP connectivity to the plug.

## What's in here right now

- `src/main.cpp` - v0.1 sketch. Toggles the plug's relay every 10s via the
  ESPHome `web_server` REST API (`/switch/switch/toggle`), basically to
  confirm both devices can talk to each other before anything fancier.

## Build & flash

Built with [PlatformIO](https://platformio.org/) on the Arduino framework.

```
pio run
pio run -t upload
pio device monitor
```

## Roadmap

1. v0.1 - HTTP toggle every 10s, just to validate connectivity with the
   ESPHome `web_server` API.
2. v0.2 - Telegram bot (long polling) to send on/off/toggle commands.
3. v0.3 - Move to MQTT via HiveMQ (cloud, TLS). The plug subscribes to a
   command topic, the DevKit publishes to it instead of using direct HTTP.
4. v0.4 - This is the fun part: add a mic (I2S, probably an INMP441) and
   build the clap detector. RMS energy + threshold, with a small state
   machine to catch double claps.
5. v1.0 - Everything connected: clap -> MQTT via HiveMQ -> plug toggles the
   relay. Trying to keep it under 500ms, otherwise it'll feel laggy.
6. v2.0 - Add a local Mosquitto broker on a Raspberry Pi as a fallback, so
   if my wifi dies I'm not stuck with a lamp I can't turn off.

## License

MIT