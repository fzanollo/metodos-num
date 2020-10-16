//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include "knn.h"
#include "pca.h"
#include "data_handler.h"

int main(int argc, char** argv){
	
	if (argc < 8) {
		std::cerr << "Error: parametros insuficientes." << std::endl;
		return 1;
	}

	uint k_nearest = 1;
	uint n_pca = 784;

	char method = *argv[2];
	char* train_path = argv[4];
	char* test_path = argv[6];
	char* out_path = argv[8];

	DataHandler dh(train_path, test_path);

	KNNClassifier kNN(k_nearest);
	
	switch (method) {

		case '0' :
			/* kNN */
			kNN.fit(dh.train_data(), dh.train_label());

			dh.classif() = kNN.predict(dh.test_data());
			break;

		case '1' :
			/* PCA + kNN */
			PCA pca(n_pca);
			pca.fit(dh.train_data());

			dh.train_data() = pca.transform(dh.train_data());
			dh.test_data() = pca.transform(dh.test_data());

			kNN.fit(dh.train_data(), dh.train_label());

			dh.classif() = kNN.predict(dh.test_data());
			break;

	}

	dh.export_classif(out_path);

	std::cout << "# Vecino mas cercanos: " << k_nearest << std::endl;
	if (method == '1')
		std::cout << "# Components principales: " << n_pca << std::endl;
	std::cout << "Tasta de reconocimiento: " << std::endl;

  	return 0;
}
