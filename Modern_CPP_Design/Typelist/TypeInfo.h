#pragma once
#include <typeinfo>
#include <cassert>

class TypeInfo{
public:
	TypeInfo(){
		class Nil{};
		pInfo_ = &typeid(Nil);
		assert(pInfo_);
	}
	TypeInfo(const std::type_info& rhs) : pInfo_(&rhs){
		assert(pInfo_);
	}
	const char* name() const{
		assert(pInfo_);
		return pInfo_->name();
	}

	bool before(const TypeInfo& rhs) const{
		assert(pInfo_ && rhs.pInfo_);
		return pInfo_->before(*(rhs.pInfo_));
	}

	const std::type_info& Get() const{
		return *pInfo_;
	}
private:
	const std::type_info* pInfo_;
};

inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs){
	return lhs.Get() == rhs.Get();
}

inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs){
	return lhs.Get() != rhs.Get();
}

inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs){
	return lhs.before(rhs);
}

inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs){
	return rhs.before(rhs);
}

inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs){
	return !(lhs > rhs);
}

inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs){
	return !(lhs < rhs);
}
