/*
  Buddy System

  Demonstrate publish/subscribe with another Particle devices.
*/

const pin_t LED = D7;
#define WAVE_BLINK_LENGTH_MS 500
#define WAVE_BLINK_FREQ 3
#define PAT_BLINK_LENGTH_MS 5000
#define PAT_BLINK_FREQ 1

String next_simulated_event = "wave";

SYSTEM_THREAD(ENABLED);

void handle_buddy_event(const char *event, const char *data) {
    const bool is_wave = String(data) == "wave";

    // Abort if event type is not recognised
    if (!is_wave && String(data) != "pat") {
        return;
    }

    // Blink LED based on configured length/frequency for event type
    const int blink_length_ms = is_wave ? WAVE_BLINK_LENGTH_MS : PAT_BLINK_LENGTH_MS;
    const int blink_freq = is_wave ? WAVE_BLINK_FREQ : PAT_BLINK_FREQ;

    for (int i = 0; i < blink_freq; i += 1) {
        digitalWrite(LED, HIGH);
        delay(blink_length_ms);
        digitalWrite(LED, LOW);
        delay(blink_length_ms);
    }
}

void setup() {
    pinMode(LED, OUTPUT);
    Particle.subscribe("Deakin_RIOT_SIT210_Photon_Buddy", handle_buddy_event); 
}

void loop() {
    // Simulate the Photon at RIoT lab publishing wave/pat signals
    Particle.publish("Deakin_RIOT_SIT210_Photon_Buddy", next_simulated_event);
    next_simulated_event = next_simulated_event == "wave" ? "pat" : "wave";
    // Wait 20-40 seconds randomly so either event occurs "every minute or so"
    int delaySeconds = rand() % 21 + 20;
    delay(1000 * delaySeconds);
}
