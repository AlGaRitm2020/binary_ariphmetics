#include <iostream>
#include <bitset>
#include <cmath>
#include <string>
#include <tuple>
#include <fstream>



using namespace std;


bool is_bit(char c) 
    {return (c == '1' || c == '0');} 


string make_pcnf(string f_arr[128], int n) {
    string pcnf = "";
    bool started = false;

    for (int i = 0; i < 1<<n ; i++) {
        if (f_arr[i] == "0") {
            if (started)
                pcnf += " * (";
            else 
                pcnf += "(";

            for (int j = n; j > 0; j--) {
                if ((i / (1<<(j-1))) % 2 ) {
                    pcnf += "/x" + to_string(n-j+1);
                }
                else {
                    pcnf += "x" + to_string(n-j+1);
                }
                started = true; 
                if (j!= 1) 
                    pcnf += "+";
            }
            pcnf += ")";
        }
    }
    return pcnf;
}

string make_pdnf(string f_arr[128], int n) {
    string pdnf = "";
    bool started = false;

    for (int i = 0; i < 1<<n ; i++) {
        if (f_arr[i] == "1") {
            if (started)
                pdnf += " + (";
            else 
                pdnf += "(";
                
            for (int j = n; j > 0; j--) {
                if ((i / (1<<(j-1))) % 2 ) {
                    pdnf += "x" + to_string(n-j+1);
                }
                else {
                    pdnf += "/x" + to_string(n-j+1);
                }
                started = true;
                if (j!= 1) 
                    pdnf += "*";
            }
            pdnf += ")";
        }
    }
    return pdnf;
}

int main() {

    std::ifstream myfile;
    myfile.open("table.txt");

    // Check if the file is successfully opened 
    if (!myfile.is_open()) { 
        cerr << "Error opening the file!" << endl; 
        return 1; 
    }

    int n=0;           // число аргументов
    int f_index = -1; // номер символа в котором находится F в каждой строке
    string f_arr[128]; // maximum strings set as 128

    std::string line;
    int i;
    for (i = 0; getline(myfile, line); i++) { 
        cout << line << endl; 

        if (i == 2) // подсчет числа аргументов и обнаружение индекса значения F
            for (int j  = 0; j < line.length(); j++){
                if (is_bit(line[j])) {
                    n++;
                    if (line[j + 2] == '%'){
                        f_index = j;
                        f_arr[i-2] = line[j];
                        n--;
                        break;
                    }

                }
        }

        if (i > 2) {
            f_arr[i-2] = line[f_index];
        }
    } 
    f_arr[i+1] = '%';


  
    // Close the file 
    myfile.close(); 

    cout << "N of args: " << n << endl;
    cout << "F index: " << f_index << endl;

    cout << "Array of F:\t";
    for (int i = 0; f_arr[i] != "%"; i++ )
        cout << f_arr[i] << '\t';
    cout << endl;

    cout << "/x1 ЗНАЧИТ ОТРИЦАНИЕ x1" << endl << endl;

    string pcnf, pdnf;
    
    pcnf = make_pcnf(f_arr, n);
    pdnf = make_pdnf(f_arr, n);
    cout << "СКНФ:\t" << pcnf << endl;
    cout << "СДНФ:\t" << pdnf << endl;


    
    // полная форма сднф для                                n = 3:  x1x2x3 + x1x2/x3 + x1/x2x3 + x1/x2/x3 + /x1x2x3 + /x1x2/x3 + /x1/x3x3 + /x1/x2/x3, где / - отрицание одного аргумента, напрмер /x2 - отрицание x2
    // запись сднф в виде массива (случайные значения)      a[n^3] = {0,       1,          0,         1,       0,      1,              1,       0    ,   -1, .....}  , где -1 сигнал остановки з
    //                                                                0         1           2           3      4        5               6         7                 



 
    return 0;

}



