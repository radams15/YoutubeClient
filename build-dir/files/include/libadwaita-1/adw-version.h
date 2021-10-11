/*
 * Copyright (C) 2017 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#pragma once

#if !defined(_ADWAITA_INSIDE) && !defined(ADWAITA_COMPILATION)
#error "Only <adwaita.h> can be included directly."
#endif

/**
 * SECTION:adw-version
 * @short_description: Adwaita version checking.
 *
 * libadwaita provides macros to check the version of the library at compile-time.
 */

/**
 * ADW_MAJOR_VERSION:
 *
 * Adw major version component (e.g. 1 if %ADW_VERSION is 1.2.3)
 */
#define ADW_MAJOR_VERSION              (1)

/**
 * ADW_MINOR_VERSION:
 *
 * Adw minor version component (e.g. 2 if %ADW_VERSION is 1.2.3)
 */
#define ADW_MINOR_VERSION              (1)

/**
 * ADW_MICRO_VERSION:
 *
 * Adw micro version component (e.g. 3 if %ADW_VERSION is 1.2.3)
 */
#define ADW_MICRO_VERSION              (0)

/**
 * ADW_VERSION
 *
 * Adw version.
 */
#define ADW_VERSION                    (1.1.0)

/**
 * ADW_VERSION_S:
 *
 * Adwaita version, encoded as a string, useful for printing and
 * concatenation.
 */
#define ADW_VERSION_S                  "1.1.0"

#define ADW_ENCODE_VERSION(major,minor,micro) \
        ((major) << 24 | (minor) << 16 | (micro) << 8)

/**
 * ADW_VERSION_HEX:
 *
 * Adwaita version, encoded as an hexadecimal number, useful for
 * integer comparisons.
 */
#define ADW_VERSION_HEX \
        (ADW_ENCODE_VERSION (ADW_MAJOR_VERSION, ADW_MINOR_VERSION, ADW_MICRO_VERSION))

/**
 * ADW_CHECK_VERSION:
 * @major: required major version
 * @minor: required minor version
 * @micro: required micro version
 *
 * Compile-time version checking. Evaluates to %TRUE if the version
 * of Adwaita is greater than the required one.
 */
#define ADW_CHECK_VERSION(major,minor,micro)   \
        (ADW_MAJOR_VERSION > (major) || \
         (ADW_MAJOR_VERSION == (major) && ADW_MINOR_VERSION > (minor)) || \
         (ADW_MAJOR_VERSION == (major) && ADW_MINOR_VERSION == (minor) && \
          ADW_MICRO_VERSION >= (micro)))

#ifndef _ADW_EXTERN
#define _ADW_EXTERN extern
#endif

#ifndef ADW_VERSION_MAX_ALLOWED
# define ADW_VERSION_MAX_ALLOWED ADW_VERSION_1_1
#endif

#define ADW_UNAVAILABLE(major, minor) G_UNAVAILABLE(major, minor) _ADW_EXTERN

#define ADW_AVAILABLE_IN_ALL _ADW_EXTERN
