#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

const int MAX_LENGTH_DATE = 10;
const int MAX_LENGTH_CONCURRENCY = 3;
class Tabla
{
public:
    char date[MAX_LENGTH_DATE];
    float open;
    float high;
    float low;
    float close;
    float volume;
    char concurrency[MAX_LENGTH_CONCURRENCY];
    void read(const std::string& line)
    {
        sscanf(line.c_str(),
               "%[^,],%f,%f,%f,%f,%f,%[^,]", date, &open, &high, &low, &close, &volume, concurrency);
    };
};

class Calculo
{
public:
    Calculo() = default;
  	virtual ~Calculo(){}
    virtual float execute(const std::vector<std::vector<float>> &data) = 0;
};

class PromedioApertura : public Calculo
{
public:
    PromedioApertura() = default;
    virtual ~PromedioApertura(){}
    float execute(const std::vector<std::vector<float>> &data) override
    {
        float suma = 0;
        for (const auto& lin : data) {
            suma = suma + lin[1];
        }
        return suma / ((float) data.size());
    };
};

class PromedioCierre : public Calculo
{
public:
    PromedioCierre() = default;
    float execute(const std::vector<std::vector<float>> &data) override
    {
        float suma = 0;
        for (const auto& lin : data) {
            suma = suma + lin[4];
        }
        return suma / ((float) data.size());
    };
};

class MediaMovil : public Calculo
{
public:
    MediaMovil() = default;
    float execute(const std::vector<std::vector<float>> &data) override
    {
    	float suma = 0;
         for(int i=data.size();i>data.size()-7;i--){
         suma = suma + data[i][4];
        }
        return suma / 7;
      
    };
};


class Context
{
private:
    shared_ptr<Calculo> calculo;
    std::vector<std::vector<float>> data;

public:
    explicit Context(const std::string& filename){
        // TODO: read file
        Tabla t{};
        std::ifstream fileRead(filename, std::ios::in);
        if (!fileRead.is_open())
            throw std::invalid_argument("File not found");
        std::string line;
        while (getline(fileRead, line))
        {
            std::vector<float> temp;
            t.read(line);
            temp.push_back(t.open);
            temp.push_back(t.high);
            temp.push_back(t.low);
            temp.push_back(t.close);
            temp.push_back(t.volume);
            data.push_back(temp);
        }
        fileRead.close();
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
    explicit Pronostico(const std::string& filename): context{filename} {}
    float run(const std::string& action)
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
    auto p = make_shared<Pronostico>("gold.csv");
    std::cout << std::to_string(p->run("PromedioApertura")) << std::endl;
    std::cout << std::to_string(p->run("PromedioCierre")) << std::endl;
    std::cout << std::to_string(p->run("MediaMovil")) << std::endl;
    return 0;
}
















































