#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <locale>

using namespace std;

class Array {

protected:
    int* data;
    int size;

public:
    Array(int arraySize) {
        size = arraySize;
        data = new int[size];
        for (int i = 0; i < size; i += 1) {
            data[i] = 0;
        }
    }

    Array(const Array& other) {
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; i += 1) {
            data[i] = other.data[i];
        }
    }

    virtual ~Array() { 
        delete[] data;
    }

    virtual void write_to_file() const = 0; 

    void print() const {
        cout << "Массив: ";
        for (int i = 0; i < size; i += 1) {
            cout << data[i];
            if (i < size - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    void setValue(int index, int value) {
        if (value >= -100 && value <= 100 && index >= 0 && index < size) {
            data[index] = value;
        }
    }

    int getValue(int index) const {
        if (index >= 0 && index < size) {
            return data[index];
        }
        return 0;
    }

    void to_the_end(int value) {
        if (value < -100 || value > 100) {
            return;
        }

        int* newData = new int[size + 1];
        for (int i = 0; i < size; i += 1) {
            newData[i] = data[i];
        }
        newData[size] = value;
        delete[] data;
        data = newData;
        size += 1;
    }

    Array& add(const Array& other) {
        int minSize = (size < other.size) ? size : other.size;
        for (int i = 0; i < minSize; i += 1) {
            data[i] += other.data[i];
        }
        return *this;
    }

    Array& subtract(const Array& other) {
        int minSize = (size < other.size) ? size : other.size;
        for (int i = 0; i < minSize; i += 1) {
            data[i] -= other.data[i];
        }
        return *this;
    }

    int getSize() const {
        return size;
    }

protected:
    const int* getData() const {
        return data;
    }

    string generateFileName(const string& extension) const {
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);

        tm bt{};
#ifdef _MSC_VER 
        gmtime_s(&bt, &in_time_t);
#else

        bt = *std::gmtime(&in_time_t);
#endif

        stringstream ss;
        ss << put_time(&bt, "%Y%m%d_%H%M%S") << "." << extension;
        return ss.str();
    }
};

class ArrTxt : public Array {
public:
    using Array::Array; 

    void write_to_file() const override {
        string filename = generateFileName("txt");
        ofstream file(filename);

        if (file.is_open()) {
            file << "Содержимое массива (TXT):\n";
            for (int i = 0; i < getSize(); i += 1) {
                file << getData()[i];
                if (i < getSize() - 1) {
                    file << ", ";
                }
            }
            file << "\n";
            file.close();
            cout << "Массив успешно записан в файл: " << filename << endl;
        }
        else {
            cerr << "Ошибка: Не удалось открыть файл для записи: " << filename << endl;
        }
    }
};

class ArrCSV : public Array {
public:
    using Array::Array; 

    void write_to_file() const override {
        string filename = generateFileName("csv");
        ofstream file(filename);

        if (file.is_open()) {
            file << "Индекс,Значение\n"; 
            for (int i = 0; i < getSize(); i += 1) {
                file << i << "," << getData()[i] << "\n";
            }
            file.close();
            cout << "Массив успешно записан в файл: " << filename << endl;
        }
        else {
            cerr << "Ошибка: Не удалось открыть файл для записи: " << filename << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian"); 

    ArrTxt arrayTxt(5);
    arrayTxt.setValue(0, 10);
    arrayTxt.setValue(1, 20);
    arrayTxt.to_the_end(30);

    ArrCSV arrayCsv(4);
    arrayCsv.setValue(0, 50);
    arrayCsv.setValue(1, 45);
    arrayCsv.setValue(2, 60);

    Array* arrays[2];
    arrays[0] = &arrayTxt;
    arrays[1] = &arrayCsv;

    for (int i = 0; i < 2; i += 1) {
        arrays[i]->print();
    }

    for (int i = 0; i < 2; i += 1) {
        arrays[i]->write_to_file();
    }

    arrayTxt.subtract(arrayCsv);
    arrayTxt.print();
    arrayTxt.write_to_file();

    return 0;
}