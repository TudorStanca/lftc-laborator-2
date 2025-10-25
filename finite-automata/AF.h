#pragma once
#include <set>
#include <map>
#include <string>
#include <optional>
#include <stdexcept>

using std::set;
using std::map;
using std::pair;
using std::string;
using std::optional;
using std::logic_error;
using std::nullopt;

class AF {

private:
	set<string> states;
	set<char> alphabet;
	string initialState;
	set<string> finalStates;
	map<pair<string, char>, set<string>> transitionFunction;

	bool isDeterministic() const;

	optional<string> deltaFunction(string state, char c) const;
	
public:
	AF() {}

	AF(const set<string>& states,
		const set<char>& alphabet,
		const string& initialState,
		const set<string>& finalStates,
		const map<pair<string, char>, set<string>>& transitionFunction);

	const set<string>& getStates() const;

	const set<char>& getAlphabet() const;

	const string& getInitialState() const;

	const set<string>& getFinalStates() const;

	const map<pair<string, char>, set<string>>& getTransitionFunction() const;

	bool acceptAFD(const string& input) const;

	string longestAcceptedPrefixAFD(const string& input) const;
};

