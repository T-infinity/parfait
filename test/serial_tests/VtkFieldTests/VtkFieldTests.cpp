#include "vtk_field.h"
#include <vector>
#include "CppUTest/CommandLineTestRunner.h"

using std::vector;

TEST_GROUP(VtkFieldTests)
{
	void setup()
	{
	}
	void teardown()
	{
	}
};

TEST(VtkFieldTests,TestInt)
{
	vector<int> vec(4,1);
	VtkField<int> field(true,"TestField",1,4,vec.data());
	int x = field.getElement(0);
	LONGS_EQUAL(1,x);
	LONGS_EQUAL(INT32_FIELD,field.Type());
}

TEST(VtkFieldTests,TestFloat)
{
	vector<float> vec(4,1.1);
	VtkField<float> field(true,"TestField",1,4,vec.data());
	float x = field.getElement(0);
	DOUBLES_EQUAL(1.1,x,1.0e-5);
	LONGS_EQUAL(FLOAT32_FIELD,field.Type());
}

TEST(VtkFieldTests,TestDouble)
{
	vector<double> vec(4,1.1);
	VtkField<double> field(true,"TestField",1,4,vec.data());
	double x = field.getElement(0);
	DOUBLES_EQUAL(1.1,x,1.0e-15);
	LONGS_EQUAL(FLOAT64_FIELD,field.Type());
}

TEST(VtkFieldTests,TestIntVectorData)
{
	vector<int> vec(4,1);
	VtkField<int> field(true,"TestField",4,1,vec.data());
	const int *x = field.getBlock(0);
	LONGS_EQUAL(1,x[0]);
}

TEST(VtkFieldTests,TestFloatVectorData)
{
	vector<float> vec(4,1.1);
	VtkField<float> field(true,"TestField",4,1,vec.data());
	const float *x = field.getBlock(0);
	DOUBLES_EQUAL(1.1,x[0],1.0e-5);
}

TEST(VtkFieldTests,TestDoubleVectorData)
{
	vector<double> vec(4,1.1);
	VtkField<double> field(true,"TestField",4,1,vec.data());
	const double *x = field.getBlock(0);
	DOUBLES_EQUAL(1.1,x[0],1.0e-15);
}

TEST(VtkFieldTests,TestAppend)
{
	vector<double> vec(1,1.1);
	VtkField<double> field(false,"TestField",1,1,vec.data());
	//field.append("appended_field.dat");
}
