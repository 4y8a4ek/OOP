#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <chrono>
#include ".\Distributions\Mixture.h"
using namespace std;
double *parametrs;         // Дисперсия, Мат ожидание, Асимметрия и Эксцесс
double *inputed_parametrs; // Входные данные
bool IsRunning = true;     // Состояние программы
int selected_action;       // Выбранное действие в программе
double x;
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

double gammaFunction(const double &y)
{
    return tgamma(y); // Используем стандартную библиотеку для Гамма-функции
}

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

double random_number()
{
    double r;
    do
        r = (double)rand() / RAND_MAX;
    while (r == 0. || r == 1.);
    return r;
}

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
double *mixed_input(int method, Mixture &mixture)
{
    double *result = new double[7];
    switch (method)
    {
    case 1:
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
        break;
    case 2:
        cout << "Введите путь к файлу с данными:" << endl;
        string source;
        cin >> source;
        mixture.load(source);
        break;
    }
    return result;
}
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
        string output;
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
            cin >> output;
            distribution.save(output);
            cout << "Запись завершена" << endl;
            break;
        case 6:
            auto start = chrono::high_resolution_clock::now();
            for (double i = -8; i < 8; i += 0.1)
            {
                cout << distribution.get_density(i) << endl;
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> time = end - start;
            cout << "Время ебания: " << time.count() << endl;
            break;
        }

        troubleshoot(selected_action, 1, 6);
    }
}
void running_mixed()
{
    Mixture mixture(0, 0, 0, 0, 0, 0, 0);
    int method;
    cout << "Выберите вариант из меню ниже:" << endl;
    cout << "1 - Ввод с клавиатуры" << endl;
    cout << "2 - Ввод из файла" << endl;
    cin >> method;
    inputed_parametrs = mixed_input(method, mixture);
    if (method == 1)
    {
        mixture.first_set_lambda(inputed_parametrs[2]);
        mixture.first_set_mu(inputed_parametrs[1]);
        mixture.first_set_nu(inputed_parametrs[0]);
        mixture.first_set_lambda(inputed_parametrs[5]);
        mixture.first_set_mu(inputed_parametrs[4]);
        mixture.first_set_nu(inputed_parametrs[3]);
        mixture.set_p(inputed_parametrs[6]);    
    }
    if (troubleshoot(method, 1, 2))
    {
        return;
    }
    while (IsRunning)
    {
        print_actions();
        cout << "5 - Сохранение параметров в файл" << endl;
        string output;
        cin >> selected_action;
        switch (selected_action)
        {

        case 1:
            IsRunning = false;
            break;
        case 2:
            cout << "Введите x0:";
            x = input_number(-100000, 100000);
            cout << "Плотность смеси распределений Гаусса в точке x0 = " << x << " :  " << mixture.get_density(x) << endl;
            break;
        case 3:
            parametrs = mixture.get_parametrs();
            print_parametrs();
            break;
        case 4:
            cout << "Смоделированная случайная величина : " << mixture.random_value() << endl;
            break;
        case 5:
            cout << "Введите путь к файлу для записи:" << endl;
            cin >> output;
            mixture.save(output);
            cout << "Запись завершена" << endl;
            break;
        }
        troubleshoot(selected_action, 1, 5);
    }
}