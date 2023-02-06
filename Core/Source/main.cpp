#include "pch.h"

#include "core/search_algorithms.h"

int main()
{
	const core::search_algorithms_specification& search_algorithms_specification = {
		{"Search Algorithms", 1000,1000, true},
		{glm::uvec2(50, 50), 3}
	};
	const unique<core::search_algorithms>& search_algorithms = create_unique<core::search_algorithms>(search_algorithms_specification);
	search_algorithms->run();
	return 0;
}
