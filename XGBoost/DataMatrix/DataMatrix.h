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
    
    // CSV to DataMatrix
    static DataMatrix from_csv(string filepath, const char deliminator = ',');
    static DataMatrix from_csv(string filepath, unsigned int num_columns, const char deliminator = ',');
    static DataMatrix from_csv(string filepath, const vector<string>& column_names, const char deliminator = ',');

    void add_row(vector<double>& row);
    void insert_row(vector<double>& row, int at);

    void add_column(const string& label, vector<double>&& column);
    void add_columns(vector<string>& labels, vector<vector<double>>&& columns);
    void add_column(vector<double>&& column);
    void add_columns(vector<vector<double>>&& columns);


    void insert_column(const string& label, vector<double>&& column, int at);
    void insert_column(vector<double>&& column, int at);

    void remove_column(const string& label);
    void remove_column(int index);

    vector<double>& operator[](const string& label);
    vector<double>& operator[](int index);

    int num_rows() const;
    int num_cols() const;
    vector<string> columns() const;

    string head(int num_rows = 10) const;
};



#endif