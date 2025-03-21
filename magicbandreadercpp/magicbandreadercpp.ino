#include <Adafruit_NeoPixel.h>
//Windows:
//https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.msi
//cmd:
//set ARDUINO_LIBRARY_ENABLE_UNSAFE_INSTALL=true
//arduino-cli lib install --git-url https://github.com/adafruit/Adafruit_NeoPixel

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 61
const int led_count = 61;
const int dims = 2;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

bool in_setup = true;
int coords[led_count][dims] = { //Radius, Angle of each pixel in series
{0, 0},
{1, 0},{1, 45},{1, 90},{1, 135},{1, 180},{1, 225},{1, 270},{1, 315},
{2, 0},{2, 30},{2, 60},{2, 90},{2, 120},{2, 150},{2, 180},{2, 210},{2, 240},{2, 270},{2, 300},{2, 330},
{3, 0},{3, 23},{3, 45},{3, 68},{3, 90},{3, 113},{3, 135},{3, 158},{3, 180},{3, 203},{3, 225},{3, 248},{3, 270},{3, 293},{3, 315},{3, 338},
{4, 0},{4, 15},{4, 30},{4, 45},{4, 60},{4, 75},{4, 90},{4, 105},{4, 120},{4, 135},{4, 150},{4, 165},{4, 180},{4, 195},{4, 210},{4, 225},{4, 240},{4, 255},{4, 270},{4, 285},{4, 300},{4, 315},{4, 330},{4, 345}
};

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(50);  // max = 255
  Serial.begin(9600);
  // startLights(coords); // Startup test now done by pi
}
void blank() {
  uint32_t black = strip.Color(0, 0, 0);
  strip.fill(black, 0, strip.numPixels());
  strip.show();
}

void reading(int coords[][2], int angle_rate) {
  int reading_ring_speed = 0;
  int reading_bar_length_degree = 60;

  for (int angle = 360; angle > 0; angle = angle - angle_rate) {
    blank();
    for (int i = 0; i < led_count; i++) {
      int led_radius = coords[i][0];
      if (led_radius == 4) {
        int led_angle = coords[i][1];
        int angle_extreme = angle + reading_bar_length_degree;
        uint32_t white = strip.Color(255, 255, 255);  // White reading swirl
        if (angle_extreme > 360 and led_angle <= angle_extreme - 360) {
          strip.setPixelColor(i, white);
        }

        else if (led_angle >= angle && led_angle <= angle_extreme) {
          strip.setPixelColor(i, white);
        }
      }
    }
    strip.show();
  }
}
float intensity;
void glow(int r, int g, int b, float time_ms) {
  for (int i = 0; i < 100; i++) {
    intensity = i / 100.0;
    uint32_t color = strip.Color(int(r * intensity), int(g * intensity), int(b * intensity));
    strip.fill(color, 0, led_count);
    strip.show();
    delay(time_ms / 100);
  }
}

void startLights(int coords[][2]) {
  reading(coords, 1);
  reading(coords, 2);
  reading(coords, 4);
  reading(coords, 6);
  reading(coords, 8);
  reading(coords, 10);
  blank();
  glow(0, 100, 0, 1000);
  delay(5000);
  blank();
}

// void Time(int coords[][2], String str){
//   //Thhmmss.sss
//       //012345678910
//        int h1 = str.charAt(1)- '0';
//        int h2 = str.charAt(2)- '0';
//        int m1 = str.charAt(3)- '0';
//        int m2 = str.charAt(4)- '0';
//        int s1 = str.charAt(5)- '0';
//        int s2 = str.charAt(6)- '0';
//        int s3 = str.charAt(8)- '0';
//        int s4 = str.charAt(9)- '0';
//        int s5 = str.charAt(10)- '0';

//        int hh = h1*10+h2;
//        int mm = m1*10+m2;
//        float ss = s1*10+s2+s3/10.0+s4/100.0+s5/1000.0;
//        Serial.println(h2);
//        Serial.println(m2);
//        Serial.println(s2);
//        Serial.println();
//        delay(1000);

//        int hh_radius_list[] = {0, 1, 2, 3, 4};
//        int mm_radius_list[] = {0, 1, 2, 3, 4};
//        int ss_radius_list[] = {4};

//        float hh_deg = Tilt(hh/12.0*360.0);
//        float mm_deg = Tilt(mm/60.0*360.0);
//        float ss_deg = Tilt(ss/60.0*360.0);

//        float hh_peak_width_deg = 5.0;
//        float mm_peak_width_deg = 25.0;
//        float ss_peak_width_deg = 25.0;


//        blank();
//       //  showBar(coords, ss_radius_list, ss_deg, ss_peak_width_deg, 255, 255, 255);
//        showBar(coords, mm_radius_list, mm_deg, mm_peak_width_deg, 0, 0, 255);
//       //  showBar(coords, hh_radius_list, hh_deg, hh_peak_width_deg, 255, 0, 0);


//        //  showBar(coords, mm_radius, mm_deg, mm_peak_width_deg, 0, 0, 100);

//        strip.show();
// }

float Tilt(float deg) {
  return deg - 90.0;
}

void showBar(int coords[][2], int radius_list[], float deg, float peak_width_deg, float r, float g, float b) {
  for (int i = 0; i < led_count; i++) {

    int led_radius = coords[i][0];

    bool valid_radius = false;
    for (int i = 0; i < sizeof(radius_list); i++) {
      if (led_radius = radius_list[i]) { valid_radius = true; }
    }

    if (valid_radius) {
      int led_angle = coords[i][1];

      float led_to_center_deg = abs(led_angle - deg);
      float bar_intensity = min(1.0, max(0.0, (peak_width_deg / (led_to_center_deg + 0.001)) - 1));

      if (bar_intensity > 0) {
        // Serial.print(i);
        uint32_t color = strip.Color(int(r * bar_intensity), int(g * bar_intensity), int(b * bar_intensity));
        strip.setPixelColor(i, color);
      }
    }
  }
}

void loop() {
  // String str = "T030130.000";
  // Time(coords, str);
  while (Serial.available() > 0) {
    String str = Serial.readString();
    // if(str.indexOf("T") == 0){
    //   Time(coords, str);
    // } //unused clock functionality is halfway implemented. Activate by sending serial "T" timestamp
    if (str.indexOf("send") > -1) {
      Serial.println("identified");
      startLights(coords);

    } else {
      Serial.println("unknown");
    }
  }
}