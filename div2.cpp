#include <iostream>
#include <bitset>
#include <cmath>
#include <string>
#include <tuple>

using namespace std;


tuple<int,int> division(int divident, int divisor) {

    // получаем знаковые биты чисел
    unsigned int s1 = *((unsigned int *)&divident) >> 31; // s1 и s2 - знаки. 0 значит положительное число, 1 значит отрицательное
    unsigned int s2 = *((unsigned int *)&divisor) >> 31; // префикс unsigned имеет значение, т.к. если представлять код "1" в знаковом виде, то при переводе в число получим -1 
    printf("sign_a: %d\tsign_b: %d \n", s1,s2);

    // вычисляем знаковый бит частного
    int sign = (s1 + s2) % 2;
    printf("Division sign: %d\n", sign);


    // ==============
    // назовем делимое d1, делитель d2

    // ищем абсолютные значения чисел; используем алгоритм дополнительного кода; ХОR числа со всеми единицами ни что иное, как инверсия всех битов
    // в данном случае переводим число из отрицательного в положительное; если же изначально положительное, то ничего не делаем
    int d1_abs=divident, d2_abs=divisor;
    int d2_minus=divisor;

    if (s1) {
        d1_abs = (divident - 1)^  0xFFFFFFFF; ;
    }

    if (s2) {  
        d2_abs = (divisor - 1) ^  0xFFFFFFFF;
        d2_minus = divisor;
    }
    else {
        d2_abs = divisor;
        d2_minus = (divisor ^ 0xFFFFFFFF) + 1;
    }

    int tmp_d1=d1_abs;
    int tmp_d2_abs=d2_abs;
    int tmp_d2_minus=d2_minus;

    printf("Divident_absolute: %d\nDivisor_plus %d \t Divisor_minus %d\n", d1_abs, d2_abs, d2_minus);

/*
    if (tmp_d1 < tmp_d2_abs) {
        remainder1 = tmp_d1;
        return 0;
    }
*/

    int cnt = 0; // количество сдвигов влево которые мы делаем, чтобы уравнять порядки делимого и делителя перед первым вычитанием

    // дописываем справа нули для делителя, пока его порядок не будет равен порядку делимого
    while(tmp_d2_abs << 1 <= tmp_d1) { 
        tmp_d2_abs = tmp_d2_abs << 1;
        tmp_d2_minus = tmp_d2_minus << 1;
        cnt++;
    }

    int quotient  = 0; // обнуляем частное
    for (int i = 0; i < cnt+1; i++) { // вычитание будет происходить cnt+1 раз

        tmp_d1 = tmp_d1 + tmp_d2_minus; // ОСНОВНОЕ ДЕЙСТВИЕ АЛГОРИТМА: складывание делимого и отрицательного делителя
        if (tmp_d1 >= 0 ) { // проверка на знак полученного значения
            quotient = (quotient << 1) + 1; // добавляем к частному 1 

        }
        else {
            quotient = quotient << 1; // добавляем к частному 0, если знак отрицательный
            tmp_d1 = tmp_d1 + tmp_d2_abs; // и возвращаем делитель, который не можем взять
        }

        tmp_d2_abs = tmp_d2_abs >> 1;  // сдвигаем делитель вправо
        tmp_d2_minus = tmp_d2_minus >> 1;
        
    }

    if (sign) // переводим частное в отрицательное число, если знак должен быть отрицательный
        quotient = (quotient ^ 0xFFFFFFFF) + 1;


    int remainder = tmp_d1; 
    if (s1) // переводим остаток в отрицательное число, если делимое было отрицательным
        remainder = (tmp_d1 ^ 0xFFFFFFFF) + 1;

    return make_tuple(quotient, remainder); // возвращаем кортеж из 2 значений



}

int main(){
    int a, b;
    int sign;
    cout << "=============INPUT=============\n";
    cout << "Divident: "; cin >> a;
    cout << "Divisior: "; cin >> b;
    cout << endl;
    
    auto [quotient, remainder] = division(a,b);
    printf("\n=============RESULT============\n");
    printf("Quotinent: %d\nRemainder: %d\n", quotient, remainder);
    printf("Full equation: %d = %d * %d + %d", a, b, quotient, remainder);
   

 
    return 0;

}



