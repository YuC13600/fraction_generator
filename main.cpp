#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <string>
using namespace std;

bool error = false;

long long mgcd(long long a, long long b) {
    if (b) while ((a%=b) && (b%=a));
    return a + b;
}

long long mlcm(long long a, long long b) {
    return a * b / mgcd(a, b);
}

class Oprand {
public:
    bool is_mixed;
    long long whole_part;
    long long numerator_part;
    long long denominator;

    void set() {
        is_mixed = rand() % 2;
        denominator = rand() % 80 + 2;
        numerator_part = rand() % 80 + 2;
        if (denominator < numerator_part) {
            swap(denominator, numerator_part);
        }
        if (denominator == numerator_part) {
            denominator += 1;
        }
        if (is_mixed) {
            whole_part = rand() % 100 + 1;
        } else {
            whole_part = (long long)NULL;
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
    void adjust(Oprand *op) {
        if (op->numerator_part >= op->denominator) {
            op->whole_part += op->numerator_part / op->denominator;
            op->numerator_part %= op->denominator;
            if (!op->numerator_part) {
                op->denominator = 1;
            }
            op->is_mixed = true;
        }
        long long gcd = mgcd(op->numerator_part, op->denominator);
        op->numerator_part /= gcd;
        op->denominator /= gcd;
    }
    void to_improper(Oprand *op) {
        if (op->is_mixed) {
            op->numerator_part += op->whole_part * op->denominator;
            op->is_mixed = false;
            op->whole_part = 0;
        }
    }
    Oprand add(Oprand *op) {
        to_improper(this);
        to_improper(op);
        long long lcm = mlcm(this->denominator, op->denominator);

        long long multi_self = lcm / this->denominator;
        this->denominator = lcm;
        this->numerator_part *= multi_self;

        long long multi_other = lcm / op->denominator;
        op->denominator = lcm;
        op->numerator_part *= multi_other;

        Oprand ret = Oprand();
        Oprand *ret_ptr = &ret;
        ret_ptr->denominator = lcm;
        ret_ptr->numerator_part = this->numerator_part + op->numerator_part;
        ret_ptr->whole_part = 0;
        ret_ptr->is_mixed = false;
        adjust(ret_ptr);
        adjust(this);
        adjust(op);
        return ret;
    }

    Oprand sub(Oprand *op) {
        to_improper(this);
        to_improper(op);
        long long lcm = mlcm(this->denominator, op->denominator);

        long long multi_self = lcm / this->denominator;
        this->denominator = lcm;
        this->numerator_part *= multi_self;

        long long multi_other = lcm / op->denominator;
        op->denominator = lcm;
        op->numerator_part *= multi_other;

        Oprand ret = Oprand();
        Oprand *ret_ptr = &ret;
        ret_ptr->denominator = lcm;
        ret_ptr->numerator_part = this->numerator_part - op->numerator_part;
        if (ret_ptr->numerator_part < 0) {
            ret_ptr->denominator = 0;
            return ret;
        }
        ret_ptr->whole_part = 0;
        ret_ptr->is_mixed = false;
        adjust(ret_ptr);
        adjust(this);
        adjust(op);
        return ret;
    }

    Oprand multi(Oprand *op) {
        to_improper(this);
        to_improper(op);
        Oprand ret = Oprand();
        Oprand *ret_ptr = &ret;
        ret_ptr->denominator = this->denominator * op->denominator;
        ret_ptr->numerator_part = this->numerator_part * op->numerator_part;
        ret_ptr->whole_part = 0;
        adjust(ret_ptr);
        adjust(this);
        adjust(op);
        return ret;
    }
    Oprand div(Oprand *op) {
        to_improper(this);
        to_improper(op);
        Oprand ret = Oprand();
        Oprand *ret_ptr = &ret;
        ret_ptr->denominator = this->denominator * op->numerator_part;
        ret_ptr->numerator_part = this->numerator_part * op->denominator;
        ret_ptr->whole_part = 0;
        adjust(ret_ptr);
        adjust(this);
        adjust(op);
        return ret;
    }
};

class Equation {
public:
    Oprand opds[3];
    string optr_list[4] = {"+", "-", "\\times", "\\div"};
    string optrs[2];

    void set() {
        for (long long i(0); i < 3; ++i) {
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

    string answer() {
        Oprand temp;
        if ((optrs[0] == "+" || optrs[0] == "-") && (optrs[1] == "\\times" || optrs[1] == "\\div")) {
            if (optrs[1] == "\\times") {
                temp = (opds[1]).multi(opds + 2);
            } else {
                temp = (opds[1]).div(opds + 2);
            }
            Oprand result;
            if (optrs[0] == "+") {
                result = (opds[0]).add(&temp);
            } else {
                result = (opds[0]).sub(&temp);
                if (result.denominator == 0) {
                    error = true;
                }
            }
            return result.get();
        } else {
            long long i = 0;
            if (optrs[i] == "+") {
                temp = (opds[i]).add(opds + i + 1);
            } else if (optrs[i] == "-") {
                temp = (opds[i]).sub(opds + i + 1);
                if (temp.denominator == 0) {
                    error = true;
                    return temp.get();
                }
            } else if (optrs[i] == "\\times") {
                temp = (opds[i]).multi(opds + i + 1);
            } else {
                temp = (opds[i]).div(opds + i + 1);
            }

            i = 1;
            if (optrs[i] == "+") {
                temp = temp.add(opds + i + 1);
            } else if (optrs[i] == "-") {
                temp = temp.sub(opds + i + 1);
                if (temp.denominator == 0) {
                    error = true;
                    return temp.get();
                }
            } else if (optrs[i] == "\\times") {
                temp = temp.multi(opds + i + 1);
            } else {
                temp = temp.div(opds + i + 1);
            }
            return temp.get();
        }
    }

};
int main(void) {
    srand(time(NULL));
    string name = to_string(rand()%10000);
    string ques = name + ".md";
    string ans = name + "_ans.md";
    ofstream fans(ans);
    ofstream fques(ques);
    for (long long i(0); i < 300; ++i) {
        Equation e = Equation();
        e.set();
        string ans = e.answer();
        if (error) {
            error = false;
            i -= 1;
            continue;
        }
        fques << "$$ " << i + 1 << " .\\;\\;\\; " << e.get() << " = $$\n";
        fans << "$$ " << i + 1 << " .\\;\\;\\; " << e.get() << " = " << ans << " $$\n";
    }
    fans.close();
    fques.close();
    return 0;
}
