#pragma once // Include guard (platform-specific but widely supported)

// Employee record structure shared between Creator, Reporter, and Main
struct Employee {
    int    num;       // Employee identification number
    char   name[10];  // Employee name (max 9 chars + null terminator)
    double hours;     // Number of hours worked
};
