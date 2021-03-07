#include <iostream>

class Intersection
{
    public:
        Intersection(float cpmNorth, float cpmEast, float cmpSouth, float cpmWest);
        void print(void);
    
    private:
        float cpmN, cpmE,cpmS, cpmW;
};

Intersection::Intersection(float cpmNorth, float cpmEast, float cmpSouth, float cpmWest)
{
    cpmN = cpmNorth;
    cpmE = cpmEast;
    cpmS = cmpSouth;
    cpmW = cpmWest;
}

void Intersection::print(void)
{
    std::cout << "North: " << cpmN << ", East: " << cpmE << ", South: " << cpmS << ", West: " << cpmW << std::endl;
}


int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: <north road cpm> <east road cpm> <south road cpm> <west road cpm>" << std::endl;
        return -1;
    }

    Intersection intersection(std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]));

    intersection.print();

    return 0;
}