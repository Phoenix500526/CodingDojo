#pragma once

bool is_leap_year(int year) {
    return (0 == year % 400) || ((0 == year % 4) && (0 != year % 100));
}