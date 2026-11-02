#define BLYNK_TEMPLATE_ID "TMPL39DzPVbuT"
#define BLYNK_TEMPLATE_NAME "Network Rack Tamper Detection And Vibration Alert"
#define BLYNK_AUTH_TOKEN "jghwUtObY_hHoBNzKdjuzLBx7NNZfIZC"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Wi-Fi details
char ssid[] = "Pinku";
char pass[] = "nopassword";

// Hardware pins
#define VIBRATION_PIN D5
#define LED_PIN D6
#define BUZZER_PIN D7

BlynkTimer timer;

// Keeps track of whether the current vibration
// has already generated a notification
bool tamperState = false;

void checkVibration()
{
  int vibration = digitalRead(VIBRATION_PIN);

  // ==========================================
  // TAMPER DETECTED
  // ==========================================
  if (vibration == HIGH)
  {
    // Physical red LED ON
    digitalWrite(LED_PIN, HIGH);

    // Buzzer ON
    digitalWrite(BUZZER_PIN, HIGH);

    // Update Blynk
    Blynk.virtualWrite(V0, "TAMPER DETECTED");
    Blynk.virtualWrite(V1, 1);
    Blynk.virtualWrite(V2, 1);

    // Send ONE notification for this incident
    if (tamperState == false)
    {
      Blynk.logEvent(
        "rack_tamper",
        "Network Rack Tampering Detected!"
      );

      tamperState = true;
    }
  }

  // ==========================================
  // SAFE
  // ==========================================
  else
  {
    // Physical red LED OFF
    digitalWrite(LED_PIN, LOW);

    // Buzzer OFF
    digitalWrite(BUZZER_PIN, LOW);

    // Update Blynk
    Blynk.virtualWrite(V0, "SAFE");
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);

    // Ready for the next tampering incident
    tamperState = false;
  }
}

void setup()
{
  // Start Serial Monitor
  Serial.begin(115200);

  // Sensor
  pinMode(VIBRATION_PIN, INPUT);

  // LED
  pinMode(LED_PIN, OUTPUT);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Initially turn everything OFF
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Connect NodeMCU to Blynk through Wi-Fi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Check sensor every 200 milliseconds
  timer.setInterval(200L, checkVibration);
}

void loop()
{
  Blynk.run();
  timer.run();
}