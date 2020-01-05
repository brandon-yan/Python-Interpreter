#ifndef BIGINT_H
#define BIGINT_H
#include <vector>
#include <iostream>
#include <cstring>
using std::string;

class Bigint {
    friend std::istream& operator>>(std::istream &in, Bigint& obj);
    friend std::ostream& operator<<(std::ostream &os, const Bigint& obj);
    friend Bigint operator+(const Bigint &x1, const Bigint &x2);
    friend Bigint operator-(const Bigint &x1, const Bigint &x2);
    friend Bigint operator*(const Bigint &x1, const Bigint &x2);
    friend double operator/(const Bigint &x1, const Bigint &x2);
    friend Bigint operator%(const Bigint &x1, const Bigint &x2);
    friend Bigint intdivide(const Bigint &x1, const Bigint &x2);
    friend Bigint operator+=(Bigint &x1, const Bigint &x2);
    friend Bigint operator-=(Bigint &x1, const Bigint &x2);
    friend Bigint operator*=(Bigint &x1, const Bigint &x2);
    friend double operator/=(Bigint &x1, const Bigint &x2);
    friend Bigint intdeng(Bigint &x1, const Bigint &x2);
    friend Bigint operator%=(Bigint &x1, const Bigint &x2);
    friend bool operator<(const Bigint &x1, const Bigint &x2);
    friend bool operator<=(const Bigint &x1, const Bigint &x2);
    friend bool operator>(const Bigint &x1, const Bigint &x2);
    friend bool operator>=(const Bigint &x1, const Bigint &x2);
    friend bool operator==(const Bigint &x1, const Bigint &x2);
    friend bool operator!=(const Bigint &x1, const Bigint &x2);
    
    private:
    std::vector<int> number; 
    int positive;

    public:
    Bigint (string x = "") {
        int len = x.length();
        if(x[0] == '-') {
            for (int i = len - 1; i > 0; --i) number.push_back(x[i] - '0');
            positive = 0;
        } 
        else {
            for (int i = len - 1; i >= 0; --i) number.push_back(x[i] - '0');
            positive = 1;
        }
        if (x[0] == '-' && x[1] == '0') positive = 1;
    }
    Bigint (int x) {
        if (x > 0) {
            positive = 1;
            while (x != 0) {
                number.push_back(x % 10);
                x /= 10;
            }
        }
        if (x == 0) {
            number.push_back(0);
            positive = 1;
        }
        if (x < 0) {
            positive = 0;
            x = 0 - x;
            while (x != 0) {
                number.push_back(x % 10);
                x /= 10;
            }
        }
    }
    Bigint oppo () const {
        Bigint x2;
        x2.number = number;
        if (positive == 1) x2.positive = 0;
        else x2.positive = 1;
        return x2;
    }
    Bigint abs() const {
        Bigint x2;
        x2.number = number;
        x2.positive = 1;
        return x2;
    }
    Bigint zero() {
        if (number.size() == 1 && number[0] == 0) positive = 1;
        return *this;
    }
    explicit operator int () const;
    explicit operator double () const;
    explicit operator string () const;
    explicit operator bool () const;
};

enum typee { INT, DOUBLE, STRING, BOOL, NONE };
class alltype {
    friend alltype operator+(const alltype &x1, const alltype &x2);
    friend alltype operator-(const alltype &x1, const alltype &x2);
    friend alltype operator*(const alltype &x1, const alltype &x2);
    friend alltype operator/(const alltype &x1, const alltype &x2);
    friend alltype operator%(const alltype &x1, const alltype &x2);
    friend alltype intdivide(const alltype &x1, const alltype &x2);
    friend alltype operator+=(alltype &x1, const alltype &x2);
    friend alltype operator-=(alltype &x1, const alltype &x2);
    friend alltype operator*=(alltype &x1, const alltype &x2);
    friend alltype operator/=(alltype &x1, const alltype &x2);
    friend alltype intdeng(alltype &x1, const alltype &x2);
    friend alltype operator%=(alltype &x1, const alltype &x2);
    friend alltype operator<(const alltype &x1, const alltype &x2);
    friend alltype operator<=(const alltype &x1, const alltype &x2);
    friend alltype operator>(const alltype &x1, const alltype &x2);
    friend alltype operator>=(const alltype &x1, const alltype &x2);
    friend alltype operator==(const alltype &x1, const alltype &x2);
    friend alltype operator!=(const alltype &x1, const alltype &x2);
    friend alltype operator&&(const alltype &x1, const alltype &x2);
    
    public:
    Bigint intval;
    double douval;
    string strval;
    bool booval;
    typee Type;
    string name = "";

    alltype(): Type(NONE) {}
    alltype(Bigint x): Type(INT), intval(x) {
        if (x == 0) booval = false;
        else booval = true;
    }
    alltype(double x): Type(DOUBLE), douval(x) {
        if (x == 0) booval = false;
        else booval = true;
    }
    alltype(string x): Type(STRING), strval(x) {
        if (x == "True") booval = true;
        if (x == "False") booval = false;
    }
    alltype(bool x): Type(BOOL), booval(x), intval(x) {}

    Bigint toINT() const {
        switch(Type) {
            case INT: return intval; break;
            case DOUBLE: return Bigint ((long long) douval);
            case STRING: return Bigint (strval); break;
            case BOOL: return intval; break;
            default: return Bigint(); break;
        }
    }
    double toDOUBLE() const {
        switch(Type) {
            case INT: return (double)intval; break;
            case DOUBLE: return douval; break;
            case STRING: return stod(strval); break;
            case BOOL: return (double)intval; break;
            default: return (double)0; break;
        }
    }
    string toSTRING() const {
        switch(Type) {
            case NONE: return string("None"); break;
            case INT: return (string)intval; break;
            case DOUBLE: return std::to_string(douval); break;
            case STRING: return strval; break;
            case BOOL: return (string)(booval ? "True" : "False"); break;  
        }
    }
    bool toBOOL() const {
        switch(Type) {
            case INT: return (intval != 0); break;
            case DOUBLE: return (douval != 0); break;
            case STRING: {
                if (strval == "") return false;
                else return true;
            } break;
            case BOOL: return booval; break;  
        }
    }
};

bool operator==(const Bigint &x1, const Bigint &x2) {
    if (x1.positive != x2.positive || x1.number.size() != x2.number.size()) return false;
    if (x1.positive == x2.positive) {
        for (unsigned i = 0; i < x1.number.size(); ++i)
        if (x1.number[i] != x2.number[i]) return false;
    }
    return true;
}
bool operator!=(const Bigint &x1, const Bigint &x2) {
    if (x1 == x2) return false;
    else return true;
}
bool operator<(const Bigint &x1, const Bigint &x2) {
    if (x1.positive > x2.positive) return false;
    if (x1.positive < x2.positive) return true;
    if (x1.positive == 1 && x1.number.size() < x2.number.size()) return true;
    if (x1.positive == 1 && x1.number.size() > x2.number.size()) return false;
    if (x1.positive == 0 && x1.number.size() < x2.number.size()) return false;
    if (x1.positive == 0 && x1.number.size() > x2.number.size()) return true;
    for (int i = x1.number.size() - 1; i >= 0; --i) {
        if (x1.positive == 1) {
            if (x1.number[i] < x2.number[i]) return true;
            if (x1.number[i] > x2.number[i]) return false;
        }
        if (x1.positive == 0) {
            if (x1.number[i] < x2.number[i]) return false;
            if (x1.number[i] > x2.number[i]) return true;
        }
    }  
    return false;
}
bool operator>(const Bigint &x1, const Bigint &x2) {
    if (x1 < x2 || x1 == x2) return false;
    else return true;
}
bool operator>=(const Bigint &x1, const Bigint &x2) {
    if (x1 > x2 || x1 == x2) return true;
    else return false;
}
bool operator<=(const Bigint &x1, const Bigint &x2) {
    if (x1 < x2 || x1 == x2) return true;
    else return false;
}
Bigint operator+(const Bigint &x1, const Bigint &x2) {
    int t = 0;
    Bigint x3;
    if (x1.positive == x2.positive) {
        for(unsigned i = 0; i < x1.number.size() || i < x2.number.size(); ++i) {
            if(i < x1.number.size()) t += x1.number[i];
            if(i < x2.number.size()) t += x2.number[i];
            x3.number.push_back(t % 10);
            t /= 10;
        }
        if(t) x3.number.push_back(1);
        if(x1.positive == 1) x3.positive = 1;
        if(x1.positive == 0) x3.positive = 0;
        return x3.zero();
    }
    if (x1.positive > x2.positive) {
        Bigint x4 = x2.oppo();
        return x1 - x4;
    }
    if (x1.positive < x2.positive) {
        Bigint x5 = x1.oppo();
        return x2 - x5;
    }
}
Bigint operator-(const Bigint &x1, const Bigint &x2) {
    int t = 0;
    Bigint x3;
    if (x1 >= x2) {
        if (x2.positive == 1) {
            for (unsigned i = 0; i < x1.number.size(); ++i){
                t = x1.number[i] - t; 
                if(i < x2.number.size()) t -= x2.number[i];  
                x3.number.push_back((t + 10) % 10); 
                if(t < 0) t = 1;           
                else t = 0;
            }
            while(x3.number.size() > 1 && x3.number.back() == 0) x3.number.pop_back(); 
            return x3.zero();
        }
        if (x1.positive == 0) {
            Bigint x4 = x2.oppo();
            return x4 + x1;
        }
    }
    if (x1 < x2) {
        if (x1.positive == 1) {
            Bigint x5 = x2 - x1;
            x5.positive = 0;
            return x5.zero();
        }
        if (x2.positive == 0) {
            Bigint x6 = x2.oppo();
            return x1 + x6;
        }   
    }
    if (x1.positive == 1 && x2.positive == 0) {
        Bigint x7 = x2.oppo();
        return x1 + x7;
    }
    if (x1.positive == 0 && x2.positive == 1) {
        Bigint x8 = x2.oppo();
        return x1 + x8;
    }
}
Bigint operator*(const Bigint &x1, const Bigint &x2) {
    Bigint x3;
    int len3 = x1.number.size() + x2.number.size();
    for (int i = 0; i < len3; ++i) x3.number.push_back(0); 
    for (unsigned i = 0; i < x1.number.size(); ++i) 
        for (unsigned j = 0; j < x2.number.size(); ++j) 
            x3.number[i + j] += x1.number[i] * x2.number[j];
    for (int i = 0; i < len3; ++i) 
        if (x3.number[i] > 9) {
            x3.number[i + 1] += x3.number[i] / 10;
            x3.number[i] %= 10;
        }
    while (x3.number.size() > 1 && !x3.number.back()) x3.number.pop_back();
    if (x1.positive == x2.positive) x3.positive = 1;
    else x3.positive = 0;
    return x3.zero();
}
Bigint intdivide(const Bigint &x1, const Bigint &x2) {
    Bigint x3, tmp;
    if (x1.abs() < x2.abs()) {
        if (x1.positive == x2.positive) {
            x3.number.push_back(0);
            x3.positive = 1;
        }
        else {
            x3.number.push_back(1);
            x3.positive = 0;
        }
    }
    else {
        for (int i = 0; i < x1.number.size(); ++i) {
            x3.number.push_back(0);
            tmp.number.push_back(0);
        }
        for (int i = x1.number.size() - 1; i >= 0; --i) {
            tmp = tmp * 10;
            tmp.number[0] = x1.number[i];
            while (tmp.abs() >= x2.abs()) {
                tmp = tmp - x2.abs();
                ++x3.number[i];
            }
        }
        while (x3.number.size() > 1 && !x3.number.back()) x3.number.pop_back();
        if (x1.positive == x2.positive) x3.positive = 1;
        else x3.positive = 0;
        if (x3.positive == 0 && x3.abs() * x2.abs() != x1.abs()) x3 = x3 - (Bigint)1;
    }   
    return x3.zero();
}
double operator/(const Bigint &x1, const Bigint &x2) {
    double a = (double)x1;
    double b = (double)x2; 
    return (double)(a / b);
}
Bigint operator%(const Bigint &x1, const Bigint &x2) {
    Bigint x3;
    x3 = x1 - intdivide(x1, x2) * x2;
    return x3.zero();
}
Bigint operator+=(Bigint &x1, const Bigint &x2) {
    x1 = x1 + x2; return x1;
}
Bigint operator-=(Bigint &x1, const Bigint &x2) {
    x1 = x1 - x2; return x2;
}
Bigint operator*=(Bigint &x1, const Bigint &x2) {
    x1 = x1 * x2; return x1;
}
double operator/=(Bigint &x1, const Bigint &x2) {
    x1 = x1 / x2; return (double)x1;
}
Bigint intdeng(Bigint &x1, const Bigint &x2) {
    x1 = intdivide(x1, x2); return x1;
}
Bigint operator%=(Bigint &x1, const Bigint &x2) {
    x1 = x1 % x2; return x1;
}
Bigint::operator int () const {
    int ret = 0;
    for (int i = number.size() - 1; i >= 0; --i) ret = ret * 10 + number[i];
    if (positive == 0) ret = 0 - ret; 
    return ret;
}
Bigint::operator double () const {
    double ret = 0;
    for (int i = number.size() - 1; i >= 0; --i) ret = ret * 10 + number[i];
    if (positive == 0) ret = 0 - ret; 
    return ret;
}
Bigint::operator string () const{
    string ret = "";
    if (positive == 1) {
        for (int i = number.size() - 1; i >= 0; --i) {
            ret = ret + (char)(number[i] + '0');
        }
    }
    else {
        ret =  ret + "-";
        for (int i = number.size() - 1; i >= 0; --i) {
            ret = ret + (char)(number[i] + '0');
        }
    }
    return ret;
}
Bigint::operator bool () const{
    if (number[0] == 0 && number.size() == 1) return false;
    else return true;
}

alltype operator==(const alltype &x1, const alltype &x2) {
    if (x1.Type == x2.Type) {
        if (x1.Type != NONE) return alltype(x1.toSTRING() == x2.toSTRING());
        else return alltype(true);
    }
    else return alltype(false);
}
alltype operator!=(const alltype &x1, const alltype &x2) {
    alltype x3 = (x1 == x2);
    if (x3.booval == true) return alltype(false);
    else return alltype(true);
}
alltype operator<(const alltype &x1, const alltype &x2) {
    if (x1.Type == STRING && x2.Type == STRING) return alltype(x1.toSTRING() < x2.toSTRING());
    if (x1.Type != STRING && x2.Type != STRING) return alltype(x1.toDOUBLE() < x2.toDOUBLE());
}
alltype operator>(const alltype &x1, const alltype &x2) {
    alltype x3 = (x1 < x2);
    if (x3.booval == true) return alltype(false);
    else return alltype(true);
}
alltype operator>=(const alltype &x1, const alltype &x2) {
    alltype x3 = (x1 > x2), x4 = (x1 == x2);
    if (x3.booval || x4.booval) return alltype(true);
    else return alltype(false);
}
alltype operator<=(const alltype &x1, const alltype &x2) {
    alltype x3 = (x1 < x2), x4 = (x1 == x2);
    if (x3.booval || x4.booval) return alltype(true);
    else return alltype(false);
}
alltype operator&&(const alltype &x1, const alltype &x2) {
    if (x1.booval && x2.booval) return alltype (true);
    else return alltype (false);
}
alltype operator+(const alltype &x1, const alltype &x2) {
    if (x1.Type == STRING && x2.Type == STRING) return alltype(x1.toSTRING() + x2.toSTRING());
    if ((x1.Type == INT || x1 .Type == BOOL) && (x2.Type == INT || x2 .Type == BOOL)) return alltype(x1.toINT() + x2.toINT());
    if (x1.Type == DOUBLE || x2.Type == DOUBLE) return alltype(x1.toDOUBLE() + x2.toDOUBLE());
}
alltype operator-(const alltype &x1, const alltype &x2) {
    if ((x1.Type == INT || x1 .Type == BOOL) && (x2.Type == INT || x2 .Type == BOOL)) return alltype(x1.toINT() - x2.toINT());
    if (x1.Type == DOUBLE || x2.Type == DOUBLE) return alltype(x1.toDOUBLE() - x2.toDOUBLE());
}
alltype operator*(const alltype &x1, const alltype &x2) {
    if ((x1.Type == INT || x1 .Type == BOOL) && (x2.Type == INT || x2 .Type == BOOL)) return alltype(x1.toINT() * x2.toINT());
    if (x1.Type == DOUBLE || x2.Type == DOUBLE) return alltype(x1.toDOUBLE() * x2.toDOUBLE());
    if (x1.Type == STRING && (x2.Type == INT || x2.Type == BOOL)) {
        alltype x3 = x1; int loop = (int)x2.toINT();
        while(--loop) x3 = x3 + x1;
        return x3;
    }
    if ((x1.Type == INT || x1.Type == BOOL) && x2.Type == STRING) return x2 * x1;
}
alltype operator/(const alltype &x1, const alltype &x2) {
    if ((x1.Type == INT || x1 .Type == BOOL) && (x2.Type == INT || x2 .Type == BOOL)) return alltype(x1.toINT() / x2.toINT());
    if (x1.Type == DOUBLE || x2.Type == DOUBLE) return alltype(x1.toDOUBLE() / x2.toDOUBLE());
}
alltype operator%(const alltype &x1, const alltype &x2) {
    if ((x1.Type == INT || x1 .Type == BOOL) && (x2.Type == INT || x2 .Type == BOOL)) return alltype(x1.toINT() % x2.toINT());
}
alltype intdivide(const alltype &x1, const alltype &x2) {
    if ((x1.Type == INT || x1 .Type == BOOL) && (x2.Type == INT || x2 .Type == BOOL)) return alltype(intdivide(x1.toINT(), x2.toINT()));
}
alltype operator+=(alltype &x1, const alltype &x2) {
    x1 = x1 + x2; return x1;
}
alltype operator-=(alltype &x1, const alltype &x2) {
    x1 = x1 - x2; return x1;
}
alltype operator*=(alltype &x1, const alltype &x2) {
    x1 = x1 * x2; return x1;
}
alltype operator/=(alltype &x1, const alltype &x2) {
    x1 = x1 / x2; return x1;
}
alltype intdeng(alltype &x1, const alltype &x2) {
    x1 = intdivide(x1, x2); return x1;
}
alltype operator%=(alltype &x1, const alltype &x2) {
    x1 = x1 % x2; return x1;
}

#endif