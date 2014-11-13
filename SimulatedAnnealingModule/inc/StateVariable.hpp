/**
 * @file StateVariable.hpp
 * @brief SearchSpace classes
 *
 * This file is created at "DoBots". It is open-source software and part of "AI".
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     Nov 5, 2014
 * @organisation             DoBots
 * @project                  AI
 */

#ifndef INC_STATEVARIABLE_HPP_
#define INC_STATEVARIABLE_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "json/json.h"

//namespace StateVariable {

enum StateVariableType {
	STATE_VAR_NONE=0,
	STATE_VAR_SET,
	STATE_VAR_INT,
	STATE_VAR_REAL
};

template <class T>
class StateVariable {
public:
	StateVariable(StateVariableType type) : _val(0), _type(type) {}
	virtual ~StateVariable() {}
	virtual const T get() const { return _val; }
	virtual void set(const T val) { _val = val; }
	StateVariableType type() { return _type; }
//	void type(StateVariableType type) { _type = type; }
	std::string& name() { return _name; }
	void name(std::string name) { _name = name; }

	friend std::ostream& operator<<(std::ostream& os, const StateVariable<T>& obj) {
		os << "name=" << obj._name << " type=" << obj._type << " val=" << obj._val;
		return os;
	}

protected:
	T _val;
	StateVariableType _type;
	std::string _name;
};


// Internally we'll store values of type T, but we expose the index of type size_t
template <class T>
class StateVariableSet : public StateVariable<size_t> {
public:
	StateVariableSet() : StateVariable<size_t>(STATE_VAR_SET) { }
	~StateVariableSet() {}
	void init(const std::vector<T>& set) { _set = set; }
	void set(const size_t val) {
		if (val >= size())
			return;
		_val = val;
	}
	size_t size() { return _set.size(); }
	const T& getSet(size_t index) { return _set[index]; }

	friend std::ostream& operator<<(std::ostream& os, const StateVariableSet<T>& obj) {
		os << static_cast<const StateVariable<size_t>& >(obj);
		if (obj._set.empty()) {
			os << " set=[]";
		}
		else {
			os << " set=[" << obj._set[0];
			for (size_t i=1; i<obj._set.size(); ++i) {
				os << ", " << obj._set[i];
			}
			os << "]";
		}
		return os;
	}

private:
	std::vector<T> _set;
};


template <class T>
class StateVariableInt : public StateVariable<T> {
public:
	StateVariableInt() : StateVariable<T>(STATE_VAR_INT) { }
	~StateVariableInt() {}
	void init(const T min, const T max) { _min = min; _max = max; }
	void set(const T val) {
		if (val > _max)
			StateVariable<T>::_val = _max;
		else if (val < _min)
			StateVariable<T>::_val = _min;
		else
			StateVariable<T>::_val = val;
	};
	const T min() { return _min; }
	const T max() { return _max; }

	friend std::ostream& operator<<(std::ostream& os, const StateVariableInt<T>& obj) {
		os << static_cast<const StateVariable<T>& >(obj);
		os << " min=" << obj._min << " max=" << obj._max;
		return os;
	}

private:
	T _min;
	T _max;
};


template <class T>
class StateVariableReal : public StateVariable<T> {
public:
	StateVariableReal() : StateVariable<T>(STATE_VAR_REAL) { }
	~StateVariableReal() {}
	void init(const T min, const T max) { _min = min; _max = max; }
	void set(const T val) {
		if (val > _max)
			StateVariable<T>::_val = _max;
		else if (val < _min)
			StateVariable<T>::_val = _min;
		else
			StateVariable<T>::_val = val;
	};
	const T min() { return _min; }
	const T max() { return _max; }

	friend std::ostream& operator<<(std::ostream& os, const StateVariableReal<T>& obj) {
		os << static_cast<const StateVariable<T>& >(obj);
		os << " min=" << obj._min << " max=" << obj._max;
		return os;
	}

private:
	T _min;
	T _max;
};


template <class ValType=double, class SetType=std::string, class IntType=int, class RealType=double>
class State {
public:
	State() { }
	~State() { }

	const ValType getStateVal(const size_t index) const {
		if (index < _setStates.size())
			return _setStates[index].get();
		else if (index < _setStates.size() + _intStates.size())
			return _intStates[index-_setStates.size()].get();
		else
			return _realStates[index-_intStates.size()-_setStates.size()].get();
	}
	void setStateVal(const size_t index, ValType val) {
		if (index < _setStates.size())
			_setStates[index].set(val);
		else
			_realStates[index-_setStates.size()].set(val);
	}
	// Read access only, use setStateVal to write
	const ValType operator[](const size_t index) const {
		return getStateVal(index);
	}

	// Helper function
	StateVariableType typeFromString(std::string typeStr) {
		StateVariableType type = STATE_VAR_NONE;
		if (!typeStr.compare("set"))
			type = STATE_VAR_SET;
		if (!typeStr.compare("int"))
			type = STATE_VAR_INT;
		if (!typeStr.compare("real"))
			type = STATE_VAR_REAL;
		return type;
	}

	// Helper function
	void readJson(const Json::Value& jsonStateVars) {
		for (Json::ArrayIndex i=0; i<jsonStateVars.size(); ++i) {
			std::cout << "Add state variable: " << jsonStateVars[i];

			// Parse type
			std::string typeStr = jsonStateVars[i].get("type","").asString();
			StateVariableType type = typeFromString(typeStr);

			switch (type) {
			case STATE_VAR_SET: {
				Json::Value jsonSet = jsonStateVars[i]["set"];
				if (jsonSet == Json::Value::null || jsonSet.empty()) {
					std::cout << "WARNING ignored state variable, since set values are not found or empty:" << jsonStateVars[i] << std::endl;
					break;
				}
				std::vector<SetType> set;
				for (Json::ArrayIndex j=0; j<jsonSet.size(); ++j) {
					set.push_back(jsonSet[j].asString());
				}
				StateVariableSet<SetType> stateVar;
				stateVar.init(set);
				stateVar.name() = jsonStateVars[i].get("name","").asString();
				addState(stateVar);
				std::cout << "Added: " << stateVar << std::endl;
				break;
			}
			case STATE_VAR_INT: {
				int min = jsonStateVars[i].get("min", 0).asInt();
				int max = jsonStateVars[i].get("max", 0).asInt();
				if (min >= max) {
					std::cout << "WARNING ignored state variable since min >= max:" << jsonStateVars[i] << std::endl;
					break;
				}
				StateVariableInt<IntType> stateVar;
				stateVar.init(min, max);
				stateVar.name() = jsonStateVars[i].get("name","").asString();
				addState(stateVar);
				std::cout << "Added: " << stateVar << std::endl;
				break;
			}
			case STATE_VAR_REAL: {
				double min = jsonStateVars[i].get("min", 0.0).asDouble();
				double max = jsonStateVars[i].get("max", 0.0).asDouble();
				if (min >= max) {
					std::cout << "WARNING ignored state variable since min >= max:" << jsonStateVars[i] << std::endl;
					break;
				}
				StateVariableReal<RealType> stateVar;
				stateVar.init(min, max);
				stateVar.name() = jsonStateVars[i].get("name","").asString();
				addState(stateVar);
				std::cout << "Added: " << stateVar << std::endl;
				break;
			}
			default:
				std::cout << "WARNING ignored state variable due to wrong type:\n" << jsonStateVars[i] << std::endl;
			}
			std::cout << "\n\n" << std::endl;

		}
	}

	Json::Value writeJson () {
		Json::Value json;
		json.resize(size());
		Json::ArrayIndex k=0;
		for (size_t i=0; i<_setStates.size(); ++i, ++k) {
			Json::Value jsonState;
			jsonState["name"] = _setStates[i].name();
			jsonState["type"] = "set";
			Json::Value jsonSet;
			jsonSet.resize(_setStates[i].size());
			for (unsigned int j=0; j<_setStates[i].size(); ++j) {
				jsonSet[j] = _setStates[i].getSet(j);
			}
			jsonState["set"] = jsonSet;
			json[k] = jsonState;
		}
		for (size_t i=0; i<_intStates.size(); ++i, ++k) {
			Json::Value jsonState;
			jsonState["name"] = _intStates[i].name();
			jsonState["type"] = "int";
			jsonState["min"] = _intStates[i].min();
			jsonState["max"] = _intStates[i].max();
			json[k] = jsonState;
		}
		for (size_t i=0; i<_realStates.size(); ++i, ++k) {
			Json::Value jsonState;
			jsonState["name"] = _realStates[i].name();
			jsonState["type"] = "real";
			jsonState["min"] = _realStates[i].min();
			jsonState["max"] = _realStates[i].max();
			json[k] = jsonState;
		}
		return json;
	}



	void addState(StateVariableSet<SetType> state) {
		_setStates.push_back(state);
	}
	void addState(StateVariableInt<IntType> state) {
		_intStates.push_back(state);
	}
	void addState(StateVariableReal<RealType> state) {
		_realStates.push_back(state);
	}
	size_t size() { return _setStates.size() + _intStates.size() + _realStates.size(); }

	std::vector<StateVariableSet<SetType> > _setStates;
	std::vector<StateVariableInt<IntType> > _intStates;
	std::vector<StateVariableReal<RealType> > _realStates;
};

//}

#endif /* INC_STATEVARIABLE_HPP_ */
