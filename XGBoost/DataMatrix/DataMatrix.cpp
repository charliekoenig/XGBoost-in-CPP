#include "DataMatrix.h"

#include <vector>
#include <string>
#include <list>
#include <map>

using namespace std;

DataMatrix::DataMatrix(const vector<string>& features, vector<vector<double>>& data) 
{
    
    if (data.empty() || features.empty()) return;
    
    for (int col = 0; col < features.size(); col++) {
        vector<double> column;
        column.reserve(data.size());

        for (const vector<double>& row : data) {
            column.push_back(row[col]);
        }

        add_column(features[col], move(column));
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


void DataMatrix::add_column(const string& feature_name, vector<double>&& column) {
    if (!data.empty() && column.size() != this->num_rows()) {
        throw invalid_argument("Column size does not match matrix length");
    }

    feature_map[feature_name] = data.size();
    data.push_back(move(column));
} 


int DataMatrix::num_rows() const {
    if (data.empty()) return 0;  
    return data[0].size();
}


int DataMatrix::num_cols() const {
    return data.size();
}