#include <iostream>
#include <vector>

/* Generic control method. */
class ControlMethod
{
    public:
        ControlMethod(float cpmLimitLower, float cpmLimitUpper, std::vector<float> &efficiencies);
        float get_cpm_lower_limit(void);
        float get_cpm_upper_limit(void);
        float get_eff(float cpmTotal);
    private:
        float cpmL, cpmU;
        std::vector<float> eff;
};

ControlMethod::ControlMethod(float cpmLimitLower, float cpmLimitUpper, std::vector<float> &efficiencies)
{
    cpmL = cpmLimitLower;
    cpmU = cpmLimitUpper;
    std::copy(efficiencies.begin(), efficiencies.end(), std::back_inserter(eff));
}

float ControlMethod::get_cpm_lower_limit(void)
{
    return cpmL;
}

float ControlMethod::get_cpm_upper_limit(void)
{
    return cpmU;
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
        void add_method(ControlMethod &method);
        ControlMethod *get_best_ctrl_method(float cpmTotal);
    private:
        std::vector<ControlMethod> methods;
};

void ControlPicker::add_method(ControlMethod &method)
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
    ControlMethod roundabout(10, 20, roundaboutEfficiencies);
    picker.add_method(roundabout);

    std::vector<float> stopSignEfficiencies = {0.4, 0.3, 0.2};
    ControlMethod stopSign(10, 20, stopSignEfficiencies);
    picker.add_method(stopSign);

    std::vector<float> trafficLightEfficiencies = {0.3, 0.75, 0.9};
    ControlMethod trafficLights(10, 20, trafficLightEfficiencies);
    picker.add_method(trafficLights);

    ControlMethod *best = picker.get_best_ctrl_method(cpmTotal);

    std::cout << "Best method: lower limit = " << best->get_cpm_lower_limit() << ", upper limit = " 
        << best->get_cpm_upper_limit() << ", efficiency: " << best->get_eff(cpmTotal) << std::endl;

    return 0;
}