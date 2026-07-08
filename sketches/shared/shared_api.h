// shared_api.h - compatibility wrappers so detector sketches can call
// the exact function names they expect while using the shared implementations
#ifndef SHARED_API_H
#define SHARED_API_H

// Include shared sensor comm helpers first so implementations that call
// readFrame()/configureSensor() are declared.
#include "sensor_comm.h"
#include "sensor_config.h"

// Canonical baseline declarations (baseline.h) must be included before
// normalize helper so baseline arrays (baseline_mean, baseline_std) exist.
#include "baseline.h"

// Normalization helper depends on baseline arrays
#include "normalize.h"

// Include shared headers (C++ .h) instead of fragile .ino includes
#include "baseline_stats.h"
#include "baseline_process.h"
#include "baseline_collect.h"
#include "baseline_storage.h"
#include "baseline_validate.h"
#include "features.h"

// Provide a lightweight declaration for runBaselineCalibration (wrapper)
#include "run_baseline_wrapper.h"

// Note: do NOT include run_baseline_impl.cpp here. If the sketch provides
// run_baseline_impl.cpp it will be compiled by the build system as a separate
// translation unit. Including it here would duplicate the symbol.

#include <Lampochka.h>
// Do NOT include .cpp implementation files here. The Arduino build system
// already compiles each .cpp file present in the sketch folder exactly once.
// Including .cpp files into a header causes those implementations to be
// compiled into the generated .ino translation unit AND as separate objects,
// leading to multiple-definition linker errors. Ensure the .cpp files are
// present in the sketch folder (or provided by a library) so they are
// compiled once by the build system.

#endif
parate Arduino library installation.
#include "baseline_stats.cpp"
#include "baseline_process.cpp"
#include "baseline_collect.cpp"
#include "baseline_storage.cpp"
#include "baseline_validate.cpp"
#include "features.cpp"

#endif
