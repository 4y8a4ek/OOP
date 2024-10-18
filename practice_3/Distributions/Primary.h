#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cstdlib>
using namespace std;
class Primary
{
private:
    double nu = 1;
    double mu = 0;
    double lambda = 1;
    double x = 0;
    double gammaFunction(const double &y);
public:
    Primary(double new_nu, double new_mu, double new_lambda);
    void set_mu(double &new_value);
    void set_nu(double &new_value);
    void set_lambda(double &new_value);
    double get_mu();
    double get_nu();
    double get_lambda();
    double get_density(double x0);
    double *get_parametrs();
    double random_number();
    double random_value();
    void save(string &filename);
    void load(string &filename);
};
