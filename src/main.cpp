#include <Arduino.h>
#include <time.h>
#include "net/wifi_manager.hpp"
#include "cloud/telegram_bot.hpp"

// Buenos Aires (UTC-3, sin horario de verano). Ajustá si tu zona es otra.
static constexpr long GMT_OFFSET_SEC      = -3 * 3600;
static constexpr int  DAYLIGHT_OFFSET_SEC = 0;

// ── Setup ─────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 2000) {}

    log_i("=== Smart Plug Bot boot ===");

    // Multi-network connect with timeout. On total failure we reboot, same as
    // the Menta device, rather than spin forever.
    if (!net::WiFiManager::begin(30000)) {
        log_e("WiFi failed — rebooting in 5 s");
        delay(5000);
        ESP.restart();
    }

    // NTP sync — needed so cloud::CommandLog can stamp /historial entries
    // with real dates instead of the 1970 epoch. Requires WiFi to be up.
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org", "time.nist.gov");
    log_i("Waiting for NTP sync...");
    time_t now = time(nullptr);
    uint32_t ntp_start = millis();
    while (now < 8 * 3600 * 2 && millis() - ntp_start < 10000) {  // sanity: year > 1970+
        delay(250);
        now = time(nullptr);
    }
    if (now < 8 * 3600 * 2) {
        log_w("NTP sync timed out — /historial timestamps will be wrong until it syncs later");
    } else {
        log_i("NTP synced");
    }

    // Boot notification so you know the device came online.
    cloud::TelegramBot::send("🔌 Smart Plug Bot online");

    log_i("Setup complete");
}

// ── Loop ──────────────────────────────────────────────────────────────────────

void loop() {
    // Silently reconnect if WiFi dropped (throttled internally to 15 s).
    net::WiFiManager::loop();

    // Poll Telegram for commands (self-throttled to POLL_INTERVAL_MS).
    cloud::TelegramBot::poll();
}
