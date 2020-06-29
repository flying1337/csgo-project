#pragma once
template <typename t>
static t callvfunc(void* class_pointer, size_t index) {
    return (*(t**)class_pointer)[index];
}
class c_debugoverlay {
public:
    virtual void    add_entity_text_overlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
    void add_box_overlay(const c_vector& origin, const c_vector& mins, const c_vector& max, c_vector const& orientation, int r, int g, int b, int a, float duration)
    {
        callvfunc<void(__thiscall*)(void*, const c_vector& origin, const c_vector& mins, const c_vector& max, c_vector const& orientation, int r, int g, int b, int a, float duration )>(this, 1)(this, origin, mins, max, orientation, r, g, b, a, duration);
    };
	virtual void    add_sphere_overlay(const c_vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
	virtual void    add_triangle_overlay(const c_vector& p1, const c_vector& p2, const c_vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void    add_line(const c_vector& origin, const c_vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void    add_text_overlay(const c_vector& origin, float duration, const char* format, ...) = 0;
	virtual void    add_text_overlay(const c_vector& origin, int line_offset, float duration, const char* format, ...) = 0;
	virtual void    add_screen_text_overlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void    add_swept_box_overlay(const c_vector& start, const c_vector& end, const c_vector& mins, const c_vector& max, const c_vector& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void    add_grid_overlay(const c_vector& origin) = 0;
	virtual void    add_coord_frame_overlay(const c_matrix3x4& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	virtual int     screen_position(const c_vector& point, c_vector& screen) = 0;
	virtual int     screen_position(float flXPos, float flYPos, c_vector& screen) = 0;
	virtual void*	get_first(void) = 0;
	virtual void*	get_next(void* current) = 0;
	virtual void    clear_dead_overlays(void) = 0;
	virtual void    clear_all_overlays(void) = 0;
	virtual void    add_text_rgb(const c_vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
	virtual void    add_text_rgb(const c_vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void    add_line_overlay_rgb(const c_vector& origin, const c_vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void    add_box_overlay(const c_vector& origin, const c_vector& mins, const c_vector& max, c_vector const& orientation, const uint8_t* faceColor, const uint8_t* edgeColor, float duration) = 0;
	virtual void	add_line(const c_vector& origin, const c_vector& dest, int r, int g, int b, int a, float, float) = 0;
	virtual void    purge_text_overlay() = 0;
};