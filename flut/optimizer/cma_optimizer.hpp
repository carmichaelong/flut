#pragma once

#include "optimizer.hpp"

namespace flut
{
	enum class cma_weights { none = 0, equal = 1, linear = 2, log = 3 };

	class FLUT_API cma_optimizer : public optimizer
	{
	public:

		cma_optimizer( int dim,
			const vec_double& init_mean, const vec_double& init_std,
			objective_func_t func = optimizer::no_objective_func,
			int lambda = 0, int seed = 123,
			cma_weights w = cma_weights::log );

		cma_optimizer( int dim,
			const vec_double& init_mean, const vec_double& init_std,
			const vec_double& lower_bounds, const vec_double& upper_bounds,
			objective_func_t func = optimizer::no_objective_func,
			int lambda = 0, int seed = 123,
			cma_weights w = cma_weights::log );

		virtual ~cma_optimizer();

		void set_boundaries( const vector< double >& lower, const vector< double >& upper );

		const std::vector< std::vector< double > >& sample_population();
		void update_distribution( const std::vector< double >& results );

		int lambda() const;
		int mu() const;
		int dim() const;
		int random_seed() const;

	protected:
		struct pimpl_t* pimpl;
	};
}
