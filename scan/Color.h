#if !defined(AFX_COLOR_H__3770AC59_0D0E_49EF_99C8_037268A33CE4__INCLUDED_)
#define AFX_COLOR_H__3770AC59_0D0E_49EF_99C8_037268A33CE4__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CColor
{
public:
	class CYMK;
	class YUVA;
	class HSVA;
	class HSLA;
	class HCYA;

	static __inline unsigned long floatColorToULong(float x,float y,float z,float h)
	{
		return	(((unsigned long)(255*h))<<24) + 
				(((unsigned long)(255*z))<<16) + 
				(((unsigned long)(255*y))<<8) + 
				 ((unsigned long)(255*x));
	}

	class RGBA
	{
	public:
		float r;
		float g;
		float b;
		float a;

		operator float*() { return (float*)(this); }
		operator const float*() const { return (const float*)this; };
		operator unsigned long()    //  little endian byte ordering
		{ return floatColorToULong(r,g,b,a); }

		operator CYMK() const;
		operator YUVA() const;
		operator HSVA() const;
		operator HSLA() const;
		operator HCYA() const;

		RGBA(float _r = 0.0f,float _g = 0.0f,float _b = 0.0f,float _a = 0.0f)
			: r(_r),g(_g),b(_b),a(_a)
		{ };

	};

	class CYMK
	{
	public:
		float c;
		float y;
		float m;
		float k;

		operator float*() { return (float*)(this); }
		operator const float*() const { return (const float*)this; };
		operator unsigned long()    //  little endian byte ordering
		{ return floatColorToULong(c,y,m,k); }

		operator YUVA() const;
		operator RGBA() const;

		CYMK(float _c = 0.0f,float _y = 0.0f,float _m = 0.0f,float _k = 0.0f)
			: c(_c),y(_y),m(_m),k(_k)
		{ };
	};

	//	Luminance calculations follow CCIR601 standard.
	class YUVA
	{
	public:
		float y;
		float u;
		float v;
		float a;

		operator float*() { return (float*)(this); }
		operator const float*() const { return (const float*)this; };
		operator unsigned long()    //  little endian byte ordering
		{ return floatColorToULong(y,u,v,a); }

		operator CYMK() const;
		operator RGBA() const;

		YUVA(float _y = 0.0f,float _u = 0.0f,float _v = 0.0f,float _a = 0.0f)
			: y(_y),u(_u),v(_v),a(_a)
		{ };
	};

	class HSVA
	{
	public:
		float h;
		float s;
		float v;
		float a;

		operator float*() { return (float*)(this); }
		operator const float*() const { return (const float*)this; };
		operator unsigned long()    //  little endian byte ordering
		{ return floatColorToULong(h,s,v,a); }

		operator RGBA() const;

		HSVA(float _h = 0.0f,float _s = 0.0f,float _v = 0.0f,float _a = 0.0f)
			: h(_h),s(_s),v(_v),a(_a)
		{ };
	};

	class HSLA
	{
	public:
		float h;
		float s;
		float l;
		float a;

		operator float*() { return (float*)(this); }
		operator const float*() const { return (const float*)this; };
		operator unsigned long()    //  little endian byte ordering
		{ return floatColorToULong(h,s,l,a); }

		operator RGBA() const;

		HSLA(float _h = 0.0f,float _s = 0.0f,float _l = 0.0f,float _a = 0.0f)
			: h(_h),s(_s),l(_l),a(_a)
		{ };
	};

	class HCYA
	{
	public:
		float h;
		float c;
		float y;
		float a;

		operator float*() { return (float*)(this); }
		operator const float*() const { return (const float*)this; };
		operator unsigned long()    //  little endian byte ordering
		{ return floatColorToULong(h,c,y,a); }

		operator RGBA() const;

		HCYA(float _h = 0.0f,float _c = 0.0f,float _y = 0.0f,float _a = 0.0f)
			: h(_h),c(_c),y(_y),a(_a)
		{ };
	};
};

#endif // !defined(AFX_COLOR_H__3770AC59_0D0E_49EF_99C8_037268A33CE4__INCLUDED_)

