#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>

#include <version.h>
#include <BuildNumber.h>

using namespace MyHomework;

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


void SeparateOutput()
{
	std::cout << std::endl;
	std::cout << "=======================" << std::endl;
	std::cout << std::endl;
}

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
	
	//https://stackoverflow.com/questions/7230621/how-can-i-iterate-over-a-packed-variadic-template-argument-list

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
			if (!it)
				return false;
		}
		
		return true;
	}
	
	
	//Not so effective since it must have been run thru all the octets while RangeFor stops at first fault.
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctetsGenericLambdasCPP14(const uint8_t... octets)
	{
		uint32_t octetID = 0;
		return do_for([&](auto octet)
		{
			if (octet != _decimalOctetsRepresentation[octetID++])
				return false;
			
			return true;
		}, octets...);
	}
	
	//Can't be used with my gcc because of:
	//https://stackoverflow.com/questions/43499015/uninitialized-captured-reference-error-when-using-lambdas-in-fold-expression
	//https://gcc.gnu.org/bugzilla/show_bug.cgi?id=47226
	//https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85305
	//Fixed only in
	// "Jason Merrill 2018-05-04 20:21:14 UTC
	//Fixed for 8.2."
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctetsGenericLambdasCPP17 (uint8_t ... octets)
	{
		uint32_t octetID = 0;
		bool filtered = true;
		
		([&] (auto octet)
		{
			if (filtered)
			{
				if (octet != _decimalOctetsRepresentation[octetID++])
				{
					filtered = false;
				}
			}
		} (octets), ...);
		
		return filtered;
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
		
	}
	
public:
	
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctets(const uint8_t... octets)
	{
		return FilterByOctetsRangeBaseLoop(octets...);
	}
	
	
	bool FilterAny(uint8_t octet)
	{
		for (auto it : _decimalOctetsRepresentation)
		{
			if (it == octet)
				return true;
		}
		
		return false;
	}

//"Dead code". Were used to test approaches. Saved as reference.
private:
	
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctetsLambdas(const uint8_t... octets)
	{
		return FilterByOctetsGenericLambdasCPP14(octets...);
	}
	
	template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
	bool FilterByOctetsLambdasCCP17(const uint8_t... octets)
	{
		return FilterByOctetsGenericLambdasCPP17(octets...);
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


//Yes, it is global variable. To save syntax like "auto var = filter(1);"
std::vector<IpAddressIPV4> ipPool;


template <typename ... uint8_t, std::enable_if_t<((sizeof...(uint8_t) < 5) && (sizeof...(uint8_t) > 0)), bool> = true>
std::vector<IpAddressIPV4> Filter(uint8_t... octets)
{
	std::vector<IpAddressIPV4> result;
	for (auto it : ipPool)
	{
		if (it.FilterByOctets(octets...))
			result.push_back(it);
	}
	return result;
}


std::vector<IpAddressIPV4> FilterAny(uint8_t octet)
{
	std::vector<IpAddressIPV4> result;
	for (auto it : ipPool)
	{
		if (it.FilterAny(octet))
			result.push_back(it);
	}
	return result;
}


int main(int argc, char const *argv[])
{
	BuildNumber bn(HomeWorkMajorNumber, HomeWorkMinorNumber, HomeWorkBuildNumber, BuildTime);
	
	std::cout << "build " << bn;
	
	try
	{
		std::vector<std::vector<std::string> > ip_pool;
		std::vector<std::pair<uint64_t , std::vector<std::string>>> weightedIpPool;

		
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

		std::cout << std::endl << "The IP Pool: " << std::endl << std::endl;
		for (auto it : ipPool)
		{
			std::cout << " IP: " << it << std::endl;
		}
		
		SeparateOutput();

		
		// 222.173.235.246
		// 222.130.177.64
		// 222.82.198.61
		// ...
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8
		
		// TODO filter by first byte and output
		// ip = filter(1)
		auto filterByFirstByteResult = Filter(1);
		std::cout << "The IP Pool filtered by first byte (1): " << std::endl << std::endl;
		for (auto it : filterByFirstByteResult)
		{
			std::cout << " IP: " << it << std::endl;
		}
		
		SeparateOutput();
		
		
		// 1.231.69.33
		// 1.87.203.225
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8
		
		// TODO filter by first and second bytes and output
		// ip = filter(46, 70)
		auto filterByFirstAndSecondByteResult = Filter(46, 70);
		std::cout << "The IP Pool filtered by first and second byte (46,70): " << std::endl << std::endl;
		for (auto it : filterByFirstAndSecondByteResult)
		{
			std::cout << " IP: " << it << std::endl;
		}
		
		SeparateOutput();
		
		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76
		
		// TODO filter by any byte and output
		// ip = filter_any(46)
		
		auto filterByAnyByteResult = FilterAny(46);
		std::cout << "The IP Pool filtered by Any Byte (46): " << std::endl << std::endl;
		for (auto it : filterByAnyByteResult)
		{
			std::cout << " IP: " << it << std::endl;
		}
		
		SeparateOutput();
		
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
