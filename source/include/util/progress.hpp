//进度条，显示渲染的进度
#pragma once

#include "thread/spin_lock.hpp"

class Progress {
public:
    Progress(size_t total, size_t step = 1);

    void update(size_t count);
private:
    size_t total, current; //total是总数，count加到current，current除total得到百分数，百分数每增加step就打印一次进度
    int percent, last_percent, step;
    SpinLock spin_lock;
};