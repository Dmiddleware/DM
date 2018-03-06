/*
 * object.h
 *
 *  Created on: 2018年3月6日
 *      Author: zjbpoping
 */
#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>

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

		unorder_map<std::string, std::vector<std::string>> inputs;

		unorder_map<std::string, std::vector<std::string>> outputs; 

		unorder_map<std::string, int> qos;

	};


}
