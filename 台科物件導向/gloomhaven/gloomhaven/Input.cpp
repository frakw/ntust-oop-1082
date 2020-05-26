#include "Gloom_Haven.h"
#include "Input.h"
using namespace std;

int character_amount_input() {
	string input;
	getline(cin, input);
	regex reg(R"(^[+]?\d+$)");
	while(!regex_match(input, reg)) {
		cout << err << endl;
		getline(cin, input);
	}
	return atoi(input.c_str());
}
void character_data_input(Character& life, Character* db, int total_chr, char code) {//brute 0 1 2 3 4 5
	string number;
	int db_index = -1;
	string input;
name_input:
	db_index = -1;
	getline(cin, input);
	stringstream ss(input);
	ss >> life.name;
	for (int i = 0;i < total_chr;i++) {
		if (db[i].name == life.name) {
			db_index = i;
			break;
		}
	}
	if (db_index == -1) {
		cout << "can't find this name in character database" << endl;
		goto name_input;
	}
	int count = 0;
	while (!ss.eof()) {
		ss >> number;
		if (is_num(number)) {
			count++;
		}
		else {
			cout << err << endl;
			goto name_input;
		}
	}
	if (count != db[db_index].card_amount) {
		cout << err << endl;
		goto name_input;
	}
	//完成檢查，開始存入
	stringstream final(input);//忘了讀名字
	final >> life.name;//忘了讀名字
	life = db[db_index];
	life.code = code;
	for (int i = 0;i < life.card_amount;i++) {
		int num;
		final >> num;

		int m;

		for (m = 0; m < db[db_index].card_total;m++) {
			if (num == db[db_index].card[m].number) {
				life.card[i] = db[db_index].card[m];
				break;
			}
		}
		if (m == db[db_index].card_total) {
			cout << err << endl;
			goto name_input;
		}
	}
}

bool is_num(string input) {
	regex reg(R"([+-]?\d+$)");
	return regex_match(input,reg);
}

void open_file(fstream& file, string filename) {//map1.txt
	file.open(filename, ios::in);
	while(!file.is_open()) {
		cout <<'"'<<filename<<'"'<<" file open error please input file path again:";
		getline(cin, filename);
		file.open(filename, ios::in);
	}
}

string wasd() {
	regex reg(R"(^[wasd]+$)");
	string input;
	getline(cin, input);
	while(!regex_match(input, reg) && input != "e") {
		cout << err << endl;
		getline(cin, input);
	}
	return input;
}
string character_card_choose() {//A 0 3 B -1，bool回傳false代表check或輸入錯誤，重新輸入
	regex reg(R"(^[A-Z] [+-]?[0-9]+ [+-]?[0-9]+$)");
	regex sleep(R"(^[A-Z] -1)");
	regex check(R"(^[A-Z] check$)");
	string input;
	getline(cin, input);
	while (!regex_match(input, reg) && !regex_match(input,sleep) && !regex_match(input, check)) {
		cout << err << endl;
		getline(cin, input);
	}
	return input;
}
string character_card_first_ud() {//角色輸入第一張使用的牌與上半部或下半部(2u)
	regex reg(R"(^[+-]?\d+[ud]$)");
	string input;
	getline(cin, input);
	while (!regex_match(input, reg) && input!="check") {
		cout << err << endl;
		getline(cin, input);
	}
	return input;
}

int getline_int() {
	regex int_check(R"([+-]?[0-9]*)");
	string in;
	getline(cin, in);
	while (!regex_match(in, int_check)) {
		cout << err << endl;
		getline(cin, in);
	}
	return atoi(in.c_str());
}

char getline_char() {
	string ch;
	getline(cin, ch);
	while (ch.length() != 1) {
		cout << err << endl;
		getline(cin, ch);
	}
	return ch[0];
}