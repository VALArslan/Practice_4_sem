#include <iostream>

double max(double a, double b){
    if (a > b){
        return a;
    } else{
        return b;
    };
};

double min(double a, double b){
    if (a > b){
        return b;
    } else{
        return a;
    };
};

class Rec{
    friend Rec operator*(const Rec & r1, const Rec & r2);
    friend Rec operator+(const Rec & r1, const Rec & r2);
    double x1, y1, a, b, x2, y2;
public:
    Rec(double x = 0, double y = 0, double a = 0, double b = 0){
        x1 = x;
        y1 = y;
        this->a = a;
        this->b = b;
        x2 = x1 + b;
        y2 = y1 + a;
    };
    Rec (const Rec & r){
        x1 = r.x1;
        y1 = r.y1;
        a = r.a;
        b = r.b;
        x2 = r.x2;
        y2 = r.y2;
    }
    // Copy Constructor basic
    void move(double dx, double dy){
        x1 += dx;
        y1 += dy;
        x2 += dx;
        y2 += dy;
    };

    void change(double a_n, double b_n){
        a = a_n;
        b = b_n;
        x2 = x1 + a;
        y2 = y1 + b;
    };

    void print() const {
        std::cout << "x-cord:" << x1 << std::endl;
        std::cout << "y-cord:" << y1 << std::endl;
        std::cout << "top side:" << a << std::endl;
        std::cout << "left side:" << b << std::endl;
    }
};

Rec operator *(const Rec & r1, const Rec & r2){
    Rec res;
    double a_n, b_n;
    if ((r1.x2 < r2.x1) or (r2.x2 < r1.x1) or (r1.y2 < r2.y1) or (r2.y2 < r1.y1)){
        return res;
    }
    res.x1 = max(r1.x1, r2.x1);
    res.y1 = max(r1.y1, r2.y1);
    b_n = min(r1.x2, r2.x2) - res.x1;
    a_n = min(r1.y2, r2.y2) - res.y1;
    res.change(a_n, b_n);
    return res;
};

Rec operator +(const Rec & r1, const Rec & r2){
    Rec res;
    double a_n, b_n;
    res.x1 = min(r1.x1, r2.x1);
    res.y1 = min(r1.y1, r2.y2);
    b_n = max(r1.x2, r2.x2) - res.x1;
    a_n = max(r1.y2, r2.y2) - res.y1;
    res.change(a_n, b_n);
    return res;
};

int main(){
    int actual_len = 0, com, vice_com, res_com;
    int maxlen = 100;
    Rec * arr = new Rec[maxlen];
    double x, y, a, b;
    std::cout << "Hello. This program demonstrate functional of class Rectangles" << std::endl;
    std::cout << "Let`s start." << std::endl;
    while (true){
        std::cout << "1. Add a Recatngle." << std::endl;
        std::cout << "2. Move Rectangle." << std::endl;
        std::cout << "3. Change sizes of Rectangle." << std::endl;
        std::cout << "4. Merge two Rectangles." << std::endl;
        std::cout << "5. Cross two Rectangles." << std::endl;
        std::cout << "6. Show all Rectangles." << std::endl;
        std::cout << "7. Exit." << std::endl;
        std::cin >> com;
        switch (com)
        {
        case 1:
            std::cout << "Choose the number of new Rectangle (0 = new)" << std::endl;
            std::cin >> com;
            if (com == 0){
                com = actual_len++;
            }else {
                com--;
            }
            std::cout << "Input x-cord, y-cord of left-down angle, top and left side of Rectangle, through space." << std::endl;
            std::cin >> x;
            std::cin >> y;
            std::cin >> a;
            std::cin >> b;
            //std::cout << x << " " << y << " " << a << " " << b << " " << std::endl;
            arr[com] = Rec(x, y, a, b);
            break;
        case 2:
            std::cout << "Choose the number of Rectangle." << std::endl;
            std::cin >> com;
            std::cout << "Input shift for x and y cords, through space." << std::endl;
            std::cin >> x;
            std::cin >> y;
            arr[com-1].move(x, y);
            break;
        case 3:
            std::cout << "Choose the number of Rectangle." << std::endl;
            std::cin >> com;
            std::cout << "Input new top and left size, throgh space." << std::endl;
            std::cin >> a;
            std::cin >> b;
            arr[com-1].change(a, b);
            break;
        case 4:
            std::cout << "Choose the numbers of two Rectangles, through space." << std::endl;
            std::cin >> com;
            std::cin >> vice_com;
            std::cout << "Choose the number of new Rectangle (0 = new)." << std::endl;
            std::cin >> res_com;
            if (res_com == 0){
                res_com = actual_len++;
            }else {
                res_com--;
            };
            arr[res_com] = arr[com-1] + arr[vice_com - 1];
            break;
        case 5:
            std::cout << "Choose the numbers of two Rectangles, through space." << std::endl;
            std::cin >> com;
            std::cin >> vice_com;
            std::cout << "Choose the number of new Rectangle (0 = new)." << std::endl;
            std::cin >> res_com;
            if (res_com == 0){
                res_com = actual_len++;
            }else {
                res_com--;
            };
            //(arr[com-1] * arr[vice_com - 1]).print();
            arr[res_com] = arr[com-1] * arr[vice_com - 1];
            break;
        case 6:
            for (int i = 0; i < actual_len; i++){
                std::cout << i+1 << " Rectangle" << std::endl;
                arr[i].print();
                std::cout << std::endl;
            }
            break;
        case 7:
            delete [] arr;
            return 0;
        default:
            std::cout << "Wrong command" << std::endl;
            break;
        }
    };
    return 1;
}