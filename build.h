// build.h
// Charles Emerson
// 21 September 2019
// Updated: 21 September 2019
//
// For CS 411 Fall 2019
// Source for function build
// Solution for Assignment 2, Exercise A


#ifndef FILE_BUILD_H_INCLUDED
#define FILE_BUILD_H_INCLUDED

#include <vector>
#include<algorithm>
#include "build_utility.h"

void sortAndCull(std::vector<std::vector<int>> & bridges)
{
    std::sort(bridges.begin(), bridges.end(), [](const auto & a, const auto & b)
    {
        return a[0] < b[0] ||
            (a[0] == b[0] &&
                (a[1] < b[1]) ||
                (a[1] == b[1] && (a[2] >= b[2])));
    });
}

int build_worker(const std::vector<std::vector<int>> & bridges, int i = 0, int w = 0, int e = 0)
{
    const int BRIDGES_SIZE = int(bridges.size());

    if (i >= BRIDGES_SIZE)
    {
        return 0;
    }

    auto max = 0;
    auto current_w = bridges[i][0];
    auto current_e = bridges[i][1];

    // If we can place a bridge
    if (current_w >= w && current_e >= e)
    {
        // Try with the bridge
        auto temp = bridges[i][2] + build_worker(bridges, i + 1, current_w + 1, current_e + 1);
        max = (max < temp)? temp : max;
    }

    // Try without the bridge
    auto temp = build_worker(bridges, i + 1, w, e);
    max = (max < temp)? temp : max;

    return max;
}

int build(int w, int e, std::vector<std::vector<int>> bridges)
{
    (void) w, (void) e, (void) bridges;

    if (bridges.empty())
    {
        return 0;
    }

    sortAndCull(bridges);

    return build_worker(bridges);
}

#endif // #ifndef FILE_BUILD_H_INCLUDED
