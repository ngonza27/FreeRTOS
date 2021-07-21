// Using 1 just 1 core for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Variables
String my_message = "";
String *message_ptr = NULL;
bool print_msg = false;

void recieve_message(void *parameter) {
  while(1) {
    if (Serial.available() > 0) {

      my_message = Serial.readStringUntil('\n');
      // Printing out number of free heap memory bytes before malloc
      //Serial.print("Heap before malloc (Bytes): ");
      //Serial.println(xPortGetFreeHeapSize());

      message_ptr = (String *) pvPortMalloc(sizeof(String));
  
      // Checking malloc output to prevent heap overflow
      if(message_ptr == NULL){
        Serial.println("Not enough heap!");
      }
    
      //Serial.print("Heap after malloc (Bytes): ");
      //Serial.println(xPortGetFreeHeapSize());

      vPortFree(message_ptr);
      print_msg = true;  
    }
  }
}

void print_message(void *parameter) {
  while(1) {
    if(print_msg == true) {
      Serial.println(my_message);

      message_ptr = NULL;
      print_msg = false;
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("Starting memory challenge");

  xTaskCreatePinnedToCore(recieve_message,
                          "Recieve message task",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(print_message,
                          "Print message task",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
  // Deleting "setup and loop" task
  vTaskDelete(NULL);
}

void loop() { }
