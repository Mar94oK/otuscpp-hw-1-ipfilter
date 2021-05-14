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
			weight +=(std::stoi(it) * bit32IpAddressGroupValues);
			return weight;
		}
	}
	
	return weight;
}


bool sortByWeightAscending(const std::pair<uint32_t , std::vector<std::string>> &a,
			   const std::pair<uint32_t , std::vector<std::string>> &b)
{
	return (a.first < b.first);
}

bool sortByWeightDescending(const std::pair<uint32_t , std::vector<std::string>> &a,
						   const std::pair<uint32_t , std::vector<std::string>> &b)
{
	return (a.first > b.first);
}


int main(int argc, char const *argv[])
{
	std::cout << "Hi, I am Ip-Filter.";
	std::cout << "I am doing only lazy commit.";
	
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

		}
		
		// TODO reverse lexicographically sort
		
		std::sort(weightedIpPool.begin(), weightedIpPool.end());

		std::cout << "My Ip Pool" << std::endl;
		for (auto it : weightedIpPool)
		{
			std::cout << " IP Weight: " << it.first  << " Actual IP: " << IpStringToString(it.second) << std::endl;
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
