#include ".\Primary.h"
double zero = 1e-15;
double Primary::gammaFunction(const double &y)
{
    return tgamma(y); // Используем стандартную библиотеку для Гамма-функции
}
Primary::Primary(double new_nu, double new_mu, double new_lambda)
{
    mu = new_mu;
    nu = new_nu;
    lambda = new_lambda;
    x = 0;
}
void Primary::set_mu(double &new_value)
{
    mu = new_value;
    return;
}
void Primary::set_nu(double &new_value)
{
    nu = new_value;
    return;
}
void Primary::set_lambda(double &new_value)
{
    lambda = new_value;
    return;
}
double Primary::get_mu()
{
    return mu;
}
double Primary::get_nu()
{
    return nu;
}
double Primary::get_lambda()
{
    return lambda;
}
double Primary::get_density(double x0)
{
    x = x0;
    double coefficient = nu / (2 * lambda * gammaFunction(1.0 / nu));
    return coefficient * exp(-pow(fabs((x - mu) / lambda), nu));
}
double *Primary::get_parametrs()
{
    double *result = new double[4];
    result[1] = gammaFunction(3.0 / nu) / gammaFunction(1.0 / nu);
    double numerator = gammaFunction(5.0 / nu) * gammaFunction(1.0 / nu);
    double denominator = pow(gammaFunction(3.0 / nu), 2);
    result[3] = (numerator / denominator) - 3; // Эксцесс
    double a = -10.0;
    double b = 10.0;
    int n = 10000;
    double h = (b - a) / n;
    double summing_one = get_density(a);
    double summing_two = get_density(b);
    double sum = 0.5 * (summing_one)*a + (summing_two * b);
    for (int i = 1; i < n; i++)
    {
        sum += get_density(a + i * h) * (a + i * h);
    }
    result[0] = 2 * h * sum; // Мат ожидание
    if (result[0] < zero)
    {
        result[0] = 0;
    }
    result[2] = 0.0; // Асимметрия
    return result;
}
double Primary::random_number()
{
    double r;
    do
        r = (double)rand() / RAND_MAX;
    while (r == 0. || r == 1.);
    return r;
}
double Primary::random_value()
{
    double res;
    if (nu >= 1 && nu < 2)
    {
        double a = 1 / nu - 1;
        double b = 1 / pow(nu, 1 / nu);
        while (true)
        {
            double r1 = random_number();
            if (r1 <= 0.5)
            {
                res = b * log(2 * r1);
            }
            else
            {
                res = -b * log(2 * (1 - r1));
            }
            double r2 = random_number();
            if (log(r2) <= (-(pow(fabs(res), nu)) + fabs(res) / b + a))
            {
                return mu + lambda * res;
            }
        }
    }
    else if (nu >= 2)
    {
        double a = 1.0 / nu - 0.5;
        double b = 1.0 / pow(nu, 1.0 / nu);
        double c = 2.0 * pow(b, 2.0);
        while (true)
        {
            double r1 = random_number();
            double r2 = random_number();
            res = b * sqrt(-2.0 * log(r1)) * cos(2 * M_PI * r2);
            double r3 = random_number();
            if (log(r3) <= (-(pow(fabs(res), nu))) + (pow(res, 2) / c) + a)
            {
                return mu + lambda * res;
            }
        }
    }
}
void Primary::save(string &filename)
{
    ofstream output(filename);
    output << nu << "\n";
    output << mu << "\n";
    output << lambda << "\n";
    output.close();
    return;
}
void Primary::load(string &filename)
{
    ifstream input(filename);
    double tempNu, tempMu, tempLambda;
    input >> tempNu;
    input >> tempMu;
    input >> tempLambda;
    input.close();
    set_nu(tempNu);
    set_mu(tempMu);
    set_lambda(tempLambda);
    return;
}