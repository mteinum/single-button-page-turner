/*
 * Kobo Page Turner - BLE Keyboard Controller for M5Stack AtomS3
 * 
 * Author: Morten Teinum (morten.teinum@gmail.com)
 * Date: December 11, 2025
 * Device: M5Stack AtomS3
 * 
 * Description:
 *   A BLE keyboard device that sends page navigation commands to Kobo e-readers.
 *   Short press: Next page (Right Arrow)
 *   Long press: Previous page (Left Arrow)
 */

#include <M5Unified.h>
#include <BleKeyboard.h>

// =========================
// Config
// =========================

static const char* BLE_DEVICE_NAME = "Kobo PageTurner";

const uint16_t LONG_PRESS_MS = 600;   // >= 600ms = previous page

// Status icon placement (small square in top-right corner)
const int ICON_SIZE = 14;
int ICON_X;   // will be set in setup() based on width
int ICON_Y = 4;

// BLE keyboard instance
BleKeyboard bleKeyboard(BLE_DEVICE_NAME, "DIY", 100);

// State for button press timing
bool     buttonPressed    = false;
uint32_t buttonPressStart = 0;

// Track connection state to update icon only when it changes
bool lastConnectedState = false;


// =========================
// UI helpers
// =========================

void drawStatusIcon(bool connected) {
  if (connected) {
    // Filled green square
    M5.Display.fillRect(ICON_X, ICON_Y, ICON_SIZE, ICON_SIZE, GREEN);
  } else {
    // Clear area, then draw red outline
    M5.Display.fillRect(ICON_X, ICON_Y, ICON_SIZE, ICON_SIZE, BLACK);
    M5.Display.drawRect(ICON_X, ICON_Y, ICON_SIZE, ICON_SIZE, RED);
  }
}

void drawBaseScreen() {
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(3);
  M5.Display.setTextDatum(middle_center);

  int cx = M5.Display.width()  / 2;
  int cy = M5.Display.height() / 2;

  M5.Display.drawString("Kobo",      cx, cy - 18);
  M5.Display.drawString("Short: ->", cx, cy);
  M5.Display.drawString("Long : <-", cx, cy + 18);
}


// =========================
// BLE key sending
// =========================

void sendKeyOnce(uint8_t keycode) {
  if (!bleKeyboard.isConnected()) {
    Serial.println("Not connected; ignoring key press");
    return;
  }

  Serial.print("Sending key 0x");
  Serial.println(keycode, HEX);

  bleKeyboard.press(keycode);
  delay(60);              // brief hold
  bleKeyboard.releaseAll();
  delay(20);              // settle
}


// =========================
// Arduino setup
// =========================

void setup() {
  Serial.begin(115200);
  delay(200);

  // ---- M5Unified init ----
  auto cfg = M5.config();
  cfg.clear_display = true;   // clear screen at boot
  // cfg.external_display = false; // (default) we just use internal
  M5.begin(cfg);              // auto-detects AtomS3 etc. 

  M5.Display.setRotation(0);

  // Compute icon X based on screen width
  ICON_X = M5.Display.width() - ICON_SIZE - 4;

  drawBaseScreen();
  drawStatusIcon(false);   // not connected initially

  Serial.println("Starting BLE keyboard...");
  bleKeyboard.begin();
}


// =========================
// Arduino loop
// =========================

void loop() {
  // Must be called regularly for buttons, etc.
  M5.update();

  // --- Update BLE connection icon if state changed ---
  bool connected = bleKeyboard.isConnected();
  if (connected != lastConnectedState) {
    lastConnectedState = connected;
    drawStatusIcon(connected);
    Serial.print("BLE state changed. Connected = ");
    Serial.println(connected ? "YES" : "NO");
  }

  // --- Handle button press timing on AtomS3 (BtnA) ---
  if (M5.BtnA.wasPressed()) {
    buttonPressed    = true;
    buttonPressStart = millis();
  }

  if (M5.BtnA.wasReleased() && buttonPressed) {
    uint32_t pressDuration = millis() - buttonPressStart;
    buttonPressed = false;

    Serial.print("Button duration: ");
    Serial.print(pressDuration);
    Serial.println(" ms");

    if (pressDuration >= LONG_PRESS_MS) {
      Serial.println("Long press -> PREVIOUS page");
      sendKeyOnce(KEY_LEFT_ARROW);
    } else {
      Serial.println("Short press -> NEXT page");
      sendKeyOnce(KEY_RIGHT_ARROW);
    }
  }

  delay(5);   // Small delay to avoid a hot loop
}
