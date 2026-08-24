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

} // namespace

// Console entry point for the Matrix-Control unit test runner. Every UnitTest
// subclass registers itself statically, so running all tests requires no manual
// registration here. Returns a non-zero exit code when any test fails so the
// runner can gate CI / local builds.
int main(int, char**)
{
    juce::ScopedJuceInitialiser_GUI juceInitialiser;

    StderrUnitTestRunner runner;
    runner.setAssertOnFailure(false);
    runner.runAllTests();

    int failures = 0;
    for (int i = 0; i < runner.getNumResults(); ++i)
    {
        const auto* result = runner.getResult(i);
        failures += result->failures;

        if (result->failures > 0)
        {
            std::cerr << "SUMMARY FAIL: " << result->unitTestName
                      << " / " << result->subcategoryName
                      << " (" << result->failures << ")" << std::endl;

            for (const auto& message : result->messages)
                std::cerr << "  " << message << std::endl;
        }
    }

    std::cerr << "TEST SUMMARY: " << failures << " failure(s)" << std::endl;
    std::cerr.flush();

    return failures == 0 ? 0 : 1;
}
