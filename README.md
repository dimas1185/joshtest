# Josh Coding challange

Challange source https://github.com/jstarllc/JoshCodingChallenge

Code challange implemented with c++20 standard   
No command line interface, Hardcoded values:   
http server: http://localhost:8080   
sleep time between sync requests: 0.5 sec   

### How to build:

    git clone https://github.com/dimas1185/joshtest
    cd joshtest
    git submodule update --init --recursive
    mkdir build && cd build && cmake .. && make -j

### How to test:

    ctest

### Submodules used:   
http: https://github.com/yhirose/cpp-httplib   
json: https://github.com/nlohmann/json   
test: https://github.com/doctest   