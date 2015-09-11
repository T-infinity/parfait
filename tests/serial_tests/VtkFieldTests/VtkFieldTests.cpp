#include <catch.hpp>
#include "VtkField.h"
#include <vector>

using std::vector;
using namespace Parfait;


TEST_CASE("VtkFieldTests,TestInt") {
	vector<int> vec(4,1);
	VtkField<int> field(true,"TestField",1,4,vec.data());
	int x = field.getElement(0);
	REQUIRE(1 == x);
	REQUIRE(INT32_FIELD == field.Type());
}

TEST_CASE("VtkFieldTests,TestFloat") {
	vector<float> vec(4,1.1);
	VtkField<float> field(true,"TestField",1,4,vec.data());
	float x = field.getElement(0);
	REQUIRE(1.1 == Approx(x));
	REQUIRE(FLOAT32_FIELD == field.Type());
}

TEST_CASE("VtkFieldTests,TestDouble") {
	vector<double> vec(4,1.1);
	VtkField<double> field(true,"TestField",1,4,vec.data());
	double x = field.getElement(0);
	REQUIRE(1.1 == Approx(x));
	REQUIRE(FLOAT64_FIELD == field.Type());
}

TEST_CASE("VtkFieldTests,TestIntVectorData") {
	vector<int> vec(4,1);
	VtkField<int> field(true,"TestField",4,1,vec.data());
	const int *x = field.getBlock(0);
	REQUIRE(1 == x[0]);
}

TEST_CASE("VtkFieldTests,TestFloatVectorData") {
	vector<float> vec(4,1.1);
	VtkField<float> field(true,"TestField",4,1,vec.data());
	const float *x = field.getBlock(0);
	REQUIRE(1.1 == Approx(x[0]));
}

TEST_CASE("VtkFieldTests,TestDoubleVectorData") {
	vector<double> vec(4,1.1);
	VtkField<double> field(true,"TestField",4,1,vec.data());
	const double *x = field.getBlock(0);
	REQUIRE(1.1 == x[0]);
}

TEST_CASE("VtkFieldTests,TestAppend") {
	vector<double> vec(1,1.1);
	VtkField<double> field(false,"TestField",1,1,vec.data());
}
