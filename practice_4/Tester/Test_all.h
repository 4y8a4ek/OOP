#include <stdexcept>
using namespace std;
#include "../Distributions/Empirical.h"
void test_distribution(IDistribution &dist)
{
    std::cout << "Testing Distribution\n";
    std::cout << "Random value: " << dist.random_value() << std::endl;
    double density = dist.get_density(5.0); // Вычисляем плотность для точки x = 5
    std::cout << "Density at x=5: " << density << std::endl;

    double *params = dist.get_parametrs();
    std::cout << "Parameters: \n";
    std::cout << "Expectation: " << params[0] << "\n";
    std::cout << "Variance: " << params[1] << "\n";
    std::cout << "Skewness: " << params[2] << "\n";
    std::cout << "Kurtosis: " << params[3] << "\n";
}

void test_persistence(IPersistent &persistentObj, std::string &filename)
{
    std::cout << "Testing Persistence\n";
    persistentObj.save(filename);
    std::cout << "Data saved to " << filename << "\n";

    // Загружаем данные обратно и проверяем
    persistentObj.load(filename);
    std::cout << "Data loaded from " << filename << "\n";
}

void test_all(){
    Primary distribution(1, 0, 1);
    Primary dist_one_mixture(1, 0, 3);
    Primary dist_two_mixture(2, 1, 3);
    Mixture<Primary, Primary> mixture(dist_one_mixture, dist_two_mixture, 0.7);

    test_distribution(distribution);
    test_distribution(mixture);
    string filename = "./Work_Files/Primary_for_test.txt";
    string filename_mixture = "./Work_Files/Mixture_for_test.txt";
    test_persistence(distribution, filename);
    test_persistence(mixture, filename_mixture);
}