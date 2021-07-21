#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Globals
static int shared_var = 0;
static SemaphoreHandle_t mutex;


void increment_task(void * parameters){
  int local_var;

  while(1){
    // Take mutex value prior to critical section
    if(xSemaphoreTake(mutex, 0) == pdTRUE) {
      local_var = shared_var;
      local_var++;
      vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
      shared_var = local_var;

      // Returning the mutex
      xSemaphoreGive(mutex);

      Serial.println(shared_var);
    } else {
      // Do something else if mutex is unavailable
    } 
  }
}

void setup() {
  randomSeed(analogRead(0));
  
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Mutex demo");
  
  // Create mutex beofre starting the tasks 
  mutex = xSemaphoreCreateMutex();
  
  //Start task
  xTaskCreatePinnedToCore(increment_task,
                          "Increment task1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
                          
  xTaskCreatePinnedToCore(increment_task,
                          "Increment task2",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
}

void loop() { }
