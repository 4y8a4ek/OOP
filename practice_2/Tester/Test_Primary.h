#include <stdexcept>
using namespace std;
bool testPrimary(Primary &distribution)
{
    double *array = new double[4];
    double nus[]{0.5, 0.75, 1, 1.5, 2, 2.5, 3};
    double means[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double disp[]{120.0, 6.719, 2.0, 0.74, 0.5, 0.413, 0.37};
    double exc[]{22.2, 6.650, 3.0, 0.76, 0.0, -0.368, -0.58};
    double densities[]{0.25, 0.419, 0.5, 0.554, 0.564, 0.564, 0.560};
    for (int i = 0; i < 7; i++)
        try
        {
            distribution.set_nu(nus[i]);
            array = distribution.get_parametrs();
            if (abs(array[1] - disp[i]) > 0.01)
            {
                throw 0;
            }
            if (abs(array[3] - exc[i]) > 0.01)
            {
                throw 0;
            }
            if (abs(distribution.get_density(0) - densities[i]) > 0.01)
            {
                throw 0;
            }
            if (abs(array[0] - means[i]) > 0.01)
            {
                throw 0;
            }
        }
        catch (int error)
        {
            if (error == 0)
            {
                cout << "Ошибка при nu =" << distribution.get_mu() << endl;
                return false;
            }
        }
    return true;
}