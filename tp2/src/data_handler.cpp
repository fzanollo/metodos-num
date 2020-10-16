#include "data_handler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


DataHandler::DataHandler() : _train_data(), _train_label(), _test_data(), _classif() {}

DataHandler::DataHandler(char* train_set_file, char* test_set_file) {
	load_train_set(train_set_file);
	load_test_set(test_set_file);
}

void DataHandler::load_train_set(char* train_set_file) {
		
	std::ifstream  f_train(train_set_file);
	if (!f_train.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << train_set_file << ".\n";
    }

	Matrix data(0, 784);
	Matrix label(0, 1);

	std::string line, word;
	std::getline(f_train, line); // skipear el header
	
	while (std::getline(f_train, line)) {
		
		data.conservativeResize(data.rows()+1, data.cols());
		label.conservativeResize(label.rows()+1, label.cols());
        
        std::stringstream ss(line);
        std::getline(ss, word, ',');
        label(label.rows()-1, 0) = std::stod(word);

        for (int k = 0; k < data.cols(); k++) {
        	std::getline(ss, word, ',');
        	data(data.rows()-1, k) = std::stod(word);
        }
	}

	f_train.close();

    this->_train_data = data;
    this->_train_label = label;
}

Matrix& DataHandler::train_data() {
	return this->_train_data;
}

Matrix& DataHandler::train_label() {
	return this->_train_label;
}

void DataHandler::load_test_set(char* test_set_file){

	std::ifstream  f_test(test_set_file);
	if (!f_test.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << test_set_file << ".\n";
    }

	Matrix data(0, 784);

	std::string line, word;

	/* skipear el header */
	std::getline(f_test, line); 
	
	while (std::getline(f_test, line)) {
		
		data.conservativeResize(data.rows()+1, data.cols());
        
        std::stringstream ss(line);

        for (int k = 0; k < data.cols(); k++) {
        	std::getline(ss, word, ',');
        	data(data.rows()-1, k) = std::stod(word);
        }
	}

	f_test.close();

    this->_test_data = data;
}

Matrix& DataHandler::test_data() {
	return this->_test_data;
}

Vector& DataHandler::classif() {
	return this->_classif;
}

void DataHandler::export_classif(char* out_file) {

	std::ofstream f_out(out_file, std::ofstream::trunc);
	if (!f_out.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << out_file << ".\n";
    }

    std::string separator = ",";

    /* header */
    f_out << "ImageId" << separator << "Label";

    for (int k = 0; k < classif().rows(); k++) {
    	f_out << std::endl << (k+1) << separator << classif()(k);
    }

    f_out.close();
}