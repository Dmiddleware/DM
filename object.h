/*
 * object.h
 *
 */
#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <limits.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdlib.h>

namespace ps {

/*QoS属性指标*/
// enum Attribute {
//   RESPONSE_TIME, THROUGHPUT, RELIABILITY, STABILITY, TRUST, OTHER
// };

struct Object {

	Object(int id) {
		uid = id;
	}

	enum DeviceType { FIXED, MOVABLE };
	//the id of the object
	int uid;
	//the ip address of the object
	std::string uri;
	//the port of the object
	int port;
	//correspond to the above mentioned "enum DeviceType"
	DeviceType type;
	//the location of the object
	float loc_x;
	float loc_y;

	//the id of worker connected to this object
	int worker_id;

	std::vector<std::string> funcs;
	//the smaller the better
	std::unordered_map<std::string, int> qos; //func->qos
	std::unordered_map<std::string, std::vector<std::string>> op_funcs_;//output->func
	//the inputs and outputs corresponding to function
	std::unordered_map<std::string, std::vector<std::string>> inputs;
	std::unordered_map<std::string, std::vector<std::string>> outputs;

	/* add a function of this object */
	void AddFunc(std::string func,  int value) {
		bool judge = FindFunc(func);
		if (judge) {
			funcs.push_back(func);
			qos[func] = value;
		} else {
			qos[func] = std::min(qos[func], value);
		}
	}
	/* find the best function(the lowest qos) for the option*/
	std::pair<std::string, int> FindFuncByop(std::string op) {
		int qostemp;
		std::pair<std::string, int> result;
		if (!op_funcs_[op].empty()) {
			qostemp = INT_MAX;
			int op_funcs_po_size = op_funcs_[op].size();
			for (int i = 0; i < op_funcs_po_size; i++) {
				std::string func = op_funcs_[op][i];
				if (qos[func] < qostemp) {
					result = std::make_pair(func, qos[func]);
					qostemp = qos[func];
				}
			}
		}
		return result;
	}
	/*look for the function */
	bool FindFunc(std::string func) {
		bool judge = 1;
		int funcs_size = funcs.size();
		for(int i = 0; i < funcs_size; i++){
			if(funcs[i] == func){
				judge = 0; 
				break;
			}
		}
		return judge;
	}
	/*
	void AddFunc(std::string func, std::vector<std::string> ips,
		std::vector<std::string> ops, int value) {


		funcs.push_back(func);
		inputs[func] = ips;
		outputs[func] = ops;
		qos = value; // qos[func] = value

		for (std::string op : ops){
			op_funcs_[op].push_back(func);
		}*/

	/*返回func以及其qos*/
	/*
	std::vector<std::string> GetOutputs() {

	}
	*/
};

}

#endif /*OBJECT_H_*/
