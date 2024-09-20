#include "stream_reassembler.hh"
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    :  //构造函数
    _output(capacity)
    , _capacity(capacity)
    , _unassembled{}
    ,  //暂存未组装结构
    _nidx(0)
    ,  //已组装字节的下一个索引/第一个未组装索引
    _unassembled_cnt(0)
    ,            //未组装字节数
    _eofidx(-1)  // size_t 的-1是一个极大值
{}
//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (index >= _nidx + _output.remaining_capacity())  // index超出输出缓冲区
        return;
    if (eof)  //处理eof：收到eof设置末尾index
        _eofidx = index + data.size();
    size_t start = max(index, _nidx);  //起始index必须大于_nidx
    size_t end =
        min(index + data.size(), _nidx + _output.remaining_capacity());  //末尾index为data末尾，或者输出区溢出前
    for (size_t i = start; i < end; i++) {
        unordered_map<size_t, char>::iterator pos = _unassembled.find(i);
        if (pos != _unassembled.end()) {           //存在重复index
            if ((*pos).second != data[i - index])  //重复index且不同字符（非必要）
                throw runtime_error("Inconsistent Substring!");
            else
                continue;                                        //相同字符，不用管
        } else {                                                 //没有找到
            _unassembled.insert(make_pair(i, data[i - index]));  //暂存
            _unassembled_cnt++;                                  //计数
        }
    }

    string temp;
    while (_nidx < _eofidx) {
        unordered_map<size_t, char>::iterator pos = _unassembled.find(_nidx);  //查找下一个有序
        if (pos == _unassembled.end())  //存在不连续字符直接退出，data无效
            break;
        temp.push_back((*pos).second);  //插入字符
        _unassembled.erase(_nidx);      //暂存区删除对应对象
        _unassembled_cnt--;             //减数
        _nidx++;                        //处理下一个有序
    }
    _output.write(temp);  //写入输出

    if (_nidx == _eofidx)     //处理完暂存区
        _output.end_input();  //结束
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_cnt; }  //返回暂存区数据长度

bool StreamReassembler::empty() const { return _unassembled_cnt == 0; }  //暂存区是否空
