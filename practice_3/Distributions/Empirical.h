#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include "Mixture.h"

class Empirical
{
private:
    double *data;
    int dataSize;
    double **get_borders_and_densities(const double *information);
    double *transform_data(const double &shift, const double &scale);
    double create_noise();
    double random_number();

public:
    double get_density(double &x);
    double *get_parametrs();
    double random_value();
    void modeling_new_data();
    void save();
    Empirical(Primary &distiburion, int size);
    Empirical(Mixture &mixture, int size);
    Empirical(int size);
    Empirical(string &filename);
    ~Empirical();
};
