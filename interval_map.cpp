#include <map>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>

void IntervalMapTest();

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val) : m_valBegin(val) {}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
		//1 Check if interval is not empty
		if (!(keyBegin<keyEnd))
			return;

		//2 Check if map is empty
		if (m_map.size() == 0)
			m_map.emplace(std::numeric_limits<K>::lowest(), m_valBegin);

		//3 Check if new interval is canonical
		typename std::map<K, V>::iterator lowBegin = m_map.lower_bound(keyBegin), upEnd = m_map.upper_bound(keyEnd);
		typename std::map<K, V>::iterator prevEnd = std::prev(upEnd), prevBegin = std::prev(lowBegin), beginIt, endIt, beginNext;

		if (!(prevEnd->second == val)) {
			if (!(prevEnd->first < keyEnd) && !(keyEnd < prevEnd->first))
				endIt = prevEnd;
			else
				endIt = m_map.insert_or_assign(upEnd, keyEnd, prevEnd->second);
		}
		else
			endIt = upEnd;

		if (!(lowBegin == m_map.begin())) {
			if (!(prevBegin->second == val))
				beginIt = m_map.insert_or_assign(lowBegin, keyBegin, val);
			else
				beginIt = prevBegin;
		}
		else
			beginIt = m_map.insert_or_assign(lowBegin, keyBegin, val);

		beginNext = std::next(beginIt);
		if (!(beginNext == m_map.end()))
			m_map.erase(beginNext, endIt);
	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.
#include <iostream>

void IntervalMapTest() {
    interval_map<int, char> myIntervalMap('A');

    // Test 1: Assign a value to a non-empty interval
    myIntervalMap.assign(1, 5, 'B');

    // Check the values within the interval
    for (int i = 1; i < 5; ++i) {
        std::cout << "Value at key " << i << ": " << myIntervalMap[i] << std::endl;
    }

    // Test 2: Assign a value to an empty interval (should do nothing)
    myIntervalMap.assign(5, 5, 'C');

    // Check the values within the interval
    std::cout << "Value at key 5: " << myIntervalMap[5] << std::endl;

    // Test 3: Assign a value to an overlapping interval
    myIntervalMap.assign(3, 7, 'D');

    // Check the values within the interval
    for (int i = 3; i < 7; ++i) {
        std::cout << "Value at key " << i << ": " << myIntervalMap[i] << std::endl;
    }
}

int main() {
    IntervalMapTest();
    return 0;
}