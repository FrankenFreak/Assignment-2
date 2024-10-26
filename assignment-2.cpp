// PHYS30762 Programming in C++
// Assignment 2
// Author: Clive Marvelous (10916086)
// Date: 21/02/24

// Program to compute mean, standard deviation and standard
// error of the a set of courses 
// Data is read from file

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<string>
#include<vector>
#include<tuple>
#include<algorithm>
#include <numeric>

// Functions to compute mean, standard deviation or for other tasks.
void getInput(const std::string& prompt, std::string& k);
void getInput(const std::string& prompt, bool& k);
void getInput(const std::string& prompt, int& k);
float st_err(float sigma, int N);
float st_dev(std::vector<float> vec, float constant, int N);
float sum(std::vector<float> vec);
float mean(std::vector<float> vec, int N);

// Main function

int main()
{
  // Define variables
  std::string filename;
  int year_preference; // Course year
  bool by_year; // True if list courses for specific year and false if list all courses
  bool by_title; // True if sort courses by title and false if by code  

  // Ask user for preferences
  getInput("Enter filename: ", filename);
  // Open file
  std::ifstream file(filename);
  if(file.fail()) 
  {
      std::cerr<<"Error: Unable to open file."<<std::endl;
      return 1;
  }

  // Ask user for preferences
  getInput("List all courses (Enter 0) or list courses for specific year (Enter 1): ", by_year);
  if(by_year)
  {
    getInput("Enter course year to list: ", year_preference); 
  }
  getInput("Sort courses by course code (Enter 0) or by course title (Enter 1): ", by_title);
  
  // Declare vector variables and temporary variables
  std::vector<std::pair<int, std::string>> data; 
  std::vector<float> relevant_score; // Relevant scores based on user choice
  float score;
  int code;
  std::string title;

  // Course code upper and lower limits
  int lower_code{year_preference*10000};
  int upper_code{(year_preference+1)*10000};

  // Extract data
  while(file>>score>>code) 
  {
    file>>std::ws; // Remove whitespace
    std::getline(file, title);
    if(by_year && code>=lower_code && code<upper_code)
    {
      data.emplace_back(code, title);
      relevant_score.push_back(score);
    }
    else if(!by_year)
    {
      data.emplace_back(code, title);
      relevant_score.push_back(score);
    }
  } 

  // Close file
  file.close();

  // Sort data
  sort(data.begin(), data.end(), [by_title](const auto& x, const auto& y) 
  {
    if(by_title) // By title
      return x.second < y.second;
    else // By code 
      return x.first < y.first;
  });
  
  // Statistics
  size_t N{relevant_score.size()}; // Size
  float mu{mean(relevant_score, N)}; // mean
  float sigma{st_dev(relevant_score, mu, N)}; // standard deviation
  float sigma_mu{st_err(sigma, N)}; // standard error

  // List courses
  std::cout<<std::endl;
  for(auto iterator=data.begin(); iterator!=data.end(); ++iterator) 
  {
    std::ostringstream output_stream;
    output_stream<<"PHYS "<<iterator->first<<" "<< iterator->second;
    std::string output_filename{output_stream.str()};
    std::cout<<output_filename<<std::endl;
    output_stream.str(""); 
  }
  
  // Print values
  std::cout<<std::endl;
  std::cout.precision(3);
  std::cout<<"Number of records: "<<N<<std::endl;
  std::cout<<"Mean score: "<<mu<<std::endl;
  std::cout<<"Standard deviation: "<<sigma<<std::endl;
  std::cout<<"Standard error: "<<sigma_mu<<std::endl;

  // Clear memory
  relevant_score.clear();
  data.clear();
  return 0; 
}

// Sum function
float sum(std::vector<float> vec)
{
  return std::accumulate(vec.begin(), vec.end(), 0.0f);
}

// Mean function
float mean(std::vector<float> vec, int N)
{
  return sum(vec)/N;
}

// Standard deviation function
float st_dev(std::vector<float> vec, float constant, int N)
{
  std::vector<float> temp_vec(N);
  // Perform (x_i - mu)**2 to vector vec
  std::transform(vec.begin(), vec.end(), temp_vec.begin(), [constant](float temp)
  { 
    return pow(temp-constant, 2); 
  });

  return sqrt(sum(temp_vec)/(N-1));
}

// Standard error function
float st_err(float sigma, int N)
{
  return sigma/sqrt(N);
}

// Ask user for string input and validate input
void getInput(const std::string& prompt, std::string& k) 
{
  // Ask user for input
  std::cout<<prompt;
  std::cin>>k;
  
  // Check for validity 
  while(std::cin.fail()) 
  {
    std::cout<<"Sorry, your input was invalid.\n"<<prompt; 
    // Clear fail bit and ignore bad input
    std::cin.clear(); 
    std::cin.ignore(); 
    std::cin>>k;
  }
}

// Ask user for bool input and validate input
void getInput(const std::string& prompt, bool& k) 
{
  // Ask user for input
  std::cout<<prompt;
  std::cin>>k;
  
  // Check for validity 
  while(std::cin.fail()) 
  {
    std::cout<<"Sorry, your input was invalid.\n"<<prompt; 
    // Clear fail bit and ignore bad input
    std::cin.clear(); 
    std::cin.ignore(); 
    std::cin>>k;
  }
}

// Ask user for int input and validate input
void getInput(const std::string& prompt, int& k) 
{
  // Ask user for input
  std::cout<<prompt;
  std::cin>>k;
  
  // Check for validity 
  while(std::cin.fail() or k>=5 or k<=0) 
  {
    std::cout<<"Sorry, your input was invalid.\n"<<prompt; 
    // Clear fail bit and ignore bad input
    std::cin.clear(); 
    std::cin.ignore(); 
    std::cin>>k;
  }
}
