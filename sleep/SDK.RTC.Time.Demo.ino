/*
    SDK.RTC.Time.Demo.ino     copied, and touched up for Arduino 8266, from SDK 3.0.2 API Reference, Appendix A
    Real Tick Counter (RTC) does not persist thru deepSleep ... API Reference
    Herein is proof.
*/

#define RTC_MAGIC 0x55aaaa55

#include <user_interface.h>
os_timer_t rtc_test_t;

#include <PolledTimeout.h>
esp8266::polledTimeout::periodicMs getSomeDeepSleep(11000);


typedef struct {
    uint64 time_acc;
    uint32 magic;
    uint32 time_base;
} RTC_TIMER_DEMO;

void rtc_count (void*)
{
    RTC_TIMER_DEMO rtc_time;
    static uint8 cnt = 0;
    system_rtc_mem_read(64, &rtc_time, sizeof(rtc_time));
    if (rtc_time.magic != RTC_MAGIC) {
        os_printf("rtc time init...\r\n");
        rtc_time.magic = RTC_MAGIC;
        rtc_time.time_acc = 0;
        rtc_time.time_base = system_get_rtc_time();
        os_printf("time base : %d \r\n", rtc_time.time_base);
    }
    os_printf("==================\r\n");
    os_printf("RTC time test : \r\n");
    uint32 rtc_ticks1, rtc_ticks2;
    uint32 st1, st2;
    uint32 cal1, cal2;
    rtc_ticks1 = system_get_rtc_time();
    st1 = system_get_time();
    cal1 = system_rtc_clock_cali_proc();
    os_delay_us(300);                       // delta between tick1/cal1 and ticks2/cal2
    st2 = system_get_time();
    rtc_ticks2 = system_get_rtc_time();
    cal2 = system_rtc_clock_cali_proc();
    os_printf(" rtc_ticks2-t1 : %d \r\n", rtc_ticks2 - rtc_ticks1);     //
    os_printf(" st2-t2 : %d \r\n", st2 - st1);
    os_printf("cal 1 : %d.%d \r\n", ((cal1 * 1000) >> 12) / 1000, ((cal1 * 1000) >> 12) % 1000 );
    os_printf("cal 2 : %d.%d \r\n", ((cal2 * 1000) >> 12) / 1000, ((cal2 * 1000) >> 12) % 1000 );
    os_printf("==================\r\n\r\n");
    rtc_time.time_acc += ( ((uint64)(rtc_ticks2 - rtc_time.time_base)) * ( (uint64)
                           ((cal2 * 1000) >> 12)) ) ;
    os_printf("rtc time acc : %lld \r\n", rtc_time.time_acc);
    os_printf("power on time : %lld us\r\n", rtc_time.time_acc / 1000);
    os_printf("power on time : %lld.%02lld S\r\n", (rtc_time.time_acc / 10000000) / 100,
              (rtc_time.time_acc / 10000000) % 100);
    rtc_time.time_base = rtc_ticks2;
    system_rtc_mem_write(64, &rtc_time, sizeof(rtc_time));
    os_printf("------------------------\r\n");
    if (5 == (cnt++)) {
        os_printf("system restart\r\n");
        system_restart();
    } else {
        os_printf("continue ...\r\n");
    }
}


void setup()
{
    Serial.begin(115200, SERIAL_8N1);       // 19200 if TX to softSerial
    Serial.println();                       // if 74880 bonus linefeed, if not terminate garbled bootloader msg

    Serial.print(__FILE__);  Serial.print(F(" ... compiled "));  Serial.print(__TIME__);
    Serial.print(F("  "));  Serial.println(__DATE__);
    Serial.println(F(" SDK Real Tick Counter, known as RTC, Demo; ported from SDK 3.0.2 API Reference, Appendix A"));

    Serial.println();

    //void user_init(void) {   // sdk runs this
    rtc_count(nullptr);
    os_printf("SDK version:%s\n", system_get_sdk_version());
    os_timer_disarm(&rtc_test_t);
    os_timer_setfn(&rtc_test_t, rtc_count, NULL);
    os_timer_arm(&rtc_test_t, 3000, 1);                 // rtc_count timer ms
    //}

}


void loop()
{
    if (getSomeDeepSleep) {
        ESP.deepSleepInstant(2e6, WAKE_NO_RFCAL);
    }
    //delayMicroseconds(10456789UL);

}
