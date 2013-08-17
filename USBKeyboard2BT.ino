/*
 * USBKeyboard2BT
 * Making an USB Keyboard into a Bluetooth Keyboard.
 *
 * !DISCLAIMER!
 * This software is absolutely NOT perfect and may damage to your keyboard.
 * Please use this software only if you have mature knowledge of electronical 
 * enginnering and understand the risk of this software.
 * I'm not accountable for the damage or detriment caused by using this software.
 */
#include <avr/pgmspace.h>

#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <address.h>
#include <hidboot.h>

#include <printhex.h>
#include <message.h>
#include <hexdump.h>
#include <parsetools.h>

class HIDKeyboardParser : public KeyboardReportParser
{
    virtual void Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

void HIDKeyboardParser::Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
    sendKeyCodesBySerial(buf[0], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
}

USB usb;
HIDBoot<HID_PROTOCOL_KEYBOARD> HidKeyboard(&usb);
uint32_t next_time;
HIDKeyboardParser parser;

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");

    if (usb.Init() == -1) {
        Serial.println("OSC did not start.");
    }
    delay(200);
    next_time = millis() + 5000;

    HidKeyboard.SetReportParser(0, (HIDKeyboardParser*)&parser);
}

void loop()
{
    usb.Task();
}

void sendKeyCodesBySerial(uint8_t modifiers,
                          uint8_t keycode0,
                          uint8_t keycode1,
                          uint8_t keycode2,
                          uint8_t keycode3,
                          uint8_t keycode4,
                          uint8_t keycode5)
{
    Serial.write(0xFD); // Raw Report Mode
    Serial.write(0x09); // Length
    Serial.write(0x01); // Descriptor 0x01=Keyboard

    /* send key codes(8 bytes all) */
    Serial.write(modifiers); // modifier keys
    Serial.write(0x00, 1);   // reserved
    Serial.write(keycode0);  // keycode0
    Serial.write(keycode1);  // keycode1
    Serial.write(keycode2);  // keycode2
    Serial.write(keycode3);  // keycode3
    Serial.write(keycode4);  // keycode4
    Serial.write(keycode5);  // keycode5
    delay(5);
}
