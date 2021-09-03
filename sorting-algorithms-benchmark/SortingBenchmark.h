#pragma once

#include "ArrayGenerators.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <functional>
#include <thread>

template <class T, class Comparator>
class Benchmark
{
public:
	class SortingMethod
	{
	public:
		SortingMethod(std::string name, std::function<void(T*, T*, Comparator)> method)
			: name{ name }
			, method{ method }
		{}
		std::string Name() const { return name; }

		void Run(T* begin, T* end, const Comparator& cmp) const
		{
			method(begin, end, cmp);
		}
	private:
		std::string name;
		std::function<void(T*, T*, Comparator)> method;
	};

	Benchmark(std::string outputFileNameNoExtension = "benchmark", unsigned nRuns = 1000u)
		: outputFile{ outputFileNameNoExtension + ".csv" }
		, nRuns{ nRuns }
		, lta{ }
		, timer{ }
	{
		pOut = std::make_unique<std::ofstream>(outputFile);
		if (!(*pOut))
			throw std::exception(("Error while opening file \"" + outputFile + "\"").c_str());
	}
	~Benchmark()
	{
		pOut->close();
	}
	void Run(const T* srcArrayBegin, const T* srcArrayEnd, const Comparator& cmp)
	{
		if (srcArrayEnd - srcArrayBegin < (int)arrSizes[arrSizes.size() - 1])
			throw std::logic_error("Benchmarking source array too small!");
		
		std::ofstream& out = *pOut;

		if (consoleLog)
		{
			std::cout << "BENCHMARK STARTED\n\n";
			std::cout << "NUMBER OF RUNS: " << nRuns << "\n\n";
		}

		out << "ELEMENTS";
		for (auto& m : testMethods)
		{
			out << ',' << m.Name();
		}
		out << '\n';

		for (unsigned arrSize : arrSizes)
		{
			out << arrSize << ',';
			T* arr = new T[arrSize]; // !!!CHECK FOR DELETE[] AT THE END OF SCOPE
			Copy(arr, srcArrayBegin, arrSize);

			for (unsigned i = 0; i < testMethods.size(); ++i)
			{
				if (consoleLog)
				{
					std::cout << "RUNNING " << testMethods[i].Name() << " ON "
						<< arrSize << " ELEMENT ARRAY...\n";
				}

				timer.Mark();	// SORT BEGIN
				for (unsigned n = 0; n < nRuns; ++n)
				{
					testMethods[i].Run(arr, arr + arrSize, cmp);
					Copy(arr, srcArrayBegin, arrSize);
				}
				float time = timer.Mark();	// SORT END
				out << time;

				if (consoleLog)
				{
					std::cout << testMethods[i].Name() << " DONE: " << time << "s\n\n";
				}


				if (consoleLog)
					std::cout << "SLEEPING...\n";
				
				std::this_thread::sleep_for(std::chrono::seconds(1));
				
				if (consoleLog)
					std::cout << "WAKING UP\n\n";


				if (i + 1 == testMethods.size())
				{
					out << '\n';
				}
				else
				{
					out << ',';
				}
			}

			delete[] arr;
		}
	}
	void AddSortingMethod(std::string name, std::function<void(T*, T*, Comparator)> method)
	{
		testMethods.emplace_back(name, method);
	}
	void EnableLogging() { consoleLog = true; }
	void DisableLogging() { consoleLog = false; }
private:
	void Copy(T* tgt, const T* src, unsigned size)
	{
		for (unsigned i = 0; i < size; ++i)
		{
			tgt[i] = src[i];
		}
	}
private:
	bool consoleLog = true;
	unsigned nRuns;
	Timer timer;
	std::unique_ptr<std::ofstream> pOut;
	const std::vector<unsigned> arrSizes =
	{ 
		100,200,300,400,500,600,700,800,900,
		1000,1200,1400,1600,1800,2000,2500,3000,4000,5000
	};
	//const std::vector<unsigned> arrSizes =
	//{
	//	1000000
	//};
	//const std::vector<unsigned> arrSizes =
	//{
	//	5,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200
	//};
	std::vector<SortingMethod> testMethods;
	std::string outputFile;
	LargeTestArrays lta;
};