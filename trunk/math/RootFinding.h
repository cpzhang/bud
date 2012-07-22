/********************************************************************
**	file: 		RootFinding.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-1-27
**	purpose:	
*********************************************************************/
#ifndef __RootFinding_h__
#define __RootFinding_h__

//
#include "Common.h"
#include "Complex.h"
#include "Basic.h"
#include "Polynomial.h"

//
namespace Zen
{
	class _MATH_EXPORT_ RootFinding
	{
	public:
		// a*x + b = 0;
		inline static void equation(Real a, Real b, std::vector<Complex>& roots)
		{
			roots.push_back(Complex(-b / a));
		}

		// a*x*x + b*x + c = 0;
		inline static void equation(Real a, Real b, Real c, std::vector<Complex>& roots)
		{
			Real delta = b*b - 4*a*c;

			if (delta > 0)
			{
				Real deltaSqrt = Basic::Sqrt(delta);
				roots.push_back((-b + deltaSqrt) / (2.0f * a));
				roots.push_back((-b - deltaSqrt) / (2.0f * a));
				return;
			}
			else if (delta == 0)
			{
				roots.push_back(-b / (2.0f * a));
				return;
			}
			else
			{
				Real deltaSqrt = Basic::Sqrt(-delta);
				Complex root1;
			
				roots.push_back(Complex(-b/ (2.0f * a),  deltaSqrt / (2.0f * a)));
				roots.push_back(Complex(-b/ (2.0f * a), -deltaSqrt / (2.0f * a)));
				return;
			}
		}

		// a*x*x*x + b*x*x + c*x + d = 0;
		inline static void equation(Real a, Real b, Real c, Real d, std::vector<Complex>& roots)
		{
		}

		//
		inline static Real bisection(Real left, Real right, std::vector<Real>& coefficients, Real delta)
		{
			//
			Real r = (left + right) / 2.0f;

			//
			if (right - left < delta)
			{
				return r;
			}

			//
			Real v = 0.0f;
			Real lv = 0.0f;
			Real rv = 0.0f;

			for (size_t i = 0; i != coefficients.size(); ++i)
			{
				lv += Basic::power(left, i) * coefficients[i];
			}
			for (size_t i = 0; i != coefficients.size(); ++i)
			{
				rv += Basic::power(right, i) * coefficients[i];
			}

			//
			if (lv * rv < 0.0f)
			{
				//
				for (size_t i = 0; i != coefficients.size(); ++i)
				{
					v += Basic::power(r, i) * coefficients[i];
				}

				//
				if (v == 0.0f)
				{
					return r;
				}
				else if (v * lv > 0)
				{
					return bisection(r, right, coefficients, delta);
				}
				else
				{
					return bisection(left, r, coefficients, delta);
				}
			}
			else
			{
				Error("Wrong Bisection!");
				return 0.0f;
			}

			Error("Wrong Root!");
			return 0.0f;
		}

		// Sturm Sequence
		inline static void generateSturmSequences(std::vector<Polynomial>& ss, Polynomial& p)
		{
			if (p.getOrder() == 0)
			{
				return;
			}

			if (ss.size() == 0)
			{
				ss.push_back(p);
			}
			else if (ss.size() == 1)
			{
				ss.push_back(p.derived());
			}
			else
			{
				Polynomial quotient;
				Polynomial remainder;
				ss[ss.size() - 2].divid(ss[ss.size() - 1], quotient, remainder);
				ss.push_back(remainder);
				generateSturmSequences(ss, remainder);
			}
		}
	};
}

#endif // __RootFinding_h__
