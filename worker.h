/*
 * worker.h
 *
 *  Created on: 2017年6月15日
 *      Author: zjbpoping
 */
#ifndef WORKER_H_
#define WORKER_H_
#include "proc.h"
#include "value.h"
#include "state.h"
#include <algorithm>
#include <utility>
#include <unordered_map>

namespace ps{
	template <typename Val>
	class Worker:public Proc{
	public:
		using Proc::customer;
		using Callback=std::function<void()>;
		using SlicedKVs = std::vector<std::pair<bool, KVPairs<Val>>>;
		using Slicer = std::function<void(
			const KVPairs<Val>& send, const std::vector<Range>& ranges,
			SlicedKVs* sliced)>;

		explicit Worker(int proc_id):Proc(){
			using namespace std::placeholders;
			slicer_=std::bind(&Worker<Val>::DefaultSlicer,this,_1,_2,_3);
			customer=new Customer(proc_id,std::bind(&Worker<Val>::Process,this,_1));
		}
		virtual ~Worker(){
			delete customer;
			customer=nullptr;
		}

		int Push(const std::vector<Key>& keys,
			const std::vector<Val>& vals,
			const std::vector<int>& lens ={},
			int cmd = 0,
			const Callback& cb=nullptr);

		/*两种Pull*/
		int Pull(const SArray<Key>& keys,
			SArray<Val>* vals,
			SArray<int>* lens =nullptr,
			int cmd = 0,
			const Callback& cb=nullptr){
			return Pull_(keys, vals, lens, cmd, cb);
		}
		int Pull(const std::vector<Key>& keys,
           std::vector<Val>* vals,
           std::vector<int>* lens = nullptr,
           int cmd = 0,
           const Callback& cb = nullptr){
    	   return Pull_(SArray<Key>(keys), vals, lens, cmd, cb);
  		}

		void Wait(int timestamp){
			customer->WaitRequest(timestamp);
		}

	private:
		template <typename C, typename D>
  		int Pull_(const SArray<Key>& keys, C* vals, D* lens, 
  			int cmd, const Callback& cb);
		void Send(int timestamp,bool push, int cmd,const KVPairs<Val>& kvs);
		void Process(const message& msg);
		void DefaultSlicer(const KVPairs<Val>& send,
                     const std::vector<Range>& ranges,
                     SlicedKVs* sliced);
		void RunCallback(int timestamp);
		void AddCallback(int timestamp,const Callback& cb){
			if(!cb) return;
			std::lock_guard<std::mutex> lk(mu_);
			callbacks_[timestamp]=cb;
		}
		/*每个时间戳的接收kvs的缓冲*/
		std::unordered_map<int, std::vector<KVPairs<Val>>> recv_kvs_;
		std::unordered_map<int, Callback> callbacks_;
		std::mutex mu_;
		Slicer slicer_;

		//std::unordered_map<int, Object*> objects_;
	};

}

#endif