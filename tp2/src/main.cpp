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

	/**
	 * Este valor queda hardcodeado, puede utlizarse
	 * normalmente mientras que k <= 1000. En caso contrario
	 * este valor que representa al k máximo de knn debe aumentarse.
	 */
	KNNClassifier kNN(100);

	switch (method) {

		case '0' :
			/* kNN */
			kNN.fit(dh.train_data(), dh.train_label());
			kNN.load(dh.test_data());
			dh.classif() = kNN.predict(k_nearest);
			break;

		case '1' :
			/* PCA + kNN */
			PCA pca(n_pca);
			pca.fit(dh.train_data());

			dh.train_data() = pca.transform(dh.train_data());
			dh.test_data() = pca.transform(dh.test_data());

			kNN.fit(dh.train_data(), dh.train_label());
			kNN.load(dh.test_data());
			dh.classif() = kNN.predict(k_nearest);
			break;

	}

	dh.export_classif(out_path);

	std::cout << "# Vecino mas cercanos: " << k_nearest << std::endl;
	if (method == '1')
		std::cout << "# Components principales: " << n_pca << std::endl;
	std::cout << "Tasta de reconocimiento: " << std::endl;

  	return 0;
}
