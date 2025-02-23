class pid_algortihms {

    public:

        pid_algortihms(float proportional_const);

        int calculate_proportional_value(int current_value, int final_value);

    private:

        float proportional_const;
};