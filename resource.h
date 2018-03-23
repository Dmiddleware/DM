/*
 * resource.h
 *
 */
#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>
#include "object.h"
#include <map>

namespace ps{

	/*worker对应的资源管理器，管理所有接入的objects*/
	class ResManager{
	public:
		void AddObject(Object* obj){
			objects_[obj->uid] = obj;
			for (auto it : obj->op_funcs){
				op_objs_[it.first]=obj;
			}
		}

		void AddFunc(int id, std::string func, std::vector<std::string> ips, 
			std::vector<std::string> ops, int value) {
			objects_[id]->AddFunc(func, ips, ops, value);
  		}
  		/*返回object和func*/
  		std::pair<int, std::string> FindFuncByop(std::string op) {
  			int qostemp;
  			std::pair<int, std::string> result;
  			if (!op_objs_[op].empty()) {
	  			qostemp = INT_MAX; 
	  			for (int i=0; i<op_objs_[op].size(); i++){
	  				int id = op_objs_[op][i];
	  				std::pair<std::string, int> obj_qos_ = objects_[id]->FindFuncByop(op);
	  				if (obj_qos_.second < qostemp){
	  					result = std::make_pair(id, obj_qos_.first);
	  					qostemp = obj_qos_.second;
	  				}
	  			}
  			}
  			return result;
  		} 

	private:
		std::unordered_map<int, Object*> objects_;
		std::unordered_map<std::string, std::vector<int>> op_objs_; //output->object
	};


}
#endif /*RESOURCE_H_*/
