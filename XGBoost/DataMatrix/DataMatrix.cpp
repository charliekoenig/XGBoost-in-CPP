#include "DataMatrix.h"

#include <fstream> 
#include <sstream> 
#include <iomanip>
#include <iostream>
#include <format>
#include <string>
#include <set>

#define MAX_CSV_COLUMNS 100

using namespace std;

DataMatrix::DataMatrix(const vector<string>& features, vector<vector<double>>& data) 
{

    if (data.empty() || features.empty()) return;
    if (features.size() != data[0].size()) {
        throw invalid_argument("Feature size does not match data size");
    }
    
    for (int col = 0; col < features.size(); col++) {
        vector<double> column;
        column.reserve(data.size());

        for (const vector<double>& row : data) {
            column.push_back(row[col]);
        }

        add_column(features[col], std::move(column));
    }
}



DataMatrix::DataMatrix(const vector<string>& features) 
{
    if (features.empty()) return;

    int feature_id = 0;
    for (const string& feature : features) {
        this->feature_map[feature] = feature_id++;
    }
}


DataMatrix DataMatrix::from_csv(string filepath, const char deliminator)
{
    ifstream csv_data;
    string line, value, label;
    DataMatrix matrix;

    csv_data.open(filepath);

    vector<string> labels;
    vector<vector<double>> columns;

    // Get header columns
    getline(csv_data, line);
    stringstream ss(line);
    while (getline(ss, label, deliminator)) {
        labels.push_back(label);
        columns.emplace_back();
    }

    while (getline(csv_data, line)) {
        stringstream ss(line);
        int col_index = 0;

        while (getline(ss, value, deliminator)) {
            columns[col_index].push_back(stod(value));
            col_index += 1;
        }
    }

    matrix.add_columns(labels, std::move(columns));

    return matrix;
}

DataMatrix DataMatrix::from_csv(string filepath, unsigned int num_columns, const char deliminator)
{
    ifstream csv_data;
    string line, value, label;
    DataMatrix matrix;
    int line_len {0};

    csv_data.open(filepath);

    vector<string> labels;
    vector<vector<double>> columns;

    // Read first line (header columns)
    getline(csv_data, line);
    stringstream ss(line);
    while (getline(ss, label, deliminator) && (line_len < num_columns)) {
        labels.push_back(label);
        columns.emplace_back();
        line_len += 1;
    }

    if (labels.size() != num_columns) {
        stringstream error;
        error << "Too few columns in CSV file. Expected " << num_columns << " columns but found " << labels.size() << endl;
        throw invalid_argument(error.str());
    }

    while (getline(csv_data, line)) {
        stringstream ss(line);
        int col_index = 0;

        while (getline(ss, value, deliminator) && (col_index < num_columns)) {
            columns[col_index].push_back(stod(value));
            col_index += 1;
        }
    }

    matrix.add_columns(labels, std::move(columns));

    return matrix;
}


DataMatrix DataMatrix::from_csv(string filepath, const vector<string>& column_names, const char deliminator)
{
    ifstream csv_data;
    string line, value, label;
    DataMatrix matrix;

    int index {-1};
    set<int> indicies {};

    csv_data.open(filepath);

    vector<string> labels;
    vector<vector<double>> columns;

    getline(csv_data, line);
    stringstream ss(line);
    while (getline(ss, label, deliminator)) {
        index += 1;
        if (find(column_names.begin(), column_names.end(), label) == column_names.end()) {
            continue;
        }
        labels.push_back(label);
        columns.emplace_back();
        indicies.insert(index);
    }

    if (labels.size() != column_names.size()) {
        throw invalid_argument("Unable to find specified columns in csv file");
    }

    while (getline(csv_data, line)) {
        stringstream ss(line);
        int col_index = -1;
        int curr_col = 0;

        while (getline(ss, value, deliminator)) {
            col_index += 1;
            if (indicies.find(col_index) == indicies.end()) {
                continue;
            }

            columns[curr_col].push_back(stod(value));
            curr_col += 1;
        }
    }

    matrix.add_columns(labels, std::move(columns));

    return matrix;
}


void DataMatrix::add_row(vector<double>& row)
{
    if (row.size() != this->num_cols()) {
        throw invalid_argument("Row size does not match matrix width");
    }

    int index = 0;
    for (vector<double>& column : this->data) {
        column.push_back(row[index++]);
    }
}

void DataMatrix::insert_row(vector<double>& row, int at)
{
    if (row.size() != this->num_cols()) {
        throw invalid_argument("Row size does not match matrix width");
    }

    if (at < 0 || at > num_rows()) {
        throw invalid_argument("Invalid row insertion position");
    }
    
    int index = 0;
    for (vector<double>& column : this->data) {
        column.insert(column.begin() + at, row[index++]);
    }
}


void DataMatrix::add_column(const string& label, vector<double>&& column)
{

    if (!data.empty() && column.size() != this->num_rows()) {
        throw invalid_argument("Column size does not match matrix length");
    }

    feature_map[label] = data.size();
    data.push_back(std::move(column));
} 


void DataMatrix::add_column(vector<double>&& column)
{

    if (!data.empty() && column.size() != this->num_rows()) {
        throw invalid_argument("Column size does not match matrix length");
    }

    data.push_back(std::move(column));
} 

void DataMatrix::add_columns(vector<string>& labels, vector<vector<double>>&& columns)
{
    if (labels.size() != columns.size()) {
        throw invalid_argument("Number of columnd does not match number of labels");
    }

    for (int i = 0; i < labels.size(); ++i) {
        this->add_column(labels[i], std::move(columns[i]));
    }
} 

void DataMatrix::add_columns(vector<vector<double>>&& columns)
{    
    for (vector<double>& column : columns) {
        this->add_column(std::move(column));
    }
} 

void DataMatrix::insert_column(const string& label, vector<double>&& column, int at)
{
    if (!data.empty() && column.size() != this->num_rows()) {
        throw invalid_argument("Column size does not match matrix length");
    }

    if (at < 0 || at > num_cols()) {
        throw invalid_argument("Invalid column insertion position");
    }

    data.insert(data.begin() + at, column);

    for (auto& [feature, id] : feature_map) {
        if (id >= at) {
            id += 1;
        } 
    }

    feature_map[label] = at;
}

void DataMatrix::insert_column(vector<double>&& column, int at)
{
    if (!data.empty() && column.size() != this->num_rows()) {
        throw invalid_argument("Column size does not match matrix length");
    }

    if (at < 0 || at > num_cols()) {
        throw invalid_argument("Invalid column insertion position");
    }

    data.insert(data.begin() + at, column);
}



void DataMatrix::remove_column(const string& label)
{
    int remove_index = feature_map[label];

    data.erase(data.begin() + remove_index);
    feature_map.erase(label);

    for (auto& [feature, index] : feature_map) {
        if (index > remove_index) {
            index -= 1;
        }
    }
}

void DataMatrix::remove_column(int index)
{

    data.erase(data.begin() + index);

    for (auto& [feature, id] : feature_map) {
        if (id > index) {
            id -= 1;
        } else if (id == index) {
            feature_map.erase(feature);
        }
    }
}

vector<double>& DataMatrix::operator[](const string& label)
{
    return data[feature_map[label]];
}


vector<double>& DataMatrix::operator[](int index)
{
    return data[index];
}


int DataMatrix::num_rows() const
{
    if (data.empty()) return 0;  
    return data[0].size();
}


int DataMatrix::num_cols() const
{
    return data.size();
}

vector<string> DataMatrix::columns() const 
{
    vector<string> columns;
    for (const auto& [feature, _] : feature_map) {
        columns.push_back(feature);
    }

    return columns;
}

string DataMatrix::head(int rows) const
{
    rows = min(rows, num_rows());

    stringstream head;

    for (const auto& [feature, index] : feature_map) {
        head << setw(12) << left << feature;
    }

    head << "\n";

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < num_cols(); col++) {
            head << setw(12) << fixed << setprecision(4) << data[col][row];
        }
        head << "\n";
    }

    return head.str();
}