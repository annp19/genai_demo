#ifndef SIMULATOR_H
#define SIMULATOR_H

void simulator_init(const char *host, int port);
void simulator_start();
void simulator_stop();
void simulator_cleanup();

#endif // SIMULATOR_H