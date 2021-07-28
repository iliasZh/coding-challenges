#include <iostream>
#include <deque>
#include <algorithm>


/*
 * REF: hackerrank.com/challenges/deque-stl/problem
 */


void printKMax(int arr[], int n, int k)
{
    std::deque<int> d{ arr,arr + k };
    auto GetElemByOffset = [&d](size_t off) -> const int& { return *(d.rend() - off); };
    auto GetMaxElOffset = [&d]() -> size_t { return (d.rend() - std::max_element(d.rbegin(), d.rend())); };
    size_t offset = GetMaxElOffset();

    auto PrintMaxElem = [&]() { std::cout << GetElemByOffset(offset) << ' '; };

    PrintMaxElem();
    for (const int* p = arr + k; p != arr + n; ++p)
    {
        d.pop_front();
        d.push_back(*p);
        --offset;

        if (offset != 0u && GetElemByOffset(offset) <= d.back())
            offset = d.rend() - d.rbegin(); // == k
        else if (offset == 0u)
            offset = GetMaxElOffset();

        PrintMaxElem();
    }
    std::cout << std::endl;
}

void PrintPartialMaxes(const int arr[], const size_t n, const size_t k)
{
    std::deque<size_t> d;
    auto PushIndex = [&d, &arr](size_t idx) -> void
    {
        while (!d.empty() && arr[idx] >= arr[d.back()])
            d.pop_back();
        d.push_back(idx);
    };

    for (size_t i = 0u; i < k; ++i) PushIndex(i);
    std::cout << arr[d.front()] << ' ';

    for (size_t i = k; i < n; ++i)
    {
        if (d.front() <= i - k) d.pop_front();
        PushIndex(i);
        std::cout << arr[d.front()] << ' ';
    }
    std::cout << std::endl;
}

int main()
{

    int t;
    std::cin >> t;
    while (t > 0)
    {
        int n, k;
        std::cin >> n >> k;
        int* arr = new int[n];	// allocating array with a variable size on stack not allowed by MSVC
        for (int i = 0; i < n; ++i)
            std::cin >> arr[i];
        PrintPartialMaxes(arr, n, k);
        delete[] arr;
        t--;
    }
    return 0;
}