// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins
#define LED_BUILTIN 2;
static const int led_pin = LED_BUILTIN;

// Our task: blink a led
void toggleLED(void *parameter) {
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(700 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}

void toggleLED2(void *parameter) {
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(330 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(330 / portTICK_PERIOD_MS);
  }
}

// portTICK_PERIOD_MS = 1ms (FreeRTOS default)

void setup() {
  //Configure pin
  pinMode(led_pin, OUTPUT);

  // Task to run forever
  xTaskCreatePinnedToCore( // Use xTaskCreate () in vanilla FreeRTOS
        toggleLED,     // Function to be called
        "Toggle LED",  // NAme of task
        1024,          // Stack size (bytes in ESP32, are called words in FreeRTOS)
        NULL,          // Parameter to pass to a function
        1,             // Task priority ( 0 - 24 )
        NULL,          // Task handle (logger)
        app_cpu);      // Run on one core for demo purposes (ESP32 only)

  xTaskCreatePinnedToCore( 
        toggleLED2,     
        "Toggle LED2",  
        1024,          
        NULL,          
        1,             
        NULL,          
        app_cpu);

  // If this was vanilla FreeRTOS , you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  // put your main code here, to run repeatedly:

}
