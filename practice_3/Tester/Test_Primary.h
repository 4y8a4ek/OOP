#include <stdexcept>
using namespace std;
bool testPrimary(Primary distribution)
{
    double *array = new double[4];
    double array_of_nu[]{0.5, 0.75, 1, 1.5, 2, 2.5, 3};
    double array_of_means[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double array_of_disp[]{120.0, 6.719, 2.0, 0.74, 0.5, 0.413, 0.37};
    double array_of_exc[]{22.2, 6.650, 3.0, 0.76, 0.0, -0.368, -0.58};
    double array_of_densities[]{0.25, 0.419, 0.5, 0.554, 0.564, 0.564, 0.560};
    for (int i = 0; i < 7; i++)
        try
        {
            distribution.set_nu(array_of_nu[i]);
            array = distribution.get_parametrs();
            if (abs(array[1] - array_of_disp[i]) > 0.01)
            {
                throw 0;
            }
            if (abs(array[3] - array_of_exc[i]) > 0.01)
            {
                throw 0;
            }
            if (abs(distribution.get_density(0) - array_of_densities[i]) > 0.01)
            {
                throw 0;
            }
            if (abs(array[0] - array_of_means[i]) > 0.01)
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