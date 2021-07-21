// Using 1 just 1 core for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

void testTask(void *parameter) {
  while(1) {
    int a = 1;
    int b[100];
    
    for(int i = 0; i < 100; ++i){
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    // Printing the remaining stack memory (words)
    Serial.print("High watermark words: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // Printing out number of free heap memory bytes before malloc
    Serial.print("Heap before malloc (Bytes): ");
    Serial.println(xPortGetFreeHeapSize());
    int *ptr = (int*) pvPortMalloc(1024 * sizeof(int));

    // Checking malloc output to prevent heap overflow
    if(ptr == NULL){
      Serial.println("Not enough heap!");
    } else {
      // Do something with memory so its not optimized by the compiler
      for(int i = 0; i < 1024; ++i){
        ptr[i] = 3;
      }
    }
    
    Serial.print("Heap after malloc (Bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    // Free up allocated memory. 
    // Only using necesary memory for first while loop iteration
    vPortFree(ptr);
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("Starting memory demo");


  // Without enough stack space (1024 bytes) the following error appears:
  // Error: Core  1 panic'ed (Unhandled debug exception)
  // Debug exception reason: Stack canary watchpoint triggered
  xTaskCreatePinnedToCore(testTask,
                          "Tast task",
                          1500,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
  // Deleting "setup and loop" task
  vTaskDelete(NULL);
}

void loop() { }
