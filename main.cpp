#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
	std::vector<std::string> r;
	
	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(d);
	while(stop != std::string::npos)
	{
		r.push_back(str.substr(start, stop - start));
		
		start = stop + 1;
		stop = str.find_first_of(d, start);
	}
	
	r.push_back(str.substr(start));
	
	return r;
}

//C++20 required
//std::vector<std::string> Foo(const std::string& input)
//{
//	std::vector<std::string> output;
//
//	for(const auto& i : input | std::ranges::views::split(' ')) {
//		output.emplace_back(std::cbegin(i), std::cend(i));
//	}
//	return output;
//}


class IpAddressIPV4
{

private:
	
	static const uint8_t delimiter = '.';
	static const uint8_t numberOfOctets = 4;
	static const uint8_t maximumValidOctetValue = 255;
	
private:
	
	
	bool ValidateOctetSize(const std::string& octet)
	{
		return ( (octet.size() < 4) && (octet.size() > 0) );
	}
	
	
	bool ValidateOctetValue(const std::string& octet)
	{
		return (std::stoi(octet) <= maximumValidOctetValue);
	}
	
	bool ValidateOctet(const std::string& octet)
	{
		return (ValidateOctetSize(octet) && ValidateOctetValue(octet));
	}
	
	void ConvertOctetsToDecimal()
	{
		_decimalOctetsRepresentation.push_back(std::stoi(vectorizedValue[0]));
		_decimalOctetsRepresentation.push_back(std::stoi(vectorizedValue[1]));
		_decimalOctetsRepresentation.push_back(std::stoi(vectorizedValue[2]));
		_decimalOctetsRepresentation.push_back(std::stoi(vectorizedValue[3]));
		
	}
	
	uint64_t GetIpWeight(const std::vector<std::string> &v)
	{
		if (v.size() > 4)
			return 0;
		
		uint64_t weight = 0;
		uint8_t powValue = 4;
		uint32_t bit32IpAddressGroupValues = 256; //one group, ex 126bit addr - 65536 values
		for (auto it : v)
		{
			if (powValue != 1)
			{
				weight += (std::stoi(it) * ( std::pow(bit32IpAddressGroupValues, powValue)));
				powValue--;
			}
			else
			{
				weight += (std::stoi(it) * bit32IpAddressGroupValues);
				return weight;
			}
		}
		
		return weight;
	}
	
private:
	
	std::string rawValue;
	std::vector<std::string> vectorizedValue;
	uint64_t weight;
	
	std::vector<int32_t > _decimalOctetsRepresentation;
	
public:
	
	friend bool operator> (const IpAddressIPV4 &lhs, const IpAddressIPV4 &rhs);
	friend std::ostream& operator<<(std::ostream& os, const IpAddressIPV4& addr);

private:
	
	bool CheckFirstOctet()
	{
	
	}
	
	//const std::size_t n = sizeof...(T);
	//https://stackoverflow.com/questions/12024304/c11-number-of-variadic-template-function-parameters
	//
	
	//C++20 ---> using "requires"
	//static_assert is BAD somehow. WHy???
	//C++14 - use const expr + SFINAE
	//Справедливо ли это?
	//https://coderoad.ru/11984768/%D0%9A%D0%BE%D0%B3%D0%B4%D0%B0-%D0%B8%D1%81%D0%BF%D0%BE%D0%BB%D1%8C%D0%B7%D0%BE%D0%B2%D0%B0%D1%82%D1%8C-static_assert-%D0%B2%D0%BC%D0%B5%D1%81%D1%82%D0%BE-SFINAE
	
	//http://www.cplusplus.com/forum/general/142625/
//	template<typename... uint8_t>
//	void FilterByOctets(const uint8_t... t)
//	{
//		constexpr std::size_t n = sizeof...(uint8_t) > 4;
//		if
//	}
	
	//Question: How to check
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctetsRangeBaseLoop ( const uint8_t... octets)
	{
		uint32_t octetID = 0;
		for(const auto it : {octets...})
		{
			if (it != _decimalOctetsRepresentation[octetID++])
				return false;
		}
		return true;
	}
	
	//C++14 Lambda
	template <class F, class... Args>
	bool do_for(F f, Args... args)
	{
		int result[] = {(f(args))...};
		
		for(const auto it : result)
		{
			std::cout << "Result: " << it << std::endl;
		}
		
		for(const auto it : result)
		{
			if (!it)
			{
				std::cout << "Returning FALSE!";
				return false;
			}
			
		}
		
		return true;
	}
	
	
	//Not so effective since it must have been run thru all the octets while RangeFor stops at first fault.
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctetsGenericLambdas(const uint8_t... octets)
	{
		uint32_t octetID = 0;
		return do_for([&](auto octet)
		{
			std::cout << "Octet: " << octet << std::endl;;
			if (octet != _decimalOctetsRepresentation[octetID++])
			{
				std::cout << "Returning false!";
				return false;
			}
			
			std::cout << "Returning true!" << std::endl;
			return true;
		}, octets...);
	}
	
public:
	
	explicit IpAddressIPV4(std::string value)
	{
		vectorizedValue = split(value, delimiter);
		
		if (vectorizedValue.size() != numberOfOctets )
			throw std::runtime_error("Bad string format for IPV4 Address: " + value + " Check octets. Vectorized value size: " + std::to_string(vectorizedValue.size()));
		
		for (auto it : vectorizedValue)
		{
			if (!ValidateOctet(it))
				throw std::runtime_error("Bad string format for IPV4 Address: " + value + " Check octet value: " + it);
		}
		
		ConvertOctetsToDecimal();
	
		rawValue = value;
		
		weight = this->GetIpWeight(vectorizedValue);
		
		if (FilterByOctetsRangeBaseLoop(1))
			std::cout << "Has 1 in first octet!" << std::endl;
	}
	
public:
	
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctets(const uint8_t... octets)
	{
		return FilterByOctetsRangeBaseLoop(octets...);
	}
	
	
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctetsLambdas(const uint8_t... octets)
	{
		return FilterByOctetsGenericLambdas(octets...);
	}

};


inline bool operator> (const IpAddressIPV4 &lhs, const IpAddressIPV4 &rhs)
{
	return lhs.weight > rhs.weight;
}


inline std::ostream& operator<<(std::ostream& os, const IpAddressIPV4& addr)
{
	os << addr.rawValue;
	return os;
}

template <typename Iter>
Iter next(Iter iter)
{
	return ++iter;
}


std::string IpStringToString(const std::vector<std::string> &v)
{
	std::string result;
	for (auto it : v)
	{
		result += it + ".";
	}
	return result;
}

//https://stackoverflow.com/questions/13756235/how-to-sort-ip-address-in-ascending-order/34441987
uint64_t GetIpWeight(const std::vector<std::string> &v)
{
	if (v.size() > 4)
		return 0;
	
	uint64_t weight = 0;
	uint8_t powValue = 4;
	uint32_t bit32IpAddressGroupValues = 256; //one group, ex 126bit addr - 65536 values
	for (auto it : v)
	{
		if (powValue != 1)
		{
			weight += (std::stoi(it) * ( std::pow(bit32IpAddressGroupValues, powValue)));
			powValue--;
		}
		else
		{
			weight += (std::stoi(it) * bit32IpAddressGroupValues);
			return weight;
		}
	}
	
	return weight;
}


int main(int argc, char const *argv[])
{
	std::cout << "Hi, I am Ip-Filter.";
	std::cout << "I am doing only lazy commit.";
	
	IpAddressIPV4 myTestAddr = IpAddressIPV4("1.2.3.4");
	IpAddressIPV4 anotherMyTestAddr = IpAddressIPV4("2.2.3.4");
	IpAddressIPV4 yetAnotherMyTestAddr = IpAddressIPV4("3.2.3.4");

//	if (myTestAddr.FilterByOctets(1))
//		std::cout << "Ip Addr: " << myTestAddr << " Filtered!" << std::endl;
//
//	if (myTestAddr.FilterByOctets(2,2,3))
//		std::cout << "Ip Addr: " << myTestAddr << " Filtered!" << std::endl;
//
//	if (myTestAddr.FilterByOctets(2,2,3,4))
//		std::cout << "Ip Addr: " << myTestAddr << " Filtered!"  << std::endl;
//
//	if (myTestAddr.FilterByOctets(2))
//		std::cout << "Ip Addr: " << myTestAddr << " Filtered!"  << std::endl;
//
	if (anotherMyTestAddr.FilterByOctetsLambdas(2,2,3,4))
		std::cout << "Ip Addr: " << anotherMyTestAddr << " Filtered!" << std::endl;
	//myTestAddr.FilterByOctetsLambdas(2,2);
	//myTestAddr.FilterByOctetsLambdas(3,2,3);
	
	try
	{
		std::vector<std::vector<std::string> > ip_pool;
		std::vector<std::pair<uint64_t , std::vector<std::string>>> weightedIpPool;
		std::vector<IpAddressIPV4> ipPool;
		
		for(std::string line; std::getline(std::cin, line);)
		{
			std::vector<std::string> v = split(line, '\t');
			ip_pool.push_back(split(v.at(0), '.'));
		
			std::string actualIpAddr = v.at(0);
			uint64_t ipAddrWeight = GetIpWeight(split(actualIpAddr, '.'));

			weightedIpPool.push_back(std::make_pair(ipAddrWeight, split(actualIpAddr, '.')));
			
			
			try
			{
				IpAddressIPV4 addr = IpAddressIPV4(v.at(0));
				ipPool.push_back(addr);
			}
			catch(std::exception& e)
			{
				std::cout << e.what() << std::endl;
				std::cout << "Failed adding Ip Line: " << v.at(0) << std::endl;
			}
			
		}
		
		// TODO reverse lexicographically sort
		
		//std::sort(weightedIpPool.begin(), weightedIpPool.end(), sortByWeightAscending);
		std::sort(weightedIpPool.begin(), weightedIpPool.end(),
				[](auto &left, auto &right)
						{
							return left.first > right.first;
						});

		std::sort(ipPool.begin(), ipPool.end(),
				[](auto &left, auto &right)
				{
					return left > right;
				});
		
//		std::cout << "My Ip Pool" << std::endl;
//		for (auto it : weightedIpPool)
//		{
//			std::cout << " IP Weight: " << it.first  << " Actual IP: " << IpStringToString(it.second) << std::endl;
//		}


		std::cout << "The Other IP Pool" << std::endl;
		for (auto it : ipPool)
		{
			std::cout << " IP: " << it << std::endl;
		}

		
		
		
//		for(std::vector<std::vector<std::string> >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
//		{
//            std::cout << "======";
//		    for(std::vector<std::string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
//			{
//				if (ip_part != ip->cbegin())
//				{
//					std::cout << ".";
//
//				}
//				std::cout << *ip_part;
//			}
//			std::cout << std::endl;
//            std::cout << "======";
//		}
		
		// 222.173.235.246
		// 222.130.177.64
		// 222.82.198.61
		// ...
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8
		
		// TODO filter by first byte and output
		// ip = filter(1)
		
		// 1.231.69.33
		// 1.87.203.225
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8
		
		// TODO filter by first and second bytes and output
		// ip = filter(46, 70)
		
		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76
		
		// TODO filter by any byte and output
		// ip = filter_any(46)
		
		// 186.204.34.46
		// 186.46.222.194
		// 185.46.87.231
		// 185.46.86.132
		// 185.46.86.131
		// 185.46.86.131
		// 185.46.86.22
		// 185.46.85.204
		// 185.46.85.78
		// 68.46.218.208
		// 46.251.197.23
		// 46.223.254.56
		// 46.223.254.56
		// 46.182.19.219
		// 46.161.63.66
		// 46.161.61.51
		// 46.161.60.92
		// 46.161.60.35
		// 46.161.58.202
		// 46.161.56.241
		// 46.161.56.203
		// 46.161.56.174
		// 46.161.56.106
		// 46.161.56.106
		// 46.101.163.119
		// 46.101.127.145
		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76
		// 46.55.46.98
		// 46.49.43.85
		// 39.46.86.85
		// 5.189.203.46
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}
