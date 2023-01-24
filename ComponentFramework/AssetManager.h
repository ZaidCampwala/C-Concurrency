#pragma once
#include <vector> 
#include <iostream>
#include <string>
#include <unordered_map>
#include "Component.h"
#include "Debug.h"

class AssetManager {

private:
	std::unordered_map<const char*, Ref<Component>> componentCatalog;

public:
	AssetManager();
	~AssetManager();
	bool OnCreate();

	void RemoveAllComponents();

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		componentCatalog[name] = t;
	}

	template<typename ComponentTemplate, typename ... Args>
		Ref<ComponentTemplate> GetComponent(const char* name) const{
			auto id = componentCatalog.find(name);
#ifdef _DEBUG
			if (id == componentCatalog.end()) {
				Debug::Error("Can't find requested component", __FILE__, __LINE__);
				return Ref<ComponentTemplate>(nullptr);
			}
#endif
			return std::dynamic_pointer_cast<ComponentTemplate>(id->second);

	}
};