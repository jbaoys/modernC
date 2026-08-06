/**
 *
 * This is a sample of fibonacci sequence
 *
 * @author: codeium
 * @date: 2022-08-10
 * @version: 1.0
 * @copyright: Copyright (c) 2022
 * @license: MIT
 * @github: https://github.com/codeium
 * @homepage: https://codeium.com
 *
 * @description: This is a sample of fibonacci sequence
 */
// include all needed header files
#include <stdio.h>
#include <iostream>
/**
 * Function to calculate fibonacci
 * @param n: number of fibonacci
 * @return: fibonacci
 */
int fibonacci(int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }   
}


int main() {
    for (int i = 0; i < 10; i++) {
        std::cout << fibonacci(i) << std::endl;
    }
    return 0;
}
