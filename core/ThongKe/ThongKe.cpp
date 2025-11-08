#include "ThongKe.h"
#include <iostream>

// Constructors
ThongKe::ThongKe() : tuNgay(), denNgay(), tieuDe("") {}

ThongKe::ThongKe(const NgayThang &tu, const NgayThang &den, const std::string &td)
    : tuNgay(tu), denNgay(den), tieuDe(td) {}

ThongKe::~ThongKe() {}
