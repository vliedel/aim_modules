/**
 * @file SearchSpace.h
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

#ifndef INC_SEARCHSPACE_H_
#define INC_SEARCHSPACE_H_

#include <vector>

template <class T>
class StateVariable {
public:
	StateVariable() {}
	virtual ~StateVariable() {}
	virtual const T& get() const { return _val; };
	virtual void set(const T& val) { _val = val; };
	virtual void init(const std::vector<T>& values) {};
//private:
	T _val;
};


template <class T>
class StateVariableSet : public StateVariable<T> {
public:
	StateVariableSet() {}
	~StateVariableSet() {}
	void Init(const std::vector<T>& values) { _set = values; }
	size_t size() { return _set.size(); }
//private:
	std::vector<T> _set;
};


template <class T>
class StateVariableContinuous : public StateVariable<T> {
public:
	StateVariableContinuous() {}
	~StateVariableContinuous() {}
	void Init(const std::vector<T>& values) {
		if (values.size() > 1) {
			_min = values[0];
			_max = values[1];
		}
	}
	void Init(const T& min, const T& max) { _min = min; _max = max; };
	void set(const T& val) {
		if (val > _max)
			StateVariable<T>::_val = _max;
		else if (val < _min)
			StateVariable<T>::_val = _min;
		else
			StateVariable<T>::_val = val;
	};
//private:
	T _min;
	T _max;
};


template <class T>
class State {
public:
	State() { }
	~State() { }

	const T& getStateVal(const size_t index) const {
		if (index < _setStates.size())
			return _setStates[index].get();
		else
			return _contStates[index-_setStates.size()].get();
	}

	void setStateVal(const size_t index, T& val) {
		if (index < _setStates.size())
			_setStates[index].set(val);
		else
			_contStates[index-_setStates.size()].set(val);
	}

	// Read access only, use setStateVal to write
	const T& operator[](const size_t index) const {
		return getStateVal(index);
	}

	void addState(StateVariableSet<T> state) {
		_setStates.push_back(state);
	}

	void addState(StateVariableContinuous<T> state) {
		_contStates.push_back(state);
	}

	size_t size() { return _setStates.size() + _contStates.size(); }

	std::vector<StateVariableSet<T> > _setStates;
	std::vector<StateVariableContinuous<T> > _contStates;
};



#endif /* INC_SEARCHSPACE_H_ */
