/*
 * object.h
 *
 *  Created on: 2018年3月6日
 *      Author: zjbpoping
 */
#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <limits.h>

namespace ps{

	/*QoS属性指标*/
	// enum Attribute {
	//   RESPONSE_TIME, THROUGHPUT, RELIABILITY, STABILITY, TRUST, OTHER
	// };

	struct Object{

		Object(int id) {
			uid = id;
		} 

		enum DeviceType { FIXED, MOVABLE };

		int uid;

		std::string uri;

		int port;

		DeviceType type;

		float loc_x;

		float loc_y;

		std::vector<std::string> funcs;

		unorder_map<std::string, std::vector<std::string>> inputs; // func->inputs

		unorder_map<std::string, std::vector<std::string>> outputs; //func->outputs

		unorder_map<std::string, int> qos; //func->qos

		unorder_map<std::string, std::vector<std::string>> op_funcs_;//output->func 

		void AddFunc(std::string func, std::vector<std::string> ips, 
			std::vector<std::string> ops, int value) {
			funcs.push_back(func);
			inputs[func] = ips;
			outputs[func] = ops;
			qos = value;

			for (std::string op : ops){
				op_funcs_[op].push_back(func);
			}
  		}
  		/*返回func以及其qos*/
  		std::pair<std::string, int> FindFunc(std::string op) {
  			int qostemp;
  			std::pair<std::string, int> result;
  			if (!op_funcs_[op].empty()) {
	  			qostemp = INT_MAX; 
	  			for (int i=0; i<op_funcs_[op].size(); i++){
	  				std::string func = op_funcs_[op][i];
	  				if (qos[func] < qostemp){
	  					result = std::make_pair(func, qos[func]);
	  					qostemp = qos[func];
	  				}
	  			}
  			}
  			return result;
  		} 

  		std::vector<std::string> GetOutputs() {
  			
  		}
	};


}
