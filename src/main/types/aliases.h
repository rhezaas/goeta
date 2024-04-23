#ifndef ALIASES_H
#define ALIASES_H

#include "Eigen/Dense"
#include "csvparser.hpp"


typedef Eigen::MatrixXd     Node;
typedef Eigen::MatrixXd     Link;

typedef csv::CSVReader      CSVReader;
typedef csv::CSVRow         CSVRow;
typedef csv::CSVField       CSVColumn;

#endif