#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Queue settings
static const uint8_t my_queue_len = 5;

// Queue
static QueueHandle_t my_queue;


void print_messages(void * parameters){
  int item;

  while(1){
                                              //timeout (# of ticks)
    if(xQueueReceive(my_queue, (void *)&item, 0) == pdTRUE) {
      Serial.println(item);
    }  
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Queue demo");

  // Create queue
  my_queue = xQueueCreate(my_queue_len, sizeof(int));
  
  //Start task
  xTaskCreatePinnedToCore(print_messages,
                          "Print messages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
}

void loop() {
  static int num = 0;

  // Try to add item to queue for 10 ticks, fail if queue is full
  if(xQueueSend(my_queue, (void *)&num, 10) != pdTRUE){
    Serial.println("Queue is full");
  }
  num++;
  vTaskDelay(500 / portTICK_PERIOD_MS); //Adding numbers to queue faster
  //vTaskDelay(1000 / portTICK_PERIOD_MS);

}
