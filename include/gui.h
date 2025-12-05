#ifndef GUI_H
#define GUI_H

#include "telemetry.h"

void gui_init(int argc, char *argv[]);
void gui_update_labels(TelemetryData data);
void gui_redraw_charts();
void gui_main_loop();

#endif // GUI_H