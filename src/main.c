#include "data.h"
#include "mqtt.h"
#include "simulator.h"
#include "gui.h"

int main(int argc, char *argv[]) {
    data_init();
    gui_init(argc, argv);

    const char *host = "localhost";
    int port = 1883;

    mqtt_init(host, port, data_add);
    mqtt_subscribe("telemetry");
    mqtt_start_loop();

    simulator_init(host, port);
    simulator_start();

    gui_main_loop();

    simulator_stop();
    simulator_cleanup();
    mqtt_stop_loop();
    mqtt_cleanup();

    return 0;
}