#include <iostream>
#include <cstring>

class Transport{
    char * title;

    int num_pass;

    int speed;

public:
    Transport(const char * t, int c_pass, int sp){
        title = new char[sizeof t];
        std::strcpy(title, t);
        num_pass = c_pass;
        speed = sp;
    };

    Transport(const Transport & d){
        title = new char[sizeof d.title];
        std::strcpy(title, d.title);
        num_pass = d.num_pass;
        speed = d.speed;
    };

    Transport & operator = (const Transport & d){
        if (this == &d){
            return *this;
        }
        delete [] title;
        title = new char[sizeof d.title];
        std::strcpy(title, d.title);
        num_pass = d.num_pass;
        speed = d.speed;
        return *this;
    }

    char * Get_title() const {
        return title;
    }

    int Get_pass() const {
        return num_pass;
    }

    int Get_speed() const {
        return speed;
    }

    void Set_pass(int num){
        num_pass = num;
    }

    void Set_title(const char * t){
        delete [] title;
        title = new char[sizeof t];
        std::strcpy(title, t);
    }

    void Set_speed(int sp){
        speed = sp;
    }

    virtual int Travel_cost(int pass) const = 0; //чистая виртуальная функция расчёта стоимости поездки на pass персон

    static int Std_price_bus;
    static int Std_price_taxi;

    virtual ~Transport(){
        delete [] title;
    }

};

class Bus: public Transport{
public:
    Bus(const char * t, int c_pass, int sp):Transport(t, c_pass, sp){};

    Bus(const Bus &b): Transport(b.Get_title(), b.Get_pass(), b.Get_speed()){};

    int Travel_cost(int pass) const {
        return Transport::Std_price_bus * (1 + this->Get_pass()/20) * pass;
    }

    Bus & operator = (const Bus & b){
        if (this == &b){
            return *this;
        }
        this->Set_title(b.Get_title());
        this->Set_pass(b.Get_pass());
        this->Set_speed(b.Get_speed());
        return *this;
    }
    friend std::ostream & operator << (std::ostream & s, const Bus & b);

};

class Taxi: public Transport{
public:
    Taxi(const char * t, int c_pass, int sp):Transport(t, c_pass, sp){};

    Taxi(const Taxi & t): Transport(t.Get_title(), t.Get_pass(), t.Get_speed()){};

    int Travel_cost(int pass) const {
        return Transport::Std_price_taxi*((pass + 1) / 2) / (this->Get_pass() + 1);
    }

    Taxi & operator = (const Taxi & t){
        if (this == &t){
            return *this;
        }
        this->Set_title(t.Get_title());
        this->Set_pass(t.Get_pass());
        this->Set_speed(t.Get_speed());
        return *this;
    }

    friend std::ostream & operator << (std::ostream & s, const Taxi & t);
};

std::ostream & operator << (std::ostream & s, const Bus & b){
    s << "Bus" << std::endl;
    s << "Bus name: " << b.Get_title() << std::endl;
    s << "Passengers count: " << b.Get_pass() << std::endl;
    s << "Transport speed: " << b.Get_speed() << std::endl;
    s << "Standard price: " << Transport::Std_price_bus << std::endl;
    s << "Current price for one person: " << b.Travel_cost(1) << std::endl;
    return s;
}

std::ostream & operator << (std::ostream & s, const Taxi & t){
    s << "Taxi" << std::endl;
    s << "Taxi name: " << t.Get_title() << std::endl;
    s << "Passengers count: " << t.Get_pass() << std::endl;
    s << "Transport speed: " << t.Get_speed() << std::endl;
    s << "Standard price: " << Transport::Std_price_taxi << std::endl;
    s << "Current price for one person: " << t.Travel_cost(1) << std::endl;
    return s;
}


int Transport::Std_price_bus = 30;
int Transport::Std_price_taxi = 100;


int main(){
    Bus b1("Makarov", 15, 30);
    std::cout << b1 << std::endl;
    Taxi t1("Saturn", 1, 60);
    std::cout << t1 << std::endl;
    std::cout << "Checking Assignment operation" << std::endl;
    Taxi t2("Yandex", 0, 40);
    std::cout << t2 << std::endl;
    t2 = t1;
    std::cout << t2 << std::endl;
    std::cout << "Count price for two person: " << t1.Travel_cost(2) << std::endl;
    return 0;
};