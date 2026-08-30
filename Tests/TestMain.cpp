#include <iostream>

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace
{

// GitHub Actions on Windows does not reliably capture juce::Logger / OutputDebugString
// from the test EXE, so mirror the UnitTestRunner log onto stderr for CI diagnosis.
class StderrUnitTestRunner final : public juce::UnitTestRunner
{
protected:
    void logMessage(const juce::String& message) override
    {
        juce::UnitTestRunner::logMessage(message);
        std::cerr << message << std::endl;
    }
};

void collectTestsMatchingArgs(int argc, char** argv, juce::Array<juce::UnitTest*>& outSelected)
{
    for (int argIndex = 1; argIndex < argc; ++argIndex)
    {
        const juce::String needle(argv[argIndex]);
        if (needle.isEmpty())
            continue;

        for (auto* unitTest : juce::UnitTest::getAllTests())
            if (unitTest->getName().containsIgnoreCase(needle))
                outSelected.addIfNotAlreadyThere(unitTest);
    }
}

int countAndReportFailures(const juce::UnitTestRunner& runner)
{
    int failures = 0;

    for (int i = 0; i < runner.getNumResults(); ++i)
    {
        const auto* result = runner.getResult(i);
        if (result == nullptr)
            continue;

        failures += result->failures;

        if (result->failures <= 0)
            continue;

        std::cerr << "SUMMARY FAIL: " << result->unitTestName
                  << " / " << result->subcategoryName
                  << " (" << result->failures << ")" << std::endl;

        for (const auto& message : result->messages)
            std::cerr << "  " << message << std::endl;
    }

    std::cerr << "TEST SUMMARY: " << failures << " failure(s)" << std::endl;
    std::cerr.flush();
    return failures;
}

} // namespace

// Console entry point for the Matrix-Control unit test runner. Every UnitTest
// subclass registers itself statically, so running all tests requires no manual
// registration here. Optional argv tokens filter by unit-test name substring
// (e.g. `Matrix-Control_Tests MutationPlayability`). Returns a non-zero exit
// code when any test fails so the runner can gate CI / local builds.
int main(int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI juceInitialiser;

    StderrUnitTestRunner runner;
    runner.setAssertOnFailure(false);

    if (argc <= 1)
    {
        runner.runAllTests();
        return countAndReportFailures(runner) == 0 ? 0 : 1;
    }

    juce::Array<juce::UnitTest*> selected;
    collectTestsMatchingArgs(argc, argv, selected);
    if (selected.isEmpty())
    {
        std::cerr << "No unit tests matched the given name filter(s)." << std::endl;
        return 1;
    }

    runner.runTests(selected);
    return countAndReportFailures(runner) == 0 ? 0 : 1;
}
