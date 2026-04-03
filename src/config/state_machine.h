#ifndef CONFIG_STATE_MACHINE_H
#define CONFIG_STATE_MACHINE_H
namespace state_machine {
    constexpr unsigned long IDLE = 0;
    constexpr unsigned long FILLING_TANK = 1;
    constexpr unsigned long READ_INITIAL_DATA = 2;
    constexpr unsigned long MEASURE_MORINGA_DOSAGE = 3;
    constexpr unsigned long DOSING = 4;
    constexpr unsigned long STIRRING = 5;
    constexpr unsigned long TRANSFER_TO_SETTLING = 6;
    constexpr unsigned long SEDIMENTATION = 7;
}
#endif