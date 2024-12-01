#include <Arduino.h>

#include "dynsched/espx/prempt_espx.h"
#include "dynsched/mem_manager.h"
#include "dynsched/prempt.h"

uint32_t millis_wrapper();
void test_task(void *data);

dynsched_prempt_interface_t *prempt_interface;
dynsched_mem_manager_t mem_manager;
dynsched_prempt_espx_config_t prempt_config = {
    .group_num = TIMER_GROUP_0,
    .timer_num = TIMER_0,
    .millis_fn = millis_wrapper};

uint32_t prempt_counter = 0;
uint32_t last_prempt_request = 0;
uint32_t prempt_interval = 1000;
uint32_t amount_of_prempts = 100;

int32_t *error_times;

void setup() {

    Serial.begin(9600);

    Serial.println("dynsched test starting...");

    mem_manager = DYNSCHED_DEFAULT_MEM;
    dynsched_mem_init(&mem_manager, NULL);

    prempt_interface = dynsched_prempt_create(
        &mem_manager,
        DYNSCHED_PREMPT_ESPX,
        &prempt_config);

    // error_times = (int32_t *)malloc(amount_of_prempts * sizeof(int32_t));
    error_times = (int32_t *)dynsched_mem_alloc(&mem_manager, (dynsched_mem_size_t)(amount_of_prempts * sizeof(int32_t)));

    dynsched_prempt_init(prempt_interface);
    dynsched_prempt_start(prempt_interface);
}

void loop() {
    if (millis() - last_prempt_request >= prempt_interval) {
        last_prempt_request = millis();
        dynsched_prempt_args_t prempt_args = {
            .task_data = NULL,
            .prempt_time = 100,
            .stack_size = 1024,
            .prempt_func = test_task,
            .after_prempt_func = NULL,
        };
        dynsched_prempt_prempt(prempt_interface, &prempt_args);
    }

    if (prempt_counter >= amount_of_prempts) {
        // read the error times
        Serial.println("Error times:");
        for (uint32_t i = 0; i < amount_of_prempts; i++) {
            Serial.print(error_times[i]);
            Serial.print(" ");
        }

        Serial.println("Stats:");
        int32_t average_error = 0;
        int32_t most_negative_error = error_times[0];
        int32_t most_positive_error = error_times[0];
        int32_t smallest_error = error_times[0];
        int32_t largest_error = error_times[0];

        for (uint32_t i = 0; i < amount_of_prempts; i++) {
            average_error += error_times[i];
            int32_t abs_error = abs(error_times[i]);
            if (abs_error > largest_error) {
                largest_error = abs_error;
            }
            if (abs_error < smallest_error) {
                smallest_error = abs_error;
            }

            if (error_times[i] < most_negative_error) {
                most_negative_error = error_times[i];
            }

            if (error_times[i] > most_positive_error) {
                most_positive_error = error_times[i];
            }
        }

        average_error /= amount_of_prempts;

        Serial.print("Average error: ");
        Serial.println(average_error);
        Serial.print("Most negative error: ");
        Serial.println(most_negative_error);
        Serial.print("Most positive error: ");
        Serial.println(most_positive_error);
        Serial.print("Smallest error: ");
        Serial.println(smallest_error);
        Serial.print("Largest error: ");
        Serial.println(largest_error);
        dynsched_prempt_stop(prempt_interface);
        dynsched_prempt_destroy(prempt_interface);
        while (1);
    }
}

uint32_t millis_wrapper() {
    return millis();
}

void test_task(void *data) {
    Serial.printf("Prempt #%d\n", prempt_counter);
    int32_t error_time = (int32_t)millis() - (int32_t)last_prempt_request;
    error_times[prempt_counter] = error_time;
    prempt_counter++;
}
