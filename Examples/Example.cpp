#include "FastLog.h"

DYNAMIC_FASTINTVARIABLE(BarHundredthsPercent, 0)
DYNAMIC_FASTSTRINGVARIABLE(BarUrl, "bar.com")

void doSomethingNew();
void doSomethingElse();
void reportMetrics();

void doSomething()
{
    doSomethingNew();
    doSomethingElse();

    const int r = RBX::globalRand() % 10000;
    if (r <= DFInt::BarHundredthsPercent)
    {
        reportMetrics(DFString::BarUrl, MetricsType::Telemetry);
    }
}