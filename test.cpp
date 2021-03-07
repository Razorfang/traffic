#include <iostream>
#include <vector>
#include <string>

/* Generic control method. */
class ControlMethod
{
    public:
        ControlMethod(float cpmLimitLower, float cpmLimitUpper, const std::vector<float> &efficiencies, const std::string &name);
        float get_cpm_lower_limit(void);
        float get_cpm_upper_limit(void);
        float get_eff(float cpmTotal);
        const std::string &get_name(void);
    private:
        float cpmL, cpmU;
        std::vector<float> eff;
        std::string nm;
};

ControlMethod::ControlMethod(float cpmLimitLower, float cpmLimitUpper, const std::vector<float> &efficiencies, const std::string &name)
{
    cpmL = cpmLimitLower;
    cpmU = cpmLimitUpper;
    std::copy(efficiencies.begin(), efficiencies.end(), std::back_inserter(eff));
    nm = name;
}

float ControlMethod::get_cpm_lower_limit(void)
{
    return cpmL;
}

float ControlMethod::get_cpm_upper_limit(void)
{
    return cpmU;
}

const std::string &ControlMethod::get_name(void)
{
    return nm;
}

float ControlMethod::get_eff(float cpmTotal)
{
    if (cpmTotal < cpmL)
    {
        return eff[0];
    }
    else if (cpmTotal >= cpmU)
    {
        return eff[2];
    }
    else
    {
        return eff[1];
    }
}

/* Chooses a control method from an internally-kept list of options. */
class ControlPicker
{
    public:
        void add_method(const ControlMethod &method);
        ControlMethod *get_best_ctrl_method(float cpmTotal);
    private:
        std::vector<ControlMethod> methods;
};

void ControlPicker::add_method(const ControlMethod &method)
{
    methods.push_back(method);
}

ControlMethod *ControlPicker::get_best_ctrl_method(float cpmTotal)
{
    ControlMethod *bestMethod = &(methods[0]);

    for (ControlMethod &m : methods)
    {
        if (m.get_eff(cpmTotal) > bestMethod->get_eff(cpmTotal))
        {
               bestMethod = &m; 
        }
    }

    /* Return the method with the highest efficiency */
    return bestMethod;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: <north road cpm> <east road cpm> <south road cpm> <west road cpm>" << std::endl;
        return -1;
    }


    float cpmNorth = std::stof(argv[1]);
    float cpmEast  = std::stof(argv[2]);
    float cpmSouth = std::stof(argv[3]);
    float cpmWest  = std::stof(argv[4]);
    float cpmTotal = cpmNorth + cpmEast + cpmSouth + cpmWest;

    ControlPicker picker;

    std::vector<float> roundaboutEfficiencies = {0.9, 0.75, 0.5};
    ControlMethod roundabout(10, 20, roundaboutEfficiencies, "Roundabout");
    picker.add_method(roundabout);

    std::vector<float> stopSignEfficiencies = {0.4, 0.3, 0.2};
    ControlMethod stopSign(10, 20, stopSignEfficiencies, "Stop Sign");
    picker.add_method(stopSign);

    std::vector<float> trafficLightEfficiencies = {0.3, 0.75, 0.9};
    ControlMethod trafficLight(10, 20, trafficLightEfficiencies, "Traffic Light");
    picker.add_method(trafficLight);

    ControlMethod *best = picker.get_best_ctrl_method(cpmTotal);

    std::cout << "The best option for a total CPM of " << cpmTotal <<  " is a " << best->get_name() << std::endl;

    return 0;
}