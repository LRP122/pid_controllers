#include <deque>
#include "esp_log.h"

class pid_algorithms {

    public:

        pid_algorithms(float proportional_const, float integral_const, float derivative_const);

        int calculate_next_value(int current_value, int final_value, std::deque<int> current_value_deque, char *ourTaskName);

    private:

        int calculate_proportional_value(int current_value, int final_value, char *ourTaskName);
        int calculate_integral_value(int current_value, int final_value, std::deque<int> current_value_deque);
        int calculate_derivative_value(int current_value, int final_value,std::deque<int> current_value_deque);

        float proportional_const;
        float derivative_const;
        float integral_const;
};