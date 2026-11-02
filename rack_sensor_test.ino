#define BLYNK_TEMPLATE_ID "TMPL39DzPVbuT"
#define BLYNK_TEMPLATE_NAME "Network Rack Tamper Detection And Vibration Alert"
#define BLYNK_AUTH_TOKEN "jghwUtObY_hHoBNzKdjuzLBx7NNZfIZC"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Pinku";
char pass[] = "nopassword";

#define VIBRATION_PIN D5

BlynkTimer timer;

void checkVibration()
{
  int vibration = digitalRead(VIBRATION_PIN);

  if (vibration == HIGH)
  {
    // Tampering detected
    Blynk.virtualWrite(V0, "TAMPER DETECTED");
    Blynk.virtualWrite(V1, 1);
    Blynk.virtualWrite(V2, vibration);

    Blynk.logEvent("rack_tamper", "Network Rack Tampering Detected!");
  }
  else
  {
    // Rack is safe
    Blynk.virtualWrite(V0, "SAFE");
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, vibration);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(VIBRATION_PIN, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(500L, checkVibration);
}

void loop()
{
  Blynk.run();
  timer.run();
}