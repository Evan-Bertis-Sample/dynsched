#include <Arduino.h>

// Define task handles
TaskHandle_t Task1Handle;
TaskHandle_t Task2Handle;
TaskHandle_t Task3Handle;

// Define semaphore
SemaphoreHandle_t xLoRaSemaphore;

// Variables to simulate events
volatile bool clientConnected = false;

// Priorities
#define TASK1_PRIORITY 2
#define TASK2_PRIORITY 1
#define TASK3_PRIORITY 1

void setup() {
  Serial.begin(115200);

  // Create semaphore
  xLoRaSemaphore = xSemaphoreCreateBinary();

  

  // Create tasks
  xTaskCreatePinnedToCore(
    Task1,          // Task function
    "DataLogging",  // Name of the task
    2048,           // Stack size
    NULL,           // Parameter
    TASK1_PRIORITY, // Priority
    &Task1Handle,   // Task handle
    1               // Core ID
  );

  xTaskCreatePinnedToCore(
    Task2,
    "WebServer",
    2048,
    NULL,
    TASK2_PRIORITY,
    &Task2Handle,
    1
  );

  xTaskCreatePinnedToCore(
    Task3,
    "LoRaResponse",
    2048,
    NULL,
    TASK3_PRIORITY,
    &Task3Handle,
    1
  );

  // Simulate external interrupt setup for LoRa (replace with actual ISR setup)
  attachInterrupt(digitalPinToInterrupt(0), LoRaISR, RISING);
}

void loop() {
  // Simulate client connection after 5 seconds
  if (millis() > 5000 && !clientConnected) {
    clientConnected = true;
    Serial.println("Client connected.");
    // Increase WebServer task priority
    vTaskPrioritySet(Task2Handle, 3);
  }

  // Simulate client disconnection after 10 seconds
  if (millis() > 10000 && clientConnected) {
    clientConnected = false;
    Serial.println("Client disconnected.");
    // Decrease WebServer task priority
    vTaskPrioritySet(Task2Handle, TASK2_PRIORITY);
  }

  // Main loop can remain empty as tasks are running on FreeRTOS
}

// Task1: Data Logging Task
void Task1(void *pvParameters) {
  for (;;) {
    // Simulate data logging
    Serial.println("Task1: Logging data...");
    vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500ms
  }
}

// Task2: Web Server Task
void Task2(void *pvParameters) {
  for (;;) {
    if (clientConnected) {
      // Handle client requests
      Serial.println("Task2: Handling client request...");
      vTaskDelay(pdMS_TO_TICKS(200)); // Simulate processing time
    } else {
      // Perform low-priority tasks or yield
      Serial.println("Task2: No client connected.");
      vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
  }
}

// Task3: LoRa Response Task
void Task3(void *pvParameters) {
  for (;;) {
    // Wait for semaphore from ISR
    if (xSemaphoreTake(xLoRaSemaphore, portMAX_DELAY) == pdTRUE) {
      // Increase priority to process message
      vTaskPrioritySet(NULL, 3);
      Serial.println("Task3: Processing LoRa message...");
      // Simulate message processing
      vTaskDelay(pdMS_TO_TICKS(300));
      // Lower priority after processing
      vTaskPrioritySet(NULL, TASK3_PRIORITY);
    }
  }
}

// ISR: Simulate LoRa message reception
void IRAM_ATTR LoRaISR() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  // Give semaphore to LoRa Response Task
  xSemaphoreGiveFromISR(xLoRaSemaphore, &xHigherPriorityTaskWoken);
  // Yield to higher priority task if necessary
  if (xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}
