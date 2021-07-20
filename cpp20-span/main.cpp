#include <fmt/format.h>
#include <array>
#include <span>

template<class Span>
void print_elements(Span s)
{
		for (const auto& elem : s)
		{
				fmt::print("Num: {}\n", elem);
		}
}


int main()
{
	const auto arr = std::array<int, 4>{1,2,3,4};
	const auto vec = std::vector<int>{5,6,7,8};

  print_elements(std::span{arr});
	print_elements(std::span{vec});

	return 0;
}
