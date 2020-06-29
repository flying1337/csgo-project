#pragma once

class c_vector;
class c_angle;
class c_matrix3x4;
class c_entity;
class c_trace;
class c_tracefilter;
struct datamap_t;

FORWARD_DECLARE_HANDLE(ThreadHandle_t);
using fnThreadFunc = unsigned(*)(void* param);

const unsigned TT_INFINITE = 0xffffffff;

class c_utils {
public:
	void wait_for_module(const char* mdl);
	unsigned int scan(const char* mdl, const char* pattern);
	float rad(float deg);
	float deg(float rad);
	bool world_to_screen(c_vector world, c_vector* screen);
	c_angle calc_angle(c_vector in1, c_vector in2);
	float map(float val, float start1, float stop1, float start2, float stop2);
	void log(const char* text, ...);
	void console_log(const char* text, ...);
	std::vector<std::string> split(const std::string& str, const char* delim);
	float calc_fov(c_angle a, c_angle b);
	void sincos(float a, float* s, float* c);
    void vector_transform(const c_vector& in1, const c_matrix3x4& in2, c_vector& out);
    void angle_vectors(const c_angle &angles, c_vector &forward, c_vector &right, c_vector &up);
    float random(float min, float max);
    c_vector cross_product(const c_vector &a, const c_vector &b);
    void vector_angles(c_vector &forward, c_vector &up, c_angle &angles);
    void rotate_matrix(c_matrix3x4* mtx, c_matrix3x4* original, c_angle angles, c_vector pos);
    void angle_matrix(const c_angle& angles, const c_vector& position, c_matrix3x4& matrix);
    void angle_matrix(const c_angle& angles, c_matrix3x4& matrix);
    void matrix_copy(const c_matrix3x4& source, c_matrix3x4& target);
    void matrix_multiply(c_matrix3x4& in1, const c_matrix3x4& in2);
    void matrix_set_column(const c_vector& in, int column, c_matrix3x4& out);
    void vector_rotate(const float* in1, const c_matrix3x4& in2, float* out);
    void vector_rotate(c_vector in1, c_angle in2, c_vector& out);
    float dot_product(const float* a, const float* b);
    float ticks_to_time(int ticks);
    void set_clantag ( const char* tag );
    int time_to_ticks(float time);
    void reposition_matrix(c_matrix3x4* mtx, c_vector old, c_vector nw);
    float approach_angle(float target, float value, float speed);
    const char* format(const char* text, ...);
    bool in_bounds(float val, float mins, float maxs);
    void trace_hull(c_vector start, c_vector end, c_vector mins, c_vector maxs, unsigned int mask, c_tracefilter* filter, c_trace* trace);
    bool is_chat_opened();
    ptr datamap_find(datamap_t* dm, const char* name);
    bool vector_collides(c_vector a, c_vector b, c_vector p, float tolerance);
};