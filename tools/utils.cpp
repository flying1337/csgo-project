#include "../cheat.h"
#include "utils.h"


#define M_PI 3.14159265358979323846

#define in_range(x, a, b) (x >= a && x <= b)
#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

using namespace std::chrono_literals;

void c_utils::wait_for_module(const char* mdl) {
	while (!GetModuleHandleA(mdl))
		std::this_thread::sleep_for(250ms);
}

unsigned int c_utils::scan(const char* mdl, const char* pattern) {
	MODULEINFO info{};
	K32GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(mdl), &info, sizeof(MODULEINFO));

	auto start = (DWORD)info.lpBaseOfDll;
	if (!start)
		return 0;

	auto end = start + info.SizeOfImage;
	auto match = 0;

	const char* pat = pattern;

	for (auto pCur = start; pCur < end; pCur++)
	{
		if (!*pat)
			return match;

		if (*(PBYTE)pat == ('\?') || *(BYTE*)pCur == get_byte(pat))
		{
			if (!match)
				match = pCur;

			if (!pat[2])
				return match;

			if (*(PWORD)pat == ('\?\?') || *(PBYTE)pat != ('\?'))
				pat += 3;
			else
				pat += 2;
		}
		else
		{
			pat = pattern;
			match = 0;
		}
	}

	return 0;
}

float c_utils::rad(float deg) {
	return deg * M_PI / 180.f;
}

float c_utils::deg(float rad) {
	return rad * 180.f / M_PI;
}

bool c_utils::world_to_screen(c_vector world, c_vector* screen) {
	static auto find_point = [](c_vector& point, int screen_w, int screen_h, int degrees) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2)));
		float r = degrees / d;

		point.x = r * point.x + (1 - r) * x2;
		point.y = r * point.y + (1 - r) * y2;
	};

	static ptr vm = 0;
	if (!vm) {
	    vm = scan(str("client_panorama.dll"), str("0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
	    vm = *(ptr*)(vm + 3) + 176;
	}

	auto matrix = *(c_matrix4x4*)vm;

	float w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];
	int screen_width, screen_height;
	csgo->engine->screen_size(screen_width, screen_height);

	float inverse_width = -1.0f / w;
	bool behind = true;

	if (w > 0.01) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen->x = (float)((screen_width / 2) + (0.5 * ((matrix[0][0] * world.x
		+ matrix[0][1] * world.y
		+ matrix[0][2] * world.z
		+ matrix[0][3]) * inverse_width) * screen_width + 0.5));

	screen->y = (float)((screen_height / 2) - (0.5 * ((matrix[1][0] * world.x
		+ matrix[1][1] * world.y
		+ matrix[1][2] * world.z
		+ matrix[1][3]) * inverse_width) * screen_height + 0.5));

	if (screen->x > screen_width || screen->x < 0 || screen->y > screen_height || screen->y < 0 || behind) {
		find_point(*screen, screen_width, screen_height, screen_height / 2);
		return false;
	}

	return !(behind);
}

c_angle c_utils::calc_angle(c_vector in1, c_vector in2) {
	auto delta = in1 - in2;

	c_angle angles;
	angles.p = asinf(delta.z / delta.length()) * 57.295779513082f;
	angles.y = atanf(delta.y / delta.x) * 57.295779513082f;
	angles.r = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}

float c_utils::map(float val, float start1, float stop1, float start2, float stop2) {
	return start2 + (stop2 - start2) * ((val - start1) / (stop1 - start1));
}

void c_utils::console_log(const char* text, ...) {
	auto buffer = (char*)malloc(1024);

	va_list va;
	va_start(va, text);
	vsprintf(buffer, text, va);
	va_end(va);

	csgo->cvar->console_color_printf(c_color((int)(config->c.menu[0] * 255.f), (int)(config->c.menu[1] * 255.f), (int)(config->c.menu[2] * 255.f)), str("uc-sflying "));
	csgo->cvar->console_printf(str("| %s\n"), buffer);
}

const char* c_utils::format(const char* text, ...) {
    auto buffer = (char*)malloc(1024);

    va_list va;
    va_start(va, text);
    vsprintf(buffer, text, va);
    va_end(va);

    return buffer;
}

void c_utils::log(const char* text, ...) {
	auto buffer = (char*)malloc(1024);

	va_list va;
	va_start(va, text);
	vsprintf(buffer, text, va);
	va_end(va);

	features->events->add(buffer);
	console_log(buffer);
}

std::vector<std::string> c_utils::split(const std::string& str, const char* delim)
{
	std::vector<std::string> dest;

	char* pTempStr = _strdup(str.c_str());
	char* pWord = strtok(pTempStr, delim);
	while (pWord != NULL)
	{
		dest.push_back(pWord);
		pWord = strtok(NULL, delim);
	}

	free(pTempStr);
	return dest;
}

float c_utils::calc_fov(c_angle a, c_angle b) {
    auto delta = a - b;
    delta.normalize();
    return sqrtf(powf(delta.p, 2.0f) + powf(delta.y, 2.0f));
}

c_vector c_angle::vector() {
    float sp, sy, cp, cy;
    utils->sincos(utils->rad(y), &sy, &cy);
    utils->sincos(utils->rad(p), &sp, &cp);
    c_vector forward;
    forward.x = cp * cy;
    forward.y = cp * sy;
    forward.z = -sp;

    return forward;
}

void c_utils::sincos(float a, float* s, float* c) {
    *s = sinf(a);
    *c = cosf(a);
}

void c_utils::vector_transform(const c_vector& in1, const c_matrix3x4& in2, c_vector& out) {
    out.x = in1.x * in2.mtx[0][0] + in1.y * in2.mtx[0][1] + in1.z * in2.mtx[0][2] + in2.mtx[0][3];
    out.y = in1.x * in2.mtx[1][0] + in1.y * in2.mtx[1][1] + in1.z * in2.mtx[1][2] + in2.mtx[1][3];
    out.z = in1.x * in2.mtx[2][0] + in1.y * in2.mtx[2][1] + in1.z * in2.mtx[2][2] + in2.mtx[2][3];
}

void c_utils::angle_vectors(const c_angle &angles, c_vector &forward, c_vector &right, c_vector &up)
{
    float sr, sp, sy, cr, cp, cy;
    sincos(rad(angles.y), &sy, &cy);
    sincos(rad(angles.p), &sp, &cp);
    sincos(rad(angles.r), &sr, &cr);

    forward.x = (cp * cy);
    forward.y = (cp * sy);
    forward.z = (-sp);

    right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
    right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
    right.z = (-1 * sr * cp);

    up.x = (cr * sp * cy + -sr * -sy);
    up.y = (cr * sp * sy + -sr * cy);
    up.z = (cr * cp);
}

float c_utils::random(float min, float max) {
    using Fn = float(*)(float, float);
    static auto fn = (Fn)(GetProcAddress(GetModuleHandleA(str("vstdlib.dll")), str("RandomFloat")));
    return fn(min, max);
}

c_vector c_utils::cross_product(const c_vector &a, const c_vector &b)
{
    return c_vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

void c_utils::vector_angles(c_vector &forward, c_vector &up, c_angle &angles)
{
    c_vector left = cross_product(up, forward);

    auto forwardDist = forward.length_2d();
    if (forwardDist > 0.001f)
    {
        angles.p = atan2f(-forward.z, forwardDist) * 180.f / M_PI;
        angles.y = atan2f(forward.y, forward.x) * 180.f / M_PI;
        float upZ = (left.y * forward.x) - (left.x * forward.y);
        angles.r = atan2f(left.z, upZ) * 180.f / M_PI;
    }

    else
    {
        angles.p = atan2f(-forward.z, forwardDist) * 180 / M_PI;
        angles.y = atan2f(-left.x, left.y) * 180 / M_PI;
        angles.r = 0;
    }
}

void c_utils::angle_matrix(const c_angle& angles, const c_vector& position, c_matrix3x4& matrix) {
    angle_matrix(angles, matrix);
    matrix_set_column(position, 3, matrix);
}

void c_utils::angle_matrix(const c_angle& angles, c_matrix3x4& matrix) {
    float sr, sp, sy, cr, cp, cy;

    sy = sin(rad(angles.y));
    cy = cos(rad(angles.y));
    sp = sin(rad(angles.p));
    cp = cos(rad(angles.p));
    sr = sin(rad(angles.r));
    cr = cos(rad(angles.r));

    matrix[0][0] = cp * cy;
    matrix[1][0] = cp * sy;
    matrix[2][0] = -sp;

    float crcy = cr * cy;
    float crsy = cr * sy;
    float srcy = sr * cy;
    float srsy = sr * sy;

    matrix[0][1] = sp * srcy - crsy;
    matrix[1][1] = sp * srsy + crcy;
    matrix[2][1] = sr * cp;

    matrix[0][2] = (sp * crcy + srsy);
    matrix[1][2] = (sp * crsy - srcy);
    matrix[2][2] = cr * cp;

    matrix[0][3] = 0.0f;
    matrix[1][3] = 0.0f;
    matrix[2][3] = 0.0f;
}

void c_utils::matrix_set_column(const c_vector& in, int column, c_matrix3x4& out) {
    out[0][column] = in.x;
    out[1][column] = in.y;
    out[2][column] = in.z;
}

void c_utils::matrix_multiply(c_matrix3x4& in1, const c_matrix3x4& in2) {
    c_matrix3x4 out;
    if (&in1 == &out)
    {
        c_matrix3x4 in1b;
        matrix_copy(in1, in1b);
        matrix_multiply(in1b, in2);
        return;
    }

    if (&in2 == &out)
    {
        c_matrix3x4 in2b;
        matrix_copy(in2, in2b);
        matrix_multiply(in1, in2b);
        return;
    }

    out[0][0] = in1[0][0] * in2.mtx[0][0] + in1[0][1] * in2.mtx[1][0] +
                in1[0][2] * in2.mtx[2][0];
    out[0][1] = in1[0][0] * in2.mtx[0][1] + in1[0][1] * in2.mtx[1][1] +
                in1[0][2] * in2.mtx[2][1];
    out[0][2] = in1[0][0] * in2.mtx[0][2] + in1[0][1] * in2.mtx[1][2] +
                in1[0][2] * in2.mtx[2][2];
    out[0][3] = in1[0][0] * in2.mtx[0][3] + in1[0][1] * in2.mtx[1][3] +
                in1[0][2] * in2.mtx[2][3] + in1[0][3];
    out[1][0] = in1[1][0] * in2.mtx[0][0] + in1[1][1] * in2.mtx[1][0] +
                in1[1][2] * in2.mtx[2][0];
    out[1][1] = in1[1][0] * in2.mtx[0][1] + in1[1][1] * in2.mtx[1][1] +
                in1[1][2] * in2.mtx[2][1];
    out[1][2] = in1[1][0] * in2.mtx[0][2] + in1[1][1] * in2.mtx[1][2] +
                in1[1][2] * in2.mtx[2][2];
    out[1][3] = in1[1][0] * in2.mtx[0][3] + in1[1][1] * in2.mtx[1][3] +
                in1[1][2] * in2.mtx[2][3] + in1[1][3];
    out[2][0] = in1[2][0] * in2.mtx[0][0] + in1[2][1] * in2.mtx[1][0] +
                in1[2][2] * in2.mtx[2][0];
    out[2][1] = in1[2][0] * in2.mtx[0][1] + in1[2][1] * in2.mtx[1][1] +
                in1[2][2] * in2.mtx[2][1];
    out[2][2] = in1[2][0] * in2.mtx[0][2] + in1[2][1] * in2.mtx[1][2] +
                in1[2][2] * in2.mtx[2][2];
    out[2][3] = in1[2][0] * in2.mtx[0][3] + in1[2][1] * in2.mtx[1][3] +
                in1[2][2] * in2.mtx[2][3] + in1[2][3];

    in1 = out;
}

void c_utils::matrix_copy(const c_matrix3x4& source, c_matrix3x4& target) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++)
            target[i][j] = source.mtx[i][j];
    }
}

void c_utils::vector_rotate(const float* in1, const c_matrix3x4& in2, float* out) {
    out[0] = dot_product(in1, in2.mtx[0]);
    out[1] = dot_product(in1, in2.mtx[1]);
    out[2] = dot_product(in1, in2.mtx[2]);
}

void c_utils::vector_rotate(c_vector in1, c_angle in2, c_vector& out) {
    c_matrix3x4 matRotate;
    angle_matrix(in2, matRotate);
    vector_rotate(&in1[0], matRotate, &out[0]);
}

float c_utils::dot_product(const float* a, const float* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void c_utils::rotate_matrix(c_matrix3x4* mtx, c_matrix3x4* original, c_angle angles, c_vector pos) {
    c_matrix3x4 tmp[128];
    for (int i = 0; i < 128; i++) {
        angle_matrix(c_angle(0, angles.y, 0), mtx[i]);
        matrix_multiply(mtx[i], original[i]);

        c_vector rotated;
        vector_rotate(c_vector(original[i].mtx[0][3], original[i].mtx[1][3], original[i].mtx[2][3]) - pos, c_angle(0, angles.y, 0), rotated);
        rotated += pos;

        mtx[i][0][3] = rotated.x;
        mtx[i][1][3] = rotated.y;
        mtx[i][2][3] = rotated.z;
    }
}

float c_utils::ticks_to_time(int ticks) {
    return csgo->globals->tickinterval *( ticks );
}

void c_utils::set_clantag ( const char* tag )
{
    using ClantagChanged = int ( __fastcall*) ( const char*, const char* );
    static uint8_t* fnClantagChanged = reinterpret_cast<uint8_t *>(scan(str ("engine.dll"),
                                                                        str ("53 56 57 8B DA 8B F9 FF 15")));

    reinterpret_cast< ClantagChanged > ( fnClantagChanged ) ( tag, tag );
}

int c_utils::time_to_ticks(float time) {
    return (int)( 0.5f + (float)(time) / csgo->globals->tickinterval );
}

void c_utils::reposition_matrix(c_matrix3x4* mtx, c_vector old, c_vector nw) {
    for (int i = 0; i < 128; i++) {
        mtx[i][0][3] = mtx[i][0][3] - old.x + nw.x;
        mtx[i][1][3] = mtx[i][1][3] - old.y + nw.y;
        mtx[i][2][3] = mtx[i][2][3] - old.z + nw.z;
    }
}

inline float anglemod(float a)
{
    a = (360.f/65536) * ((int)(a*(65536.f/360.0f)) & 65535);
    return a;
}

float c_utils::approach_angle(float target, float value, float speed) {
    target = anglemod( target );
    value = anglemod( value );

    float delta = target - value;

    // Speed is assumed to be positive
    if ( speed < 0 )
        speed = -speed;

    if ( delta < -180 )
        delta += 360;
    else if ( delta > 180 )
        delta -= 360;

    if ( delta > speed )
        value += speed;
    else if ( delta < -speed )
        value -= speed;
    else
        value = target;

    return value;
}

bool c_utils::in_bounds(float val, float mins, float maxs) {
    return val >= mins && val <= maxs;
}

void c_utils::trace_hull(c_vector start, c_vector end, c_vector mins, c_vector maxs, unsigned int mask, c_tracefilter* filter, c_trace* trace) {
    c_ray ray;
    ray.init(start, end, mins, maxs);

    csgo->enginetrace->trace_ray(ray, mask, filter, trace);
}

bool c_utils::is_chat_opened() {
    static ptr hud = 0;
    if (!csgo->local_player) {
        hud = 0;
        return false;
    }

    using fnFindHudElement = void*(__thiscall*)(void*, const char*);
    static auto find_hud_element = (fnFindHudElement)scan(str("client_panorama.dll"), str("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
    if (!find_hud_element) return false;

    if (!hud)
        hud = *(ptr*)(scan(str("client_panorama.dll"), str("B9 ? ? ? ? E8 ? ? ? ? 85 C0 74 05 8D 48 EC EB 02 33 C9 8D 41 58 F7 D9 1B C9 23 C8 74 05")) + 1);

    if (hud) {
        auto chat = find_hud_element((void*)hud, str("CCSGO_HudChat"));
        if (chat)
            return g_offset(bool, chat, 0x58);
    } else
        return false;
}

ptr c_utils::datamap_find(datamap_t *dm, const char *name) {
    auto d = dm;
    while (d) {
        for (auto i = 0; i < d->dataNumFields; i++) {
            auto t = &d->dataDesc[i];
            if (t->fieldType == FIELD_VOID) continue;

            if (t->fieldType == FIELD_EMBEDDED && t->td)
            {
                auto offset = datamap_find(t->td, name);
                if (offset) return offset;
            }

            if (!strcmp(t->fieldName, name))
                return t->fieldOffset[0];
        }
        d = d->baseMap;
    }

    return 0;
}

bool c_utils::vector_collides(c_vector a, c_vector b, c_vector p, float tol) {
    auto v = calc_angle(a, b).vector();
    return  ((a.y - p.y) * v.z - (a.z - p.z) * v.y) / v.length() <= tol &&
            ((a.z - p.z) * v.x - (a.x - p.x) * v.z) / v.length() <= tol &&
            ((a.x - p.x) * v.y - (a.y - p.y) * v.x) / v.length() <= tol;
}
