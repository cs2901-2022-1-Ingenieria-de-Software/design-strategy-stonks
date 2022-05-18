
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Calculo
{
public:
    Calculo() = default;
    virtual float execute(const std::vector<std::vector<float>> &data) = 0;
};

class PromedioApertura : public Calculo
{
public:
    PromedioApertura() = default;
    float execute(const std::vector<std::vector<float>> &data) override
    {
        return 1;
    };
};

class PromedioCierre : public Calculo
{
public:
    PromedioCierre() = default;
    float execute(const std::vector<std::vector<float>> &data) override
    {
        return 1;
    };
};

class MediaMovil : public Calculo
{
public:
    MediaMovil() = default;
    float execute(const std::vector<std::vector<float>> &data) override
    {
        return 1;
    };
};


class Context
{
private:
    shared_ptr<Calculo> calculo;
    std::vector<std::vector<float>> data;

public:
    Context(std::string filename){
        // TODO: read file
        ofstream fileRead(filename, std::ios::in);
        if (fileRead.is_open()) {}
    };
    void setStrategy(const shared_ptr<Calculo>& cal)
    {
        this->calculo = cal;
    }
    float executeStrategy()
    {
        return calculo->execute(data);
    };
};

class Pronostico
{
private:
    Context context;
public:
    Pronostico(std::string filename): context{filename} {}
    float run(std::string action)
    {
        if (action == "PromedioApertura") {
            context.setStrategy(make_shared<PromedioApertura>());
        } else if (action == "PromedioCierre") {
            context.setStrategy(make_shared<PromedioCierre>());
        } else if (action == "MediaMovil") {
            context.setStrategy(make_shared<MediaMovil>());
        } else {
            throw std::invalid_argument("No se encontro la accion");
        }
        return context.executeStrategy();
    }
};

int main()
{
    auto p = make_shared<Pronostico>("pronostico.csv");
    std::cout << std::to_string(p->run("PromedioApertura")) << std::endl;
    std::cout << std::to_string(p->run("PromedioCierre")) << std::endl;
    std::cout << std::to_string(p->run("MediaMovil")) << std::endl;
    return 0;
}
