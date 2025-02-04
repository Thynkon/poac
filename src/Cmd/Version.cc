#include "Version.hpp"

#include "../Git2/Version.hpp"
#include "../Logger.hpp"
#include "Global.hpp"

#include <cstdlib>
#include <curl/curl.h>
#include <iostream>
#include <span>

#ifndef POAC_PKG_VERSION
#  error "POAC_PKG_VERSION is not defined"
#endif
#ifndef POAC_COMMIT_SHORT_HASH
#  error "POAC_COMMIT_SHORT_HASH is not defined"
#endif
#ifndef POAC_COMMIT_HASH
#  error "POAC_COMMIT_HASH is not defined"
#endif
#ifndef POAC_COMMIT_DATE
#  error "POAC_COMMIT_DATE is not defined"
#endif

static constexpr char
firstMonthChar(const char m1) noexcept {
  return (m1 == 'O' || m1 == 'N' || m1 == 'D') ? '1' : '0';
}
static constexpr char
secondMonthChar(const char m1, const char m2, const char m3) noexcept {
  if (m1 == 'J') {
    if (m2 == 'a') {
      // Jan
      return '1';
    } else if (m3 == 'n') {
      // Jun
      return '6';
    } else {
      // Jul
      return '7';
    }
  } else if (m1 == 'F') {
    // Feb
    return '2';
  } else if (m1 == 'M') {
    if (m2 == 'a') {
      // Mar
      return '3';
    } else {
      // May
      return '5';
    }
  } else if (m1 == 'A') {
    if (m2 == 'p') {
      // Apr
      return '4';
    } else {
      // Aug
      return '8';
    }
  } else if (m1 == 'S') {
    // Sep
    return '9';
  } else if (m1 == 'O') {
    // Oct
    return '0';
  } else if (m1 == 'N') {
    // Nov
    return '1';
  } else if (m1 == 'D') {
    // Dec
    return '2';
  } else {
    // Error
    return '0';
  }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
static inline constexpr char COMPILE_DATE[] = {
  // Year
  __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10],

  '-',

  // Month
  firstMonthChar(__DATE__[0]),
  secondMonthChar(__DATE__[0], __DATE__[1], __DATE__[2]),

  '-',

  // Day
  __DATE__[4] == ' ' ? '0' : __DATE__[4], __DATE__[5],

  '\0'
};

int
versionMain(const std::span<const StringRef> args) noexcept {
  // Parse args
  for (usize i = 0; i < args.size(); ++i) {
    const StringRef arg = args[i];
    HANDLE_GLOBAL_OPTS({ { "version" } })

    else {
      Logger::error("invalid argument: ", arg);
      return EXIT_FAILURE;
    }
  }

  std::cout << "poac " << POAC_PKG_VERSION << " (" << POAC_COMMIT_SHORT_HASH
            << ' ' << POAC_COMMIT_DATE << ")\n";

  if (isVerbose()) {
    std::cout << "release: " << POAC_PKG_VERSION << '\n'
              << "commit-hash: " << POAC_COMMIT_HASH << '\n'
              << "commit-date: " << POAC_COMMIT_DATE << '\n'
              << "compiler: " << __VERSION__ << '\n'
              << "compile-date: " << COMPILE_DATE << '\n'
              << "libgit2: " << git2::Version() << '\n';

    const curl_version_info_data* curlData = curl_version_info(CURLVERSION_NOW);
    if (curlData) {
      std::cout << "libcurl: " << curlData->version << " (ssl: ";
      if (curlData->ssl_version) {
        std::cout << curlData->ssl_version;
      } else {
        std::cout << "none";
      }
      std::cout << ")\n";
    }
  }

  return EXIT_SUCCESS;
}

void
versionHelp() noexcept {
  std::cout << versionDesc << '\n';
  std::cout << '\n';
  printUsage("version", "[OPTIONS]");
  std::cout << '\n';
  printHeader("Options:");
  printGlobalOpts();
}
