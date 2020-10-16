#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include "types.h"

class DataHandler {
	public:
		DataHandler();
		DataHandler(char* train_set_file, char* test_set_file);

		void load_train_set(char* train_set_file);
		Matrix& train_data();
		Matrix& train_label();

		void load_test_set(char* test_set_file);
		Matrix& test_data();

		Vector& classif();
		void export_classif(char* out_file);

	private:
		Matrix _train_data;
		Matrix _train_label;

		Matrix _test_data;

		Vector _classif;
};

#endif /* DATA_HANDLER_H */