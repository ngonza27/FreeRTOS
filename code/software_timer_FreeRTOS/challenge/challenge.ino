#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Globals
static TimerHandle_t one_shot_timer = NULL;

// Pins
#define LED_BUILTIN 2;
static const int led_pin = LED_BUILTIN;

static int delay_time = 1000 / portTICK_PERIOD_MS;

// Timer callback function
void turn_off_led(TimerHandle_t xTimer) {
  digitalWrite(led_pin, 0);
}


void serial_monitor(void *parameter) {
  while(1) {
    if(Serial.available() > 0) {
      digitalWrite(led_pin, 1);
      String serial_input = Serial.readStringUntil('\n');
      Serial.println(serial_input);
      xTimerStart(one_shot_timer, portMAX_DELAY);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Starting software timer demo");

  one_shot_timer = xTimerCreate("Turn off led",          // Name of timer
                                delay_time,                  // Period of timer (int ticks)
                                pdFALSE,                     // Auto-reload?
                                (void *)0,                   // Timer Id
                                turn_off_led);               // Callback funtion to be executed

  xTaskCreatePinnedToCore(serial_monitor,
                          "Serial monitor task",         
                          1024, 
                          NULL,                   
                          1,                 
                          NULL,
                          app_cpu);           
  
  // Check to make sure timers were created
  if(one_shot_timer == NULL){
    Serial.println("Error, could not create one of the timers");
  }
  vTaskDelete(NULL);
}

void loop() { }
