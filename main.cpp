#include "LaserScannerDriver.h"
#include <random>

int main(void)
{
    LaserScannerDriver lsd{0.4};
    std::vector<double> vec;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 2);
    int dice_roll = distribution(generator);
    for (int i = 0; i < 500; i++)
    {
        vec.push_back(i);
    }

    for (int i = 0; i < 40; i++)
    {
        if(distribution(generator)==1) {
            lsd.new_scan(vec);
        } else {
            lsd.get_scan();
        }
    }

    LaserScannerDriver a{lsd};

    std::cout << a;
    std::cout << a.get_distance(0.3);
}