#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include "Mixture.h"

class Empirical : IDistribution, IPersistent
{
private:
    double *data;
    int dataSize;
    double **get_borders_and_densities(const double *information);
    double *transform_data(const double &shift, const double &scale);
    double create_noise();
    double random_number();

public:
    double get_density(double x) override;
    double *get_parametrs() override;
    double random_value() override;
    void modeling_new_data();
    void save(string &filename) override;
    void load(string &filename) override;
    Empirical(int size);
    Empirical(string &filename);
    Empirical(int n0, IDistribution& d);
    ~Empirical();
};
