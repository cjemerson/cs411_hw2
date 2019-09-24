// build.h
// Charles Emerson
// 21 September 2019
// Updated: 23 September 2019
//
// For CS 411 Fall 2019
// Source for function build
// Solution for Assignment 2, Exercise A


#ifndef FILE_BUILD_H_INCLUDED
#define FILE_BUILD_H_INCLUDED

#include <vector> // For std::vector
#include <algorithm> // For std::sort, std::remove_if

void sortAndCull(std::vector<std::vector<int>> & bridges)
{
    // Sort bridges in the following order:
    //   1) Ascending order of West city numbers (Lowest to Highest)
    //   2) Ascending order of East city numbers (Lowest to Highest)
    //   3) Descending order of bridge tolls (Highest to Lowest)
    std::sort(bridges.begin(), bridges.end(),
        [](const auto & a, const auto & b)
        {
            return a[0] < b[0] ||
                (a[0] == b[0] &&
                    ((a[1] < b[1]) ||
                    (a[1] == b[1] && (a[2] >= b[2]))));
        });

    // Cull every subsequent duplicate bridge after the first.
    // (Duplicate bridges, which have the same West and East cities,
    // are consecutive in order of highest toll to lowest toll.)
    int last_w = -1, last_e = -1;

    auto it = std::remove_if(bridges.begin(), bridges.end(),
        [&](const auto & bridge)
        {
            auto value = bridge[0] == last_w && bridge[1] == last_e;

            last_w = bridge[0];
            last_e = bridge[1];

            return value;
        });

    bridges.erase(it, bridges.end());
}


int build_worker(const std::vector<std::vector<int>> & bridges,
    int i = 0, int nextEastCity = 0)
{
    const int BRIDGES_SIZE = int(bridges.size());

    // If we are at the end of the list of bridges
    if (i == BRIDGES_SIZE)
    {
        return 0;
    }

    // The following optimizizes a data set with many bridges from
    // only a few West cities. We find the bridges in the index range
    // of [i, j) which start at the same West city. Then we skip
    // those bridges (in East city ascending order) that have a
    // smaller East city than nextEastCity.

    auto current_w = bridges[i][0];

    auto j = i + 1;
    for ( ; j < BRIDGES_SIZE && bridges[j][0] == current_w; ++j);

    auto x = i; 
    for ( ; x < j && bridges[x][1] < nextEastCity; ++x);

    auto max = 0;

    for ( ; x < j; ++x)
    {
        // All of the following bridges are valid choices from
        // current_w to some East city that is greater than any
        // current ones.

        auto current_e = bridges[x][1];

        // Try with the bridge from current_w to current_e
        auto temp = bridges[x][2] + build_worker(bridges, j, current_e + 1);
        max = (max < temp)? temp : max;
    }

    // Try without any bridges from current_w
    auto temp = build_worker(bridges, j, nextEastCity);
    max = (max < temp)? temp : max;

    return max;
}

int build(int w, int e, std::vector<std::vector<int>> bridges)
{
    // For a data set with a great many bridges, it might be useful
    // to provide e (the maximum East city + 1) because if any edge
    // meets e - 1 we can return immediately without following the
    // recursive tree further. I excluded it from build_worker()
    // because I did not see any benefits -CJ

    (void) w, (void) e; // To suppress -Wunused-parameter

    if (bridges.empty())
    {
        return 0;
    }

    sortAndCull(bridges);

    return build_worker(bridges);
}

#endif // #ifndef FILE_BUILD_H_INCLUDED
