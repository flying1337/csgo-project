#pragma once

#define DECL_ALIGN(x) __declspec(align(x))

#define ALIGN4 DECL_ALIGN(4)
#define ALIGN8 DECL_ALIGN(8)
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN32 DECL_ALIGN(32)
#define ALIGN128 DECL_ALIGN(128)

#define ALIGN4_POST
#define ALIGN8_POST
#define ALIGN16_POST
#define ALIGN32_POST
#define ALIGN128_POST

class c_vector;

class c_angle {
public:
	c_angle() {}
	c_angle(float p, float y) { this->p = p; this->y = y; }
	c_angle(float p, float y, float r) { this->p = p; this->y = y; this->r = r; }

	c_angle normalize() {
		if (p < -89.0f)
			p = -89.0f;
		if (p > 89.0f)
			p = 89.0f;
		while (y < -180.0f)
			y += 360.0f;
		while (y > 180.0f)
			y -= 360.0f;

		r = 0.0f;
		return *this;
	}
    float &operator[](int i)
    {
        return ((float*)this)[i];
    }
    c_angle operator-(const c_angle& other) const { return c_angle(p - other.p, y - other.y, r - other.r); }
    c_angle operator+(const c_angle& other) const { return c_angle(p + other.p, y + other.y, r + other.r); }
    c_angle operator*(const c_angle& other) const { return c_angle(p * other.p, y * other.y, r * other.r); }
    c_angle operator/(const c_angle& other) const { return c_angle(p / other.p, y / other.y, r / other.r); }
    c_angle operator-(float other) const { return c_angle(p - other, y - other, r - other); }
    c_angle operator+(float other) const { return c_angle(p + other, y + other, r + other); }
    c_angle operator*(float other) const { return c_angle(p * other, y * other, r * other); }
    c_angle operator/(float other) const { return c_angle(p / other, y / other, r / other); }
    c_angle& operator-=(float other) { p -= other; y -= other; r -= other; return *this; }
    c_angle& operator-=(const c_angle& other) { p -= other.p; y -= other.y; r -= other.r; return *this; }

    c_vector vector();

	float p = 0, y = 0, r = 0;
};

struct c_vector {
public:
	c_vector() {}
	c_vector(float x, float y) { this->x = x; this->y = y; }
	c_vector(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

    bool is_valid() const
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }
    float operator[](int i) const
    {
        return ((float*)this)[i];
    }
    c_vector operator+(float fl) const
    {
        return c_vector(x + fl, y + fl, z + fl);
    }
    c_vector operator-(float fl) const
    {
        return c_vector(x - fl, y - fl, z - fl);
    }
	float length() { return sqrtf(x * x + y * y + z * z); }
	float length_sqr() { return x * x + y * y + z * z; }
	float length_2d() { return sqrtf(x * x + y * y); }
	float length_2d_sqr() { return x * x + y * y; }
	float dist(c_vector& const other) { return (*this - other).length(); }

	c_vector operator-(const c_vector& other) const { return c_vector(x - other.x, y - other.y, z - other.z); }
	c_vector operator+(const c_vector& other) const { return c_vector(x + other.x, y + other.y, z + other.z); }
    c_vector operator*(const float& v) const { return c_vector(x * v, y * v, z * v); }
    c_vector operator/(const float& v) const { return c_vector(x / v, y / v, z / v); }

    float dot_product(c_vector b) {return(x * b.x + y * b.y + z * b.z);}
	c_vector& operator*=(float v) { x *= v; y *= v; z *= v; return *this; }
	c_vector& operator+=(c_vector v) { x += v.x; y += v.y; z += v.z; return *this; }
	bool operator==(c_vector v) { return x == v.x && y == v.y && z == v.z; }

	c_vector cross(c_vector b) {
	    c_vector result;
        result.x = y * b.z - z * b.y;
        result.y = z * b.x - x * b.z;
        result.z = x * b.y - y * b.x;
        return result;
	}

	float& operator[](int i) { return ((float*)this)[i]; }

	c_angle angle() {
		c_angle angles;
		float tmp, yaw, pitch;

		if (y == 0 && x == 0)
		{
			yaw = 0;

			if (z > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(y, x) * 180 / 3.14159265358979323846);

			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(x * x + y * y);
			pitch = (atan2(-z, tmp) * 180 / 3.14159265358979323846);

			if (pitch < 0)
				pitch += 360;
		}

		angles.p = pitch;
		angles.y = yaw;
		angles.r = 0;

		return angles;
	}

	float x = 0, y = 0, z = 0;
};

class ALIGN16 c_vectoraligned : public c_vector
{
public:
	c_vectoraligned() {};
	c_vectoraligned(float X, float Y, float Z) { x = X; y = Y; z = Z; }

	explicit c_vectoraligned(const c_vector& vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
	}

	c_vectoraligned& operator=(const c_vector& vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	float w = 0;
} ALIGN16_POST;