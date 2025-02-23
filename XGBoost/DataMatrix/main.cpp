#include "DataMatrix.h"

#include <iostream>
#include <vector>
#include <list>
#include <set>


int main() {

    // Test data
    vector<vector<double>> data = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    // Test with vector of strings
    vector<string> features = {"feature1", "feature2", "feature3"};
    DataMatrix matrix(features, data);


    cout << "Constructions successful" << endl;

    return 0;
}