#ifndef OTUSCPP_HW_1_BUILDNUMBER_H
#define OTUSCPP_HW_1_BUILDNUMBER_H

#include <string>
#include <ostream>

namespace MyHomework
{
	class BuildNumber
	{

	private:

		const int _majorNumber;
		const int _minorNumber;
		const int _buildNumber;
		std::string _buildTime;

	public:

		const int GetMajorNumber() const { return _majorNumber; }
		const int GetMinorNumber() const { return _minorNumber; }
		const int GetBuildNumber() const { return _buildNumber; }
		const std::string &GetBuildTime() const { return _buildTime; }

	public:

		explicit BuildNumber(int mjrNumber, int minorNumber, int buildNumber, std::string_view builtAt) :
			_majorNumber(mjrNumber), _minorNumber(minorNumber), _buildNumber(buildNumber), _buildTime(builtAt)
		{ }

		friend std::ostream &operator<<(std::ostream &os, const BuildNumber& bn);

	};


	std::ostream &operator<<(std::ostream &os, const BuildNumber& bn)
	{
	  return os << bn.GetMajorNumber() << '.' << bn.GetMinorNumber() << '.' << bn.GetBuildNumber() << " Built at: " << bn.GetBuildTime() << "\n";
	}
}




#endif //OTUSCPP_HW_1_BUILDNUMBER_H
