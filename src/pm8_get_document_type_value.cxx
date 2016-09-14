#include <iostream>
//#include <stdio.h>
#include <string.h>
//#include <cstring>
#include <stdarg.h>
//#include <iterator>
//#include <vector>

#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <tc/preferences.h>
#include <ug_va_copy.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include "misc.hxx"
#include "process_error.hxx"
#include "pm8_get_document_type_value.hxx"

using namespace std;

//int getDocumentType(vector<char*>, string);

int pm8_get_document_type_value(METHOD_message_t *mess, va_list args) {
	try{
		va_list vargs;
		va_copy(vargs, args);
		char **rvalue = va_arg(vargs, char**);
		va_end(vargs);

		ResultCheck erc;
		char* result = "result";
		char* item_id;
		//string item_id_short;
		//std::vector<char*> shortTypes;
		//std::vector<char*> longTypes;

		/*cout << "Asking object desc" << endl;
		erc = AOM_ask_value_string(mess->object_tag, "object_desc", &item_id);
		cout << item_id << endl;

		cout << "Asking preference" << endl;
		int n_values = 0;
		char **values = NULL;
		erc = PREF_ask_char_values("Pm8_DocumentTypes", &n_values, &values);
		cout << "Printing preference" << endl;
		printf("\t n_values: %d \n", n_values);

		if(n_values>0){
			cout << "Splitting" << endl;
			char* splitedString;
			for(int ii = 0; ii < n_values; ii++){
				splitedString = strtok(values[ii], "=");
				shortTypes.push_back(splitedString);
				splitedString = strtok (NULL, "=");
				longTypes.push_back(splitedString);
			}
			for (int jj = 0; jj < n_values; jj++){
				cout << shortTypes[jj] << " " << longTypes[jj] << endl;
			}
		} else {
			cout << "Nothing found" << endl;
			MEM_free(values);
			values=NULL;
			return ITK_ok;
		}

		string item_id_string(item_id);
		size_t posOfSpace = item_id_string.find_last_of(" ");
		if(posOfSpace!=string::npos){
			item_id_short = item_id_string.substr(posOfSpace + 1, item_id_string.length());
			cout << item_id_short << endl;
			int pos = getDocumentType(shortTypes, item_id_short);
			if(pos!=-1){
				result = (char*) MEM_alloc(strlen(longTypes.at(pos))+1);
				strcpy(result, longTypes.at(pos));
				cout << longTypes.at(pos) << endl;
			}
		}

		MEM_free(values);
		values=NULL;

		cout << "5" << endl;*/
		if(result){
			cout << result << endl;
			cout << strlen(result) << endl;
			*rvalue = (char*) MEM_alloc(strlen(result)+1);
			cout << "6" << endl;
			strcpy(*rvalue, result);
			//MEM_free(result);
		}
	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}

/*int getDocumentType(vector<char*> typesShort, string idShort){
	int result = -1;
	int pos = 0;
	for (std::vector<char*>::iterator it = typesShort.begin() ; it != typesShort.end(); ++it){
		if(strcmp(*it, idShort.c_str())==0){
			return pos;
		}
		pos++;
	}
	return result;
}*/
