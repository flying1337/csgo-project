#include "../../cheat.h"

void c_bullettracer::add(c_vector a, c_vector b) {
	s_beaminfo beam;
	beam.type = TE_BEAMPOINTS;
	beam.start = a;
	beam.end = b;
	beam.model_name = str("sprites/purplelaser1.vmt");
	beam.model_index = -1;
	beam.halo_scale = 0.f;
	beam.life = 4.f;
	beam.width = config->i.vis_tracerwidth;
	beam.end_width = config->i.vis_tracerwidth;
	beam.fade_length = 0.f;
	beam.amplitude = 2.f;
	beam.brightness = config->c.tracer[3] * 255.f;
	beam.speed = 0.2f;
	beam.startframe = 0;
	beam.framerate = 0.f;
	beam.red = config->c.tracer[0] * 255.f;
	beam.green = config->c.tracer[1] * 255.f;
	beam.blue = config->c.tracer[2] * 255.f;
	beam.renderable = true;
	beam.segments = 2;
	beam.flags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM | FBEAM_FADEIN;

	auto bo = csgo->beamrenderer->create_beam_points(beam);
	if (bo)
		csgo->beamrenderer->draw_beam(bo);
}

void c_bullettracer::sound_add(c_player *p) {
    if (!csgo->local_player || !p || p->is_dormant() || csgo->local_player->index() == p->index() || p->team() == csgo->local_player->team())
        return;
    if (!config->b.vis_sound)
        return;
    s_beaminfo beam_info;
    beam_info.type = TE_BEAMRINGPOINT;
    beam_info.model_name = str("sprites/physbeam.vmt");
    beam_info.model_index = csgo->modelinfo->get_model_index ( str ( "sprites/physbeam.vmt" ) );
    beam_info.halo_index = -1;
    beam_info.halo_scale = 3;
    beam_info.life = 2.f;
    beam_info.width = 4.f;
    beam_info.fade_length = 1.f;
    beam_info.amplitude = 0.f;
    beam_info.red = config->c.soundcolor[0] * 255.f;
    beam_info.green = config->c.soundcolor[1] * 255.f;
    beam_info.blue = config->c.soundcolor[2] * 255.f;
    beam_info.brightness = config->c.soundcolor[3] * 255.f;
    beam_info.speed = 0.f;
    beam_info.startframe = 0.f;
    beam_info.framerate = 60.f;
    beam_info.segments = -1;
    beam_info.flags = FBEAM_FADEOUT;
    beam_info.center = p->origin( ) + c_vector ( 0, 0, 5 );
    beam_info.start_radius = 5.f;
    beam_info.end_radius = 200.f;

    auto beam = csgo->beamrenderer->create_beam_ring_point(beam_info);
    if(beam)
        csgo->beamrenderer->draw_beam(beam);
}