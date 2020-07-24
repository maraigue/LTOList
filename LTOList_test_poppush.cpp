#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Add or remove an element at the beginning or the end") {
    SECTION("pop_front"){
        for(size_t n = 4; n < 18; ++n){
            LTOList<double> ot;
            for(size_t i = 0; i < n; ++i) ot.insert(ot.end(), i);

            // Before pop_front
            for(size_t i = 0; i < n; ++i){
                CHECK(ot[i] == i);
            }

            ot.pop_front();

            // After pop_front
            CHECK(ot.size() == n - 1);
            for(size_t i = 0; i < n - 1; ++i){
                CHECK(ot[i] == i + 1);
            }
        }
    }

    SECTION("pop_back"){
        for(size_t n = 4; n < 10; ++n){
            LTOList<double> ot;
            for(size_t i = 0; i < n; ++i) ot.insert(ot.end(), i);

            // Before pop_back
            for(size_t i = 0; i < n; ++i){
                CHECK(ot[i] == i);
            }

            ot.pop_back();

            // After pop_back
            CHECK(ot.size() == n - 1);
            for(size_t i = 0; i < n - 1; ++i){
                CHECK(ot[i] == i);
            }
        }
    }

    SECTION("push_front"){
        for(size_t n = 4; n < 10; ++n){
            LTOList<double> ot;
            for(size_t i = 0; i < n; ++i) ot.insert(ot.end(), i);

            // Before push_front
            for(size_t i = 0; i < n; ++i){
                CHECK(ot[i] == i);
            }

            ot.push_front(100);

            // After push_front
            CHECK(ot.size() == n + 1);
            CHECK(ot[0] == 100);
            for(size_t i = 0; i < n; ++i){
                CHECK(ot[i + 1] == i);
            }
        }
    }

    SECTION("push_back"){
        for(size_t n = 4; n < 10; ++n){
            LTOList<double> ot;
            for(size_t i = 0; i < n; ++i) ot.insert(ot.end(), i);

            // Before push_front
            for(size_t i = 0; i < n; ++i){
                CHECK(ot[i] == i);
            }

            ot.push_back(100);

            // After push_front
            CHECK(ot.size() == n + 1);
            CHECK(ot[n] == 100);
            for(size_t i = 0; i < n; ++i){
                CHECK(ot[i] == i);
            }
        }
    }
}
