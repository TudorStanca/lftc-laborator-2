#include <iostream>
#include <fstream>
#include <sstream>
#include "AF.h"

using namespace std;

AF loadFromFile(const string& path) {
	ifstream fin(path);

	if (!fin.is_open()) {
		throw new runtime_error("Could not open file: " + path);
	}

	set<string> states, finalStates;
	set<char> alphabet;
	map<pair<string, char>, set<string>> transitionFunction;
	int nStates, nSymbols, nFinalStates, nTransitions;
	string buffer, initialState;

	fin >> nStates;

	for (int i = 0; i < nStates; ++i) {
		fin >> buffer;
		states.insert(buffer);
	}

	fin >> nSymbols;

	for (int i = 0 ; i < nSymbols; ++i) {
		fin >> buffer;
		if (buffer.size() != 1) {
			throw new runtime_error("Invalid character in file: " + buffer);
		}

		alphabet.insert(buffer[0]);
	}
	
	fin >> initialState;
	states.insert(initialState);

	fin >> nFinalStates;

	for (int i = 0; i < nFinalStates; ++i) {
		fin >> buffer;
		finalStates.insert(buffer);
		states.insert(buffer);
	}

	fin >> nTransitions;

	for (int i = 0; i < nTransitions; ++i) {
		string fromState, toState;
		char symbol;
		fin >> fromState >> symbol >> toState;
		transitionFunction[{fromState, symbol}].insert(toState);
		states.insert(fromState);
		states.insert(toState);
		alphabet.insert(symbol);
	}

	return AF{ states, alphabet, initialState, finalStates, transitionFunction };
}

AF readFromConsole() {
	set<string> states, finalStates;
	set<char> alphabet;
	map<pair<string, char>, set<string>> transitionFunction;
	string initialState;

	cout << "Introduceti starile (ex: q0, q1, q2):\n";

	string line; 
	
	getline(cin, line);
	istringstream iss(line);
	string q;

	while (iss >> q) {
		states.insert(q);
	}

	cout << "Introduceti alfabetul (caractere separate prin spatiu, ex: 0 1 2 x a b):\n";

	getline(cin, line);
	istringstream iss2(line);
	string s;

	while (iss2 >> s) {
		if (s.size() == 1) {
			alphabet.insert(s[0]);
		}
	}

	cout << "Starea initiala:\n";
	getline(cin, line);
	initialState = line;
	states.insert(initialState);

	cout << "Stari finale (ex: q0, q1, q2):\n";

	getline(cin, line);
	istringstream iss3(line);

	while (iss3 >> q) {
		finalStates.insert(q);
		states.insert(q);
	}

	cout << "Introduceti tranzitii (format: from simbol to), o linie goala = stop\n";

	while (true) {
		cout << "tranzitie> ";

		getline(cin, line);

		if (line.empty()) {
			break;
		}

		istringstream it(line);
		string fromState, toState, symbol;

		if (!(it >> fromState >> symbol >> toState)) {
			cout << "Format invalid, incercati din nou.\n";
			continue;
		}

		if (symbol.size() != 1) {
			cout << "Simbol invalid (trebuie un singur caracter)\n";
			continue;
		}

		transitionFunction[{fromState, symbol[0]}].insert(toState);
		states.insert(fromState);
		states.insert(toState);
		alphabet.insert(symbol[0]);
	}

	return AF{ states, alphabet, initialState, finalStates, transitionFunction };
}

void menu() {
	cout << "\nMeniu:\n"
		<< "1) Afiseaza automat\n"
		<< "2) Verifica daca o secventa este acceptata (AFD)\n"
		<< "3) Determina cel mai lung prefix acceptat (AFD)\n"
		<< "4) Iesire\n"
		<< "Alege optiunea: ";
}

int main() {
	cout << "Automat finit - program\n";
	cout << "Citire din fisier? (y/n): ";

	AF A;
	string mode;
	getline(cin, mode);

	if (!mode.empty() && (mode[0] == 'y' || mode[0] == 'Y')) {
		cout << "Cale fisier: ";

		string path;
		getline(cin, path);

		A = loadFromFile(path);
	}
	else {
		A = readFromConsole();
	}

	while (true) {
		menu();

		string choice;
		getline(cin, choice);

		if (choice == "1") {
			cout << "Multimea starilor: ";
			for (const auto& q : A.getStates()) {
				cout << q << " ";
			}

			cout << "\nAlfabet: ";
			for (const auto& c : A.getAlphabet()) {
				cout << c << " ";
			}

			cout << "\nInitial: " << A.getInitialState() << "\nFinale: ";

			for (const auto& f : A.getFinalStates()) {
				cout << f << " ";
			}

			cout << "\nTranzitii:\n";
			for (const auto& p : A.getTransitionFunction()) {
				const auto& from = p.first.first;
				const auto& sym = p.first.second;

				for (const auto& to : p.second) {
					cout << "  " << from << " -- '" << sym << "' --> " << to << "\n";
				}
			}
		}
		else if (choice == "2") {
			cout << "Introduceti secventa (fara spatii): ";

			string seq; 
			getline(cin, seq);

			if (A.acceptAFD(seq)) {
				cout << "Secventa ACCEPTATA\n";

				continue;
			}
			else cout << "Secventa RESPINSA\n";
		}
		else if (choice == "3") {
			cout << "Introduceti secventa (fara spatii): ";

			string seq; getline(cin, seq);
			string pref = A.longestAcceptedPrefixAFD(seq);

			if (A.getFinalStates().find(A.getInitialState()) != A.getFinalStates().end() && pref.empty()) {
				cout << "Cel mai lung prefix acceptat: (string gol)\n";

				continue;
			}

			if (pref.empty()) {
				cout << "Niciun prefix acceptat (string gol)\n";

				continue;
			}

			cout << "Cel mai lung prefix acceptat: '" << pref << "'\n";
		}
		else if (choice == "4") {
			break;
		}
		else {
			cout << "Optiune invalida.\n";
		}
	}

	return 0;
}