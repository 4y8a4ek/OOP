#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <algorithm>
#include ".\Distributions\Primary.h"
#include ".\Tester\Test_Primary.h"
using namespace std;
double inf = 1e15;
double *parametrs;         // Дисперсия, Мат ожидание, Асимметрия и Эксцесс
double *inputed_parametrs; // Входные данные
double *data;              // Выборка для эмпирического
int dataSize;              // Размер выборки
double x;                  // Икс для плотностей
bool IsRunning = true;     // Состояние программы
int selected_action;       // Выбранное действие в программе
// Проверка данных 2 функции
double input_number(const double &lower, double higher)
{
    double number;
    if (higher < lower)
    {
        higher = numeric_limits<float>::max();
    }
    do
    {
        cin >> number;
        if (cin.fail())
        {
            cin.clear();                                         // Сбросить ошибку ввода
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистить буфер
            cout << "Ошибка: это не число. Попробуйте снова." << endl;
        }
        else if (number < lower || number > higher)
        {
            cout << "Ошибка: число вне диапазона. Попробуйте снова." << endl;
        }
    } while (number < lower || number > higher);
    return number;
}
bool troubleshoot(const int &arg, const int &lower, const int &higher)
{
    if (arg < lower || arg > higher)
    {
        cout << "Ошибка! Неверно введенный номер! Повторите попытку!" << endl;
        return true;
    }
    return false;
}
// Гамма функция
double gammaFunction(const double &y)
{
    return tgamma(y); // Используем стандартную библиотеку для Гамма-функции
}
// Вывод данных 2 функции
void print_actions()
{
    cout << "Выберите желаемое действие с распределением:" << endl
         << "1 - Переход в главное меню" << endl
         << "2 - Расчет плотности распределения" << endl
         << "3 - Вычисление математического ожидания, дисперсии и коэффициентов асимметрии и эксцесса" << endl
         << "4 - Моделирование случайной величины из распределения" << endl;
    return;
}
void print_parametrs()
{
    cout << "Математическое ожидание: " << parametrs[0] << endl
         << "Дисперсия: " << parametrs[1] << endl
         << "Коэффициент асимметрии: " << parametrs[2] << endl
         << "Коэффициент эксцесса: " << parametrs[3] << endl
         << endl;
}
// Плотности 3 функции ( get_borders_and_densities функции для нахождения интервальной плотности в эмпирическом распределении)
double mixed_get_density(Primary first, Primary second)
{
    return inputed_parametrs[6] * first.get_density(x) + (1 - inputed_parametrs[6]) * second.get_density(x);
}
double **get_borders_and_densities(const double *information)
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
double get_empirical_density()
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
// Расчет 4 параметров: Мат. ожидание, дисперсия, коэффициенты асимметрии и эксцесса 2 функции
double *mixed_get_parametrs(Primary first, Primary second)
{
    double *result = new double[4];
    double *first_parametrs = first.get_parametrs();
    double *second_parametrs = second.get_parametrs();
    cout << "Все посчитано" << endl;
    result[0] = (inputed_parametrs[6]) * (first_parametrs[0]) + (1 - inputed_parametrs[6]) * (second_parametrs[0]);
    result[1] = (inputed_parametrs[6] * (pow(first_parametrs[0], 2) + first_parametrs[1]) + ((1 - inputed_parametrs[6]) * (pow(second_parametrs[0], 2) + second_parametrs[1]))) - pow(result[0], 2);
    result[2] = 0.0;
    result[3] = ((inputed_parametrs[6] * pow(first_parametrs[0] - result[0], 4) + 6 * pow(first_parametrs[0] - result[0], 2) * result[1] + pow(first_parametrs[1], 2) * (first_parametrs[3] + 3)) + ((1 - inputed_parametrs[6]) * pow(second_parametrs[0] - result[0], 4) + 6 * pow(second_parametrs[0] - result[0], 2) * result[1] + pow(second_parametrs[1], 2) * (second_parametrs[3] + 3))) / pow(result[1], 2) - 3;
    return result;
}
double *empirical_get_parametrs()
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
// Моделирование случайной величины 4 функции (transform_data - сдвиг-масштабное преобразование)
double random_number()
{
    double r;
    do
        r = (double)rand() / RAND_MAX;
    while (r == 0. || r == 1.);
    return r;
}
double mixed_random_value(Primary first, Primary second)
{
    double r1 = random_number();
    if (r1 > inputed_parametrs[6])
    {
        return first.random_value();
    }
    else
    {
        return second.random_value();
    }
}
double *transform_data(const double &shift, const double &scale)
{
    double *result = new double[dataSize];
    for (int i = 0; i < dataSize; i++)
    {
        result[i] = data[i] / scale - shift;
    }
    return result;
}
double empirical_random_value()
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
// Перемоделирование выборки
double create_noise()
{
    return -0.001 + static_cast<double>(rand()) / RAND_MAX * (0.002);
}
double *modeling_new_data()
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
    return result;
}
// Задание данных 3 функции
double *gauss_input(int method, Primary &distribution)
{
    double *result = new double[3];
    switch (method)
    {
    case 1:
        cout << "Введите параметр nu > 0 для распределения:";
        result[0] = input_number(0, 100000);
        cout << "Введите параметр mu для распределения:";
        result[1] = input_number(-100000, 100000);
        cout << "Введите параметр lambda для распределения:";
        result[2] = input_number(0, 100000);
        cout << endl;
        break;
    case 2:
        cout << "Введите путь к файлу с данными:" << endl;
        string source;
        cin >> source;
        distribution.load(source);
        break;
    }
    return result;
}
double *mixed_input()
{
    double *result = new double[7];
    cout << "Введите параметр nu > 0 для первого распределения:";
    result[0] = input_number(0, 100000);
    cout << "Введите параметр mu для первого распределения:";
    result[1] = input_number(-100000, 100000);
    cout << "Введите параметр lambda для первого распределения:";
    result[2] = input_number(0, 100000);
    cout << endl;
    cout << "Введите параметр nu > 0 для второго распределения:";
    result[3] = input_number(0, 100000);
    cout << "Введите параметр mu для второго распределения:";
    result[4] = input_number(-100000, 100000);
    cout << "Введите параметр lambda для второго распределения:";
    result[5] = input_number(0, 100000);
    cout << "Введите параметр смеси p в интервале [0, 1]:";
    result[6] = input_number(0, 1);
    cout << endl;
    return result;
}
double *generate_emp()
{
    int method;
    cout << "Введите способ создания выборки:" << endl;
    cout << "1 - Генерация выборки на основе основного распределения" << endl;
    cout << "2 - Генерация выборки на основе смеси основных распределений" << endl;
    cout << "3 - Ввод выборки с клавиатуры" << endl;
    cin >> method;
    cout << "Введите количество элементов в выборке: ";
    dataSize = input_number(2, 10000000);
    if (troubleshoot(method, 1, 3))
    {
        return NULL;
    };
    Primary distribution(0, 0, 0);
    Primary primary_first(0, 0, 0);
    Primary primary_second(0, 0, 0);
    double *emp = new double[dataSize];
    switch (method)
    {
    case 1:
        inputed_parametrs = gauss_input(1, distribution);
        distribution.set_mu(inputed_parametrs[0]);
        distribution.set_nu(inputed_parametrs[1]);
        distribution.set_lambda(inputed_parametrs[2]);
        for (int i = 0; i < dataSize; i++)
        {
            emp[i] = distribution.random_value();
        }
        break;
    case 2:
        inputed_parametrs = mixed_input();
        primary_first.set_nu(inputed_parametrs[0]);
        primary_first.set_mu(inputed_parametrs[1]);
        primary_first.set_lambda(inputed_parametrs[2]);
        primary_second.set_nu(inputed_parametrs[3]);
        primary_second.set_mu(inputed_parametrs[4]);
        primary_second.set_lambda(inputed_parametrs[5]);
        for (int i = 0; i < dataSize; i++)
        {
            emp[i] = mixed_random_value(primary_first, primary_second);
        }
        break;
    case 3:
        for (int i = 0; i < dataSize; i++)
        {
            cout << "Введите " << i + 1 << " элемент: ";
            cin >> emp[i];
        }
    }
    return emp;
}
// Основные функции для работы программы (front)
void running_gauss()
{
    Primary distribution(0, 0, 0);
    int method;
    cout << "Выберите вариант из меню ниже:" << endl;
    cout << "1 - Ввод с клавиатуры" << endl;
    cout << "2 - Ввод из файла" << endl;
    cin >> method;
    inputed_parametrs = gauss_input(method, distribution);
    if (method == 1)
    {
        distribution.set_lambda(inputed_parametrs[2]);
        distribution.set_mu(inputed_parametrs[1]);
        distribution.set_nu(inputed_parametrs[0]);
    }
    if (troubleshoot(method, 1, 2))
    {
        return;
    }
    cout << "Введеные параметры: nu = " << distribution.get_nu() << " mu = " << distribution.get_mu() << " lambda = " << distribution.get_lambda() << endl;
    while (IsRunning)
    {
        print_actions();
        cout << "5 - Сохранение параметров в файл" << endl;
        cin >> selected_action;
        switch (selected_action)
        {
        case 1:
            IsRunning = false;
            break;
        case 2:
            cout << "Введите x0:";
            x = input_number(-100000, 100000);
            cout << "Плотность распределения Гаусса в точке x0 = " << x << " :  " << distribution.get_density(x) << endl;
            break;
        case 3:
            parametrs = distribution.get_parametrs();
            print_parametrs();
            break;
        case 4:
            cout << "Смоделированная случайная величина : " << distribution.random_value() << endl;
            break;
        case 5:
            cout << "Введите путь к файлу для записи:" << endl;
            string output;
            cin >> output;
            distribution.save(output);
            cout << "Запись завершена" << endl;
            break;
        }
        troubleshoot(selected_action, 1, 5);
    }
}
void running_mixed()
{
    inputed_parametrs = mixed_input();
    Primary primary_first(inputed_parametrs[0], inputed_parametrs[1], inputed_parametrs[2]);
    Primary primary_second(inputed_parametrs[3], inputed_parametrs[4], inputed_parametrs[5]);
    while (IsRunning)
    {
        print_actions();
        cin >> selected_action;
        switch (selected_action)
        {

        case 1:
            IsRunning = false;
            break;
        case 2:
            cout << "Введите x0:";
            x = input_number(-100000, 100000);
            cout << "Плотность смеси распределений Гаусса в точке x0 = " << x << " :  " << mixed_get_density(primary_first, primary_second) << endl;
            break;
        case 3:
            parametrs = mixed_get_parametrs(primary_first, primary_second);
            print_parametrs();
            break;
        case 4:
            cout << "Смоделированная случайная величина : " << mixed_random_value(primary_first, primary_second) << endl;
            break;
        }
        troubleshoot(selected_action, 1, 4);
    }
}
void running_emp()
{
    data = generate_emp();
    if (data == NULL)
    {
        return;
    };
    double **borders_density;
    int k;
    while (IsRunning)
    {
        print_actions();
        cout << "5 - Пересоздание выборки основываясь на текущей" << endl;
        cin >> selected_action;
        switch (selected_action)
        {
        case 1:
            IsRunning = false;
            break;
        case 2:
            cout << "Введите x0:";
            x = input_number(-100000, 100000);
            cout << "Плотность эмпирического распределения в точке x0 = " << x << " :  " << get_empirical_density() << endl;
            break;
        case 3:
            parametrs = empirical_get_parametrs();
            print_parametrs();
            break;
        case 4:
            cout << "Смоделированная случайная величина : " << empirical_random_value() << endl;
            break;
        case 5:
            data = modeling_new_data();
            break;
        }
        troubleshoot(selected_action, 1, 5);
    }
}
// Мейн чисто с главным меню и все
int main()
{
    bool IsRunningMain = true;
    Primary Distr(1.0, 0.0, 1.0);
    if (testPrimary(Distr))
    {
        cout << "Все Unit тесты выполнены. Программа работает корректно!" << endl;
        while (IsRunningMain)
        {
            cout << "Выберите желаемое распределение:" << endl
                 << "1 - Нормальное распределение Гаусса" << endl
                 << "2 - Смесь двух нормальных распределений Гаусса" << endl
                 << "3 - Эмпирическое распределение построенное по выборке" << endl
                 << "4 - Завершение работы программы" << endl;
            cin >> selected_action;
            IsRunning = true;
            switch (selected_action)
            {
            case 1:
                running_gauss();
                break;
            case 2:
                running_mixed();
                break;
            case 3:
                running_emp();
                break;
            case 4:
                IsRunningMain = false;
                break;
            }
            troubleshoot(selected_action, 1, 4);
        }
    }
    else
    {
        cout << "Ошибка в Unit Тестах, программа работает некорректно!" << endl;
    }
}