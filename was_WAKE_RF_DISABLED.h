#define MS Serial.print(millis()); Serial.write(' ');

// Enable 8266 (3.0.2) RF 'inline' or 'on-the-fly' when inadvertently RFMode is WAKE_RF_DISABLED.
//  might be brittle; depends on mysterious conditions that leave status WL_IDLE_STATUS.
//  Case: power on with residual RF disabled, eg. a logger that only woke with wifi infrequently.
//  Low power/time cost if RF enabled.
//  use: if (was_WAKE_RF_DISABLED()) { // enabled, carry on }
//
bool was_WAKE_RF_DISABLED ()
{
    int8_t wl_status = WiFi.status();
    if (wl_status != WL_CONNECTED)      // SDK may already have connected Wifi (without enableWiFiAtBootTime)
    {
        int8_t wl_mode   = WiFi.getMode();
        bool was_disabled = false;

        // test wifi mode
        if (WIFI_OFF == wl_mode) {
            MS Serial.println ("WIFI_OFF; so enableSTA(true)");
        } else {
            MS Serial.print(rfModeTranslate (wl_mode));
            Serial.println ("; but perturb to update status");
        }
        WiFi.enableSTA(true);                   // 2ms if already enabled, else 100ms, but regained if begin called

        // updated status
        wl_status = WiFi.status();
        if (WL_IDLE_STATUS == wl_status) {      // magic happened here
            Serial.print(millis());  Serial.println (" WAKE_RF_DISABLED so ...");
            was_disabled = true;
        }

        ///  RF ENABLE and Wifi  ///
        if (was_disabled)
        {
            // ~100ms
            MS Serial.println ("forceSleepBegin; we don't need no *****ing delays");    WiFi.forceSleepBegin();
            // ~200ms
            MS Serial.println ("forceSleepWake; restoring previous wifi mode");     WiFi.forceSleepWake();
            // ~1ms, did not enable RF
            //MS Serial.println ("wifi_station_connect"); wifi_station_connect();  // 
        }
        return was_disabled;
    } else {
        return false;
    }
}
