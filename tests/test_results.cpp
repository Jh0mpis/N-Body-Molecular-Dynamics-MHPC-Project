
#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iomanip>
/*Error tolerance*/
const double tol = 10E-6;

void get_results(std::vector<double> &data, std::string filename)
{


    std::ifstream file(filename);

    std::string line;


    /* Reading the results data*/
    if (!file.is_open())
    {
        std::cerr << "There has been an issue opening the file" << filename << std::endl;
    }
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        double value;
        while (iss >> value)
        {
            data.push_back(value);
        }
    }
    file.close();
}

/* Test the result for 108 atoms*/
TEST(OUTPUT_RESULT, REFRENCE_RESULT)
{

    std::vector<double> results_data;
    std::vector<double> reference_data;

    get_results(results_data, "../examples/argon_108.dat");
    get_results(reference_data, "../reference/argon_108.dat");

    ASSERT_EQ(reference_data.size(), results_data.size());
    size_t nrows = (reference_data.size() / 5) / 5000; /*Checking the first 2000 rows*/
    int ncols{5};
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            EXPECT_EQ(reference_data[i * 5 + j], results_data[i * 5 + j]);
        }
    }
}
/* Test the result for 2916 atoms*/
TEST(OUTPUT_RESULT2, REFRENCE_RESULT2)
{
    std::vector<double> results_data;
    std::vector<double> reference_data;

    get_results(results_data, "../examples/argon_2916.dat");
    get_results(reference_data, "../reference/argon_2916.dat");

    ASSERT_EQ(reference_data.size(), results_data.size());
    size_t nrows = reference_data.size() / 5; /*Checking all the results*/
    int ncols{5};
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            EXPECT_NEAR(reference_data[i * 5 + j], results_data[i * 5 + j], tol)
                << "There tests fail at index i: " << i << " and j: " << j;
        }
    }
}

/* Test the result for 78732 atoms*/
TEST(OUTPUT_RESULT3, REFRENCE_RESULT3)
{
    std::vector<double> results_data;
    std::vector<double> reference_data;

    get_results(results_data, "../examples/argon_78732.dat");
    get_results(reference_data, "../reference/argon_78732.dat");

    ASSERT_EQ(reference_data.size(), results_data.size());
    size_t nrows = reference_data.size() / 5; /*Checking all the results*/
    int ncols{5};
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            EXPECT_NEAR(reference_data[i * 5 + j], results_data[i * 5 + j], tol)
                << "There tests fail at index i: " << i << " and j: " << j;
        }
    }
}
