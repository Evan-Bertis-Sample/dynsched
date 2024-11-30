# dynsched

A C interface for creating preemptive schedulers, primarily for use in embedded systems. Designed to be as flexible as possible, create schedulers without worrying about underlying preemption mechanisms. Similarly, create preemption mechanisms without worrying about the scheduler.

Bundled in this repository is a simple example of a preemptive scheduler and support for the `ESP32` platform. 

Dynsched was originally created for `Northwestern Formula Racing` to manage the scheduling of tasks in their 2025 car, specifcally for the `Data Acquisition` system, which has to balance the timing of multiple sensors, actuators, telemetry, and other tasks.

**This library is still in active development and is not yet stable.**


## Installation
### PlatformIO
Add the following to your `platformio.ini` file:
```ini
lib_deps = https://github.com/evan-bertis-sample/dynsched.git
```

For other platforms, simply download the `incldue/dynsched` folder and include it in your project. Similarly, download the `src/dynsched` folder and place it into the `src` folder of your project. As this is designed for `Northwestern Formula Racing`, which uses `PlatformIO`, I have not bothered to add first-party support for other platforms.

## Notable Files

As this is still really early in development, I have not yet written a full documentation. However, here are some notable files to check out:

- `include/dynsched/sched.h`: Describes the scheduler interface, which is how you can create custom schedulers.
- `include/dynsched/prempt.h`: Describes the preemption interface, which is how you can create custom preemption mechanisms.
- `src/dynsched/espx/prempt_espx.c`: An example of a preemption mechanism for the `ESP32` platform.
- `src/dynsched/dynsched.c`: An example of a simple preemptive scheduler.