#include "RandomUtil.h"

default_random_engine gen (time (0));
uniform_real_distribution<double> urand (0.0, 1.0);

double RandomUtil::uniform_random() {
    return urand(gen);;
}

double RandomUtil::random_angle() {
    return 2 * M_PI * uniform_random();
}

double RandomUtil::uniform_random(double minvalue, double maxvalue) {
    return (uniform_random() * abs(minvalue - maxvalue)) + min(minvalue, maxvalue);
}
