/**
 *  @example oglplus/001_triangle.cpp
 *  @brief Shows the basic usage of OGLplus by drawing a simple red triangle.
 *
 *  @oglplus_screenshot{001_triangle}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>

#include <oglplus/texgen/circles_node.hpp>
#include <oglplus/texgen/border_node.hpp>
#include <oglplus/texgen/invert_node.hpp>
#include <oglplus/texgen/offset_node.hpp>
#include <oglplus/texgen/arithmetic_node.hpp>
#include <oglplus/texgen/random_node.hpp>
#include <oglplus/texgen/texture_node.hpp>
#include <oglplus/texgen/uniform_node.hpp>
#include <oglplus/texgen/swizzle_node.hpp>
#include <oglplus/texgen/newton_node.hpp>
#include <oglplus/texgen/mandelbrot_node.hpp>
#include <oglplus/texgen/normal_map_node.hpp>
#include <oglplus/texgen/blur_node.hpp>
#include <oglplus/texgen/checker_node.hpp>
#include <oglplus/texgen/stripes_node.hpp>
#include <oglplus/texgen/mix_node.hpp>
#include <oglplus/texgen/global_node.hpp>
#include <oglplus/texgen/render_node.hpp>

#include <iostream>

#include <oglplus/all.hpp>

#include "example.hpp"

namespace oglplus {
namespace texgen {

} // namespace texgen

class TriangleExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	texgen::GlobalNode gn;
	texgen::CirclesNode ci;
	texgen::InvertNode in;
	texgen::OffsetNode of;
	texgen::UnaryArithmeticNode ua;
	texgen::BinaryArithmeticNode ba;
	texgen::BinaryArithmeticNode ad;
	texgen::UniformNode u1, u2;
	texgen::NewtonNode nt;
	texgen::MandelbrotNode mb;
	texgen::CheckerNode c0;
	texgen::StripesNode st;
	texgen::MixNode m1, m2;
	texgen::SwizzleNode s1, s2;
	texgen::Blur2DNode b1;
	texgen::Grow2DNode gr;
	texgen::NormalMapNode nm;
	texgen::Random2DRGBANode tx;
	texgen::RenderNode rn;

public:
	TriangleExample(void)
	 : gl()
	 , ua(texgen::UnaryArithmeticOp::Exp)
	 , ba(texgen::BinaryArithmeticOp::Greater)
	 , ad(texgen::BinaryArithmeticOp::Addition)
	 , u1(texgen::SlotDataType::FloatVec4)
	 , u2(texgen::SlotDataType::FloatVec4)
	 , nt(texgen::NewtonFunction::Xe4minus1)
	 , s1("rrra")
	 , s2("g")
	 , tx(0, 256, 256)
	 , rn(16,16)
	{
		//st.SetCoeff(Vec3f(-1,1,1));
		//m1.SetZero(0.0f);
		//m1.SetOne(1.0f);
		of.SetOffset(Vec3f(100,0,0));
		

		Connect(u1.Output(0), m1.Input(0));
		Connect(u2.Output(0), m1.Input(1));

		Connect(gn.Output(0), ad.Input(0));
		ad.SetB(Vec3f(-0.5,-0.5,0));
		Connect(ad.Output(0), ci.Input(0));
		Connect(ci.Output(0), gr.Input(0));
		Connect(gr.Output(0), in.Input(0));
		Connect(in.Output(0), ua.Input(0));
		Connect(ua.Output(0), m1.Input(2));
		Connect(m1.Output(0), s1.Input(0));
		Connect(s1.Output(0), nm.Input(0));

		Connect(tx.Output(0), ba.Input(0));
		Connect(m1.Output(0), ba.Input(1));

		Connect(ba.Output(0), m2.Input(2));

		Connect(m2.Output(0), b1.Input(0));

b1.Output(0).Definitions(std::cout, 150) << std::endl;
b1.Output(0).Expression(std::cout, 150) << std::endl;


		Connect(b1.Output(0), rn.Input(0));

		rn.Update();
		rn.Activate();
		u1.BindUniform();
		u2.BindUniform();

		gl.Disable(Capability::DepthTest);

	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double time)
	{
		if(rn.Render())
		{
			u1.SetValue(Vec4f(
				SineWave01(time / 5.1),
				SineWave01(time / 9.7),
				SineWave01(time / 7.7),
				1
			));
			u2.SetValue(Vec4f(
				CosineWave01(time / 7.9),
				CosineWave01(time / 9.9),
				CosineWave01(time / 8.1),
				1
			));
		}
	}

	bool Continue(double duration)
	{
		return duration < 180;
	}
};

void setupExample(ExampleParams& /*params*/){ }

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& /*example*/,
	unsigned /*thread_id*/,
	const ExampleParams& /*params*/
){ return std::unique_ptr<ExampleThread>(); }

std::unique_ptr<Example> makeExample(const ExampleParams& /*params*/)
{
	return std::unique_ptr<Example>(new TriangleExample);
}

} // namespace oglplus
