#define interface class
#include <string>
interface IDistribution{
public:
    virtual double get_density(double x) = 0;
    virtual double* get_parametrs() = 0;
    virtual double random_value() = 0;
    virtual ~IDistribution() = default;
};
interface IPersistent {
public:
    virtual void save(std::string& filename) = 0;
    virtual void load(std::string& filename) = 0;
    virtual ~IPersistent() = default;
};