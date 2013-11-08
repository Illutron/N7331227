/*
 *  BasePlugin.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "BasePlugin.h"


void BasePlugin::init(myData * _data){
	data = _data;
	
}
 void BasePlugin::update(){
};
void BasePlugin::draw(){
};

bool  BasePlugin::tryLock()
{
	return (0 == pthread_mutex_trylock(&myMutex)); 
};

bool BasePlugin::isLocked()
{
	if (0 == pthread_mutex_trylock(&myMutex))
	{
		pthread_mutex_unlock(&myMutex);
		return false;
	}	
	return true;

};


bool BasePlugin::isLocked(pthread_mutex_t * _mutex)
{
	if (0 == pthread_mutex_trylock(_mutex))
	{
		pthread_mutex_unlock(_mutex);
		return false;
	}	
	return true;
	
};
/*
BasePlugin* BasePlugin::getFilter(string _name){
 for(list<BasePlugin*>::iterator it=plugins->begin(); it!=plugins->end(); it++){
	 if((*it)->name == _name){
		 return (*it);
	 }
	}
	return NULL;
};
*/


