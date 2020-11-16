#include <iostream>
#include<vector>
#include<string>
#include<thread>
#include<mutex>
#include<fstream>
#include<algorithm>

using namespace std;
mutex my_mutex;
//Генерация массива
vector<int> generate_array(int size) {
    vector<int> v1(size);
    for (size_t i = 0; i < v1.size(); i++)
    {
        v1[i] = 1 + rand();
    }
    return v1;
}
// Проверка на простое число
void is_prime(int n, bool& flag) {
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            flag = false;
            return;
        }
    }
    flag = true;
}
// Возможно ли записать значение в int
bool try_parse(string n) {
    try
    {
        int a = stoi(n);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
// Ознакомительные сообщения и получение данных от пользователя 
int  introduce(int& thr_count) {
    std::cout << "Karabash Radimir BSE195\n\n";
    std::cout << "Variant 13. Determine the set of indices i for which (A [i] - B [i]) or (A [i] + B [i]) are prime numbers.\nInput data: arrays of positive integers A and B, arbitrary length must be equal or more than 1000. \nThe number of streams is an input parameter.\n";
    cout << "Input number of elements in array: ";
    string n;
    cin >> n;
    int size;
    if (try_parse(n))
    {
        size = stoi(n);
        if (size < 1000)
        {
            throw new exception("Size must be more or equal 1000!\n");
        }
    }
    else
    {
        throw new exception("Invalid size of array!\n");
    }
    cout << "Input number of threads: ";
    n = "";
    cin >> n;
    if (try_parse(n))
    {
        thr_count = stoi(n);
        if (thr_count < 1 || thr_count > 10)
        {
            throw new exception("Quantity of threads must be more or equal than 1 and less than 11!\n");
        }
    }
    else
    {
        throw new exception("Invalid quantity of threads!\n");
    }
    return size;
}
// Логирование входных и выходных данных
void log(vector<int> a, vector<int> b) {
    fstream file("../input.txt", fstream::out);
    file << "Array A: ";
    for (size_t i = 0; i < a.size(); i++)
    {
        file << to_string(a[i]) + " ";
    }
    file << "\n\nArray B: ";
    for (size_t i = 0; i < b.size(); i++)
    {
        file << to_string(b[i]) + " ";
    }
}
void log(vector<int> a) {
    fstream file("../output.txt", fstream::out);
    file << "Indexes: ";
    for (size_t i = 0; i < a.size(); i++)
    {
        file << to_string(a[i]) + " ";
    }
}
// Получение индексов простых чисел
void get_indexes(int left_border, int right_border, vector<int> a, vector<int> b, vector<int>& indexes) {
    my_mutex.lock();
    bool left, right;
    for (size_t i = left_border; i < right_border; i++)
    {
        is_prime(abs(a[i] - b[i]), left);
        is_prime(abs(a[i] + b[i]), right);
        if (left || right)
        {
            indexes.push_back(i);
        }
    }
    my_mutex.unlock();
}
int main()
{
    try {
        int thr_count;
        int size = introduce(thr_count);
        vector<int> a = generate_array(size);
        vector<int> b = generate_array(size);
        log(a, b);
        vector<thread> threads;
        vector<int> indexes;
        int det = size / thr_count;
        get_indexes(0, det, a, b, indexes);
        for (size_t i = 1; i < thr_count; i++)
        {
            if (i != thr_count - 1)
                threads.push_back(thread(get_indexes, det * i, det * (i + 1), a, b, ref(indexes)));
            else
                threads.push_back(thread(get_indexes, det * i, det * (i + 1) + size % thr_count, a, b, ref(indexes)));
        }
        for (size_t i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
        sort(indexes.begin(), indexes.end());
        log(indexes);
        for (size_t i = 0; i < indexes.size(); i++)
        {
            cout << indexes[i] << endl;
        }
        cout << "Logging of input and output data is implemented to check the correctness of the algorithm! (files input.txt and output.txt)";
    }
    catch (exception* e) {
        cout << "Error occured: " << (*e).what() << endl;
    }
}
