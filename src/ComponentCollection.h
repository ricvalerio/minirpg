/*
 * ComponentCollection.h
 *
 *  Created on: 18 de Dez de 2013
 *      Author: Ricardo
 */

#ifndef COMPONENTCOLLECTION_H_
#define COMPONENTCOLLECTION_H_

#include <tr1/unordered_map>
#include <typeinfo>
#include "IComponent.h"
#include "Components.h"


class ComponentCollection {
private:
	std::tr1::unordered_map<const std::type_info*, IComponent *> components;
public:
	ComponentCollection();
	virtual ~ComponentCollection();


	template <typename T>
	void addComponent(T* component) {
		components[ &typeid(T) ] = component;
	};


	template <typename T>
	T* getComponent() {
	    if(components.count( &typeid(T) ) != 0) {
	        return (T*) (components[&typeid(T)]);
	    } else {
	        return NULL;
	    }
	};


	template <typename T>
	void removeComponent() {
		components.erase( &typeid(T) );
	};

};

#endif /* COMPONENTCOLLECTION_H_ */
