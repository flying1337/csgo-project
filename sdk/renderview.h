#pragma once

class c_renderview {
public:
	virtual void	draw_brush_model(c_entity* baseentity, c_model* model, const c_vector& origin, const c_angle& angles, bool bUnused) = 0;
	virtual void	draw_id_brush_model(void* pList, c_model* model) = 0;
	virtual void	touch_light(struct dlight_t* light) = 0;
	virtual void	draw_3d_overlays(void) = 0;
	virtual void	set_blend(float blend) = 0;
	virtual float	get_blend(void) = 0;
	virtual void	set_color_modulation(float const* blend) = 0;
	virtual void	get_color_modulation(float* blend) = 0;
};