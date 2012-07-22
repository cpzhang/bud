/********************************************************************
**	file: 		Polynomial.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-1-27
**	purpose:	
*********************************************************************/
#ifndef __Polynomial_h__
#define __Polynomial_h__

//
#include "Common.h"
#include "Basic.h"

//
namespace Zen
{
	class _MATH_EXPORT_ Polynomial
	{
	public:
		typedef std::pair<size_t, Real> OrderCoefficientPair;
		typedef std::vector<OrderCoefficientPair> OrderCoefficientPairVector;
		OrderCoefficientPairVector orderCoefficients;

	public:
		Polynomial()
		{

		}

		Polynomial(const Polynomial& p)
		{
			for (size_t i = 0; i != p.orderCoefficients.size(); ++i)
			{
				orderCoefficients[i] = p.orderCoefficients[i];
			}
		}
	public:
		static size_t tOrder;
		static bool cmp(const OrderCoefficientPair& a, const OrderCoefficientPair& b)
		{
			return a.first < b.first;
		}

		static bool predicate(const OrderCoefficientPair& a)
		{
			return a.first == tOrder;
		}
	public:

		inline Real& operator[](size_t order)
		{
			tOrder = order;
			OrderCoefficientPairVector::iterator it = std::find_if(orderCoefficients.begin(), orderCoefficients.end(), predicate);
		
			return it->second;
		}

		inline bool isOrderPresent(size_t order)
		{
			tOrder = order;
			OrderCoefficientPairVector::iterator it = std::find_if(orderCoefficients.begin(), orderCoefficients.end(), predicate);

			return it != orderCoefficients.end();
		}

		inline u32 getOrder()
		{
			if (orderCoefficients.size() == 0)
			{
				return 0;
			}
			
			OrderCoefficientPairVector::iterator it = std::max_element(orderCoefficients.begin(), orderCoefficients.end(), cmp);
			
			return it->first;
		}

		inline Real getValue(Real x)
		{
			Real v = 0.0f;
			for (size_t i = 0; i != orderCoefficients.size(); ++i)
			{
				v += orderCoefficients[i].second * Basic::power(x, (Real)orderCoefficients[i].first);
			}

			return v;
		}

		inline Polynomial& derived()
		{
			for (size_t i = 0; i != orderCoefficients.size() - 1; ++i)
			{
				orderCoefficients[i].second *= orderCoefficients[i].first--;
			}

			return *this;
		}

		inline void divid(Polynomial& divisor, Polynomial& quotient, Polynomial& remainder) const
		{
			if (remainder.getOrder() < divisor.getOrder())
			{
				return;
			}


		}
	};
}

#endif // __Polynomial_h__
