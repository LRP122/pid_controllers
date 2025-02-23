#include "pid.h"

#include <cmath>

pid_algorithms::pid_algorithms(float proportional_const, float derivative_const, float integral_const) : 
        proportional_const(proportional_const), derivative_const(derivative_const), integral_const(integral_const) {}

int pid_algorithms::calculate_integral_value(int current_value, int final_value, std::deque<int> current_value_deque)
{
    int integral_value=0;

    for (int i : current_value_deque) {
        integral_value += (final_value - i);
    }
    return integral_value * integral_const;
}

int pid_algorithms::calculate_derivative_value(int current_value, int final_value, std::deque<int> current_value_deque)
{
    int deque_size = current_value_deque.size();
    int derivative_value = current_value_deque[deque_size-1]-current_value_deque[deque_size] * (-1);
    return derivative_value * derivative_const;
}

int pid_algorithms::calculate_proportional_value(int current_value, int final_value, char *ourTaskName)
{
    int value_off = (final_value-current_value);
    int change_value = value_off * proportional_const;
    return change_value;
}

int pid_algorithms::calculate_next_value(int current_value, int final_value, std::deque<int> current_value_deque, char *ourTaskName)
{

    int next_value_proportional = calculate_proportional_value(current_value,final_value,ourTaskName);
    int next_value_integral = calculate_integral_value(current_value,final_value,current_value_deque);
    int next_value_derivative = calculate_derivative_value(current_value,final_value,current_value_deque);

    int next_value = current_value + next_value_proportional + next_value_integral + next_value_derivative;
    return next_value;
}





