#include <stdio.h>

double calculatePi(int n) {
    double pi = 0.0;
    int sign = 1;

    for (int i = 0; i < n; i++) {
        pi += sign * 1.0 / (2 * i + 1);
        sign = -sign;
    }

    return 4 * pi;
}

int main() {
    int n;
    printf("请输入计算的项数：");
    scanf("%d", &n);

    double pi = calculatePi(n);
    printf("计算得到的pi值为：%lf\n", pi);

    return 0;
}