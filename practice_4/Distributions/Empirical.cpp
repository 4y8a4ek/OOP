#include "Empirical.h"
double inf = 1e15;

double **Empirical::get_borders_and_densities(const double *information)
{
    double **result = new double *[3];
    int k = log2(dataSize) + 1;
    double *density = new double[k];
    double *lower_borders = new double[k];
    double *higher_borders = new double[k];
    double mx = -inf;
    double mn = inf;
    for (int i = 0; i < dataSize; i++)
    {
        mx = max(information[i], mx);
        mn = min(information[i], mn);
    }
    mx = static_cast<int>(ceil(mx));
    mn = static_cast<int>(ceil(mn));
    double step = fabs(mx - mn) / k;
    int counter = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < dataSize; j++)
        {
            if (information[j] <= (mn + step * (i + 1)) && information[j] > (mn + step * i))
            {
                counter++;
            }
        }
        lower_borders[i] = mn + step * i;
        higher_borders[i] = mn + step * (i + 1);
        density[i] = (double)counter / dataSize;
        counter = 0;
    }
    result[0] = lower_borders;
    result[1] = higher_borders;
    result[2] = density;
    return result;
}

double Empirical::get_density(double x)
{
    double result;
    double **borders_density = new double *[3];
    borders_density = get_borders_and_densities(data);
    int k = 3.322 * log10(dataSize) + 10;
    for (int i = 0; i < k; i++)
    {
        if (borders_density[0][i] < x && borders_density[1][i] >= x)
        {
            result = borders_density[2][i];
            return result;
        }
    }
    return 0;
}
double *Empirical::get_parametrs()
{
    double *result = new double[4];
    double sum = 0.0;
    for (int i = 0; i < dataSize; i++)
    {
        sum += data[i];
    }
    result[0] = sum / dataSize;
    sum = 0.0;
    for (int i = 0; i < dataSize; i++)
    {
        sum += pow(data[i] - result[0], 2);
    }
    result[1] = sum / dataSize;
    sum = 0.0;
    for (int i = 0; i < dataSize; i++)
    {
        sum += (pow(data[i] - result[0], 3));
    }
    result[2] = sum / (dataSize * pow(result[1], 1.5));
    sum = 0.0;
    for (int i = 0; i < dataSize; i++)
    {
        sum += (pow(data[i] - result[0], 4));
    }
    result[3] = sum / (dataSize * pow(result[1], 2)) - 3;
    return result;
}

double *Empirical::transform_data(const double &shift, const double &scale)
{
    double *result = new double[dataSize];
    for (int i = 0; i < dataSize; i++)
    {
        result[i] = data[i] / scale - shift;
    }
    return result;
}
double Empirical::random_number()
{
    double r;
    do
        r = (double)rand() / RAND_MAX;
    while (r == 0. || r == 1.);
    return r;
}
double Empirical::random_value()
{
    double scale = 10;
    double curr = -inf;
    for (int i = 0; i < dataSize; i++)
    {
        if (data[i] > curr)
            curr = data[i];
    }
    double shift = curr / scale + 1;
    double *TransformedData = new double[dataSize];
    TransformedData = transform_data(shift, scale);
    int index;
    index = random_number() * scale;
    return (TransformedData[index] + shift) * scale;
}

double Empirical::create_noise()
{
    return -0.001 + static_cast<double>(rand()) / RAND_MAX * (0.002);
}

void Empirical::modeling_new_data()
{
    double *result = new double[dataSize];
    sort(data, data + dataSize);
    int index;
    for (int i = 0; i < dataSize; i++)
    {
        index = static_cast<int>(random_number() * dataSize);
        if (index >= dataSize)
            index = dataSize - 1;
        result[i] = data[index] + create_noise();
    }
    data = result;
}
void Empirical::save(string &filename)
{
    ofstream output(filename);
    for (int i = 0; i < dataSize; i++)
    {
        output << data[i] << "\n";
    }
    output.close();
}

void Empirical::load(string &filename){}
Empirical::Empirical(int n0, IDistribution &d)
{
    dataSize = n0;
    double *tempData = new double[dataSize];
    for (int i = 0; i < dataSize; i++)
    {
        tempData[i] = d.random_value();
    }
    data = tempData;
}
Empirical::Empirical(int size)
{
    dataSize = size;
    double *tempData = new double[dataSize];
    for (int i = 0; i < dataSize; i++)
    {
        cout << "Введите " << i + 1 << " элемент: ";
        cin >> tempData[i];
    }
    data = tempData;
}

Empirical::Empirical(string &filename)
{
    ifstream counter(filename);
    int i = 0, count = 0;
    double tempNum;
    while (counter >> tempNum)
    {
        count++;
    }
    double *tempData = new double[count];
    counter.close();
    ifstream input(filename);
    while (input >> tempNum)
    {
        tempData[i] = tempNum;
        i++;
    }
    data = tempData;
    input.close();
}

Empirical::~Empirical()
{
    delete[] data;
}
