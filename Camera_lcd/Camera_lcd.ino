#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "esp_camera.h"
#include <JPEGDecoder.h>

#define TFT_CS    12
#define TFT_RST   13
#define TFT_DC    15
#define TFT_MOSI   14  //SDA
#define TFT_SCLK   2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);

  // Khởi tạo màn hình TFT
  tft.initR(INITR_BLACKTAB);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  delay(1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("DANG KHOI DONG");
  delay(2000);

  // Khởi tạo camera
  camera_config_t config;
  // ... (cấu hình chân camera)
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void loop() {
  // Chụp hình ảnh từ camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Hiển thị hình ảnh lên màn hình TFT
  displayJPEG(fb->buf, fb->len);

  // Giải phóng bộ nhớ được cấp phát cho dữ liệu hình ảnh của camera
  esp_camera_fb_return(fb);
  Serial.println("OK");
  delay(1000);  // Chờ 1 giây trước khi chụp hình ảnh tiếp theo
}

void displayJPEG(uint8_t *image, size_t length) {
  // Hiển thị hình ảnh JPEG lên màn hình TFT
  tft.fillScreen(ST77XX_BLACK);
  JPEGDecoder decoder;
  decoder.setDCTMethod(FastDCT); // Có thể chọn DCTMethod khác tùy thuộc vào board và yêu cầu về tốc độ

  // Bắt đầu giải mã
  int16_t width, height;
  decoder.getSize(image, length, width, height);
  decoder.decode(image, length);

  // Tạo buffer để lưu trữ dữ liệu pixel từ hình ảnh JPEG
  uint16_t *buffer = (uint16_t *)malloc(width * sizeof(uint16_t));
  if (!buffer) {
    Serial.println("Memory allocation failed");
    return;
  }

  // Hiển thị từng dòng của hình ảnh lên màn hình TFT
  for (int16_t y = 0; y < height; y++) {
    decoder.getScanLine(image, length);

    // Chuyển đổi dữ liệu pixel sang màu 16-bit và hiển thị lên màn hình TFT
    for (int16_t x = 0; x < width; x++) {
      buffer[x] = decoder.pixel565(x);
    }
    tft.pushColors(buffer, width);
  }

  // Giải phóng bộ nhớ được cấp phát cho buffer
  free(buffer);
}
