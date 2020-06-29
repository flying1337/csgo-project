#pragma once

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,	// An array of the base types (can't be of datatables).
	DPT_DataTable,
	DPT_NUMSendPropTypes

} e_sendproptype;

class c_recvtable;

class c_recvprop {
public:
	const char* name;
	e_sendproptype type;
	int flags;
	int string_buffer_size;
	bool is_inside_array;
	void* extra_data;
	c_recvprop* array_prop;
	void* array_length_proxy;
	void* proxy;
	void* dt_proxy;
	c_recvtable* table;
	int offset;
	int elements_stride;
	int num_elements;
	const char* parent_name;
};

class c_recvtable {
public:
	c_recvprop* props;
	int num;
	void* decoder;
	const char* name;
};