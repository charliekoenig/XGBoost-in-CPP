#ifndef DATA_MATRIX_H
#define DATA_MATRIX_H

#include <vector>
#include <string>
#include <list>
#include <map>

using namespace std;


class DataMatrix {

private:
    vector<vector<double>> data;
    map<string, int> feature_map;

public:

    // Constructors
    DataMatrix() = default;

    DataMatrix(const vector<string>& features, vector<vector<double>>& data);
    DataMatrix(const vector<string>& features);

    void add_row(vector<double>& row);

    void add_column(const string& feature_name, vector<double>&& column);
    // void add_column(vector<double>&& col);

    // void remove_column(const string& label);
    // void remove_column(int index);

    // vector<double>& operator[](const string& label);
    // vector<double>& operator[](int index);

    int num_rows() const;
    int num_cols() const;
    // vector<string> get_features() const;
};



#endif