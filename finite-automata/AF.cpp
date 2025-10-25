#include "AF.h"

bool AF::isDeterministic() const {
    for (const auto& [key, value] : transitionFunction) {
        if (value.size() > 1) {
            return false;
        }
    }

	return true;
}

optional<string> AF::deltaFunction(string state, char c) const {
    auto it = transitionFunction.find({ state, c });

    if (it != transitionFunction.end() && !it->second.empty()) {
        return *(it->second.begin());
    }

	return nullopt;
}

AF::AF(const set<string>& states, const set<char>& alphabet, const string& initialState, const set<string>& finalStates, const map<pair<string, char>, set<string>>& transitionFunction)
    : states{ states }, alphabet{ alphabet }, initialState{ initialState }, finalStates{ finalStates }, transitionFunction{ transitionFunction } {}

const set<string>& AF::getStates() const {
	return states;
}

const set<char>& AF::getAlphabet() const {
	return alphabet;
}

const string& AF::getInitialState() const {
	return initialState;
}

const set<string>& AF::getFinalStates() const {
	return finalStates;
}

const map<pair<string, char>, set<string>>& AF::getTransitionFunction() const {
	return transitionFunction;
}

bool AF::acceptAFD(const string& input) const {
    if (!isDeterministic()) {
        throw std::logic_error("The automaton is not deterministic.");
    }

    string currentState = initialState;

    for (char c : input) {
        optional<string> nextState = deltaFunction(currentState, c);

        if (!nextState.has_value()) {
            return false;
        }

        currentState = nextState.value();
    }

	return finalStates.find(currentState) != finalStates.end();
}

string AF::longestAcceptedPrefixAFD(const string& input) const {
    if (!isDeterministic()) {
        throw std::logic_error("The automaton is not deterministic.");
    }

    string currentState = initialState;
    size_t lastAcceptedIndex = -1;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        optional<string> nextState = deltaFunction(currentState, c);

        if (!nextState.has_value()) {
            break;
        }

        currentState = nextState.value();

        if (finalStates.find(currentState) != finalStates.end()) {
            lastAcceptedIndex = i;
        }
    }

    if (lastAcceptedIndex == -1) {
        return "";
    }

	return input.substr(0, lastAcceptedIndex + 1);
}
