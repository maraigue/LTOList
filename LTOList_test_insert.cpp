#include "LTOList.hpp"
#include <vector>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Inserting at the any position") {
    std::vector<double> m;
    for(size_t i = 100; i < 103; ++i) m.push_back(i);

    for(size_t n = 4; n < 10; ++n){
        for(size_t k = 0; k < n; ++k) {
            LTOList<double> ot;

            LTOList<double>::iterator target;
            for(size_t i = 0; i < n; ++i) {
                LTOList<double>::iterator target_tmp = ot.insert(ot.end(), i);
                if(i == k) target = target_tmp;
            }

            // Before insertion
            REQUIRE(ot.size() == n);
            for(size_t p = 0; p < n; ++p){
                REQUIRE(ot[p] == p);
            }

            ot.insert(target, m.begin(), m.end());

            // After insertion
            REQUIRE(ot.size() == n + m.size());
            for(size_t p = 0; p < n + m.size(); ++p){
                if(p >= k && p < k + m.size()){
                    REQUIRE(ot[p] == m[p-k]);
                }else if(p < k){
                    REQUIRE(ot[p] == p);
                }else{ // p >= k + m.size()
                    REQUIRE(ot[p] == p - m.size());
                }
            }
        }
    }
}
