/**
 *  @file oglplus/query.hpp
 *  @brief Query object wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_QUERY_1107121519_HPP
#define OGLPLUS_QUERY_1107121519_HPP

#include <oglplus/error.hpp>
#include <oglplus/object.hpp>
#include <oglplus/friend_of.hpp>
#include <cassert>

namespace oglplus {

/// Wrapper for asynchronous query functions
/** @note Do not use this class directly, use Query instead.
 */
class QueryOps
 : public Named
{
protected:
	static void _init(GLsizei count, GLuint& _name)
	{
		::glGenQueries(count, &_name);
		ThrowOnError(OGLPLUS_ERROR_INFO(GenQueries));
	}

	static void _cleanup(GLsizei count, GLuint& _name)
	{
		assert(_name != 0);
		::glDeleteQueries(count, &_name);
	}

	static GLboolean _is_x(GLuint _name)
	{
		assert(_name != 0);
		return ::glIsQuery(_name);
	}
public:

	/// Query bint target
	enum class Target : GLenum {
#include <oglplus/enums/query_target.ipp>
	};

	/// Begin the query on the specified @p target
	void Begin(Target target) const
	{
		assert(_name != 0);
		::glBeginQuery(GLenum(target), _name);
		AssertNoError(OGLPLUS_OBJECT_ERROR_INFO(
			BeginQuery,
			Query,
			_name
		));
	}

	/// End the query on the specified @p target
	void End(Target target) const
	{
		assert(_name != 0);
		::glEndQuery(GLenum(target));
		AssertNoError(OGLPLUS_OBJECT_ERROR_INFO(
			EndQuery,
			Query,
			_name
		));
	}

	/// Returns true if the query result is available
	bool ResultAvailable(void) const
	{
		assert(_name != 0);
		GLuint result = GL_FALSE;
		::glGetQueryObjectuiv(
			_name,
			GL_QUERY_RESULT_AVAILABLE,
			&result
		);
		AssertNoError(OGLPLUS_OBJECT_ERROR_INFO(
			GetQueryObjectuiv,
			Query,
			_name
		));
		return result == GL_TRUE;
	}

	/// Get the query result
	void Result(GLint& result) const
	{
		assert(_name != 0);
		::glGetQueryObjectiv(
			_name,
			GL_QUERY_RESULT,
			&result
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			GetQueryObjectiv,
			Query,
			_name
		));
	}

	/// Get the query result
	void Result(GLuint& result) const
	{
		assert(_name != 0);
		::glGetQueryObjectuiv(
			_name,
			GL_QUERY_RESULT,
			&result
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			GetQueryObjectuiv,
			Query,
			_name
		));
	}

	/// Get the query result
	void Result(GLint64& result) const
	{
		assert(_name != 0);
		::glGetQueryObjecti64v(
			_name,
			GL_QUERY_RESULT,
			&result
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			GetQueryObjecti64v,
			Query,
			_name
		));
	}

	/// Get the query result
	void Result(GLuint64& result) const
	{
		assert(_name != 0);
		::glGetQueryObjectui64v(
			_name,
			GL_QUERY_RESULT,
			&result
		);
		ThrowOnError(OGLPLUS_OBJECT_ERROR_INFO(
			GetQueryObjectui64v,
			Query,
			_name
		));
	}

	/// Waits for and queries the result
	template <typename ResultType>
	void WaitForResult(ResultType& result) const
	{
		while(!ResultAvailable())
			glFinish();
		Result(result);
	}

	/// A helper class automatically executing a query
	/** Instances of this class begin the query in the constructor
	 *  and end the query in the destructor. It is more convenient
	 *  not to use this class directly, use the Execute() function
	 *  instead.
	 *
	 *  @see Execute
	 */
	template <typename ResultType>
	class Execution
	{
	private:
		typename QueryOps::Target _target;
		const QueryOps& _query;
		ResultType& _result;
		bool _alive;
	public:
		Execution(
			typename QueryOps::Target target,
			const QueryOps& query,
			ResultType& result
		): _target(target)
		 , _query(query)
		 , _result(result)
		 , _alive(false)
		{
			_query.Begin(_target);
			_alive = true;
		}

		Execution(const Execution&) = delete;

		Execution(Execution&& temp)
		 : _target(temp._target)
		 , _query(temp._query)
		 , _result(temp._result)
		 , _alive(temp._alive)
		{
			temp._alive = false;
		}

		~Execution(void)
		{
			if(_alive)
			{
				_query.End(_target);
				_query.WaitForResult(_result);
			}
		}
	};

	/// Executes the query on the specified @p target and gets the @p result
	/** This function creates an instance of the Execution class which
	 *  begins a query on the specified @p target when it is constructed
	 *  and ends this query and gets its @p result when it is destroyed.
	 */
	template <typename ResultType>
	Execution<ResultType> Execute(Target target, ResultType& result) const
	{
		return Execution<ResultType>(target, *this, result);
	}
};


#ifdef OGLPLUS_DOCUMENTATION_ONLY
/// An @ref oglplus_object encapsulating the OpenGL asynchronous query functionality
/**
 *  @ingroup objects
 */
class Query
 : public QueryOps
{ };
#else
typedef Object<QueryOps, true> Query;
#endif

} // namespace oglplus

#endif // include guard
