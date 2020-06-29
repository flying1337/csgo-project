#pragma once

class c_entityinfo {
public:
	void* create;
	void* create_event;
	const char* name;
	c_recvtable* table;
	c_entityinfo* next;
	int class_id;
};