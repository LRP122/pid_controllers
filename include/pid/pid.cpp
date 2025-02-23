#include "pid.h"

#include <cmath>

pid_algortihms::pid_algortihms(float proportional_const) : proportional_const(proportional_const) {}

int pid_algortihms::calculate_proportional_value(int current_value, int final_value)
{
    int value_off = (final_value-current_value);

    int change_value = value_off * proportional_const;

    return current_value + change_value;
}
