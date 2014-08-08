//  File doc/quickbook/eglplus/quickref/enums/color_buffer_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/color_buffer_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2014 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_color_buffer_type
namespace eglplus {

enum class ColorBufferType : EGLenum
{
	RGBBuffer       = EGL_RGB_BUFFER,
	LuminanceBuffer = EGL_LUMINANCE_BUFFER
};

#if !__OGLPLUS_NO_ENUM_VALUE_RANGES
template <>
__Range<ColorBufferType> __EnumValueRange<ColorBufferType>(void);
#endif

#if !__OGLPLUS_NO_ENUM_VALUE_NAMES
__StrCRef __EnumValueName(ColorBufferType);
#endif

} // namespace eglplus
//]