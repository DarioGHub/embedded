#define MS Serial.print(millis()); Serial.write(' ');

// Further research required, have seen it not reenable RF. Look into sniffing UART1 TX FIFO hardware buffer.

// A method to detect if awake with RFMode inadvertently WAKE_RF_DISABLED (8266 core 3.0.2),
//  and enable RF 'on-the-fly' without another deepSleep call looking for a flag, but with forceSleepBegin/forceSleepWake.
//  That combo was consuming ~350ms, or about 50% longer than a well configured built-in auto connect (sdk).
//  Use case: a project that woke many times from deepSleep only to take readings, before a 'wake with wifi' cycle.
//  Then normal operation is interrupted by an unexpected restart, eg. power outage, that would boot up with the last known RF & wifi modes.
//  Feature: Low power/time cost if RF enabled. Limitation: brittle as depends on conditions that leave wifi status WL_IDLE_STATUS.
//  Use: was_WAKE_RF_DISABLED(); // if was disabled now enabled, carry on normally
//  inspired by https://www.bakke.online/index.php/2017/05/22/reducing-wifi-power-consumption-on-esp8266-part-3/,
//  and https://github.com/esp8266/Arduino/issues/3072#issuecomment-741524721 by rforro
//
bool was_WAKE_RF_DISABLED ()
{
    bool was_disabled = false;
    int8_t wl_status = WiFi.status();
    if (wl_status != WL_CONNECTED)      // SDK may already have connected Wifi (without enableWiFiAtBootTime)
    {
        int8_t wl_mode = WiFi.getMode();

        // report wifi mode upon entry
        if (WIFI_OFF == wl_mode) {
            MS Serial.println ("WIFI_OFF, so enableSTA(true)");
        } else {
            MS Serial.print(rfModeTranslate (wl_mode));
            Serial.println (", but perturb to update status");
        }
        WiFi.enableSTA(true);           // 2ms if already enabled, else 100ms, but regained if begin called

        // infer current RFMode from wifi status
        wl_status = WiFi.status();
        if (WL_IDLE_STATUS == wl_status) {      // <-- brittle artifact here
            Serial.print(millis());  Serial.println (" WAKE_RF_DISABLED so ...");
            was_disabled = true;
        }

        // Enable RF if disabled
        if (was_disabled)
        {
            MS Serial.println ("forceSleepBegin; we don't need no *****ing delays");
            WiFi.forceSleepBegin();     // ~110ms
            MS Serial.println ("forceSleepWake; restoring previous wifi mode");
            WiFi.forceSleepWake();      // ~220ms, ~100ms if RF was already enabled
            //MS Serial.println ("wifi_station_connect"); wifi_station_connect();   // ~1ms, did not enable RF
        }
    }
    return was_disabled;
}
