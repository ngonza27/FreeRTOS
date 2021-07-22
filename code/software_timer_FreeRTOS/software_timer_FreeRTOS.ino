#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Globals
static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

// Timer callback function
void timer_callback(TimerHandle_t xTimer) {
  // Print message if timer 0 finished
  if((uint32_t)pvTimerGetTimerID(xTimer) == 0){
    Serial.println("One shot timer has expired");
  }

  // Print message if timer 0 finished
  if((uint32_t)pvTimerGetTimerID(xTimer) == 1){
    Serial.println("Auto-timer has expired");
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Starting software timer demo");

  one_shot_timer = xTimerCreate("One-shot timer",          // Name of timer
                                2000 / portTICK_PERIOD_MS, // Period of timer (int ticks)
                                pdFALSE,                   // Auto-reload?
                                (void *)0,                 // Timer Id
                                timer_callback);           // Callback funtion to be executed

  auto_reload_timer = xTimerCreate("Auto-shot timer",          // Name of timer
                                1000 / portTICK_PERIOD_MS, // Period of timer (int ticks)
                                pdTRUE,                   // Auto-reload?
                                (void *)1,                 // Timer Id
                                timer_callback);           // Callback funtion to be executed
  
  // Check to make sure timers were created
  if(one_shot_timer == NULL || auto_reload_timer == NULL){
    Serial.println("Error, could not create one of the timers");
  } else {
    Serial.println("Starting timers");
    xTimerStart(one_shot_timer, portMAX_DELAY);
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }
  vTaskDelete(NULL);
}

void loop() { }
