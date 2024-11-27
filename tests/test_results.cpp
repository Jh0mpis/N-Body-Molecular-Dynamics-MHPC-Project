
#include "gtest/gtest.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
/* Test the result for 108 atoms*/
TEST(OUTPUT_RESULT, REFRENCE_RESULT)
{
    double tol = 10E-3;
    std::string results_filename = "../examples/argon_108.dat";
    std::string reference_filename = "../reference/argon_108.dat";

    std::ifstream results_file(results_filename);
    std::ifstream reference_file(reference_filename);


    std::string results_line; 
    std::string reference_line;

    std::vector<double> results_data;
    std::vector<double> reference_data;



    /* Reading the results data*/
    if(!results_file.is_open()){
        std::cerr<<"There has been an issue opening the file"<<results_filename<<std::endl;
    }
    while(std::getline(results_file, results_line)){
        std::istringstream iss(results_line);
        double value;
        while(iss >> value){
            results_data.push_back(value);
        }
    }
    results_file.close();

    /*Reading the refrence data*/
    if(!reference_file.is_open()){
        std::cerr<<"There has been an issue opening the file"<<reference_filename<<std::endl;
    }
    while(std::getline(reference_file, reference_line)){
        std::istringstream iss(reference_line);
        double value;
        while(iss >> value){
            reference_data.push_back(value);
        }
    }
    reference_file.close();

    ASSERT_EQ(reference_data.size(), results_data.size());
    size_t nrows{(reference_data.size() / 5) / 5000}; /*Checking the first 2000 rows*/
    int ncols{5};
    for(int i=0; i <  nrows;i++){
        for (int j = 0; j < ncols; j++)
        {
            EXPECT_NEAR(reference_data[i * 5 + j], results_data[i * 5 + j], tol)
            <<"There tests fail at index i: "<<i<<" and j: "<<j;
        }
        
    }
}
/* Test the result for 2916 atoms*/
TEST(OUTPUT_RESULT2, REFRENCE_RESULT2)
{
    double tol = 10E-3;
    std::string results_filename = "../examples/argon_2916.dat";
    std::string reference_filename = "../reference/argon_2916.dat";

    std::ifstream results_file(results_filename);
    std::ifstream reference_file(reference_filename);


    std::string results_line; 
    std::string reference_line;

    std::vector<double> results_data;
    std::vector<double> reference_data;



    /* Reading the results data*/
    if(!results_file.is_open()){
        std::cerr<<"There has been an issue opening the file"<<results_filename<<std::endl;
    }
    while(std::getline(results_file, results_line)){
        std::istringstream iss(results_line);
        double value;
        while(iss >> value){
            results_data.push_back(value);
        }
    }
    results_file.close();

    /*Reading the refrence data*/
    if(!reference_file.is_open()){
        std::cerr<<"There has been an issue opening the file"<<reference_filename<<std::endl;
    }
    while(std::getline(reference_file, reference_line)){
        std::istringstream iss(reference_line);
        double value;
        while(iss >> value){
            reference_data.push_back(value);
        }
    }
    reference_file.close();

    ASSERT_EQ(reference_data.size(), results_data.size());
    size_t nrows{(reference_data.size() / 5) / 5000}; /*Checking the first 2000 rows*/
    int ncols{5};
    for(int i=0; i <  nrows;i++){
        for (int j = 0; j < ncols; j++)
        {
            EXPECT_NEAR(reference_data[i * 5 + j], results_data[i * 5 + j], tol)
            <<"There tests fail at index i: "<<i<<" and j: "<<j;
        }
        
    }
}

/* Test the result for 78732 atoms*/
TEST(OUTPUT_RESULT3, REFRENCE_RESULT3)
{
    double tol = 10E-3;
    std::string results_filename = "../examples/argon_78732.dat";
    std::string reference_filename = "../reference/argon_78732.dat";

    std::ifstream results_file(results_filename);
    std::ifstream reference_file(reference_filename);


    std::string results_line; 
    std::string reference_line;

    std::vector<double> results_data;
    std::vector<double> reference_data;



    /* Reading the results data*/
    if(!results_file.is_open()){
        std::cerr<<"There has been an issue opening the file"<<results_filename<<std::endl;
    }
    while(std::getline(results_file, results_line)){
        std::istringstream iss(results_line);
        double value;
        while(iss >> value){
            results_data.push_back(value);
        }
    }
    results_file.close();

    /*Reading the refrence data*/
    if(!reference_file.is_open()){
        std::cerr<<"There has been an issue opening the file"<<reference_filename<<std::endl;
    }
    while(std::getline(reference_file, reference_line)){
        std::istringstream iss(reference_line);
        double value;
        while(iss >> value){
            reference_data.push_back(value);
        }
    }
    reference_file.close();

    ASSERT_EQ(reference_data.size(), results_data.size());
    size_t nrows{(reference_data.size() / 5) / 5000}; /*Checking the first 2000 rows*/
    int ncols{5};
    for(int i=0; i <  nrows;i++){
        for (int j = 0; j < ncols; j++)
        {
            EXPECT_NEAR(reference_data[i * 5 + j], results_data[i * 5 + j], tol)
            <<"There tests fail at index i: "<<i<<" and j: "<<j;
        }
        
    }
}

int main(int argc, char** argv) {
    // Initialize Google Test framework
    ::testing::InitGoogleTest(&argc, argv);

    // Run all tests and return the result
    return RUN_ALL_TESTS();
}