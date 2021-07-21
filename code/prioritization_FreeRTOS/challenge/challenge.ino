#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Led blink time
static int blink_delay = 500;

unsigned int serial_input;

// Pins
#define LED_BUILTIN 2;
static const int led_pin = LED_BUILTIN;

// Task: print to Serial Terminal with lower priority
void startTask1(void *parameter) {
  while(1) {
    if(Serial.available() > 0) {
      String inputt = Serial.readStringUntil('\n') ;
      serial_input = inputt.toInt();
      blink_delay = serial_input;
      Serial.print("Updted delay period: ");
      Serial.println(blink_delay);
    }
  }
}

// Task: print character to serial terminal with higher priority
void startTask2(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(blink_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(blink_delay / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);
  Serial.print("Program starting, enter a time (ms): ");

  xTaskCreatePinnedToCore(startTask2,
                          "Task 2",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(startTask1,
                          "Task 1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
                          
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() { }
