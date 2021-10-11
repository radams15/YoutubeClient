/*
 * Copyright (C) 2019 Zander Brown <zbrown@gnome.org>
 * Copyright (C) 2019 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#pragma once

#if !defined(_ADWAITA_INSIDE) && !defined(ADWAITA_COMPILATION)
#error "Only <adwaita.h> can be included directly."
#endif

#include "adw-version.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ADW_TYPE_VIEW_SWITCHER (adw_view_switcher_get_type())

ADW_AVAILABLE_IN_ALL
G_DECLARE_FINAL_TYPE (AdwViewSwitcher, adw_view_switcher, ADW, VIEW_SWITCHER, GtkWidget)

typedef enum {
  ADW_VIEW_SWITCHER_POLICY_AUTO,
  ADW_VIEW_SWITCHER_POLICY_NARROW,
  ADW_VIEW_SWITCHER_POLICY_WIDE,
} AdwViewSwitcherPolicy;

ADW_AVAILABLE_IN_ALL
GtkWidget *adw_view_switcher_new (void);

ADW_AVAILABLE_IN_ALL
AdwViewSwitcherPolicy adw_view_switcher_get_policy (AdwViewSwitcher *self);
ADW_AVAILABLE_IN_ALL
void                  adw_view_switcher_set_policy (AdwViewSwitcher       *self,
                                                    AdwViewSwitcherPolicy  policy);

ADW_AVAILABLE_IN_ALL
PangoEllipsizeMode adw_view_switcher_get_narrow_ellipsize (AdwViewSwitcher *self);
ADW_AVAILABLE_IN_ALL
void               adw_view_switcher_set_narrow_ellipsize (AdwViewSwitcher    *self,
                                                           PangoEllipsizeMode  mode);

ADW_AVAILABLE_IN_ALL
GtkStack *adw_view_switcher_get_stack (AdwViewSwitcher *self);
ADW_AVAILABLE_IN_ALL
void      adw_view_switcher_set_stack (AdwViewSwitcher *self,
                                       GtkStack        *stack);

G_END_DECLS
