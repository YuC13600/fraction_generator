#include <iostream>
#include <ctime>
#include <algorithm>
#include <string>
using namespace std;


class oprand {
public:
    bool is_mixed;
    int whole_part;
    int numerator_part;
    int denominator;

    void set() {
        is_mixed = rand() % 2;
        denominator = rand() % 100 + 2;
        numerator_part = rand() % 100 + 2;
        if (denominator < numerator_part) {
            swap(denominator, numerator_part);
        }
        if (denominator == numerator_part) {
            denominator += 1;
        }
        if (is_mixed) {
            whole_part = rand() % 1000 + 1;
        } else {
            whole_part = (int)NULL;
        }
    }

    string get() {
        string ret = "";
        if (is_mixed) {
            ret += to_string(whole_part);
        }
        ret += "\\frac{" + to_string(numerator_part) + "}{" + to_string(denominator) + "}";

        return ret;
    }
};

class equation {
public:
    oprand opds[3];
    string optr_list[4] = {"+", "-", "\\times", "\\div"};
    string optrs[2];

    void set() {
        for (int i(0); i < 3; ++i) {
            opds[i].set();
        }
        optrs[0] = optr_list[rand()%4];
        optrs[1] = optr_list[rand()%4];
    }

    string get() {
        string ret = "";
        ret += opds[0].get() + " ";
        ret += optrs[0] + " ";
        ret += opds[1].get() + " ";
        ret += optrs[1] + " ";
        ret += opds[2].get();

        return ret;
    }

};
int main(void) {
    srand(time(NULL));
    for (int i(0); i < 100; ++i) {
        cout << "$$ ";
        equation e = equation();
        e.set();
        cout << e.get() << " $$\n";
    }
    return 0;
}
